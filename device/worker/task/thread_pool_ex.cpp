/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "thread_pool_ex.hpp"

#define TAG "thread_pool_ex"

namespace axcl::worker {

thread_pool_ex::thread_pool_ex(size_t threads,
                              std::shared_ptr<task_queue> queue,
                              const std::string& token,
                              const int32_t& sched_policy,
                              const uint32_t& sched_priority)
    : m_task_queue(queue),
      m_threads_num(threads),
      m_threads_name(token),
      m_stop(false)
{
    initialize_workers(sched_policy, sched_priority);
}

void thread_pool_ex::initialize_workers(const int32_t& sched_policy, const uint32_t& sched_priority) {
    for (size_t i = 0; i < m_threads_num; ++i) {
        m_workers.emplace_back([this, i]() {
            std::string thread_name;

            if (m_threads_num > 1) {
                thread_name = m_threads_name + "_" + std::to_string(i + 1);
            } else {
                thread_name = m_threads_name;
            }

            pthread_setname_np(pthread_self(), thread_name.c_str());

            for (;;) {
                std::shared_ptr<task> task = nullptr;

                {
                    std::unique_lock<std::mutex> lock(this->m_mutex);
                    this->m_condition.wait(lock, [this] {
                        return this->m_stop.load(std::memory_order_relaxed) || !this->m_task_queue->empty();
                    });

                    if (this->m_stop.load(std::memory_order_relaxed) && this->m_task_queue->empty()) {
                        LOG_MM_I(TAG, "thread({}) exiting: stop flag set and queue empty", thread_name);
                        return;
                    }

                    task = this->m_task_queue->pop();
                }
                if (task) {
                    this->m_task_queue->execute_task(task);
                }
            }
        });

        {
            sched_param sch;
            int policy;
            pthread_getschedparam(pthread_self(), &policy, &sch);
            sch.sched_priority = sched_priority;
            pthread_setschedparam(pthread_self(), sched_policy, &sch);
        }
    }
}

thread_pool_ex::~thread_pool_ex() {
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_stop.store(true, std::memory_order_release);
    }
    m_condition.notify_all();

    for (size_t i = 0; i < m_workers.size(); ++i) {
        m_workers[i].join();
    }

    LOG_M_I(TAG, "Thread pool({}) destroyed successfully, all {} threads exited cleanly",
             m_threads_name, m_workers.size());
}

} // namespace axcl::worker