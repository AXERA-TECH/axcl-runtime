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

RUNTIME_ENGINE_API_IMPL(CREATE_CONTEXT) {
    LOG_MM_D(TAG, "axclrt engine create context...");

    std::lock_guard lock(this->m_mutex);
    const auto handle = response->get_param_handle();
    if (0 == this->m_map.count(handle)) {
        request->set_status(-1);
        return -1;
    }

    AX_ENGINE_CONTEXT_T context = {};
    const auto ret = ::AX_ENGINE_CreateContextV2(reinterpret_cast<void *>(handle), &context);

    request->set_param_context(reinterpret_cast<uint64_t>(context));
    request->set_status(ret);
    return ret;
}
