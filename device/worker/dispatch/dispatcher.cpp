/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "dispatcher.hpp"
#include <chrono>
#include <string>
#include <vector>
#include "port.hpp"
#include "axcl_logger.hpp"
#include "../context/context_manager.hpp"
#include "../stream/stream.hpp"
#include "../event/event_manager.hpp"
#include "../channel/channel.hpp"
#include "../task/task.hpp"
#include "runtime_api.hpp"
#include "package/include/runtime/system/type.hpp"

#define TAG "dispatcher"

#define STREAM_RT_SYSTEM_CONTROL_HEAD_TYPE CREATE_PACKAGE_HEAD_TYPE(axcl::pkg::PROTO, axcl::pkg::module::RUNTIME, axcl::pkg::runtime::module::SYSTEM, axcl::pkg::runtime::system::module::CONTROL)

namespace axcl::worker {

dispatcher::dispatcher(port* port) : m_port(port) {
    m_impl = std::make_shared<axcl::worker::impl>(this);

    m_context_manager = std::make_shared<context_manager>();
    m_event_manager = std::make_shared<event_manager>();

    port->register_sink(this);
}

dispatcher::~dispatcher() {
    std::lock_guard<std::mutex> lck(m_mtx);
}

void dispatcher::on_channel_data(std::shared_ptr<channel_data> data) {
    if (data->size && data->data) {
        const auto hd = static_cast<const pkg::head *>(data->data);

        /* hd->type & ~0x3F000000 */
        /* hd->type & 0xC0FFFFFF */
        if ((hd->type & 0xC0FFFFFF) == STREAM_RT_SYSTEM_CONTROL_HEAD_TYPE
            && hd->command != static_cast<uint32_t>(axcl::pkg::runtime::system::command::SYNC_STREAM)
            && hd->command != static_cast<uint32_t>(axcl::pkg::runtime::system::command::RECORD_EVENT)
            && hd->command != static_cast<uint32_t>(axcl::pkg::runtime::system::command::WAIT_EVENT)) {

            m_impl->run(data->data, data->size);

        } else {

            uint32_t context_id = data->context;
            uint32_t stream_id = data->stream;
            auto stream = m_context_manager->get_or_create_stream(context_id, stream_id);
            if (!stream) {
                LOG_MM_E(TAG, "Failed to get or create stream({}) for context({})", stream_id, context_id);
                return;
            }

            /* Enqueue task to stream */
            auto task = make_data_task<channel_data>(
                [this](std::shared_ptr<channel_data> task_data) {
                    this->impl(task_data);
                },
                data
            );

            stream->enqueue_task(task);
        }
    }
}

void dispatcher::impl(std::shared_ptr<channel_data> data) {
    m_impl->run(data->data, data->size);
}

void dispatcher::ack(const void* data, size_t size, int64_t flag, int32_t timeout) {
    m_port->send(data, size, flag, timeout);
}

std::shared_ptr<axcl::comm::icomm> dispatcher::get_comm() const {
    if (m_port) {
        return m_port->get_comm();
    }
    return nullptr;
}

} /* namespace axcl::worker */
