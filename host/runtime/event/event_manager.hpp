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

namespace axcl::rt {

class device;
class event;
class stream;
class event_manager {
public:
    event_manager(device *dev);

    event* create_event();
    bool destory_event(event* event);
    bool record_event(event* event, stream* stream);
    bool wait_event(event* event, stream* stream, int32_t timeout_ms);

private:
    device *m_device;
    uint32_t m_event_id;
    std::mutex m_mtx;
    std::unordered_map<uint32_t /* event id */, std::shared_ptr<event>> m_events;
};

}  // namespace axcl::rt
