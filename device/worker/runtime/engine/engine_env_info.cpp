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

RUNTIME_ENGINE_API_IMPL(GET_NPU_TYPE) {
    LOG_MM_D(TAG, "axclrt engine get visual npu type...");

    AX_ENGINE_NPU_ATTR_T attr = {};
    if (const auto ret = AX_ENGINE_GetVNPUAttr(&attr); ret != 0) {
        LOG_MM_E(TAG, "axclrt engine get visual npu type failed{{{}}}.", ret);
        request->set_status(ret);
        return ret;
    }

    request->set_param_npu_type(static_cast<axcl::pkg::runtime::engine::NPU>(attr.eHardMode));
    request->set_status(0);
    LOG_MM_D(TAG, "axclrt engine visual npu type was got.");
    return 0;
}

RUNTIME_ENGINE_API_IMPL(GET_ENGINE_VERSION) {
    LOG_MM_D(TAG, "axclrt engine get engine version...");

    const auto version = AX_ENGINE_GetVersion();
    if (version == nullptr) {
        LOG_MM_E(TAG, "axclrt engine get version failed.");
        request->set_status(-1);
        return -1;
    }

    request->set_param_engine_version(version);
    request->set_status(0);
    LOG_MM_D(TAG, "axclrt engine version was got.");
    return 0;
}
