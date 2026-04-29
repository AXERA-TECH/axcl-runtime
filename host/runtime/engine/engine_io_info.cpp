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

#include "engine_strings.hpp"
#include "axcl_logger.hpp"

AXCL_EXPORT axclError axclrtEngineGetIOInfo(const uint64_t modelId, axclrtEngineIOInfo *ioInfo) {
    LOG_MM_D(TAG, "Start...");
    if (0 == modelId) {
        LOG_MM_E(TAG, ENGINE_ERR_MODEL_ID, modelId);
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }
    if (nullptr == ioInfo) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "ioInfo");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }

    const axcl::rt::engine api;
    return api.get_model_info(modelId, ioInfo);
}

AXCL_EXPORT axclError axclrtEngineDestroyIOInfo(axclrtEngineIOInfo ioInfo) {
    LOG_MM_D(TAG, "Start...");
    if (nullptr == ioInfo) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "ioInfo");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }

    return axcl::rt::engine::destroy_model_info(ioInfo);
}

AXCL_EXPORT axclError axclrtEngineGetShapeGroupsCount(axclrtEngineIOInfo ioInfo, int32_t *count) {
    LOG_MM_D(TAG, "Start...");
    if (nullptr == ioInfo) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "ioInfo");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }
    if (nullptr == count) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "count");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }

    return axcl::rt::engine::get_shape_group_count(ioInfo, count);
}

AXCL_EXPORT uint32_t axclrtEngineGetNumInputs(axclrtEngineIOInfo ioInfo) {
    LOG_MM_D(TAG, "Start...");
    if (nullptr == ioInfo) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "ioInfo");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }

    return axcl::rt::engine::get_input_count(ioInfo);
}

AXCL_EXPORT uint32_t axclrtEngineGetNumOutputs(axclrtEngineIOInfo ioInfo) {
    LOG_MM_D(TAG, "Start...");
    if (nullptr == ioInfo) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "ioInfo");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }

    return axcl::rt::engine::get_output_count(ioInfo);
}

AXCL_EXPORT uint64_t axclrtEngineGetInputSizeByIndex(axclrtEngineIOInfo ioInfo, const uint32_t group, const uint32_t index) {
    LOG_MM_D(TAG, "Start...");
    if (nullptr == ioInfo) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "ioInfo");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }

    return axcl::rt::engine::get_input_size(ioInfo, group, index);
}

AXCL_EXPORT uint64_t axclrtEngineGetOutputSizeByIndex(axclrtEngineIOInfo ioInfo, const uint32_t group, const uint32_t index) {
    LOG_MM_D(TAG, "Start...");
    if (nullptr == ioInfo) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "ioInfo");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }

    return axcl::rt::engine::get_output_size(ioInfo, group, index);
}

AXCL_EXPORT const char *axclrtEngineGetInputNameByIndex(axclrtEngineIOInfo ioInfo, const uint32_t index) {
    LOG_MM_D(TAG, "Start...");
    if (nullptr == ioInfo) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "ioInfo");
        return nullptr;
    }

    return axcl::rt::engine::get_input_name(ioInfo, index);
}

AXCL_EXPORT const char *axclrtEngineGetOutputNameByIndex(axclrtEngineIOInfo ioInfo, const uint32_t index) {
    LOG_MM_D(TAG, "Start...");
    if (nullptr == ioInfo) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "ioInfo");
        return nullptr;
    }

    return axcl::rt::engine::get_output_name(ioInfo, index);
}

AXCL_EXPORT int32_t axclrtEngineGetInputIndexByName(axclrtEngineIOInfo ioInfo, const char *name) {
    LOG_MM_D(TAG, "Start...");
    if (nullptr == ioInfo) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "ioInfo");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }
    if (nullptr == name) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "name");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }

    return axcl::rt::engine::get_input_index(ioInfo, name);
}

