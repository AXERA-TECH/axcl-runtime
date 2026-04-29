/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <algorithm>
#include "axcl_rt_control_internal.h"
#include "axcl_rt_device.h"
#include "device/device_manager.hpp"
#include "axcl_logger.hpp"

#define TAG "device"


AXCL_EXPORT axclError axclrtSetDevice(int32_t deviceId) {
    return DEVICE_MANAGER()->active(deviceId);
}

AXCL_EXPORT axclError axclrtResetDevice(int32_t deviceId) {
    return DEVICE_MANAGER()->deactive(deviceId);
}

AXCL_EXPORT axclError axclrtGetDevice(int32_t *deviceId) {
    if (!deviceId) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_DEVICE_NULL_POINTER;
    }

    *deviceId = DEVICE_MANAGER()->get_current_thread_device_id();
    if (*deviceId < 0) {
        LOG_MM_E(TAG, "current thread has no active device");
        return AXCL_ERR_DEVICE_NO_ACTIVE_DEVICE;
    }

    return AXCL_SUCC;
}

AXCL_EXPORT axclError axclrtGetDeviceCount(uint32_t *count) {
    if (!count) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_DEVICE_NULL_POINTER;
    }

    *count = DEVICE_MANAGER()->get_device_count();
    return AXCL_SUCC;
}

AXCL_EXPORT axclError axclrtGetDeviceList(axclrtDeviceList *deviceList) {
    if (!deviceList) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_DEVICE_NULL_POINTER;
    }

    DEVICE_MANAGER()->get_device_list(deviceList);
    return AXCL_SUCC;
}

AXCL_EXPORT axclError axclrtSynchronizeDevice() {
    return DEVICE_MANAGER()->sync();
}

AXCL_EXPORT axclError axclrtGetDeviceBusInfo(int32_t deviceId, axclrtDeviceBusInfo *busInfo) {
    if (!busInfo) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_DEVICE_NULL_POINTER;
    }

    return DEVICE_MANAGER()->get_bus_info(deviceId, *busInfo);
}

AXCL_EXPORT axclError axclrtGetDevicePidInfo(int32_t deviceId, axclrtDevicePidInfo *pidInfo) {
    if (!pidInfo) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_DEVICE_NULL_POINTER;
    }

    return DEVICE_MANAGER()->get_pid_info(deviceId, *pidInfo);
}

AXCL_EXPORT axclError axclrtRebootDevice(int32_t deviceId) {
#if defined (WINDOWS)
    LOG_MM_E(TAG, "reboot device is not supported on Windows platform, and it will be supported later");
    return AXCL_ERR_DEVICE_UNSUPPORT;
#else
    return DEVICE_MANAGER()->reboot(deviceId);
#endif
}

