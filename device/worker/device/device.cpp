/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "device.hpp"
#include <string.h>
#include "axcl_logger.hpp"
#include "os.hpp"

#define TAG "device"

namespace axcl::worker {

device::device(axcl::comm::COMM_TYPE type, const std::array<uint32_t, 4> ports, uint32_t dma_buf_size)
    : m_comm_type(type) {
    m_ports = ports;

    m_msg_port = std::make_shared<port>(type, ports, dma_buf_size);
}

bool device::open() {
    LOG_MM_I(TAG, "+++");

    // open msg port
    if (!m_msg_port->open()) {
        return false;
    }

    LOG_MM_I(TAG, "---");
    return true;
}

bool device::start() {
    LOG_MM_I(TAG, "+++");

    if (!m_msg_port->start()) {
        return false;
    }

    LOG_MM_I(TAG, "---");
    return true;
}

void device::close() {
    LOG_MM_I(TAG, "+++");

    m_msg_port->close();

    LOG_MM_I(TAG, "---");
}

}  // namespace axcl::worker
