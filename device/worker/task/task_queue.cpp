/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "task_queue.hpp"
#include "axcl_logger.hpp"

#include "runtime_api.hpp"
#include "../channel/channel.hpp"
#include "package/include/runtime/system/type.hpp"
#include "package/include/runtime/type.hpp"
#include "package/include/type.hpp"
#include "../stream/stream.hpp"
#include "package/device/runtime/system/control/control_response.hpp"

// RUNTIME system control command type definition
#define CONTROL_HEAD_TYPE CREATE_PACKAGE_HEAD_TYPE(axcl::pkg::PROTO, static_cast<uint8_t>(axcl::pkg::module::RUNTIME), static_cast<uint8_t>(axcl::pkg::runtime::module::SYSTEM), static_cast<uint8_t>(axcl::pkg::runtime::system::module::CONTROL))

#define TAG     "task_queue"

namespace axcl::worker {

void standard_task_queue::push(std::shared_ptr<task> task) {
    if (!task) {
        return;
    }

    std::unique_lock<std::mutex> lock(m_mutex);

    if (task->is_data_task() && m_stream && m_stream->get_stream_id() != 0) {
        void* data_ptr = task->get_data_ptr();
        if (data_ptr) {
            auto channel_data_ptr = static_cast<axcl::worker::channel_data*>(data_ptr);
            if (channel_data_ptr && channel_data_ptr->data && channel_data_ptr->size > sizeof(axcl::pkg::head)) {
                const auto hd = static_cast<const axcl::pkg::head*>(channel_data_ptr->data);

                if ((hd->type & 0xC0FFFFFF) == CONTROL_HEAD_TYPE
                    && hd->command == static_cast<uint32_t>(axcl::pkg::runtime::system::command::SYNC_STREAM)) {

                    axcl::pkg::device::control_response decoder;
                    if (decoder.decode(channel_data_ptr->data, channel_data_ptr->size)) {
                        int32_t timeout_ms = decoder.get_timeout();

                        m_stream->set_timeout(timeout_ms);
                        LOG_M_D(TAG, "SYNC_STREAM task detected in push, set timeout to {} ms for stream({})",
                                timeout_ms, m_stream->get_stream_id());
                    } else {
                        LOG_M_E(TAG, "Failed to decode SYNC_STREAM message");
                    }
                }
            }
        }
    }

    m_tasks.push(task);
}

void standard_task_queue::execute_task(std::shared_ptr<task> task) {
    if (!task) {
        return;
    }

    bool is_sync_stream = false;
    if (task->is_data_task()) {
        void* data_ptr = task->get_data_ptr();
        if (data_ptr) {
            auto channel_data_ptr = static_cast<axcl::worker::channel_data*>(data_ptr);
            if (channel_data_ptr && channel_data_ptr->data && channel_data_ptr->size > sizeof(axcl::pkg::head)) {
                const auto hd = static_cast<const axcl::pkg::head*>(channel_data_ptr->data);
                if ((hd->type & 0xC0FFFFFF) == CONTROL_HEAD_TYPE
                    && hd->command == static_cast<uint32_t>(axcl::pkg::runtime::system::command::SYNC_STREAM)) {
                    is_sync_stream = true;
                }
            }
        }
    }

    if (!is_sync_stream && !should_execute_task()) {
        return;
    }

    try {
        task->execute();
        if (is_sync_stream) {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_last_error.clear();
        }
    }
    catch (const worker::RuntimeApiException& e) {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_last_error = TaskError("RuntimeApiException",
                                 "RuntimeApiException: module=" + e.getModule() +
                                 ", api=" + std::to_string(e.getApi()),
                                 e.getErrorCode());

        LOG_MM_E(TAG, "RuntimeApiException caught: module={}, api={}, error_code={}",
                 e.getModule(), e.getApi(), static_cast<uint32_t>(e.getErrorCode()));
    }
}

} // namespace axcl

