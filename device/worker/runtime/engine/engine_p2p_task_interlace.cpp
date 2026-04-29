/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "engine_p2p_task_impl.hpp"

#include "axcl_logger.hpp"
#include "timer.hpp"

#define TAG "engine"

bool task_router::impl::launch_interlace_task(std::vector<float>& elapsed) {
    auto send_status = true;
    auto recv_status = true;

    for (auto& one_time : elapsed) {
        timer timer;
        for (uint32_t i = 0; i < this->send_map_.size(); ++i) {
            send_status = this->send_slice(i);
            recv_status = this->recv_slice(i);
        }
        one_time = timer.elapsed();

        if (!send_status || !recv_status) {
            break;
        }
    }

    LOG_MM_D(TAG, "send & recv interlace exit {}.", send_status && recv_status);
    return  send_status && recv_status;
}
