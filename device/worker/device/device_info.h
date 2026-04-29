/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/
#ifndef __DEVICE_INFO_H__
#define __DEVICE_INFO_H__

#include <sys/ioctl.h>
#include "ax_base_type.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DEV_HEARTBEATS_INTERVAL (10000)

#define IOC_AXCL_MAGIC 'A'
#define IOC_AXCL_PORT_MANAGE _IOWR(IOC_AXCL_MAGIC, 1, struct device_info)
#define IOC_AXCL_CONN_STATUS _IOR(IOC_AXCL_MAGIC, 2, struct device_connect_status)
#define IOC_AXCL_HEART_BEATS _IOW(IOC_AXCL_MAGIC, 3, struct device_heart_beat)

enum device_cmd {
    AXCL_PORT_CREATE = 1,
    AXCL_PORT_DESTROY = 2,
    AXCL_PORT_CREATE_COMPLETION = 3,
    AXCL_PORT_DESTROY_COMPLETION = 4,
    AXCL_PORT_CREATE_FAIL = 5,
    AXCL_PORT_DESTROY_FAIL = 6,
};

struct device_info {
    /* IN:  CMD_PORT_ALLOC: allocate ports; CMD_PORT_DESTORY: detroy ports  */
    unsigned int cmd;
    unsigned int device;
    unsigned int pid;
    unsigned int port_num;
    unsigned int ports[4];
    unsigned int dma_buf_size; /* pcie dma buf size */
    unsigned int log_level;    /* device log level */
};

struct device_connect_status {
    unsigned int status[256];
};

struct device_heart_beat {
    unsigned int device;          /* EP target id (local id) */
    unsigned int interval;        /* EP heartbeat interval, unit: ms */
    unsigned long long timestamp; /* EP heartbeat timestamp */
};

#ifdef __cplusplus
}
#endif

#endif /* __DEVICE_INFO_H__ */
