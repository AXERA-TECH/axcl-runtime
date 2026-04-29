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

#include <condition_variable>
#include <list>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <atomic>
#include <chrono>
#include "task.hpp"
#include "task_queue.hpp"
#include "thread_pool_ex.hpp"
#include "../../../toolkit/threadx.hpp"

namespace axcl::worker {

class stream_manager;
class stream {
public:
    stream(stream_manager* mgr, uint32_t stream_id, bool enable_timeout_monitor = true);
    virtual ~stream();

    bool enqueue_task(std::shared_ptr<task> task);

    void set_timeout(int32_t timeout_ms);

    void reset_timeout_monitor();

    uint32_t get_stream_id() const {
        return m_stream_id;
    }

    stream_manager* get_stream_manager() {
        return m_stream_manager;
    }

    TaskError last_error() const {
        return m_task_queue->last_error();
    }

    std::shared_ptr<task_queue> get_task_queue() {
        return m_task_queue;
    }

    bool is_active() const {
        return m_is_active.load();
    }

    void set_inactive() {
        m_is_active.store(false);
        std::unique_lock<std::mutex> lock(m_timeout_mutex);
        m_timeout_cv.notify_all();
    }

private:
    void timeout_monitor_thread();

private:
    stream_manager* m_stream_manager;
    uint32_t m_stream_id;
    std::shared_ptr<task_queue> m_task_queue;
    std::shared_ptr<thread_pool_ex> m_thread_pool;
    std::atomic<bool> m_is_active{false};

    std::mutex m_timeout_mutex;
    std::condition_variable m_timeout_cv;
    axcl::threadx m_timeout_thread;
    std::atomic<int32_t> m_timeout_ms{0};
    std::atomic<bool> m_monitoring{false};
    std::atomic<bool> m_stop_monitoring{false};
    std::chrono::steady_clock::time_point m_timeout_start;
    bool m_enable_timeout_monitor{true};
};

}  // namespace axcl::worker