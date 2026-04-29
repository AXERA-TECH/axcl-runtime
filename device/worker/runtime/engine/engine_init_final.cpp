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

RUNTIME_ENGINE_API_IMPL(INIT) {
    LOG_MM_D(TAG, "axclrt engine init npu{}...", static_cast<int>(response->get_param_npu_type()));

    AX_ENGINE_NPU_ATTR_T attr = {};
    attr.eHardMode = static_cast<AX_ENGINE_NPU_MODE_T>(response->get_param_npu_type());
    const auto ret = AX_ENGINE_Init(&attr);
    if (ret != 0) {
        LOG_MM_E(TAG, "axclrt engine init failed.");
    }
    LOG_MM_D(TAG, "axclrt engine inited.");

    request->set_status(ret);
    return ret;
}

RUNTIME_ENGINE_API_IMPL(FINAL) {
    LOG_MM_D(TAG, "axclrt engine final...");

    const auto ret = AX_ENGINE_Deinit();
    request->set_status(ret);
    return ret;
}
