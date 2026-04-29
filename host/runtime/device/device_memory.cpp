/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "device_memory.hpp"
#include "axcl_logger.hpp"

#define TAG "device"

void axcl::rt::device_memory::print() {
    if (!m_mem.empty()) {
        LOG_MM_C(TAG, "allocated device memories: {}", m_mem.size());
        for (auto &&m : m_mem) {
            LOG_MM_C(TAG, "[{:#x}] : {:#x} bytes", m.first, m.second);
        }
    }
}
