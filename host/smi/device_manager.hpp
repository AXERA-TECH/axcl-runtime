/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#pragma once

#include <cstdint>
#include <vector>
#include "axcl.h"
#include "singleton.hpp"

class device_manager : public axcl::singleton<device_manager> {
    friend class axcl::singleton<device_manager>;

public:
    bool set_devices(const std::vector<uint32_t /* device index */>& devices);
    const std::vector<int32_t>& get_devices() const;

private:
    device_manager();
    virtual ~device_manager();

private:
    bool m_inited = false;
    std::vector<int32_t> m_devices;
    axclrtDeviceList m_conn_devices;
};

#define DEVCIE_MANAGER device_manager::get_instance

class active_device_guard {
public:
    active_device_guard(int32_t device);
    ~active_device_guard();

    operator bool() const noexcept {
        return m_device > 0;
    }

private:
    active_device_guard(const active_device_guard&) = delete;
    active_device_guard& operator=(const active_device_guard&) = delete;

private:
    int32_t m_device;
};

class context_guard final {
public:
    explicit context_guard(int32_t device) {
        ::axclrtCreateContext(&m_context, device);
    }

    ~context_guard() {
        ::axclrtDestroyContext(m_context);
    }

    operator bool() const noexcept {
        return m_context ? true : false;
    }

private:
    axclrtContext m_context = nullptr;
};
