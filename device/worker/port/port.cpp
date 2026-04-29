/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "port.hpp"
#include <pthread.h>
#include <algorithm>
#include "axcl_logger.hpp"
#include "package/include/type.hpp"

#define TAG "port"

namespace axcl::worker {

port::port(axcl::comm::COMM_TYPE type, std::array<uint32_t, 4> ports, uint32_t dma_buf_size) : m_comm_type(type), m_ports(ports) {
    m_comm = axcl_create_comm_instance(type);
    if (dma_buf_size > 0) {
        m_comm->set_attr(axcl::comm::icomm::DMA_BUF_SIZE_ATTR, dma_buf_size);
    }
    m_channel = std::make_unique<channel>(m_comm, ports);
    m_dispatcher = std::make_unique<dispatcher>(this);
}

bool port::open() {
    LOG_MM_I(TAG, "port {} {} {} {} +++", m_ports[0], m_ports[1], m_ports[2], m_ports[3]);

    axcl::comm::comm_args_t args;
    switch (m_comm_type) {
        case axcl::comm::COMM_TYPE::COMM_TYPE_PCIE:
            args.pcie.handshake = true;
            args.pcie.target = 0;
            args.pcie.port[0] = m_ports[0];
            args.pcie.port[1] = m_ports[1];
            args.pcie.port[2] = m_ports[2];
            args.pcie.port[3] = m_ports[3];
            break;
        default:
            break;
    }
    if (!m_comm->open(args)) {
        return false;
    }

    LOG_MM_I(TAG, "port {} {} {} {} ---", m_ports[0], m_ports[1], m_ports[2], m_ports[3]);

    return true;
}

bool port::start() {
    LOG_MM_I(TAG, "port {} {} {} {} +++", m_ports[0], m_ports[1], m_ports[2], m_ports[3]);

    if (!m_channel->start()) {
        m_comm->close();
        return false;
    }

    LOG_MM_I(TAG, "port {} {} {} {} ---", m_ports[0], m_ports[1], m_ports[2], m_ports[3]);

    return true;
}

void port::close() {
    LOG_MM_I(TAG, "port {} {} {} {} +++", m_ports[0], m_ports[1], m_ports[2], m_ports[3]);

    m_channel->offline();
    m_comm->close();
    m_channel->stop();

    LOG_MM_I(TAG, "port {} {} {} {} ---", m_ports[0], m_ports[1], m_ports[2], m_ports[3]);
}

bool port::send(const void *data, size_t size, int64_t flag, int32_t timeout) {
    return m_channel->send(data, size, flag, timeout);
}

bool port::register_sink(sinker *sink) {
    LOG_MM_I(TAG, "port {} {} {} {} +++", m_ports[0], m_ports[1], m_ports[2], m_ports[3]);

    bool ret = m_channel->register_sink(sink);

    LOG_MM_I(TAG, "port {} {} {} {} ---", m_ports[0], m_ports[1], m_ports[2], m_ports[3]);

    return ret;
}

bool port::unregister_sink(sinker *sink) {
    LOG_MM_I(TAG, "port {} {} {} {} +++", m_ports[0], m_ports[1], m_ports[2], m_ports[3]);

    bool ret = m_channel->unregister_sink(sink);

    LOG_MM_I(TAG, "port {} {} {} {} ---", m_ports[0], m_ports[1], m_ports[2], m_ports[3]);

    return ret;
}

}  // namespace axcl::worker
