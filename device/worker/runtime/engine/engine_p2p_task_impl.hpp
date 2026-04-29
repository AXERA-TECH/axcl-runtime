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

#include "engine_p2p_task.hpp"
#include "ax_pcie_p2p_user.h"

#include <sys/select.h>

struct task_router::impl
{
    impl();
    ~impl();

    [[nodiscard]] bool init();

    [[nodiscard]] bool set_payload(uint32_t size, bool block = false);
    [[nodiscard]] bool verify_buffer() const;

    [[nodiscard]] bool launch_parallel_task(std::vector<float>& elapsed);
    [[nodiscard]] bool launch_sequential_task(std::vector<float>& elapsed);
    [[nodiscard]] bool launch_interlace_task(std::vector<float>& elapsed);

private:
    using p2p_config_t = p2p_device_config_t;
    using p2p_send_map_t = std::vector<p2p_item_send_t>;

    [[nodiscard]] bool get_local_id();
    [[nodiscard]] bool get_local_index();
    [[nodiscard]] bool get_world_size();

    void get_addr_map();
    void get_send_map();
    void dump_send_map() const;

    [[nodiscard]] bool send();
    [[nodiscard]] bool send_slice(uint32_t rank_index);

    [[nodiscard]] bool recv();
    [[nodiscard]] bool recv_slice(uint32_t rank_index);

    [[nodiscard]] bool reset_buffer() const;

    uint32_t rank_size_{0};
    bool block_{false};

    int32_t fd_{-1};
    uint32_t id_{0};
    uint32_t index_{0};
    p2p_config_t p2p_cfg_{};
    uint32_t world_size_{0};
    timeval timeout_{};
    std::vector<std::vector<uint64_t>> address_map_;
    p2p_send_map_t send_map_{};
    std::vector<p2p_item_get_t> recv_items_;
};
