/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "engine.hpp"

#include "engine_dump_io.hpp"
#include "res_guard.hpp"
#include "axcl_logger.hpp"

#include <ax_sys_api.h>
#include <ax_engine_api.h>

#define TAG "engine"
#define LEADING_NAME "pid"

struct engine_holder {
    explicit engine_holder(AX_ENGINE_HANDLE handle) : handle(handle) {}

    ~engine_holder() {
        if (nullptr != handle) {
            AX_ENGINE_DestroyHandle(handle);
        }
    }

    AX_ENGINE_HANDLE handle;
};

axcl::pkg::runtime::engine::LAYOUT trans_layout(const AX_ENGINE_TENSOR_LAYOUT_T layout) {
    switch (layout) {
    case AX_ENGINE_TENSOR_LAYOUT_NHWC:
        return axcl::pkg::runtime::engine::LAYOUT::NHWC;
    case AX_ENGINE_TENSOR_LAYOUT_NCHW:
        return axcl::pkg::runtime::engine::LAYOUT::NCHW;
    default:
        return axcl::pkg::runtime::engine::LAYOUT::NONE;
    }
}

axcl::pkg::runtime::engine::FORMAT trans_format(const AX_ENGINE_DATA_TYPE_T format) {
    switch (format) {
    case AX_ENGINE_DT_UINT8:
        return axcl::pkg::runtime::engine::FORMAT::UINT8;
    case AX_ENGINE_DT_UINT16:
        return axcl::pkg::runtime::engine::FORMAT::UINT16;
    case AX_ENGINE_DT_FLOAT32:
        return axcl::pkg::runtime::engine::FORMAT::FP32;
    case AX_ENGINE_DT_SINT16:
        return axcl::pkg::runtime::engine::FORMAT::INT16;
    case AX_ENGINE_DT_SINT8:
        return axcl::pkg::runtime::engine::FORMAT::INT8;
    case AX_ENGINE_DT_SINT32:
        return axcl::pkg::runtime::engine::FORMAT::INT32;
    case AX_ENGINE_DT_UINT32:
        return axcl::pkg::runtime::engine::FORMAT::UINT32;
    case AX_ENGINE_DT_FLOAT64:
        return axcl::pkg::runtime::engine::FORMAT::FP64;
    case AX_ENGINE_DT_BFLOAT16:
        return axcl::pkg::runtime::engine::FORMAT::BF16;
    default:
        return axcl::pkg::runtime::engine::FORMAT::NONE;
    }
}

void fill_engine_io_info(const std::vector<AX_ENGINE_IO_INFO_T *>& src, axcl::pkg::runtime::engine::meta& dst) {
    const auto group_count = src.size();
    const auto input_count = src[0]->nInputSize;
    const auto output_count = src[0]->nOutputSize;

    dst.inputs.resize(input_count);
    dst.outputs.resize(output_count);

    for (size_t i = 0; i < input_count; i++) {
        dst.inputs[i].groups.resize(group_count);
    }
    for (size_t i = 0; i < output_count; i++) {
        dst.outputs[i].groups.resize(group_count);
    }

    for (decltype(src[0]->nInputSize) i = 0; i < input_count; i++) {
        dst.inputs[i].name = src[0]->pInputs[i].pName;
        dst.inputs[i].layout = trans_layout(src[0]->pInputs[i].eLayout);
        dst.inputs[i].format = trans_format(src[0]->pInputs[i].eDataType);
    }
    for (decltype(src[0]->nOutputSize) o = 0; o < output_count; o++) {
        dst.outputs[o].name = src[0]->pOutputs[o].pName;
        dst.outputs[o].layout = trans_layout(src[0]->pOutputs[o].eLayout);
        dst.outputs[o].format = trans_format(src[0]->pOutputs[o].eDataType);
    }

    for (size_t g = 0; g < group_count; g++) {
        for (decltype(src[0]->nInputSize) i = 0; i < input_count; i++) {
            auto &[size, shape, stride] = dst.inputs[i].groups[g];
            auto &selected_src = src[g]->pInputs[i];
            size = selected_src.nSize;
            shape.resize(selected_src.nShapeSize);
            for (decltype(selected_src.nShapeSize) j = 0; j < selected_src.nShapeSize; j++) {
                shape[j] = selected_src.pShape[j];
            }
            if (nullptr != selected_src.pStride) {
                stride.resize(selected_src.nShapeSize);
                for (decltype(selected_src.nShapeSize) j = 0; j < selected_src.nShapeSize; j++) {
                    stride[j] = selected_src.pStride[j];
                }
            }
        }
    }
    for (size_t g = 0; g < group_count; g++) {
        for (decltype(src[0]->nOutputSize) o = 0; o < output_count; o++) {
            auto &[size, shape, stride] = dst.outputs[o].groups[g];
            auto &selected_src = src[g]->pOutputs[o];
            size = selected_src.nSize;
            shape.resize(selected_src.nShapeSize);
            for (decltype(selected_src.nShapeSize) j = 0; j < selected_src.nShapeSize; j++) {
                shape[j] = selected_src.pShape[j];
            }
            if (nullptr != selected_src.pStride) {
                stride.resize(selected_src.nShapeSize);
                for (decltype(selected_src.nShapeSize) j = 0; j < selected_src.nShapeSize; j++) {
                    stride[j] = selected_src.pStride[j];
                }
            }
        }
    }
}

