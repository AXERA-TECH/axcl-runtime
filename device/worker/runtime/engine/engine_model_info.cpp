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
#include "axcl_logger.hpp"

#include <ax_engine_api.h>

#define TAG "engine"

RUNTIME_ENGINE_API_IMPL(GET_VERSION) {
    LOG_MM_D(TAG, "axclrt engine get model version...");

    std::lock_guard lock(this->m_mutex);
    const auto handle = response->get_param_handle();
    if (0 == this->m_map.count(handle)) {
        request->set_status(-1);
        return -1;
    }

    const auto version = ::AX_ENGINE_GetModelToolsVersion(reinterpret_cast<void *>(handle));
    if (nullptr == version) {
        request->set_status(-1);
        return -1;
    }

    request->set_param_model_version(version);
    request->set_status(0);
    return 0;
}

RUNTIME_ENGINE_API_IMPL(GET_TYPE) {
    LOG_MM_D(TAG, "axclrt engine get type...");
    AX_ENGINE_MODEL_TYPE_T type{};

    std::lock_guard lock(this->m_mutex);
    const auto handle = response->get_param_handle();
    if (0 == this->m_map.count(handle)) {
        request->set_status(-1);
        return -1;
    }

    if (const auto ret = ::AX_ENGINE_GetHandleModelType(reinterpret_cast<void *>(handle), &type); 0 != ret) {
        request->set_status(ret);
        return ret;
    }

    request->set_param_model_type(static_cast<pkg::runtime::engine::MODEL>(type));
    request->set_status(0);
    return 0;
}

RUNTIME_ENGINE_API_IMPL(GET_USAGE) {
    LOG_MM_D(TAG, "axclrt engine get usage...");

    AX_ENGINE_CMM_INFO info{};
    std::lock_guard lock(this->m_mutex);
    const auto handle = response->get_param_handle();
    if (0 == this->m_map.count(handle)) {
        request->set_status(-1);
        return -1;
    }

    if (const auto ret = ::AX_ENGINE_GetCMMUsage(reinterpret_cast<void *>(handle), &info); 0 != ret) {
        request->set_status(ret);
        return ret;
    }

    request->set_param_model_usage(0, info.nCMMSize);
    request->set_status(0);
    return 0;
}

RUNTIME_ENGINE_API_IMPL(GET_INFO) {
    LOG_MM_D(TAG, "axclrt engine get info...");

    std::lock_guard lock(this->m_mutex);
    const auto handle = response->get_param_handle();
    if (0 == this->m_map.count(handle)) {
        request->set_status(-1);
        return -1;
    }

    request->set_param_meta(this->m_map.at(handle));
    request->set_status(0);
    return 0;
}
