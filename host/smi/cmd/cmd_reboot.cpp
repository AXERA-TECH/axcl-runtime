/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "cmd_reboot.hpp"
#include <stdio.h>
#include "axcl_rt_device_internal.h"
#include "device_manager.hpp"

void axcl_smi_reboot_device(int32_t device_index) {
    if (axclError ret = axclInit(nullptr); AXCL_SUCC == ret) {
        axclrtDeviceList device_list;
        axclrtGetDeviceList(&device_list);
        if (device_index >= (int32_t)device_list.num) {
            return;
        }

        if (device_index < 0) {
            device_index = 0;
        }

        int32_t device_id = device_list.devices[device_index];
        if (ret = axclrtRebootDevice(device_id); AXCL_SUCC == ret) {
            if (ret = axclrtSetDevice(device_id); AXCL_SUCC == ret) {
                axclrtResetDevice(device_id);
                printf("reboot device %d success\n", device_id);
            }
        }

        axclFinalize();
    }
}