RUNTIME_ENGINE_API_IMPL(LOAD) {
    LOG_MM_D(TAG, "axclrt engine load...");

    std::lock_guard lock(this->m_mutex);
    const auto address = response->get_param_address();
    const auto size = response->get_param_size();

    LOG_MM_D(TAG, "load engine from address: {:#x}, size: {}", reinterpret_cast<AX_U64>(address), size);

    auto map_holder = res_guard<void*>(
        AX_SYS_Mmap(reinterpret_cast<AX_U64>(address), size),
        [size](void* ptr) { if (nullptr != ptr) {AX_SYS_Munmap(ptr, size);} }
    );

    if (nullptr == map_holder.get()) {
        LOG_MM_E(TAG, "Failed mmap phy({:#x}).", reinterpret_cast<AX_U64>(address));
        request->set_status(AX_ERR_CMM_MMAP_FAIL);
        return AX_ERR_CMM_MMAP_FAIL;
    }
    LOG_MM_D(TAG, "Mmap phy({:#x}) to vir({:#x}).", reinterpret_cast<AX_U64>(address), reinterpret_cast<AX_U64>(map_holder.get()));

    const auto pid = response->get_env().pid;
    const auto model_name = LEADING_NAME + std::to_string(pid);

    AX_ENGINE_HANDLE handle{};
    AX_ENGINE_HANDLE_EXTRA_T extra{};
    extra.pName = (AX_S8*)(model_name.c_str());

    auto ret = ::AX_ENGINE_CreateHandleV2(&handle, map_holder.get(), size, &extra);
    if (0 != ret) {
        request->set_status(ret);
        return ret;
    }

    engine_holder holder(handle);

    AX_U32 group_count = 0;
    if (ret = ::AX_ENGINE_GetGroupIOInfoCount(handle, &group_count); 0 != ret) {
        request->set_status(ret);
        return ret;
    }

    std::vector<AX_ENGINE_IO_INFO_T *> info_group(group_count);
    if (1 == group_count) {
        if (ret = ::AX_ENGINE_GetIOInfo(handle, &info_group[0]); 0 != ret) {
            request->set_status(ret);
            return ret;
        }
    } else {
        for (AX_U32 i = 0; i < group_count; i++) {
            if (ret = ::AX_ENGINE_GetGroupIOInfo(handle, i, &info_group[i]); 0 != ret) {
                request->set_status(ret);
                return ret;
            }
        }
    }

    pkg::runtime::engine::meta info;
    fill_engine_io_info(info_group, info);
    engine_info_dump(&info);

    this->m_map.emplace(reinterpret_cast<uint64_t>(handle), info);
    holder.handle = nullptr;

    LOG_MM_D(TAG, "Handle: {:#x}, ret: {}.", reinterpret_cast<uint64_t>(handle), ret);

    request->set_param_handle(reinterpret_cast<AX_U64>(handle));
    request->set_status(0);
    return 0;
}

RUNTIME_ENGINE_API_IMPL(UNLOAD) {
    LOG_MM_D(TAG, "axclrt engine unload...");

    std::lock_guard lock(this->m_mutex);
    const auto handle = response->get_param_handle();
    if (0 == this->m_map.count(handle)) {
        request->set_status(-1);
        return -1;
    }

    this->m_map.extract(handle);
    const auto ret = ::AX_ENGINE_DestroyHandle(reinterpret_cast<void *>(handle));

    request->set_status(ret);
    return ret;
}
