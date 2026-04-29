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
#include "axcl_comm.hpp"
#include "channel/channel.hpp"
#include "device_info.h"
#include "device_memory.hpp"

namespace axcl::rt {

class context_manager;
class event_manager;
class device_manager;
class device {
public:
    device(device_manager* mgr, axcl::comm::COMM_TYPE type);

    bool open(const struct device_info& info);
    void sync();
    void close();
    void offline();
    void online();

    event_manager* get_event_manager() {
        return m_event_manager.get();
    }

    context_manager* get_context_manager() {
        return m_context_manager.get();
    }

    device_manager* get_device_manager() {
        return m_device_manager;
    }

    channel* get_channel() {
        return m_channel.get();
    }

    uint32_t get_device_id() const {
        return m_info.device;
    }

    device_memory *get_device_memory() {
        return &m_alloc_mem;
    }

    bool is_opened() const {
        return m_opened;
    }

    struct device_info get_device_info() {
        return m_info;
    }

protected:
    device_manager* m_device_manager;
    axcl::comm::COMM_TYPE m_comm_type;
    struct device_info m_info;
    std::unique_ptr<channel> m_channel;
    std::shared_ptr<axcl::comm::icomm> m_comm;
    std::shared_ptr<context_manager> m_context_manager;
    std::shared_ptr<event_manager> m_event_manager;
    device_memory m_alloc_mem;
    bool m_opened = false;
};

}  // namespace axcl::rt