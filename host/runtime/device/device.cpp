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
#include "context/context_manager.hpp"
#include "device/device_manager.hpp"
#include "event/event_manager.hpp"
#include "axcl_logger.hpp"
#include "os.hpp"

#define TAG "device"

namespace axcl::rt {

device::device(device_manager* mgr, axcl::comm::COMM_TYPE type) : m_device_manager(mgr), m_comm_type(type) {
    m_comm = axcl_create_comm_instance(type);
    m_channel = std::make_unique<channel>(m_comm);
    m_context_manager = std::make_shared<context_manager>(this);
    m_event_manager = std::make_shared<event_manager>(this);
}

bool device::open(const struct device_info& info) {
    m_info = info;
    LOG_MM_I(TAG, "{} +++", m_info.device);

    axcl::comm::comm_args_t args;
    switch (m_comm_type) {
        case axcl::comm::COMM_TYPE::COMM_TYPE_PCIE:
            if (m_info.port_num < 4) {
                LOG_MM_E(TAG, "pcie device {} needs at least 4 ports", m_info.device);
                return false;
            }

            args.pcie.handshake = true;
            args.pcie.target = m_info.device;
            args.pcie.port[0] = m_info.ports[0];
            args.pcie.port[1] = m_info.ports[1];
            args.pcie.port[2] = m_info.ports[2];
            args.pcie.port[3] = m_info.ports[3];
            break;
        default:
            break;
    }

    m_comm->set_attr(axcl::comm::icomm::DMA_BUF_SIZE_ATTR, axcl::comm::attr{static_cast<uint32_t>(m_info.dma_buf_size)});
    if (!m_comm->open(args)) {
        return false;
    }

    if (!m_channel->start()) {
        m_comm->close();
        return false;
    }

    m_opened = true;
    LOG_MM_I(TAG, "{} ---", m_info.device);
    return true;
}

void device::close() {
    LOG_MM_I(TAG, "{} +++", m_info.device);

    if (m_opened) {
        m_channel->offline();
        m_comm->close();
        m_channel->stop();

        m_alloc_mem.print();

        m_opened = false;
    }

    LOG_MM_I(TAG, "{} ---", m_info.device);
}

void device::sync() {
    m_context_manager->sync();
}

void device::offline() {
    m_channel->offline();
}

void device::online() {
    m_channel->online();
}

}  // namespace axcl::rt
