/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "cmd_set.hpp"
#include <iostream>
#include <string>
#include "axcl_rt_control_internal.h"
#include "device_manager.hpp"
#include "smi_log.hpp"

#define TAG "smi"
#define TIMEOUT_1S (1000)

void axcl_smi_set_cpu_freq(uint32_t freq) {
    std::string cmd = "echo " + std::to_string(freq) + " > /sys/devices/system/cpu/cpufreq/policy0/scaling_setspeed";
    const auto &lst = DEVCIE_MANAGER()->get_devices();
    for (const auto &d : lst) {
        active_device_guard active(d);
        if (!active) {
            continue;
        }

        const char *output = nullptr;
        if (axclError ret = axclrtControlExecuteShellCmd(cmd.c_str(), nullptr, 0, &output, TIMEOUT_1S); AXCL_SUCC != ret) {
            LOG_M_E(TAG, "set cpu frequency %d to device %d fail, ret = 0x%X", freq, d, ret);
            continue;
        }

        printf("set cpu frequency %d to device %d succeed.\n", freq, d);
        printf("\n");
    }
}
