/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstddef>
#include <thread>
#include "../type.hpp"
#include "axcl_pcie_msg_api.h"

static int fd = -1;
static int device = -1;
static int running = 1;
static AX_S32 get_device_id();
static void listen();

static void SigInt(int sigNo) {
    printf("catch signal %d\n", sigNo);
    running = 0;
}

int main(int argc, char const* argv[]) {
    signal(SIGINT, SigInt);

    device = get_device_id();
    if (device < 0) {
        return 1;
    }

    fd = open(HOST_DEV_PATH, O_RDWR);
    if (fd < 0) {
        printf("open %s fail\n", HOST_DEV_PATH);
        return 1;
    }

    int ret;
    struct device_list_t dev_list = {0};
    ret = ioctl(fd, IOC_AXCL_DEVICE_LIST, &dev_list);
    if (ret < 0) {
        printf("ioctl(IOC_AXCL_DEVICE_LIST) fail, error = 0x%x\n", errno);
        close(fd);
        return 1;
    } else {
        printf("device type: %d, num: %d\n", dev_list.type, dev_list.num);
        for (unsigned int i = 0; i < dev_list.num; ++i) {
            printf("\tdevice id = %u\n", dev_list.devices[i]);
        }
    }

    struct device_info_t dev_info = {0};
    dev_info.cmd = AXCL_PORT_CREATE;
    dev_info.device = device;
    dev_info.pid = getpid();

    printf("start to request ports for pid %d ...\n", dev_info.pid);
    ret = ioctl(fd, IOC_AXCL_PORT_MANAGE, &dev_info);
    if (ret < 0) {
        printf("ioctl(IOC_AXCL_PORT_MANAGE) fail, error = 0x%x\n", errno);
        close(fd);
        return 1;
    }

    if (dev_info.port_num <= 0) {
        printf("no ports is allocated\n");
        close(fd);
        return 1;
    }

    printf("allocated ports num: %d\n", dev_info.port_num);
    for (unsigned int i = 0; i < dev_info.port_num; ++i) {
        printf("\tport %d = %d\n", i + 1, dev_info.ports[i]);
    }

    std::thread t(listen);

    while(running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    t.join();

    close(fd);
    return 0;
}

static AX_S32 get_device_id() {
    AX_S32 ret;

    AX_S32 local = -1;
    ret = AX_PCIe_GetLocalId(&local);
    if (ret < 0) {
        printf("AX_PCIe_GetLocalId fail, ret = 0x%x\n", ret);
        return -1;
    }

    AX_S32 cnt = 0;
    AX_S32 targets[AX_MAX_MAP_DEV];
    ret = AX_PCIe_GetTargetId(targets, &cnt);
    if (ret < 0) {
        printf("AX_PCIe_GetTargetId fail, ret = 0x%x\n", ret);
        return -1;
    }

    if (cnt <= 0) {
        printf("no ep connected\n");
        return -1;
    }

    int target = targets[0];

#ifdef HANDSHAKE
    printf("handshake with EP %d +++\n", target);
    ret = AX_PCIe_ShareMemInit(target);
    printf("handshake with EP %d ---\n", target);
    if (ret < 0) {
        printf("AX_PCIe_ShareMemInit(%d) fail, ret = 0x%x\n", target, ret);
        return -1;
    }
#endif

    return target;
}

static void listen() {
    const int fd_max = fd + 1;
    fd_set set;

    while (running) {
        /* should set by every time */
        FD_ZERO(&set);
        FD_SET(fd, &set);

        int ret = select(fd_max, &set, NULL, NULL, NULL /* infinite */);
        if (ret < 0) {
            perror("select");
        } else if (0 == ret) {
            printf("select timeout\n");
        } else {
            if (FD_ISSET(fd, &set)) {
                struct device_connect_status_t status = {0};
                ret = ioctl(fd, IOC_AXCL_CONN_STATUS, &status);
                printf("==> device %d status %d - %s\n", device, status.status[device], (0 == status.status[device]) ? "dead" : "alive");

                if (0 == status.status[device]) {
                    /* current device is dead, quit listen thread */
                    continue;
                }
            }
        }
    }
}
