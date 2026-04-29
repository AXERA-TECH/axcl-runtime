/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#pragma once
#include "axcl_logger.hpp"

#ifdef __cplusplus
extern "C"
{
#endif

#define SKEL_LOG_TAG "SKEL"

#define ALOGE(fmt, ...) LOG_MM_E(SKEL_LOG_TAG, fmt, ##__VA_ARGS__)
#define ALOGW(fmt, ...) LOG_MM_W(SKEL_LOG_TAG, fmt, ##__VA_ARGS__)
#define ALOGI(fmt, ...) //LOG_MM_I(SKEL_LOG_TAG, fmt, ##__VA_ARGS__)
#define ALOGD(fmt, ...) //LOG_MM_D(SKEL_LOG_TAG, fmt, ##__VA_ARGS__)
#define ALOGN(fmt, ...) //LOG_MM_W(SKEL_LOG_TAG, fmt, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif
