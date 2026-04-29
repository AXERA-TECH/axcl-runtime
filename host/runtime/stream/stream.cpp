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
#include "context/context.hpp"
#include "context/context_manager.hpp"
#include "device/device.hpp"
#include "device/device_manager.hpp"
#include "axcl_logger.hpp"
#include "stream/stream_manager.hpp"

#define TAG "stream"

namespace axcl::rt {

stream::stream(stream_manager* mgr, uint32_t stream_id) : m_stream_manager(mgr), m_stream_id(stream_id) {
    auto ctx = mgr->get_context();
    auto ctx_mgr = ctx->get_context_manager();
    auto dev = ctx_mgr->get_device();

    m_context_id = ctx->get_context_id();
    m_device_id = dev->get_device_id();
    m_channel = dev->get_channel();

    m_channel->register_sink(this);
    m_thread.start("stream" + std::to_string(m_stream_id), &stream::execute_task_thread, this);
}

stream::~stream() {
    m_channel->unregister_sink(this);
    m_thread.stop();
    m_submit_event.set();
    m_cv_submit.notify_one();
    m_thread.join();
}

void stream::on_channel_data(std::shared_ptr<channel_data> data) {
    if (m_device_id != data->device || m_context_id != data->context || m_stream_id != data->stream) {
        return;
    }

    std::lock_guard<std::mutex> lck(m_mtx_response);
    auto it = m_responses.find(data->token);
    if (it != m_responses.end()) {
        it->second->wakeup(data);
        m_responses.erase(it);
    }
}

bool stream::submit_task(std::shared_ptr<task> sp) {
    m_submit_event.wait(-1);

    if (!m_thread.running()) {
        return false;
    }

    {
        std::lock_guard<std::mutex> lck(m_mtx_submit);
        m_tasks.push_back(sp);
    }

    m_cv_submit.notify_one();
    return true;
}

void stream::remove_task(std::shared_ptr<task> sp) {
    sp->deactive();

    if (!sp->has_got_response()) {
        std::lock_guard<std::mutex> lck(m_mtx_response);
        auto it = m_responses.find(sp->get_token());
        if (it != m_responses.end()) {
            m_responses.erase(it);
        }
    }
}

bool stream::execute(const void* data, size_t size, int64_t flag, int32_t timeout) {
    return m_channel->send(data, size, flag, timeout);
}

void stream::execute_task_thread() {
    /* enable to submit task */
    m_submit_event.set();

    while (m_thread.running()) {
        std::shared_ptr<task> task;
        {
            /* wait until task has submitted */
            std::unique_lock<std::mutex> lck(m_mtx_submit);
            m_cv_submit.wait(lck, [this] { return !m_thread.running() || !m_tasks.empty(); });

            if (!m_thread.running()) {
                break;
            }

            task = m_tasks.front();
            m_tasks.pop_front();

            if (task->is_deactived()) {
                /* if deactive, no need to execute */
                task = nullptr;
                continue;
            }
        }

        if (task->has_response()) {
            /* add response before execute */
            const auto token = task->get_token();
            m_mtx_response.lock();
            m_responses.emplace(token, task);
            m_mtx_response.unlock();

            if (!task->execute()) {
                std::lock_guard<std::mutex> lck(m_mtx_response);
                m_responses.erase(token);
            }
        } else {
            (void)task->execute();
        }
    }
}

bool stream::sync(int32_t timeout) {
    /* hold to no accept new task */
    m_submit_event.reset();

    bool ret = false;
    using clock = std::chrono::steady_clock;
    auto start = clock::now();
    do {
        /* fixme: how to wait both task and response list are empty */
        {
            std::lock_guard<std::mutex> lck(m_mtx_response);
            if (m_responses.empty()) {
                ret = true;
                break;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

    } while (timeout < 0 || std::chrono::duration_cast<std::chrono::milliseconds>(clock::now() - start).count() < timeout);

    m_submit_event.set();

    return ret;
}

}  // namespace axcl::rt