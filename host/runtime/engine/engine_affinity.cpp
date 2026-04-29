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

AXCL_EXPORT axclError axclrtEngineSetAffinity(const uint64_t modelId, const axclrtEngineSet set) {
    LOG_MM_D(TAG, "Start...");
    if (0 == modelId) {
        LOG_MM_E(TAG, ENGINE_ERR_MODEL_ID, modelId);
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }
    if (0 == set || 0b111 < set) {
        LOG_MM_E(TAG, "invalid affinity set");
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }

    const axcl::rt::engine api;
    return api.set_affinity(modelId, set);
}

AXCL_EXPORT axclError axclrtEngineGetAffinity(const uint64_t modelId, axclrtEngineSet *set) {
    LOG_MM_D(TAG, "Start...");
    if (0 == modelId) {
        LOG_MM_E(TAG, ENGINE_ERR_MODEL_ID, modelId);
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }
    if (nullptr == set) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "set");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }

    const axcl::rt::engine api;
    return api.get_affinity(modelId, set);
}

AXCL_EXPORT axclError axclrtEngineSetContextAffinity(const uint64_t modelId, uint64_t contextId, const axclrtEngineSet set) {
    LOG_MM_D(TAG, "Start...");
    if (0 == modelId) {
        LOG_MM_E(TAG, ENGINE_ERR_MODEL_ID, modelId);
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }
    if (0 == contextId) {
        LOG_MM_E(TAG, ENGINE_ERR_CONTEXT_ID, contextId);
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }
    if (0 == set || 0b111 < set) {
        LOG_MM_E(TAG, "invalid affinity set");
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }

    return AXCL_ERR_ENGINE_AXCL_UNSUPPORTED;
}

AXCL_EXPORT axclError axclrtEngineGetContextAffinity(const uint64_t modelId, uint64_t contextId, axclrtEngineSet *set) {
    LOG_MM_D(TAG, "Start...");
    if (0 == modelId) {
        LOG_MM_E(TAG, ENGINE_ERR_MODEL_ID, modelId);
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }
    if (0 == contextId) {
        LOG_MM_E(TAG, ENGINE_ERR_CONTEXT_ID, contextId);
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }
    if (nullptr == set) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "set");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }

    return AXCL_ERR_ENGINE_AXCL_UNSUPPORTED;
}

axclError axcl::rt::engine::set_affinity(const uint64_t id, const axclrtEngineSet set) const {
    axclrtStream stream = nullptr;
    axclrtEnvironment env;
    axclError ret;
    if (ret = get_runtime_environment(&stream, env); AXCL_SUCC != ret) {
        return ret;
    }

    this->m_req.set_affinity(id, set);
    if (ret = invoke(stream, env, NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    return AXCL_SUCC;
}

axclError axcl::rt::engine::get_affinity(const uint64_t id, axclrtEngineSet *set) const {
    axclrtStream stream = nullptr;
    axclrtEnvironment env;
    axclError ret;
    if (ret = get_runtime_environment(&stream, env); AXCL_SUCC != ret) {
        return ret;
    }

    this->m_req.get_affinity(id);
    if (ret = invoke(stream, env, NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    *set = this->m_rsp.get_param_affinity();
    return AXCL_SUCC;
}
