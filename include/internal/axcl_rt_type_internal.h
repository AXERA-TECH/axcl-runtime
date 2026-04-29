/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#ifndef __AXCL_RT_TYPE_INTERNAL_H__
#define __AXCL_RT_TYPE_INTERNAL_H__

#include <cstdint>

typedef struct {
    int32_t device;
    int32_t context;
    int32_t stream;
} axclrtEnvironment;

typedef struct {
    unsigned int domain;
    unsigned int bus;
    unsigned int slot;
    unsigned int func;
} axclrtDeviceBusInfo;

typedef struct {
    unsigned int device;
    unsigned int num;
    unsigned int *pid;
} axclrtDevicePidInfo;

typedef enum axclrtCtrlKind {
    AXCL_RT_CTRL_CREATE_CONTEXT,
    AXCL_RT_CTRL_DESTROY_CONTEXT
} axclrtCtrlKind;

typedef uint32_t axclrtLogKind;

typedef uint32_t axclrtFlagKind;

#endif /* __AXCL_RT_TYPE_INTERNAL_H__ */