AXCL_EXPORT int32_t axclrtEngineGetOutputIndexByName(axclrtEngineIOInfo ioInfo, const char *name) {
    LOG_MM_D(TAG, "Start...");
    if (nullptr == ioInfo) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "ioInfo");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }
    if (nullptr == name) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "name");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }

    return axcl::rt::engine::get_output_index(ioInfo, name);
}

AXCL_EXPORT int32_t axclrtEngineGetInputDataType(axclrtEngineIOInfo ioInfo, const uint32_t index, axclrtEngineDataType *type) {
    LOG_MM_D(TAG, "Start...");
    if (nullptr == ioInfo) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "ioInfo");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }

    if (0 != axcl::rt::engine::get_input_data_type(ioInfo, index, type)) {
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }

    return 0;
}

AXCL_EXPORT int32_t axclrtEngineGetOutputDataType(axclrtEngineIOInfo ioInfo, const uint32_t index, axclrtEngineDataType *type) {
    LOG_MM_D(TAG, "Start...");
    if (nullptr == ioInfo) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "ioInfo");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }

    if (0 != axcl::rt::engine::get_output_data_type(ioInfo, index, type)) {
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }

    return 0;
}

AXCL_EXPORT int32_t axclrtEngineGetInputDataLayout(axclrtEngineIOInfo ioInfo, uint32_t index, axclrtEngineDataLayout *layout)
{
    LOG_MM_D(TAG, "Start...");
    if (nullptr == ioInfo) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "ioInfo");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }

    if (0 != axcl::rt::engine::get_input_data_layout(ioInfo, index, layout)) {
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }

    return 0;
}

AXCL_EXPORT int32_t axclrtEngineGetOutputDataLayout(axclrtEngineIOInfo ioInfo, uint32_t index, axclrtEngineDataLayout *layout)
{
    LOG_MM_D(TAG, "Start...");
    if (nullptr == ioInfo) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "ioInfo");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }

    if (0 != axcl::rt::engine::get_output_data_layout(ioInfo, index, layout)) {
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }

    return 0;
}

AXCL_EXPORT axclError axclrtEngineGetInputDims(axclrtEngineIOInfo ioInfo, const uint32_t group, const uint32_t index, axclrtEngineIODims *dims) {
    LOG_MM_D(TAG, "Start...");
    if (nullptr == ioInfo) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "ioInfo");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }
    if (nullptr == dims) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "dims");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }

    return axcl::rt::engine::get_input_dims(ioInfo, group, index, dims);
}

AXCL_EXPORT axclError axclrtEngineGetOutputDims(axclrtEngineIOInfo ioInfo, const uint32_t group, const uint32_t index, axclrtEngineIODims *dims) {
    LOG_MM_D(TAG, "Start...");
    if (nullptr == ioInfo) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "ioInfo");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }
    if (nullptr == dims) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "dims");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }

    return axcl::rt::engine::get_output_dims(ioInfo, group, index, dims);
}

