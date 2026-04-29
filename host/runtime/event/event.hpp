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

namespace axcl::rt {

class event_manager;
class event  {

public:
    event(event_manager* mgr, uint32_t event_id);
    virtual ~event();

    uint32_t get_event_id() const {
        return m_event_id;
    }

    event_manager* get_event_manager() {
        return m_event_manager;
    }

private:
    event_manager* m_event_manager;
    uint32_t m_event_id;
};

}  // namespace axcl::rt