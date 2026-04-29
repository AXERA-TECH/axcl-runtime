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

bool task_router::impl::launch_sequential_task(std::vector<float>& elapsed) {
    static auto send_status = true;
    static auto recv_status = true;

    auto p2p_send_task = [this]() -> void {
        timer timer;
        send_status = this->send();
        LOG_MM_D(TAG, "send payload size {:#08x} elapsed: {} ms.", this->rank_size_, timer.elapsed());
    };

    auto p2p_recv_task = [this]() -> void {
        timer timer;
        recv_status = this->recv();
        LOG_MM_D(TAG, "recv payload elapsed: {} ms.", timer.elapsed());
    };

    for (float & i : elapsed) {
        timer timer;
        p2p_send_task();
        p2p_recv_task();
        i = timer.elapsed();
        if (!send_status || !recv_status) {
            break;
        }
    }

    LOG_MM_D(TAG, "main thread task exit {}.", send_status && recv_status);
    return send_status && recv_status;
}
