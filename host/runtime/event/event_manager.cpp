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
#include "axcl_rt_event.h"
#include "axcl_rt_control_internal.h"
#include "context/context.hpp"
#include "stream/stream.hpp"
#include "axcl_logger.hpp"
#include "event/event.hpp"

#define TAG "event manager"

namespace axcl::rt {

event_manager::event_manager(device *dev) : m_device(dev), m_event_id(1) {
}

event *event_manager::create_event() {

    std::lock_guard<std::mutex> lck(m_mtx);

    auto sp = std::make_shared<event>(this, m_event_id);
    if (!sp) {
        LOG_MM_E(TAG, "create event instance fail");
        return nullptr;
    }

    auto *s = sp.get();
    m_events[m_event_id] = sp;

    /* control: create event */
    if (axclError ret = axclrtControlCreateEvent(static_cast<int32_t>(m_event_id)); ret != AXCL_SUCC) {
        LOG_MM_E(TAG, "control: create event({}:{}) failed, ret {:#x}", m_event_id, static_cast<uint32_t>(ret));
    }

    LOG_MM_I(TAG, "event {} is created", m_event_id);
    ++m_event_id;
    return s;
}

bool event_manager::destory_event(event *event) {

    std::lock_guard<std::mutex> lck(m_mtx);

    auto event_id = event->get_event_id();

    auto it = m_events.find(event_id);
    if (m_events.end() == it) {
        LOG_MM_E(TAG, "unknown event {}", event_id);
        return false;
    }

    /* control: destroy event */
    if (axclError ret = axclrtControlDestroyEvent(static_cast<int32_t>(event_id)); ret != AXCL_SUCC) {
        LOG_MM_E(TAG, "control: destroy event({}) failed, ret {:#x}", m_event_id, static_cast<uint32_t>(ret));
    }

    m_events.erase(it);

    LOG_MM_I(TAG, "event {} is destoried", event_id);
    return true;
}

bool event_manager::record_event(event* event, stream* stream) {
    std::lock_guard<std::mutex> lck(m_mtx);

    auto event_id = event->get_event_id();
    if (m_events.end() == m_events.find(event_id)) {
        LOG_MM_E(TAG, "unknown event {}", event_id);
        return false;
    }

    /* control: destroy event */
    if (axclError ret = axclrtControlRecordEvent(static_cast<void*>(stream), event_id); ret != AXCL_SUCC) {
        LOG_MM_E(TAG, "control: record event({}) to stream({}) failed, ret {:#x}", m_event_id, stream->get_stream_id(), static_cast<uint32_t>(ret));
    }

    return true;
}

bool event_manager::wait_event(event* event, stream* stream, int32_t timeout_ms) {
    std::lock_guard<std::mutex> lck(m_mtx);

    auto event_id = event->get_event_id();
    if (m_events.end() == m_events.find(event_id)) {
        LOG_MM_E(TAG, "unknown event {}", event_id);
        return false;
    }

    /* control: destroy event */
    if (axclError ret = axclrtControlStreamWaitEvent(static_cast<void*>(stream), event_id, timeout_ms); ret != AXCL_SUCC) {
        LOG_MM_E(TAG, "control: wait event({}) in stream({}) failed, ret {:#x}", m_event_id, stream->get_stream_id(), static_cast<uint32_t>(ret));
    }

    return true;
}

}  // namespace axcl::rt