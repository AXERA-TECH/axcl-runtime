/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "event_manager.hpp"
#include "event.hpp"
#include "axcl_logger.hpp"

#define TAG "event manager"

namespace axcl::worker {

event_manager::event_manager() {
}

event *event_manager::create_event(uint32_t event_id) {

    std::lock_guard<std::mutex> lck(m_mtx);

    auto it = m_events.find(event_id);
    if (m_events.end() != it) {
        LOG_MM_E(TAG, "event exist {}", event_id);
        return nullptr;
    }

    auto sp = std::make_shared<event>(this, event_id);
    if (!sp) {
        LOG_MM_E(TAG, "create event instance fail");
        return nullptr;
    }

    auto *s = sp.get();
    m_events[event_id] = sp;

    LOG_MM_I(TAG, "event {} is created", event_id);
    return s;
}

bool event_manager::destory_event(uint32_t event_id) {

    std::shared_ptr<event> event_to_destroy;
    {
        std::lock_guard<std::mutex> lck(m_mtx);
        auto it = m_events.find(event_id);
        if (m_events.end() == it) {
            LOG_MM_E(TAG, "Stream({}) not found, cannot destroy", event_id);
            return false;
        }

        event_to_destroy = it->second;
        m_events.erase(it);
        LOG_M_I(TAG, "event({}) removed from manager", event_id);
    }

    return true;
}

bool event_manager::record_event(uint32_t event_id, uint32_t stream_id) {

    std::shared_ptr<event> event_to_record;
    {
        std::lock_guard<std::mutex> lck(m_mtx);
        auto it = m_events.find(event_id);
        if (m_events.end() == it) {
            LOG_MM_E(TAG, "unknown event {}", event_id);
            return false;
        }

        event_to_record = it->second;
    }

    event_to_record->signal();

    LOG_MM_I(TAG, "event {} signaled for stream {}", event_id, stream_id);
    return true;
}

bool event_manager::wait_event(uint32_t event_id, uint32_t stream_id, int32_t timeout_ms) {
    std::shared_ptr<event> event_to_wait;
    {
        std::lock_guard<std::mutex> lck(m_mtx);
        auto it = m_events.find(event_id);
        if (m_events.end() == it) {
            LOG_MM_E(TAG, "unknown event {}", event_id);
            return false;
        }

        event_to_wait = it->second;
    }

    bool result = event_to_wait->wait(timeout_ms);
    if (!result) {
        LOG_MM_E(TAG, "wait event {} timeout or error for stream {}", event_id, stream_id);
    } else {
        LOG_MM_I(TAG, "wait event {} success for stream {}", event_id, stream_id);
    }

    return result;
}

}  // namespace axcl::worker