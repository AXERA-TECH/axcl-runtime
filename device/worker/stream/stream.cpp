/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "stream.hpp"
#include <chrono>
#include <string>
#include <future>

#include "stream_manager.hpp"
#include "axcl_logger.hpp"
#include "axcl_rt_stream.h"

#define TAG "stream"

namespace axcl::worker {

stream::stream(stream_manager* mgr, uint32_t stream_id, bool enable_timeout_monitor) :
        m_stream_manager(mgr), m_stream_id(stream_id), m_enable_timeout_monitor(enable_timeout_monitor){

    m_task_queue = std::make_shared<standard_task_queue>();
    m_task_queue->set_stream(this);

    m_thread_pool = std::make_shared<thread_pool_ex>(
        1,                                      // Number of threads
        m_task_queue,                           // Task queue
        "stream_" + std::to_string(stream_id)   // Token
    );

    m_is_active.store(true);

    if (m_enable_timeout_monitor) {
        m_stop_monitoring.store(false);
        std::string thread_name = "stmto_" + std::to_string(stream_id);
        m_timeout_thread.start(thread_name, &stream::timeout_monitor_thread, this);
    }

    LOG_M_I(TAG, "stream({}) created", stream_id);
}

stream::~stream() {

    if (m_is_active.load()) {
        set_inactive();
    }

    if (m_stream_id != 0) {
        {
            std::unique_lock<std::mutex> lock(m_timeout_mutex);
            m_stop_monitoring.store(true);
            m_timeout_cv.notify_all();
        }

        if (m_enable_timeout_monitor && m_timeout_thread.running()) {
            m_timeout_thread.stop();
            m_timeout_thread.join();
        }
    }

    LOG_M_I(TAG, "stream({}) destroyed successfully", m_stream_id);
}

bool stream::enqueue_task(std::shared_ptr<task> task) {

    if (!task) {
        LOG_MM_E(TAG, "stream({}) cannot enqueue null task", m_stream_id);
        return false;
    }

    if (!m_is_active.load()) {
        LOG_MM_E(TAG, "stream({}) is not active, cannot enqueue task", m_stream_id);
        return false;
    }

    try {
        m_thread_pool->add_task(task);
        return true;
    } catch (const std::exception& e) {
        LOG_MM_E(TAG, "failed to enqueue task to stream({}): {}", m_stream_id, e.what());
        return false;
    }
}

void stream::set_timeout(int32_t timeout_ms) {
    if (m_stream_id == 0) {
        return;
    }

    std::unique_lock<std::mutex> lock(m_timeout_mutex);
    m_timeout_ms.store(timeout_ms);

    if (timeout_ms > 0) {
        m_monitoring.store(true);
        m_timeout_start = std::chrono::steady_clock::now();
        m_timeout_cv.notify_one();
        LOG_M_I(TAG, "stream({}) timeout set to {} ms", m_stream_id, timeout_ms);
    } else if (timeout_ms == 0) {
        m_monitoring.store(false);
        LOG_M_I(TAG, "stream({}) timeout set to immediate return", m_stream_id);
    } else {
        m_monitoring.store(false);
        LOG_M_I(TAG, "stream({}) timeout set to wait indefinitely", m_stream_id);
    }
}

void stream::reset_timeout_monitor() {
    if (m_stream_id == 0) {
        return;
    }

    std::unique_lock<std::mutex> lock(m_timeout_mutex);
    m_monitoring.store(false);
    LOG_M_I(TAG, "stream({}) timeout monitor reset", m_stream_id);
}

void stream::timeout_monitor_thread() {

    while (m_is_active.load() && !m_stop_monitoring.load()) {
        {
            std::unique_lock<std::mutex> lock(m_timeout_mutex);
            m_timeout_cv.wait(lock, [this]() {
                return m_monitoring.load() || m_stop_monitoring.load() || !m_is_active.load();
            });

            if (m_stop_monitoring.load() || !m_is_active.load()) {
                break;
            }
        }

        const int32_t timeout = m_timeout_ms.load();
        if (timeout > 0 && m_monitoring.load()) {
            auto deadline = m_timeout_start + std::chrono::milliseconds(timeout);

            {
                std::unique_lock<std::mutex> lock(m_timeout_mutex);
                auto status = m_timeout_cv.wait_until(lock, deadline, [this]() {
                    return !m_monitoring.load() || m_stop_monitoring.load() || !m_is_active.load();
                });

                if (!status && m_monitoring.load()) {
                    LOG_M_D(TAG, "stream({}) operation timed out after {} ms", m_stream_id, timeout);
                    m_task_queue->set_error("Timeout", "Stream operation timed out", AXCL_ERR_STREAM_SYNC_TIMEOUT);
                    m_monitoring.store(false);
                }
            }
        }
    }

    LOG_M_I(TAG, "stream({}) timeout monitor thread stopped", m_stream_id);
}

}  // namespace axcl::worker