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

namespace axcl::worker {

class event_manager;
class event  {

public:
    enum class status {
        created,    // Event just created
        signaled,   // Event has been signaled
        waiting,    // Event is waiting
        timeout,    // Event wait timeout
        error       // Event error occurred
    };

    event(event_manager* mgr, uint32_t event_id);
    virtual ~event();

    uint32_t get_event_id() const {
        return m_event_id;
    }

    event_manager* get_event_manager() {
        return m_event_manager;
    }

    bool wait(int32_t timeout_ms = -1);
    void signal();
    void reset();

    status get_status() const {
        return m_status;
    }

private:
    event_manager* m_event_manager;
    uint32_t m_event_id;

    std::mutex m_mutex;
    std::condition_variable m_cv;
    status m_status{status::created};
    bool m_signaled{false};
};

}  // namespace axcl::worker