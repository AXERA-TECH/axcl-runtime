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

AXCL_EXPORT axclError axclrtEngineExecute(uint64_t modelId, const uint64_t contextId, const uint32_t group, axclrtEngineIO io) {
    LOG_MM_D(TAG, "Start...");
    if (0 == modelId) {
        LOG_MM_E(TAG, ENGINE_ERR_MODEL_ID, modelId);
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }
    if (0 == contextId) {
        LOG_MM_E(TAG, ENGINE_ERR_CONTEXT_ID, contextId);
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }
    if (nullptr == io) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "io");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }

    const axcl::rt::engine api;
    return api.execute(modelId, contextId, group, io, nullptr);
}

AXCL_EXPORT axclError axclrtEngineExecuteAsync(uint64_t modelId, uint64_t contextId, uint32_t group, axclrtEngineIO io, axclrtStream stream) {
    LOG_MM_D(TAG, "Start...");
    if (0 == modelId) {
        LOG_MM_E(TAG, ENGINE_ERR_MODEL_ID, modelId);
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }

    if (0 == contextId) {
        LOG_MM_E(TAG, ENGINE_ERR_CONTEXT_ID, contextId);
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }

    if (nullptr == io) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "io");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }

    const axcl::rt::engine api;
    return api.execute(modelId, contextId, group, io, stream, false);
}

axclError axcl::rt::engine::execute(const uint64_t id, const uint64_t context, const uint32_t group, axclrtEngineIO io, axclrtStream axclStream, bool has_resp) const {
    axclrtStream stream = axclStream;
    axclrtEnvironment env;

    axclError ret = get_runtime_environment(&stream, env);
    if (AXCL_SUCC != ret) {
        return ret;
    }

    const auto io_ = static_cast<pkg::runtime::engine::desc*>(io);

    LOG_MM_D(TAG, "io batch: {}, input: {}, output: {}.", io_->batch, io_->inputs.size(), io_->outputs.size());

    this->m_req.run(id, context, group, *io_);
    ret = invoke(stream, env, NO_TIMEOUT, has_resp);
    if (AXCL_SUCC != ret) {
        return ret;
    }

    return AXCL_SUCC;
}
