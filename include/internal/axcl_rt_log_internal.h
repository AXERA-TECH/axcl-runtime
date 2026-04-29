/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#ifndef __AXCL_RT_LOGDUMP_INTERNAL_H__
#define __AXCL_RT_LOGDUMP_INTERNAL_H__

#include "axcl_rt_context.h"
#include "axcl_rt_type_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

#define AXCL_DEF_LOG_ERR(e)              AXCL_DEF_RUNTIME_ERR(AXCL_RUNTIME_SYSLOG, (e))
#define AXCL_ERR_LOG_DUMP                AXCL_DEF_LOG_ERR(0x81)

// kind: -1: all, bit mask: 0x01: daemon;  0x02: worker;  0x10: syslog;  0x20: kernel
// flag: reserved
// output_path: NULL: current working directory; others: specified directory
AXCL_EXPORT axclError axclrtLog(axclrtLogKind kind, axclrtFlagKind flag, const char* output_path);

#ifdef __cplusplus
}
#endif

#endif /* __AXCL_RT_LOGDUMP_INTERNAL_H__ */