axclError axcl::rt::engine::get_model_info(const uint64_t id, axclrtEngineIOInfo *info) const {
    axclrtStream stream = nullptr;
    axclrtEnvironment env;
    axclError ret;
    if (ret = get_runtime_environment(&stream, env); AXCL_SUCC != ret) {
        return ret;
    }

    this->m_req.get_info(id);
    if (ret = invoke(stream, env, NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    // FIXME: using axclrtMalloc to allocate memory
    *info = new pkg::runtime::engine::meta;
    const auto f = static_cast<pkg::runtime::engine::meta*>(*info);

    *f = this->m_rsp.get_param_meta();
    return AXCL_SUCC;
}

axclError axcl::rt::engine::destroy_model_info(axclrtEngineIOInfo info) {
    const auto f = static_cast<pkg::runtime::engine::meta*>(info);

    // FIXME: using axclrtFree to free memory
    delete f;

    return AXCL_SUCC;
}

axclError axcl::rt::engine::get_shape_group_count(axclrtEngineIOInfo info, int32_t *count) {
    const auto f = static_cast<pkg::runtime::engine::meta*>(info);
    *count = static_cast<int32_t>(f->inputs[0].groups.size());

    return AXCL_SUCC;
}

int32_t axcl::rt::engine::get_input_count(axclrtEngineIOInfo info) {
    const auto f = static_cast<pkg::runtime::engine::meta*>(info);
    return static_cast<int32_t>(f->inputs.size());
}

int32_t axcl::rt::engine::get_output_count(axclrtEngineIOInfo info) {
    const auto f = static_cast<pkg::runtime::engine::meta*>(info);
    return static_cast<int32_t>(f->outputs.size());
}

uint64_t axcl::rt::engine::get_input_size(axclrtEngineIOInfo info, const uint32_t group, const uint32_t index) {
    const auto f = static_cast<pkg::runtime::engine::meta*>(info);
    if (index >= f->inputs.size()) {
        LOG_MM_E(TAG, ENGINE_ERR_PARAM, "index", index);
        return AXCL_ERR_ENGINE_INVALID_INDEX;
    }
    if (group >= f->inputs[index].groups.size()) {
        LOG_MM_E(TAG, ENGINE_ERR_PARAM, "group", group);
        return AXCL_ERR_ENGINE_INVALID_INDEX;
    }

    return f->inputs[index].groups[group].size;
}

uint64_t axcl::rt::engine::get_output_size(axclrtEngineIOInfo info, const uint32_t group, const uint32_t index) {
    const auto f = static_cast<pkg::runtime::engine::meta*>(info);
    if (index >= f->outputs.size()) {
        LOG_MM_E(TAG, ENGINE_ERR_PARAM, "index", index);
        return AXCL_ERR_ENGINE_INVALID_INDEX;
    }
    if (group >= f->outputs[index].groups.size()) {
        LOG_MM_E(TAG, ENGINE_ERR_PARAM, "group", group);
        return AXCL_ERR_ENGINE_INVALID_INDEX;
    }

    return f->outputs[index].groups[group].size;
}

const char *axcl::rt::engine::get_input_name(axclrtEngineIOInfo info, const uint32_t index) {
    const auto f = static_cast<pkg::runtime::engine::meta*>(info);
    if (index >= f->inputs.size()) {
        LOG_MM_E(TAG, ENGINE_ERR_PARAM, "index", index);
        return nullptr;
    }

    return f->inputs[index].name.c_str();
}

const char *axcl::rt::engine::get_output_name(axclrtEngineIOInfo info, const uint32_t index) {
    const auto f = static_cast<pkg::runtime::engine::meta*>(info);
    if (index >= f->outputs.size()) {
        LOG_MM_E(TAG, ENGINE_ERR_PARAM, "index", index);
        return nullptr;
    }

    return f->outputs[index].name.c_str();
}

int32_t axcl::rt::engine::get_input_data_type(axclrtEngineIOInfo info, const uint32_t index, axclrtEngineDataType *type) {
    const auto f = static_cast<pkg::runtime::engine::meta*>(info);
    if (index >= f->inputs.size()) {
        LOG_MM_E(TAG, ENGINE_ERR_PARAM, "index", index);
        return AXCL_ERR_ENGINE_INVALID_INDEX;
    }

    *type = static_cast<axclrtEngineDataType>(f->inputs[index].format);
    return 0;
}

int32_t axcl::rt::engine::get_output_data_type(axclrtEngineIOInfo info, const uint32_t index, axclrtEngineDataType *type) {
    const auto f = static_cast<pkg::runtime::engine::meta*>(info);
    if (index >= f->outputs.size()) {
        LOG_MM_E(TAG, ENGINE_ERR_PARAM, "index", index);
        return AXCL_ERR_ENGINE_INVALID_INDEX;
    }

    *type = static_cast<axclrtEngineDataType>(f->outputs[index].format);
    return 0;
}

int32_t axcl::rt::engine::get_input_data_layout(axclrtEngineIOInfo info, const uint32_t index, axclrtEngineDataLayout *layout) {
    const auto f = static_cast<pkg::runtime::engine::meta*>(info);
    if (index >= f->inputs.size()) {
        LOG_MM_E(TAG, ENGINE_ERR_PARAM, "index", index);
        return AXCL_ERR_ENGINE_INVALID_INDEX;
    }

    *layout = static_cast<axclrtEngineDataLayout>(f->inputs[index].layout);
    return 0;
}

int32_t axcl::rt::engine::get_output_data_layout(axclrtEngineIOInfo info, const uint32_t index, axclrtEngineDataLayout *layout) {
    const auto f = static_cast<pkg::runtime::engine::meta*>(info);
    if (index >= f->outputs.size()) {
        LOG_MM_E(TAG, ENGINE_ERR_PARAM, "index", index);
        return AXCL_ERR_ENGINE_INVALID_INDEX;
    }

    *layout = static_cast<axclrtEngineDataLayout>(f->outputs[index].layout);
    return 0;
}

int32_t axcl::rt::engine::get_input_index(axclrtEngineIOInfo info, const char *name) {
    const auto f = static_cast<pkg::runtime::engine::meta*>(info);
    for (size_t i = 0; i < f->inputs.size(); i++) {
        if (f->inputs[i].name == name) {
            return static_cast<int32_t>(i);
        }
    }

    LOG_MM_E(TAG, ENGINE_ERR_PARAM, "name", name);
    return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
}

int32_t axcl::rt::engine::get_output_index(axclrtEngineIOInfo info, const char *name) {
    const auto f = static_cast<pkg::runtime::engine::meta*>(info);
    for (size_t i = 0; i < f->outputs.size(); i++) {
        if (f->outputs[i].name == name) {
            return static_cast<int32_t>(i);
        }
    }

    LOG_MM_E(TAG, ENGINE_ERR_PARAM, "name", name);
    return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
}

axclError axcl::rt::engine::get_input_dims(axclrtEngineIOInfo info, const uint32_t group, const uint32_t index, axclrtEngineIODims *dims) {
    const auto f = static_cast<pkg::runtime::engine::meta*>(info);
    if (index >= f->inputs.size()) {
        LOG_MM_E(TAG, ENGINE_ERR_PARAM, "index", index);
        return AXCL_ERR_ENGINE_INVALID_INDEX;
    }
    if (group >= f->inputs[index].groups.size()) {
        LOG_MM_E(TAG, ENGINE_ERR_PARAM, "group", group);
        return AXCL_ERR_ENGINE_INVALID_INDEX;
    }

    const auto& shape = f->inputs[index].groups[group].shape;
    dims->dimCount = static_cast<int32_t>(shape.size());
    for (int i = 0; i < dims->dimCount; i++) {
        dims->dims[i] = static_cast<int32_t>(shape[i]);
    }

    return AXCL_SUCC;
}

axclError axcl::rt::engine::get_output_dims(axclrtEngineIOInfo info, const uint32_t group, const uint32_t index, axclrtEngineIODims *dims) {
    if (nullptr == info || nullptr == dims) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "info or(and) dims");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }

    const auto f = static_cast<pkg::runtime::engine::meta*>(info);
    if (index >= f->outputs.size()) {
        LOG_MM_E(TAG, ENGINE_ERR_PARAM, "index", index);
        return AXCL_ERR_ENGINE_INVALID_INDEX;
    }
    if (group >= f->outputs[index].groups.size()) {
        LOG_MM_E(TAG, ENGINE_ERR_PARAM, "group", group);
        return AXCL_ERR_ENGINE_INVALID_INDEX;
    }

    const auto& shape = f->outputs[index].groups[group].shape;
    dims->dimCount = static_cast<int32_t>(shape.size());
    for (int i = 0; i < dims->dimCount; i++) {
        dims->dims[i] = static_cast<int32_t>(shape[i]);
    }

    return AXCL_SUCC;
}
