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

AXCL_EXPORT axclError axclrtEngineInit(const axclrtEngineVNpuKind npuKind) {
    LOG_MM_D(TAG, "Start...");
    const axcl::rt::engine api;
    return api.init(npuKind);
}

AXCL_EXPORT axclError axclrtEngineFinalize() {
    LOG_MM_D(TAG, "Start...");
    const axcl::rt::engine api;
    return api.finalize();
}

AXCL_EXPORT axclError axclrtEngineGetVNpuKind(axclrtEngineVNpuKind *npuKind) {
    LOG_MM_D(TAG, "Start...");
    if (nullptr == npuKind) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "npuKind");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }

    const axcl::rt::engine api;
    return api.get_vnpu_kind(npuKind);
}

axclError axcl::rt::engine::init(axclrtEngineVNpuKind kind) const {
    axclrtStream stream = nullptr;
    axclrtEnvironment env;
    axclError ret;
    if (ret = get_runtime_environment(&stream, env); AXCL_SUCC != ret) {
        return ret;
    }

    this->m_req.init_npu(static_cast<pkg::runtime::engine::NPU>(kind));
    if (ret = invoke(stream, env, NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    return AXCL_SUCC;
}

axclError axcl::rt::engine::get_vnpu_kind(axclrtEngineVNpuKind *kind) const {
    axclrtStream stream = nullptr;
    axclrtEnvironment env;
    axclError ret;
    if (ret = get_runtime_environment(&stream, env); AXCL_SUCC != ret) {
        return ret;
    }

    this->m_req.get_npu_type();
    if (ret = invoke(stream, env, NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    *kind = static_cast<axclrtEngineVNpuKind>(this->m_rsp.get_param_npu_type());
    return AXCL_SUCC;
}

axclError axcl::rt::engine::finalize() const {
    axclrtStream stream = nullptr;
    axclrtEnvironment env;
    axclError ret;
    if (ret = get_runtime_environment(&stream, env); AXCL_SUCC != ret) {
        return ret;
    }

    this->m_req.final_npu();
    if (ret = invoke(stream, env, NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    return AXCL_SUCC;
}
