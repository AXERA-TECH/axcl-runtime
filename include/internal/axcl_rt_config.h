
/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/
#ifndef __AXCL_RT_CONFIG_H__
#define __AXCL_RT_CONFIG_H__

#include "axcl_base.h"

#ifdef __cplusplus
extern "C"
{
#endif

    // clang-format off
#define AXCL_DEF_CONFIG_ERR(e)      AXCL_DEF_RUNTIME_ERR(AXCL_RUNTIME_CONFIG, (e))
#define AXCL_ERR_CONFIG_LOAD        AXCL_DEF_CONFIG_ERR(0x81)

AXCL_EXPORT axclError   axclrtConfigLoad(const char *json);
AXCL_EXPORT void        axclrtConfigUnLoad();

AXCL_EXPORT const char *axclrtConfigGetLogPath();
AXCL_EXPORT int32_t     axclrtConfigGetLogLevel(int32_t device);
AXCL_EXPORT uint32_t    axclrtConfigGetDmaBufSize();
    // clang-format on

#ifdef __cplusplus
}
#endif

#endif /* __AXCL_RT_CONFIG_H__ */