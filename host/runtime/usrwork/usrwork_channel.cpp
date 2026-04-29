/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/
#include "usrwork_channel.hpp"
#include <cstring>
#include <string>
#include "axcl_rt_usrwork_type.h"
#include "axcl_logger.hpp"

#define TAG "usrworkchannel"

namespace axcl::rt {

usrworkchannel::usrworkchannel() {
}

usrworkchannel::~usrworkchannel() {
    close();
}

int32_t usrworkchannel::open(const axcl::comm::comm_args_t& args) {
    if (m_comm) {
        LOG_MM_W(TAG, "channel for pid {} is already open", m_pid);
        return AXCL_SUCC;
    }

    m_comm = axcl_create_comm_instance(axcl::comm::COMM_TYPE::COMM_TYPE_PCIE);
    if (!m_comm) {
        LOG_MM_E(TAG, "create channel instance for pid {} fail", m_pid);
        return AXCL_ERR_USRWORK_OPEN_CHANNEL;
    }

    if (!m_comm->open(args)) {
        m_comm = nullptr;
        LOG_MM_E(TAG, "open channel for pid {} fail", m_pid);
        return AXCL_ERR_USRWORK_OPEN_CHANNEL;
    }

    m_args = args;
    std::string name = "usrchn_" + std::to_string(m_pid);
    m_thread.start(name, &usrworkchannel::recv_thread, this);

    LOG_MM_D(TAG, "open channel for pid {} with target {} succ", m_pid, args.pcie.target);
    return AXCL_SUCC;
}

void usrworkchannel::close() {
    if (!m_comm) {
        return;
    }

    m_thread.stop();
    m_queue.wakeup();
    m_comm->close();
    m_thread.join();

    clear();

    m_comm = nullptr;
    LOG_MM_I(TAG, "close channel for pid {} succ", m_pid);
}

int32_t usrworkchannel::send(const void* buf, size_t size, int32_t timeout) {
    if (!buf || size == 0) {
        LOG_MM_E(TAG, "illegal parameters");
        return AXCL_ERR_USRWORK_ILLEGAL_PARAM;
    }

    if (!m_comm) {
        LOG_MM_E(TAG, "channel for pid {} is not open", m_pid);
        return AXCL_ERR_USRWORK_OPEN_CHANNEL;
    }

    size_t sent = m_comm->send(buf, size, axcl::comm::PCIE_DMA, timeout);
    if (sent != size) {
        LOG_MM_E(TAG, "send data for pid {} fail, sent {}/{} bytes", m_pid, sent, size);
        return AXCL_ERR_USRWORK_SEND_DATA;
    }

    LOG_MM_D(TAG, "send{} bytes for pid {} succ", size, m_pid);
    return AXCL_SUCC;
}

int32_t usrworkchannel::recv(void* buf, uint32_t bufsize, uint32_t* recvlen, int32_t timeout) {
    if (!buf || 0 == bufsize) {
        LOG_MM_E(TAG, "illegal parameters");
        return AXCL_ERR_USRWORK_ILLEGAL_PARAM;
    }

    if (!m_comm) {
        LOG_MM_E(TAG, "channel for pid {} is not open", m_pid);
        return AXCL_ERR_USRWORK_OPEN_CHANNEL;
    }

    if (recvlen) {
        *recvlen = 0;
    }

    RecvBuffer data;
    if (!m_queue.pop(data, timeout)) {
        return AXCL_ERR_USRWORK_TIMEOUT;
    }

    if (recvlen) {
        *recvlen = static_cast<uint32_t>(data.size);
    }

    if (bufsize < data.size) {
        std::memcpy(buf, data.data, bufsize);
        m_comm->free(data.data);
        LOG_MM_E(TAG, "user buffer too small, {} < {}", bufsize, data.size);
        return AXCL_ERR_USRWORK_BUFFER_TOO_SMALL;
    } else {
        std::memcpy(buf, data.data, data.size);
        m_comm->free(data.data);
    }

    LOG_MM_D(TAG, "recv {} bytes for pid {} succ", data.size, m_pid);
    return AXCL_SUCC;
}

size_t usrworkchannel::get_capacity() const {
    return m_queue.get_capacity();
}

void usrworkchannel::set_capacity(size_t capacity) {
    m_queue.set_capacity(capacity);
    LOG_MM_I(TAG, "set queue capacity to {} for pid {} succ", capacity, m_pid);
}

void usrworkchannel::clear() {
    size_t count = m_queue.size();
    for (size_t i = 0; i < count; i++) {
        RecvBuffer data;
        m_queue.pop(data, 0);
        if (m_comm) {
            m_comm->free(data.data);
        }
    }

    LOG_MM_I(TAG, "clear queue for pid {} succ", m_pid);
}

void usrworkchannel::recv_thread() {
    LOG_MM_I(TAG, "recv thread for pid {} +++", m_pid);

    constexpr int32_t timeout = -1;
    while (m_thread.running()) {
        RecvBuffer data;
        data.size = m_comm->recv(&data.data, axcl::comm::PCIE_DMA, timeout);
        if (data.size > 0) {
            m_queue.push(data);
        }
    }

    LOG_MM_I(TAG, "recv thread for pid {} ---", m_pid);
}

}  // namespace axcl::rt