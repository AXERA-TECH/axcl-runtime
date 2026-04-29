/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "device_manager.hpp"
#include <algorithm>
#include <iterator>
#include "smi_log.hpp"

#define TAG "smi"

device_manager::device_manager() {
    if (axclError ret = axclInit(nullptr); AXCL_SUCC != ret) {
        LOG_M_E(TAG, "axcl init fail, ret = 0x%x", ret);
        return;
    }

    m_inited = true;

    axclrtGetDeviceList(&m_conn_devices);
    //std::sort(m_conn_devices.devices, m_conn_devices.devices + m_conn_devices.num);
    m_devices.assign(std::begin(m_conn_devices.devices), std::begin(m_conn_devices.devices) + m_conn_devices.num);
}

device_manager::~device_manager() {
    if (m_inited) {
        axclFinalize();
    }
}

bool device_manager::set_devices(const std::vector<uint32_t>& devices) {
    std::vector<int32_t> lst;
    for (auto&& m : devices) {
        if (m >= m_conn_devices.num) {
            LOG_M_E(TAG, "device index %d is out of connected devices number %d", m, m_conn_devices.num);
            return false;
        }

        lst.push_back(m_conn_devices.devices[m]);
    }

    m_devices = lst;
    return true;
}

const std::vector<int32_t>& device_manager::get_devices() const {
    return m_devices;
}

active_device_guard::active_device_guard(int32_t device) : m_device(device) {
    if (axclError ret = axclrtSetDevice(m_device); AXCL_SUCC != ret) {
        LOG_M_E(TAG, "active device %d fail, ret = 0x%x", m_device, ret);
        m_device = -1;
    }
}

active_device_guard::~active_device_guard() {
    if (m_device > 0) {
        axclrtResetDevice(m_device);
    }
}