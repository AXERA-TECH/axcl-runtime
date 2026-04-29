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

#include <array>
#include <cstdint>
#include <mutex>
#include <utility>
#include <map>
#include "device.hpp"
#include "threadx.hpp"
#include "event.hpp"

namespace axcl::daemon {

#define PORT_ALIVE_INTERVAL (3000)

typedef std::pair<std::pair<uint32_t, uint32_t>, std::pair<uint32_t, uint32_t>> port_token_t;

class port : public sinker {
public:
    port() = default;
    ~port() = default;

    bool open();
    void close();

    bool start();

public:
    void on_port_allocate(const device_info &device_info) override;

private:
    void launch_process(const device_info &device_info, uint32_t dma_buf_size, uint32_t log_level);
    void kill_process(const uint32_t ports[4]);
    void alive();
    bool is_alive(const pid_t &pid);

private:
    axcl::threadx m_alive_thread;
    axcl::event m_alive_task;

    std::mutex m_mtx_maps;
    std::map<port_token_t, std::array<pid_t, 2>> m_maps; // pid[0]: host, pid[1]: slave
};

}  // namespace axcl::daemon
