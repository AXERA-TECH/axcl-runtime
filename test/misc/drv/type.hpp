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

#include <sys/ioctl.h>
#include <cstddef>
#include "ax_base_type.h"

#define MAX_DEV_NUMBER 0x100

enum cmd_type {
    AXCL_PORT_CREATE = 1,
    AXCL_PORT_DESTROY = 2,
    AXCL_PORT_CREATE_COMPLETION = 3,
    AXCL_PORT_DESTROY_COMPLETION = 4,
    AXCL_PORT_CREATE_FAIL = 5,
    AXCL_PORT_DESTROY_FAIL = 6,
};

struct device_info_t {
    unsigned int cmd;
    unsigned int device;
    unsigned int pid;
    unsigned int port_num;
    unsigned int ports[2];
};

struct device_connect_status_t {
    unsigned int status[MAX_DEV_NUMBER];
};

struct device_heart_beat_t {
    unsigned int device;          /* EP target id (local id) */
    unsigned int interval;        /* EP heartbeat interval, unit: ms */
    unsigned long long timestamp; /* EP heartbeat timestamp */
};

struct device_list_t {
    unsigned int type; /* 0: pcie */
    unsigned int num; /* device connected num */
    unsigned int devices[MAX_DEV_NUMBER];  /* ep target id */
};


#define HOST_DEV_PATH "/dev/axcl_host"
#define DEVICE_DEV_PATH "/dev/axcl_device"

#define IOC_AXCL_MAGIC 'A'

#define IOC_AXCL_PORT_MANAGE _IOWR(IOC_AXCL_MAGIC, 1, struct device_info_t)
#define IOC_AXCL_CONN_STATUS _IOR(IOC_AXCL_MAGIC, 2, struct device_connect_status_t)
#define IOC_AXCL_HEART_BEATS _IOW(IOC_AXCL_MAGIC, 3, struct device_heart_beat_t)
#define IOC_AXCL_DEVICE_LIST _IOWR(IOC_AXCL_MAGIC, 4, struct device_list_t)

#define HEART_BEAT_INTEVAL (5000)

// #define HANDSHAKE