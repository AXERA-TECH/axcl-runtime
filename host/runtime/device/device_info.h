/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/
#ifndef __DEVICE_INFO_H_9F68C88D_29C6_4247_AD11_6ECB5004FB7E__
#define __DEVICE_INFO_H_9F68C88D_29C6_4247_AD11_6ECB5004FB7E__

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_DEVICE_NUM (0x100)
#define MAX_PORT_NUM (0x100)
#define MAX_PID_NUM (64)

#define DEVICE_OFFLINE (0)
#define PCIE_DEVICE (0)

enum device_cmd {
    AXCL_PORT_CREATE = 1,
    AXCL_PORT_DESTROY = 2,
    AXCL_PORT_CREATE_COMPLETION = 3,
    AXCL_PORT_DESTROY_COMPLETION = 4,
    AXCL_PORT_CREATE_FAIL = 5,
    AXCL_PORT_DESTROY_FAIL = 6,
    AXCL_PORT_REQUEST_USR_PORT = 7,
    AXCL_PORT_RECYCLE_USR_PORT = 8,
};

struct device_info {
    unsigned int cmd; /* device cmd */
    unsigned int device;
    unsigned int pid;
    unsigned int port_num;
    unsigned int ports[4];
    unsigned int dma_buf_size; /* pcie dma buf size */
    unsigned int log_level;    /* device log level */
};

struct device_connect_status {
    unsigned int status[256]; /* DEVICE_OFFLINE(0): offline */
};

struct device_heart_beat {
    unsigned int device;          /* EP target id (local id) */
    unsigned int interval;        /* EP heartbeat interval, unit: ms */
    unsigned int timestamp; /* EP heartbeat timestamp */
    unsigned int last_timestamp;
};

struct device_list {
    unsigned int type; /* PCIE_DEVICE(0): pcie */
    unsigned int num;
    unsigned int devices[MAX_DEVICE_NUM];
};

struct bus_info_t {
	unsigned int device; /* bus */
	unsigned int domain;
	unsigned int slot;
	unsigned int func;
};

struct pid_num_t {
	unsigned int device;
	unsigned int num;
};

struct pid_info_t {
	unsigned int device;
	unsigned int num;
	unsigned long long pid;  /* pid = malloc(sizeof(unsigned int) * num) */
};

#ifdef __cplusplus
}
#endif

#endif /* __DEVICE_INFO_H_9F68C88D_29C6_4247_AD11_6ECB5004FB7E__ */