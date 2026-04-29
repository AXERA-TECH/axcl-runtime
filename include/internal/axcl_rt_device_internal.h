/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#ifndef __AXCL_RT_DEVICE_INTERNAL_H__
#define __AXCL_RT_DEVICE_INTERNAL_H__

#include "axcl_rt_device.h"
#include "axcl_rt_type_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

AXCL_EXPORT axclError axclrtGetDeviceBusInfo(int32_t deviceId, axclrtDeviceBusInfo *busInfo);
AXCL_EXPORT axclError axclrtGetDevicePidInfo(int32_t deviceId, axclrtDevicePidInfo *pidInfo);
AXCL_EXPORT axclError axclrtRebootDevice(int32_t deviceId);

#ifdef __cplusplus
}
#endif

#endif /* __AXCL_RT_DEVICE_INTERNAL_H__ */
