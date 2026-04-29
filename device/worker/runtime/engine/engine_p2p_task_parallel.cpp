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

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

#define TAG "engine"

bool task_router::impl::launch_parallel_task(std::vector<float>& elapsed) {
    auto send_status = true;
    auto recv_status = true;

    std::atomic<bool> thread_exit = false;
    bool send_ready = false;
    bool recv_ready = false;

    std::mutex send_mtx;
    std::mutex recv_mtx;
    std::condition_variable cond;

    auto p2p_send_task = [&]() -> void {
        LOG_MM_D(TAG, "task send thread start.");
        while (!thread_exit) {
            std::unique_lock lock(send_mtx);
            cond.wait(lock, [&send_ready, &thread_exit] { return send_ready || thread_exit; });
            if (thread_exit) {
                break;
            }

            timer timer;
            send_status = this->send();
            send_ready = false;
            cond.notify_all();
            LOG_MM_D(TAG, "send payload size {:#08x} elapsed: {} ms.", this->rank_size_, timer.elapsed());
        }
        LOG_MM_D(TAG, "task send thread exit {}.", send_status);
    };

    auto p2p_recv_task = [&]() -> void {
        LOG_MM_D(TAG, "task recv thread start.");
        while (!thread_exit) {
            std::unique_lock lock(recv_mtx);
            cond.wait(lock, [&recv_ready, &thread_exit] { return recv_ready || thread_exit; });
            if (thread_exit) {
                break;
            }

            timer timer;
            recv_status = this->recv();
            recv_ready = false;
            cond.notify_all();
            LOG_MM_D(TAG, "recv payload elapsed: {} ms.", timer.elapsed());
        }
        LOG_MM_D(TAG, "task recv thread exit {}.", recv_status);
    };

    std::thread send_thread(p2p_send_task);
    std::thread recv_thread(p2p_recv_task);

    for (auto& i : elapsed) {
        {
            std::lock_guard send_lock(send_mtx);
            std::lock_guard recv_lock(recv_mtx);
            send_ready = true;
            recv_ready = true;
        }

        timer timer;
        cond.notify_all();
        {
            std::unique_lock send_lock(send_mtx);
            std::unique_lock recv_lock(recv_mtx);
            cond.wait(send_lock, [&send_ready, &thread_exit] { return !send_ready || thread_exit; });
            cond.wait(recv_lock, [&recv_ready, &thread_exit] { return !recv_ready || thread_exit; });
        }
        i = timer.elapsed();

        if (thread_exit) {
            break;
        }

        if (!send_status || !recv_status) {
            thread_exit = true;
            cond.notify_all();
            break;
        }
    }

    thread_exit = true;
    cond.notify_all();

    send_thread.join();
    recv_thread.join();

    LOG_MM_D(TAG, "task main thread exit {}.", send_status && recv_status);
    return send_status && recv_status;
}
