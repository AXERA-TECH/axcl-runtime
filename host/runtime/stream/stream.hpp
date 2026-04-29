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
#include "channel/channel.hpp"
#include "event.hpp"
#include "task/task.hpp"
#include "threadx.hpp"
#include "axcl_rt_task.h"

namespace axcl::rt {

class stream_manager;
class stream : public sinker {
    friend class task;

public:
    stream(stream_manager* mgr, uint32_t stream_id);
    virtual ~stream();

    bool submit_task(std::shared_ptr<task> sp);
    void remove_task(std::shared_ptr<task> sp);
    bool sync(int32_t timeout = -1);

    uint32_t get_stream_id() const {
        return m_stream_id;
    }

    stream_manager* get_stream_manager() {
        return m_stream_manager;
    }

public:
    void on_channel_data(std::shared_ptr<channel_data> data) override;

protected:
    bool execute(const void* data, size_t size, int64_t flag, int32_t timeout);
    void execute_task_thread();

private:
    axcl::threadx m_thread;
    stream_manager* m_stream_manager;
    channel* m_channel;

    uint32_t m_stream_id;
    uint32_t m_context_id;
    uint32_t m_device_id;

    std::list<std::shared_ptr<task>> m_tasks;
    std::mutex m_mtx_submit;
    std::condition_variable m_cv_submit;

    std::mutex m_mtx_response;
    std::unordered_map<uint64_t /* token */, std::shared_ptr<task>> m_responses;

    axcl::event m_submit_event;
};

}  // namespace axcl::rt