AXCL_EXPORT axclError axclrtGetDeviceProperties(int32_t deviceId, axclrtDeviceProperties *properties) {
    if (!properties) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_DEVICE_NULL_POINTER;
    }

    memset(properties, 0, sizeof(axclrtDeviceProperties));
    axclError ret;

    axclrtDeviceBusInfo bus = {0};
    if (ret = axclrtGetDeviceBusInfo(deviceId, &bus); AXCL_SUCC != ret) {
        return ret;
    } else {
        properties->pciDomain = bus.domain;
        properties->pciBusID = bus.bus;
        properties->pciDeviceID = bus.slot;
    }

    if (ret = axclrtSetDevice(deviceId); AXCL_SUCC != ret) {
        return ret;
    }

    /* get sw version */
    constexpr uint32_t timeout = 1000;
    char *output = nullptr;
    if (ret = axclrtControlExecuteShellCmd("cat /proc/ax_proc/version", nullptr, 0, (const char **)&output, timeout); AXCL_SUCC != ret) {
        axclrtResetDevice(deviceId);
        return ret;
    } else {
        if (output) {
            strncpy(properties->swVersion, output, sizeof(properties->swVersion) - 1);
            properties->swVersion[sizeof(properties->swVersion) - 1] = '\0';
        }
    }

    /* get unique id */
    if (ret = axclrtControlExecuteShellCmd("cat /proc/ax_proc/uid", nullptr, 0, (const char **)&output, timeout); AXCL_SUCC != ret) {
        axclrtResetDevice(deviceId);
        return ret;
    } else {
        if (output) {
            /* ax_uid: 0x6007080746221049 */
            char *pos = strstr(output, "ax_uid: 0x");
            if (pos) {
                properties->uid = strtoul(pos + strlen("ax_uid: 0x"), nullptr, 16);
            }
        }
    }

    /* get temperature */
    if (ret = axclrtControlExecuteShellCmd("cat /sys/class/thermal/thermal_zone0/temp", nullptr, 0, (const char **)&output, timeout);
        AXCL_SUCC != ret) {
        axclrtResetDevice(deviceId);
        return ret;
    } else {
        if (output) {
            properties->temperature = atoi(output);
        }
    }

    /* get os memory info */
    if (ret =
            axclrtControlExecuteShellCmd(R"(cat /proc/meminfo | grep -E "MemTotal|MemFree")", nullptr, 0, (const char **)&output, timeout);
        AXCL_SUCC != ret) {
        axclrtResetDevice(deviceId);
        return ret;
    } else {
        if (output) {
            /**
             * MemTotal:         968356 kB
             * MemFree:          816844 kB
             */
            char *pos;
            pos = strstr(output, "MemTotal:");
            if (pos) {
                sscanf(pos, "MemTotal: %u", &properties->totalMemSize);
            }

            pos = strstr(output, "MemFree:");
            if (pos) {
                sscanf(pos, "MemFree: %u", &properties->freeMemSize);
            }
        }
    }

    /* get cmm memory info */
    if (ret = axclrtControlExecuteShellCmd(R"(cat /proc/ax_proc/mem_cmm_info  | grep "total size")", nullptr, 0, (const char **)&output,
                                           timeout);
        AXCL_SUCC != ret) {
        axclrtResetDevice(deviceId);
        return ret;
    } else {
        if (output) {
            /**
             * total size=7208960KB(7040MB),used=18876KB(18MB + 444KB),remain=7190084KB(7021MB + 580KB),partition_number=1,block_number=23
             */
            char *pos;
            pos = strstr(output, "total size=");
            if (pos) {
                sscanf(pos, "total size=%u", &properties->totalCmmSize);
            }

            pos = strstr(output, "remain=");
            if (pos) {
                sscanf(pos, "remain=%u", &properties->freeCmmSize);
            }
        }
    }

    /* get cpu loading */
    if (ret = axclrtControlExecuteShellCmd(R"(top -bn1 | grep -E "CPU:" | grep -v "grep")", nullptr, 0, (const char **)&output, timeout);
        AXCL_SUCC != ret) {
        axclrtResetDevice(deviceId);
        return ret;
    } else {
        if (output) {
            /* CPU:  0.0% usr  0.0% sys  0.0% nic  100% idle  0.0% io  0.0% irq  0.0% sirq */
            float usr = 0.0f, sys = 0.0f, nic = 0.0f, idle = 0.0f;
            char *pos = strstr(output, "CPU:");
            if (pos) {
                if (sscanf(pos, "CPU: %f%% usr %f%% sys %f%% nic %f%% idle", &usr, &sys, &nic, &idle) >= 4) {
                    properties->cpuLoading = (uint32_t)((100.0f - idle) * 100);
                }
            }
        }
    }

    /* get npu loading */
    if (ret = axclrtControlExecuteShellCmd("cat /proc/ax_proc/npu/top", nullptr, 0, (const char **)&output, timeout); AXCL_SUCC != ret) {
        axclrtResetDevice(deviceId);
        return ret;
    } else {
        if (output) {
            /**
             * core:vnpu-Non
             * time:1
             * period:1000000
             * utilization:81%
             */
            const char *utilization = "utilization:";
            int total = 0;
            int count = 0;
            char *pos = output;
            char *end = output + strlen(output);
            while ((pos = strstr(pos, utilization)) != nullptr) {
                int rate = 0;
                sscanf(pos, "utilization:%d%%", &rate);
                total += rate;
                count++;

                pos += strlen(utilization);

                if (pos >= end) {
                    break;
                }
            }

            if (count > 0) {
                properties->npuLoading = (uint32_t)(total * 100.0f / count);
            }
        }
    }

    axclrtResetDevice(deviceId);
    return AXCL_SUCC;
}
