/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "event.hpp"
#include <chrono>
#include <string>

#include "event_manager.hpp"
#include "axcl_logger.hpp"

#define TAG "event"

namespace axcl::worker {

event::event(event_manager* mgr, uint32_t event_id) : m_event_manager(mgr), m_event_id(event_id) {
}

event::~event() {
}

bool event::wait(int32_t timeout_ms) {

    std::unique_lock<std::mutex> lock(m_mutex);
    m_status = status::waiting;

    if (timeout_ms < 0) {  // infinite wait
        m_cv.wait(lock, [this] { return m_signaled; });
        m_status = status::signaled;
        return true;
    } else if (timeout_ms == 0) {  // immediate return
        bool result = m_signaled;
        if (result) {
            m_status = status::signaled;
        } else {
            m_status = status::timeout;
        }
        return result;
    } else {  // timed wait
        auto result = m_cv.wait_for(lock,
            std::chrono::milliseconds(timeout_ms),
            [this] { return m_signaled; });

        if (result) {
            m_status = status::signaled;
        } else {
            m_status = status::timeout;
        }

        return result;
    }
}

void event::signal() {

    std::unique_lock<std::mutex> lock(m_mutex);
    m_signaled = true;
    m_status = status::signaled;
    m_cv.notify_all();
}

void event::reset() {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_signaled = false;
    m_status = status::created;
}

}  // namespace axcl::worker