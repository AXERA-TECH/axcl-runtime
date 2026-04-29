/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "npu.hpp"

#include "npu_local.hpp"
#include "npu_string.hpp"
#include "npu_io_meta.hpp"
#include "npu_io_holder.hpp"

std::mutex g_npu_map_mutex;
std::shared_ptr<info_map_holder> g_npu_map = std::make_shared<info_map_holder>();

void npu::dump_io_info(const AX_ENGINE_IO_INFO_T* info) {
    LOG_MM_D(TAG, "Info ptr: {:#x}.", reinterpret_cast<AX_U64>(info));

    for (AX_U32 i = 0; i < info->nInputSize; i++) {
        std::string shape_string;
        shape_string += "{";
        for (int j = 0; j < info->pInputs[i].nShapeSize - 1; j++) {
            shape_string += std::to_string(info->pInputs[i].pShape[j]);
            shape_string += " ";
        }
        shape_string += std::to_string(info->pInputs[i].pShape[info->pInputs[i].nShapeSize - 1]) + "}";

        LOG_MM_D(TAG, "Input[{}] ptr: {:#x}.", i, reinterpret_cast<AX_U64>(&(info->pInputs[i])));
        LOG_MM_D(TAG, "Input[{}]: name: {}, shape: {}, size: {}, layout: {}, format: {}.",
            i, std::string(info->pInputs[i].pName), shape_string, info->pInputs[i].nSize,
            static_cast<int>(info->pInputs[i].eLayout), static_cast<int>(info->pInputs[i].eDataType));
    }

    for (AX_U32 i = 0; i < info->nOutputSize; i++) {
        std::string shape_string;
        shape_string += "{";
        for (int j = 0; j < info->pOutputs[i].nShapeSize - 1; j++) {
            shape_string += std::to_string(info->pOutputs[i].pShape[j]);
            shape_string += " ";
        }
        shape_string += std::to_string(info->pOutputs[i].pShape[info->pOutputs[i].nShapeSize - 1]) + "}";

        LOG_MM_D(TAG, "Output[{}] ptr: {:#x}.", i, reinterpret_cast<AX_U64>(&(info->pOutputs[i])));
        LOG_MM_D(TAG, "Output[{}]: name: {}, shape: {}, size: {}, layout: {}, format: {}.",
            i, std::string(info->pOutputs[i].pName), shape_string, info->pOutputs[i].nSize,
            static_cast<int>(info->pOutputs[i].eLayout), static_cast<int>(info->pOutputs[i].eDataType));
    }
}

void npu::dump_io(const AX_ENGINE_IO_T* pIO) {
    for (AX_U32 i = 0; i < pIO->nInputSize; i++)
    {
        LOG_MM_D(TAG, "pInputs[{}]: phy: {:#x}, vir: {:#x}, size: {}.",
            i, pIO->pInputs[i].phyAddr, reinterpret_cast<AX_U64>(pIO->pInputs[i].pVirAddr), pIO->pInputs[i].nSize);
        if (nullptr != pIO->pInputs[i].pStride)
        {
            char stride_string[1024];
            sprintf(stride_string, "pInputs[%d]: stride[%d]: {", i, pIO->pInputs[i].nStrideSize);
            for (AX_U8 j = 0; j < pIO->pInputs[i].nStrideSize - 1; j++)
            {
                sprintf(stride_string, "%d, ", pIO->pInputs[i].pStride[j]);
            }
            sprintf(stride_string, "%d}.", pIO->pInputs[i].pStride[pIO->pInputs[i].nStrideSize - 1]);
            LOG_MM_D(TAG, "{}", std::string{stride_string});
        }
        LOG_MM_D(TAG, ".\n");
    }
    for (AX_U32 i = 0; i < pIO->nOutputSize; i++)
    {
        LOG_MM_D(TAG, "pOutputs[{}]: phy {:#x}, vir: {:#x}, size: {}.",
            i, pIO->pOutputs[i].phyAddr, reinterpret_cast<AX_U64>(pIO->pOutputs[i].pVirAddr), pIO->pOutputs[i].nSize);
        if (nullptr != pIO->pOutputs[i].pStride)
        {
            char stride_string[1024];
            sprintf(stride_string, "pOutputs[%d]: stride[%d]: {", i, pIO->pOutputs[i].nStrideSize);
            for (AX_U8 j = 0; j < pIO->pOutputs[i].nStrideSize - 1; j++)
            {
                sprintf(stride_string, "%d, ", pIO->pOutputs[i].pStride[j]);
            }
            sprintf(stride_string, "%d}", pIO->pOutputs[i].pStride[pIO->pOutputs[i].nStrideSize - 1]);
            LOG_MM_D(TAG, "{}", std::string{stride_string});
        }
    }
}

