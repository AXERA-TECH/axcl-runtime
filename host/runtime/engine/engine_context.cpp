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

AXCL_EXPORT axclError axclrtEngineCreateContext(const uint64_t modelId, uint64_t *contextId) {
    LOG_MM_D(TAG, "Start...");
    if (0 == modelId) {
        LOG_MM_E(TAG, ENGINE_ERR_MODEL_ID, modelId);
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }
    if (nullptr == contextId) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "contextId");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }

    const axcl::rt::engine api;
    return api.create_context(modelId, contextId);
}

axclError axcl::rt::engine::create_context(const uint64_t id, uint64_t *context) const {
    axclrtStream stream = nullptr;
    axclrtEnvironment env;
    axclError ret;
    if (ret = get_runtime_environment(&stream, env); AXCL_SUCC != ret) {
        return ret;
    }

    this->m_req.create_context(id);
    if (ret = invoke(stream, env, NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    *context = this->m_rsp.get_param_context();
    return AXCL_SUCC;
}
