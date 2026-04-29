/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "axcl_rt_config.h"
#include "config/config.hpp"

static axcl::config g_config;

AXCL_EXPORT axclError axclrtConfigLoad(const char *json) {
    if (!g_config.load(json)) {
        return AXCL_ERR_CONFIG_LOAD;
    }

    return AXCL_SUCC;
}

AXCL_EXPORT void axclrtConfigUnLoad() {
    g_config.unload();
}

AXCL_EXPORT const char *axclrtConfigGetLogPath() {
    return g_config.get_log_path();
}

AXCL_EXPORT int32_t axclrtConfigGetLogLevel(int32_t device) {
    return g_config.get_log_level(device);
}

AXCL_EXPORT uint32_t axclrtConfigGetDmaBufSize() {
    return g_config.get_dma_buf_size();
}
