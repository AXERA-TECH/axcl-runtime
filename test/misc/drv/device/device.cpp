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
#include <chrono>
#include <cstddef>
#include <cstring>
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

    fd = open(DEVICE_DEV_PATH, O_RDWR);
    if (fd < 0) {
        printf("open %s fail\n", DEVICE_DEV_PATH);
        return 1;
    }

    std::thread t(listen);

    struct device_heart_beat_t hb = {0};
    hb.device = 0;
    hb.interval = HEART_BEAT_INTEVAL;
    while (running) {
        // hb.timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        ++hb.timestamp;
        int ret = ioctl(fd, IOC_AXCL_HEART_BEATS, &hb);
        if (ret < 0) {
            printf("ioctl(IOC_AXCL_HEART_BEATS) fail, error = 0x%x\n", errno);
        }

        /* wait */
        std::this_thread::sleep_for(std::chrono::milliseconds(HEART_BEAT_INTEVAL - 200));
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

#ifdef HANDSHAKE
    printf("handshake with RC +++\n");
    ret = AX_PCIe_ShareMemInit(0);
    printf("handshake with RC ---\n");
    if (ret < 0) {
        printf("AX_PCIe_ShareMemInit(0) fail, ret = 0x%x\n", ret);
        return -1;
    }
#endif

    return local;
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
                struct device_info_t dev_info = {0};
                ret = ioctl(fd, IOC_AXCL_PORT_MANAGE, &dev_info);
                if (ret < 0) {
                    printf("ioctl(IOC_AXCL_PORT_MANAGE) fail, error = 0x%x\n", errno);
                    continue;
                }

                if (AXCL_PORT_CREATE == dev_info.cmd) {
                    printf("receive port create cmd: \n");
                    printf("\tdevice = %d, host pid = %d\n", dev_info.device, dev_info.pid);
                    printf("\tports num: %d\n", dev_info.port_num);
                    for (unsigned int i = 0; i < dev_info.port_num; ++i) {
                        printf("\t\tport %d = %d\n", i + 1, dev_info.ports[i]);
                    }

                    memset(&dev_info, 0, sizeof(struct device_info_t));
                    dev_info.cmd = AXCL_PORT_CREATE_COMPLETION;
                    dev_info.device = 0;
                    ret = ioctl(fd, IOC_AXCL_PORT_MANAGE, &dev_info);
                    if (ret < 0) {
                        printf("ioctl(IOC_AXCL_PORT_MANAGE) fail, error = 0x%x\n", errno);
                        continue;
                    }

                } else if (AXCL_PORT_DESTROY == dev_info.cmd) {
                    printf("received port destory cmd: \n");
                    printf("\tdevice = %d, host pid = %d\n", dev_info.device, dev_info.pid);
                    printf("\tports num: %d\n", dev_info.port_num);
                    for (unsigned int i = 0; i < dev_info.port_num; ++i) {
                        printf("\t\tport %d = %d\n", i + 1, dev_info.ports[i]);
                    }

                    /* TODO: The HOST process has hung, and the corresponding service process needs to be destroyed to reclaim resources. */

                } else {
                    printf("received unexpected cmd: %d host pid = %d\n", dev_info.device, dev_info.pid);
                }
            }
        }
    }
}
