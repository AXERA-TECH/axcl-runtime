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

#include <vector>
#include <thread>
#include <mutex>
#include <future>
#include <functional>
#include <stdexcept>
#include <atomic>
#include <condition_variable>
#include "axcl_logger.hpp"
#include "task.hpp"
#include "task_queue.hpp"

namespace axcl::worker {

class thread_pool_ex {
public:
    thread_pool_ex(size_t threads, std::shared_ptr<task_queue> queue = std::make_shared<standard_task_queue>(),
                const std::string& token = "threads",
                const int32_t& sched_policy = SCHED_OTHER,
                const uint32_t& sched_priority = 0);

    void add_task(std::shared_ptr<task> task) {
        if (m_stop.load(std::memory_order_relaxed)) {
            throw std::runtime_error("add_task on stopped thread_pool_ex");
        }

        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_task_queue->push(task);
        }

        m_condition.notify_one();
    }

    ~thread_pool_ex();

    void set_task_queue(std::shared_ptr<task_queue> queue) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_task_queue = queue;
    }

    std::shared_ptr<task_queue> get_task_queue() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_task_queue;
    }

private:
    void initialize_workers(const int32_t& sched_policy, const uint32_t& sched_priority);

    std::shared_ptr<task_queue> m_task_queue;
    size_t m_threads_num;
    std::string m_threads_name;
    std::vector<std::thread> m_workers;

    mutable std::mutex m_mutex;
    std::condition_variable m_condition;
    std::atomic<bool> m_stop;
};

} // namespace axcl::worker