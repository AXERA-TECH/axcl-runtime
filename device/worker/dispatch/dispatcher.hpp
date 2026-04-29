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

#include <mutex>
#include <unordered_map>
#include <memory>
#include "impl.hpp"
#include "idispatcher.hpp"
#include "context.hpp"
#include "stream.hpp"
#include "stream_manager.hpp"
#include "../channel/channel.hpp"
#include "axcl_comm.hpp"

namespace axcl::worker {

class port;
class context_manager;
class event_manager;
class dispatcher : public sinker, public idispatcher {
public:
    dispatcher(port* port);
    virtual ~dispatcher();

public:
    void on_channel_data(std::shared_ptr<channel_data> data) override;
    void ack(const void* data, size_t size, int64_t flag, int32_t timeout) override;

    /* Get context_manager and event_manager */
    std::shared_ptr<context_manager> get_context_manager() { return m_context_manager; }
    std::shared_ptr<event_manager> get_event_manager() { return m_event_manager; }

    /* Get m_comm from m_port */
    std::shared_ptr<axcl::comm::icomm> get_comm() const;

protected:
    void impl(std::shared_ptr<channel_data> data);

private:
    std::mutex m_mtx;
    port* m_port;
    std::shared_ptr<axcl::worker::impl> m_impl;

    std::shared_ptr<context_manager> m_context_manager;
    std::shared_ptr<event_manager> m_event_manager;
};

} /* namespace axcl::worker */
