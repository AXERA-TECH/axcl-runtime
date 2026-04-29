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
#include <list>
#include <mutex>
#include "axcl_comm.hpp"
#include "device_info.h"
#include "threadx.hpp"
#include "event.hpp"

namespace axcl::daemon {

class sinker {
public:
    virtual ~sinker() = default;
    virtual void on_port_allocate(const device_info &device_info) = 0;
};

class device {
public:
    device(axcl::comm::COMM_TYPE type);
    ~device() = default;

    bool open();
    void close();

    bool start();

    bool register_sink(sinker *sink);
    bool unregister_sink(sinker *sink);

private:
    void listen();
    void heartbeats();
    void dispatch(const device_info &device_info);

private:
    axcl::threadx m_listen_thread;
    axcl::threadx m_heartbeats_thread;
    axcl::event m_heartbeats_task;

    std::mutex m_mtx_sinks;
    std::list<sinker *> m_sinks;

    axcl::comm::COMM_TYPE m_comm_type;
    int m_fd;
};

}  // namespace axcl::daemon
