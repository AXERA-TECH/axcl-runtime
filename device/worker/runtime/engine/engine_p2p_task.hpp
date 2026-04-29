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

class task_router
{
public:
    task_router();
    ~task_router();

    [[nodiscard]] bool init() const;

    [[nodiscard]] bool verify_buffer() const;

    [[nodiscard]] bool set_payload(uint32_t size, bool block = false) const;

    [[nodiscard]] bool launch_parallel_task(std::vector<float>& elapsed) const;
    [[nodiscard]] bool launch_sequential_task(std::vector<float>& elapsed) const;
    [[nodiscard]] bool launch_interlace_task(std::vector<float>& elapsed) const;

private:
    struct impl;
    impl *impl_;
};
