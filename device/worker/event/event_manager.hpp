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

#include <cstdint>
#include <memory>
#include <mutex>
#include <unordered_map>

namespace axcl::worker {

class event;
class stream;
class event_manager {
public:
    event_manager();

    event* create_event(uint32_t event_id);
    bool destory_event(uint32_t event_id);
    bool record_event(uint32_t event_id, uint32_t stream_id);
    bool wait_event(uint32_t event_id, uint32_t stream_id, int32_t timeout_ms);

private:
    std::mutex m_mtx;
    std::unordered_map<uint32_t /* event id */, std::shared_ptr<event>> m_events;
};

}  // namespace axcl::event
