/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "cmd_shell.hpp"
#include <iostream>
#include "axcl_rt_control_internal.h"
#include "device_manager.hpp"
#include "smi_log.hpp"

#define TAG "smi"

void axcl_smi_execute_shell_cmd(const char *cmd, const char *const args[], size_t argc) {
    int32_t timeout = 10000;
    const char *env = std::getenv("AXCL_SHELL_TIMEOUT");
    if (env) {
        timeout = atoi(env);
    }

    const auto &lst = DEVCIE_MANAGER()->get_devices();
    for (const auto &d : lst) {
        active_device_guard active(d);
        if (!active) {
            continue;
        }

        const char *output = nullptr;
        if (axclError ret = axclrtControlExecuteShellCmd(cmd, args, argc, &output, timeout); AXCL_SUCC != ret) {
            LOG_M_E(TAG, "execute shell %s on device %d fail, ret = 0x%x", cmd, d, ret);
            continue;
        }

        if (output) {
            std::cout << output;
        }

        std::cout << std::endl;
    }
}