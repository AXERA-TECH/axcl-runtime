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
#include "axcl_rt_engine_internal.h"

#include "engine_strings.hpp"
#include "axcl_logger.hpp"

AXCL_EXPORT axclError axclrtEngineP2pBwTest(axclrtEngineP2pBwRes* bw, const uint32_t mode, const uint64_t size) {
    LOG_MM_D(TAG, "Start...");
    if (nullptr == bw) {
        LOG_MM_E(TAG, ENGINE_ERR_PARAM, "bw");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }
    if (0 == mode) {
        LOG_MM_E(TAG, ENGINE_ERR_PARAM, "mode");
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }
    if (0 == size) {
        LOG_MM_E(TAG, ENGINE_ERR_PARAM, "size");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }
    LOG_MM_D(TAG, "Start P2P bandwidth test, mode: {}, size: {}", mode, size);

    const axcl::rt::engine api;
    std::vector<float> elapsed;
    const auto ret = api.start_p2p(elapsed, mode, size);
    if (AXCL_SUCC != ret) {
        bw->count = 0;
        LOG_MM_E(TAG, "Failed to run P2P bandwidth test, ret: {:#x}", ret);
    } else {
        const auto count = std::min(static_cast<uint32_t>(elapsed.size()), MAX_P2P_TEST_COUNT);
        bw->count = count;
        for (uint32_t i = 0; i < count; ++i) {
            bw->elapsed[i] = elapsed[i];
        }
        for (uint32_t i = count; i < MAX_P2P_TEST_COUNT; ++i) {
            bw->elapsed[i] = 0.0f;
        }
    }
    LOG_MM_D(TAG, "Finish P2P bandwidth test.");
    return ret;
}

axclError axcl::rt::engine::start_p2p(std::vector<float>& elapsed, const uint32_t mode, const uint64_t size) const {
    LOG_MM_D(TAG, "axclrt engine p2p...");
    axclrtStream stream = nullptr;
    axclrtEnvironment env;
    axclError ret;
    if (ret = get_runtime_environment(&stream, env); AXCL_SUCC != ret) {
        return ret;
    }

    this->m_req.start_p2p(mode, size);
    if (ret = invoke(stream, env, NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }
    elapsed = this->m_rsp.get_param_elapsed();
    return AXCL_SUCC;
}
