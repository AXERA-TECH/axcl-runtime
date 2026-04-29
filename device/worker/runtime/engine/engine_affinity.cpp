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

RUNTIME_ENGINE_API_IMPL(SET_AFFINITY) {
    LOG_MM_D(TAG, "axclrt engine set affinity...");

    std::lock_guard lock(this->m_mutex);
    const auto handle = response->get_param_handle();
    if (0 == this->m_map.count(handle)) {
        request->set_status(-1);
        return -1;
    }

    const auto set = response->get_param_affinity();
    const auto ret = ::AX_ENGINE_SetAffinity(reinterpret_cast<void *>(handle), set);

    request->set_status(ret);
    return ret;
}

RUNTIME_ENGINE_API_IMPL(GET_AFFINITY) {
    LOG_MM_D(TAG, "axclrt engine get affinity...");

    std::lock_guard lock(this->m_mutex);
    const auto handle = response->get_param_handle();
    if (0 == this->m_map.count(handle)) {
        request->set_status(-1);
        return -1;
    }

    AX_ENGINE_NPU_SET_T set{};
    const auto ret = ::AX_ENGINE_GetAffinity(reinterpret_cast<void *>(handle), &set);

    request->set_param_affinity(set);
    request->set_status(ret);
    return ret;
}