void npu::decode_meta(AX_ENGINE_IOMETA_T* meta_array, const array& meta_package, const AX_U32& package_count) {
    LOG_MM_D(TAG, "Will decode meta.");
    const auto one_meta_size = meta_package.size / package_count;

    for (AX_U32 i = 0; i < package_count; i++) {
        const auto src_meta = reinterpret_cast<meta*>(meta_package.data + i * one_meta_size);
        const auto src_name = reinterpret_cast<const char*>(meta_package.data + i * one_meta_size + sizeof(meta));

        std::string shape_string;
        shape_string += "{";
        for (uint32_t j = 0; j < src_meta->dim - 1; j++) {
            shape_string += std::to_string(src_meta->shape[j]);
            shape_string += " ";
        }
        shape_string += std::to_string(src_meta->shape[src_meta->dim - 1]) + "}";
        LOG_MM_D(TAG, "Meta[{}]: name: {}, shape: {}, size: {}, layout: {}, format: {}.",
                i, std::string(src_name), shape_string, src_meta->size, src_meta->layout, src_meta->format);

        auto& meta = meta_array[i];

        meta.pName = new AX_CHAR[strlen(src_name) + 1];
        ::strcpy(meta.pName, src_name);

        meta.pShape = new AX_S32[src_meta->dim];
        for (uint32_t j = 0; j < src_meta->dim; j++) {
            meta.pShape[j] = static_cast<AX_S32>(src_meta->shape[j]);
        }
        meta.nShapeSize = static_cast<AX_U8>(src_meta->dim);
        meta.eLayout = static_cast<AX_ENGINE_TENSOR_LAYOUT_T>(src_meta->layout);
        meta.eMemoryType = static_cast<AX_ENGINE_MEMORY_TYPE_T>(src_meta->memory);
        meta.eDataType = static_cast<AX_ENGINE_DATA_TYPE_T>(src_meta->format);
        if (std::numeric_limits<uint32_t>::max() != src_meta->color) {
            meta.pExtraMeta = new AX_ENGINE_IOMETA_EX_T();
            meta.pExtraMeta->eColorSpace = static_cast<AX_ENGINE_COLOR_SPACE_T>(src_meta->color);
        }
        else {
            meta.pExtraMeta = nullptr;
        }
        meta.nSize = src_meta->size;
        meta.nQuantizationValue = 0;

        if (0 != src_meta->stride[0]) {
            meta.pStride = new AX_S32[src_meta->dim];
            for (AX_U8 j = 0; j < meta.nShapeSize; j++) {
                meta.pStride[j] = static_cast<AX_S32>(src_meta->stride[j]);
            }
        }
        else {
            meta.pStride = nullptr;
        }
    }
}

uint8_t* npu::alloc_info_buffer(const array& input_package, const AX_U32& input_size, const array& output_package, const AX_U32& output_size) {
    const auto info = new AX_ENGINE_IO_INFO_T;
    info->nInputSize = input_size;
    info->nOutputSize = output_size;
    info->pInputs = new AX_ENGINE_IOMETA_T[input_size];
    info->pOutputs = new AX_ENGINE_IOMETA_T[output_size];

    decode_meta(info->pInputs, input_package, input_size);
    decode_meta(info->pOutputs, output_package, output_size);

    return reinterpret_cast<uint8_t*>(info);
}
