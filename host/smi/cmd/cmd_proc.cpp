/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "cmd_proc.hpp"
#include <iostream>
#include "axcl_rt_control_internal.h"
#include "device_manager.hpp"
#include "smi_log.hpp"

#define TAG "smi"
#define TIMEOUT_3S (3000)

static void show_proc(const char *cmd) {
    const auto &lst = DEVCIE_MANAGER()->get_devices();
    for (const auto &d : lst) {
        active_device_guard active(d);
        if (!active) {
            continue;
        }

        const char *output = nullptr;
        if (axclError ret = axclrtControlExecuteShellCmd(cmd, nullptr, 0, &output, TIMEOUT_3S); AXCL_SUCC != ret) {
            continue;
        }

        if (output) {
            std::cout << output;
        }
    }
}

void axcl_smi_show_vdec_proc() {
    show_proc("cat /proc/ax_proc/vdec");
}

void axcl_smi_show_venc_proc() {
    show_proc("cat /proc/ax_proc/venc");
}

void axcl_smi_show_jenc_proc() {
    show_proc("cat /proc/ax_proc/jenc");
}

void axcl_smi_show_ivps_proc() {
    show_proc("cat /proc/ax_proc/ivps");
}

void axcl_smi_show_rgn_proc() {
    show_proc("cat /proc/ax_proc/rgn");
}

void axcl_smi_show_ive_proc() {
    show_proc("cat /proc/ax_proc/ive");
}

void axcl_smi_show_pool_proc() {
    show_proc("cat /proc/ax_proc/pool");
}

void axcl_smi_show_link_proc() {
    show_proc("cat /proc/ax_proc/link_table");
}

void axcl_smi_show_cmm_proc() {
    show_proc("cat /proc/ax_proc/mem_cmm_info");
}