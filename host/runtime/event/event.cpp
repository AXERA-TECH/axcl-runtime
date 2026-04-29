/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "event/event.hpp"
#include <chrono>
#include <string>

#include "event/event_manager.hpp"
#include "axcl_logger.hpp"

#define TAG "event"

namespace axcl::rt {

event::event(event_manager* mgr, uint32_t event_id) : m_event_manager(mgr), m_event_id(event_id) {
}

event::~event() {
}

}  // namespace axcl::rt