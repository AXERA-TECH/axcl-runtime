/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#ifndef __AXCL_RT_LATENCY_INTERNAL_H__
#define __AXCL_RT_LATENCY_INTERNAL_H__

#include "axcl_rt_context.h"
#include "axcl_rt_type_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

#define AXCL_DEF_LATENCY_ERR(e)             AXCL_DEF_RUNTIME_ERR(AXCL_RUNTIME_LATENCY, (e))
#define AXCL_ERR_LATENCY_NULL_POINTER       AXCL_DEF_SYSCTRL_ERR(AXCL_ERR_NULL_POINTER)
#define AXCL_ERR_LATENCY_ENCODE             AXCL_DEF_SYSCTRL_ERR(AXCL_ERR_ENCODE)
#define AXCL_ERR_LATENCY_DECODE             AXCL_DEF_SYSCTRL_ERR(AXCL_ERR_DECODE)
#define AXCL_ERR_LATENCY_VERIFY_FAIL        AXCL_DEF_SYSCTRL_ERR(0x81)

AXCL_EXPORT axclError axclrtLatencyLoopback(const void *data, uint32_t size, bool check, int comm_mode);

#ifdef __cplusplus
}
#endif

#endif /* __AXCL_RT_LATENCY_INTERNAL_H__ */
