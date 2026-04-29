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
#include "res_guard.hpp"
#include "axcl_logger.hpp"

#define MAX_VER_LENGTH 256

AXCL_EXPORT const char* axclrtEngineGetModelCompilerVersion(const uint64_t modelId) {
    LOG_MM_D(TAG, "Start...");
    if (0 == modelId) {
        LOG_MM_E(TAG, ENGINE_ERR_MODEL_ID, modelId);
        return nullptr;
    }

    const axcl::rt::engine api;
    return api.get_model_version(modelId);
}

AXCL_EXPORT axclError axclrtEngineGetUsageFromMem(const void *model, const uint64_t modelSize, int64_t *sysSize, int64_t *cmmSize) {
    LOG_MM_D(TAG, "Start...");
    if (nullptr == model) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "model");
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }
    if (0 == modelSize) {
        LOG_MM_E(TAG, ENGINE_ERR_MODEL_SIZE, modelSize);
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }
    if (nullptr == sysSize) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "sysSize");
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }
    if (nullptr == cmmSize) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "cmmSize");
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }

    auto handle_holder = res_guard<uint64_t>(
        [&model, modelSize]() {
            uint64_t id = 0;
            axclrtEngineLoadFromMem(model, modelSize, &id);
            return id;
        },
        [](const uint64_t& id) { if (0 != id) axclrtEngineUnload(id); }
    );

    if (0 == handle_holder.get()) {
        LOG_MM_E(TAG, ENGINE_ERR_LOAD_FAIL);
        return AXCL_ERR_MEMORY_EXECUTE_FAIL;
    }

    return axclrtEngineGetUsageFromModelId(handle_holder.get(), sysSize, cmmSize);
}

AXCL_EXPORT axclError axclrtEngineGetUsageFromModelId(const uint64_t modelId, int64_t *sysSize, int64_t *cmmSize) {
    LOG_MM_D(TAG, "Start...");
    if (nullptr == sysSize) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "sysSize");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }
    if (nullptr == cmmSize) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "cmmSize");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }

    const axcl::rt::engine api;
    return api.get_usage(modelId, sysSize, cmmSize);
}

AXCL_EXPORT axclError axclrtEngineGetUsage(const char *modelPath, int64_t *sysSize, int64_t *cmmSize) {
    LOG_MM_D(TAG, "Start...");
    if (nullptr == modelPath) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "modelPath");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }
    if (nullptr == sysSize) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "sysSize");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }
    if (nullptr == cmmSize) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "cmmSize");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }

    auto handle_holder = res_guard<uint64_t>(
        [modelPath]() {
            uint64_t modelId = 0;
            axclrtEngineLoadFromFile(modelPath, &modelId);
            return modelId;
        },
        [](const uint64_t id) { if (0 != id) axclrtEngineUnload(id); }
    );

    if (0 == handle_holder.get()) {
        LOG_MM_E(TAG, ENGINE_ERR_LOAD_FILE_FAIL, modelPath);
        return AXCL_ERR_MEMORY_EXECUTE_FAIL;
    }

    return axclrtEngineGetUsageFromModelId(handle_holder.get(), sysSize, cmmSize);
}

AXCL_EXPORT axclError axclrtEngineGetModelType(const char *modelPath, axclrtEngineModelKind *modelType) {
    LOG_MM_D(TAG, "Start...");
    if (nullptr == modelPath) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "modelPath");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }
    if (nullptr == modelType) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "modelType");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }

    auto handle_holder = res_guard<uint64_t>(
        [modelPath]() {
            uint64_t modelId = 0;
            axclrtEngineLoadFromFile(modelPath, &modelId);
            return modelId;
        },
        [](const uint64_t id) { if (0 != id) axclrtEngineUnload(id); }
    );

    if (0 == handle_holder.get()) {
        LOG_MM_E(TAG, ENGINE_ERR_LOAD_FILE_FAIL, modelPath);
        return AXCL_ERR_MEMORY_EXECUTE_FAIL;
    }

    return axclrtEngineGetModelTypeFromModelId(handle_holder.get(), modelType);
}

AXCL_EXPORT axclError axclrtEngineGetModelTypeFromMem(const void *model, const uint64_t modelSize, axclrtEngineModelKind *modelType) {
    LOG_MM_D(TAG, "Start...");
    if (nullptr == model) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "model");
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }
    if (0 == modelSize) {
        LOG_MM_E(TAG, ENGINE_ERR_MODEL_SIZE, modelSize);
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }
    if (nullptr == modelType) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "modelType");
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }

    auto handle_holder = res_guard<uint64_t>(
        [&model, modelSize]() {
            uint64_t id = 0;
            axclrtEngineLoadFromMem(model, modelSize, &id);
            return id;
        },
        [](const uint64_t& id) { if (0 != id) axclrtEngineUnload(id); }
    );

    if (0 == handle_holder.get()) {
        LOG_MM_E(TAG, ENGINE_ERR_LOAD_FAIL);
        return AXCL_ERR_MEMORY_EXECUTE_FAIL;
    }

    return axclrtEngineGetModelTypeFromModelId(handle_holder.get(), modelType);
}

AXCL_EXPORT axclError axclrtEngineGetModelTypeFromModelId(const uint64_t modelId, axclrtEngineModelKind *modelType) {
    LOG_MM_D(TAG, "Start...");
    if (0 == modelId) {
        LOG_MM_E(TAG, ENGINE_ERR_MODEL_ID, modelId);
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }
    if (nullptr == modelType) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "modelType");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }

    const axcl::rt::engine api;
    return api.get_model_type(modelId, modelType);
}

const char *axcl::rt::engine::get_model_version(const uint64_t id) const {
    thread_local char ver_str[MAX_VER_LENGTH] = {};

    axclrtStream stream = nullptr;
    axclrtEnvironment env;
    axclError ret;
    if (ret = get_runtime_environment(&stream, env); AXCL_SUCC != ret) {
        return nullptr;
    }

    this->m_req.get_version(id);
    if (ret = invoke(stream, env, NO_TIMEOUT); AXCL_SUCC != ret) {
        return nullptr;
    }

    const auto ver = this->m_rsp.get_param_model_version();
    const auto ver_size = std::min(ver.size(), static_cast<size_t>(MAX_VER_LENGTH - 1));
    memcpy(ver_str, ver.data(), ver_size);
    ver_str[ver_size] = 0;

    return ver_str;
}

axclError axcl::rt::engine::get_usage(const uint64_t id, int64_t *sysSize, int64_t *cmmSize) const {
    axclrtStream stream = nullptr;
    axclrtEnvironment env;
    axclError ret;
    if (ret = get_runtime_environment(&stream, env); AXCL_SUCC != ret) {
        return ret;
    }

    this->m_req.get_usage(id);
    if (ret = invoke(stream, env, NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    *sysSize = static_cast<int64_t>(this->m_rsp.get_param_sys_usage());
    *cmmSize = static_cast<int64_t>(this->m_rsp.get_param_cmm_usage());
    return AXCL_SUCC;
}

axclError axcl::rt::engine::get_model_type(const uint64_t id, axclrtEngineModelKind *type) const {
    axclrtStream stream = nullptr;
    axclrtEnvironment env;
    axclError ret;
    if (ret = get_runtime_environment(&stream, env); AXCL_SUCC != ret) {
        return ret;
    }

    this->m_req.get_type(id);
    if (ret = invoke(stream, env, NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    *type = static_cast<axclrtEngineModelKind>(this->m_rsp.get_param_model_type());
    return AXCL_SUCC;
}
