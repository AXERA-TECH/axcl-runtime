/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "cmd_log.hpp"
#include "axcl_rt_log_internal.h"
#include "device_manager.hpp"
#include "smi_log.hpp"

#define TAG "smi"

void axcl_smi_dump_device_log(axclrtLogKind kind, const char *output_path) {
    const auto &lst = DEVCIE_MANAGER()->get_devices();
    for (const auto &d : lst) {
        active_device_guard active(d);
        if (!active) {
            continue;
        }

        if (axclError ret = axclrtLog(kind, 0, output_path); AXCL_SUCC != ret) {
            LOG_M_E(TAG, "dump log from device %d fail, ret = 0x%X", d, ret);
            continue;
        }
    }
}