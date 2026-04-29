/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "socket.hpp"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <zmq.h>
#include "axcl_logger.hpp"

#define TAG "socket"
#define INFINITE (-1)

socket::socket() : base(axcl::comm::COMM_TYPE::COMM_TYPE_SOCKET) {
}

bool socket::open(const axcl::comm::comm_args_t &args) {
    char addr_pub[64] = {0};
    char addr_sub[64] = {0};
    if (args.socket.type == axcl::comm::SOCKET_TYPE::SOCKET_TYPE_SERVER) {
        sprintf(addr_pub, "tcp://*:%d", args.socket.port);
        if (strlen(args.socket.ip) > 0) {
            sprintf(addr_sub, "tcp://%s:%d", args.socket.ip, args.socket.port - 1);
        } else {
            sprintf(addr_sub, "tcp://localhost:%d", args.socket.port - 1);
        }
        ::memcpy(m_addr, addr_pub, sizeof(addr_pub));
        ::memcpy(m_addr_pub, addr_pub, sizeof(addr_pub));
        ::memcpy(m_addr_sub, addr_sub, sizeof(addr_sub));
    } else if (args.socket.type == axcl::comm::SOCKET_TYPE::SOCKET_TYPE_CLIENT) {
        if (strlen(args.socket.ip) > 0) {
            sprintf(addr_sub, "tcp://%s:%d", args.socket.ip, args.socket.port);
        } else {
            sprintf(addr_sub, "tcp://localhost:%d", args.socket.port);
        }

        sprintf(addr_pub, "tcp://*:%d", args.socket.port - 1);
        ::memcpy(m_addr, addr_sub, sizeof(addr_sub));
        ::memcpy(m_addr_pub, addr_pub, sizeof(addr_pub));
        ::memcpy(m_addr_sub, addr_sub, sizeof(addr_sub));
    }
    LOG_MM_I(TAG, "[{}] open  +++", m_addr);

    int32_t rc = -1;
    // pub
    if (args.socket.type == axcl::comm::SOCKET_TYPE::SOCKET_TYPE_SERVER) {
        m_ctx_pub = zmq_ctx_new();
        if (!m_ctx_pub) {
            LOG_MM_E(TAG, "[{}] zmq_ctx_new pub fail: [{}]", m_addr, zmq_strerror(errno));
            return false;
        }

        errno = 0;
        m_pub = zmq_socket(m_ctx_pub, ZMQ_PUB);
        if (!m_pub) {
            this->release();
            LOG_MM_E(TAG, "[{}] zmq_socket pub fail: [{}]", m_addr, zmq_strerror(errno));
            return false;
        }

        errno = 0;
        rc = zmq_bind(m_pub, addr_pub);
        if (0 != rc) {
            this->release();
            LOG_MM_E(TAG, "[{}] zmq_bind pub {} fail: [{}], rc = {}\n", m_addr, addr_pub, zmq_strerror(errno), rc);
            return false;
        }

        LOG_MM_I(TAG, "[{}] open: {}", m_addr, addr_pub);

        sleep(1);
        this->send_cmd2sub(m_pub);
    }

    // sub
    m_ctx_sub = zmq_ctx_new();
    if (!m_ctx_sub) {
        LOG_MM_E(TAG, "[{}] zmq_ctx_new sub fail: [{}]", m_addr, zmq_strerror(errno));
        return false;
    }

    errno = 0;
    m_sub = zmq_socket(m_ctx_sub, ZMQ_SUB);
    if (!m_sub) {
        this->release();
        LOG_MM_E(TAG, "[{}] zmq_socket sub fail: [{}]", m_addr, zmq_strerror(errno));
        return false;
    }

    errno = 0;
    rc = zmq_connect(m_sub, addr_sub);
    if (0 != rc) {
        this->release();
        LOG_MM_E(TAG, "[{}] zmq_connect sub {} fail: [{}], rc = {}\n", m_addr, addr_sub, zmq_strerror(errno), rc);
        return false;
    }

    errno = 0;
    rc = zmq_setsockopt(m_sub, ZMQ_SUBSCRIBE, "", 0);
    if (rc < 0) {
        this->release();
        LOG_MM_E(TAG, "[{}] zmq_setsockopt ZMQ_SUBSCRIBE fail: [{}]", m_addr_sub, zmq_strerror(errno));
        return false;
    }

    LOG_MM_I(TAG, "[{}] open: {}", m_addr, addr_sub);

    if (args.socket.type == axcl::comm::SOCKET_TYPE::SOCKET_TYPE_CLIENT) {
        this->recv_cmdfrompub(m_sub);
    }

    LOG_MM_I(TAG, "[{}] open  ---", m_addr);
    return true;
}

void socket::close() {
    LOG_MM_I(TAG, "[{}] close +++", m_addr);

    this->release();

    LOG_MM_I(TAG, "[{}] close ---", m_addr);
    memset(m_addr, 0, sizeof(m_addr));
}

void socket::clear() {
}

size_t socket::send(const void *buf, size_t size, int64_t flag, int32_t timeout) {
    std::lock_guard<std::mutex> lck(m_mtx_send);

    if (!m_pub) {
        LOG_MM_E(TAG, "[{}] socket is null!", m_addr_pub);
        return 0;
    }

    errno = 0;
    if (zmq_setsockopt(m_pub, ZMQ_SNDTIMEO, &timeout, sizeof(timeout)) < 0) {
        LOG_MM_E(TAG, "[{}] zmq_setsockopt fail: [{}]", m_addr_pub, zmq_strerror(errno));
        return 0;
    }

    int32_t rc = 0;
    errno = 0;
    zmq_msg_t msg;
    zmq_msg_init_size(&msg, size);
    ::memcpy(zmq_msg_data(&msg), buf, size);
    rc = zmq_msg_send(&msg, m_pub, 0);
    if (rc < 0) {
        LOG_MM_E(TAG, "[{}] zmq_msg_send fail: [{}] / [{}]", m_addr_pub, rc, zmq_strerror(errno));
        rc = 0;
    } else {
        // LOG_MM_I(TAG, "[{}] send data: {}/{}", m_addr_pub, rc, size);
    }
    zmq_msg_close(&msg);
    return rc;
}

void socket::free(void *buf) {
    m_mem.free(buf);
}

size_t socket::recv(void **buf, int64_t flag, int32_t timeout) {
    std::lock_guard<std::mutex> lck(m_mtx_recv);

    if (!buf) {
        LOG_MM_E(TAG, "[{}] nil parameter", m_addr_sub);
        return 0;
    }

    if (!m_sub) {
        LOG_MM_E(TAG, "[{}] socket is null!", m_addr_sub);
        return 0;
    }

    errno = 0;
    if (zmq_setsockopt(m_sub, ZMQ_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        LOG_MM_E(TAG, "[{}] zmq_setsockopt fail: [{}]", m_addr_sub, zmq_strerror(errno));
        return 0;
    }

    int32_t rc = 0;

    zmq_msg_t msg;
    zmq_msg_init(&msg);

    errno = 0;
    while (true) {
        rc = zmq_msg_recv(&msg, m_sub, 0);
        if (rc > 0) {
            break;
        } else {
            LOG_MM_E(TAG, "[{}] zmq_msg_recv fail: [{}] / [{}]", m_addr_sub, rc, zmq_strerror(errno));
            rc = 0;
        }
    }

    int32_t total_size = rc;
    void *mem = m_mem.malloc(total_size);
    ::memcpy(mem, zmq_msg_data(&msg), rc);

    *buf = mem;
    // LOG_MM_I(TAG, "[{}] recv data: {}", m_addr_sub, rc);
    zmq_msg_close(&msg);
    return rc;
}

size_t socket::send_cmd2sub(void *sock) {
    char cmd[] = "create: pub";
    int32_t len = strlen(cmd);

    int32_t rc = 0;
    errno = 0;
    zmq_msg_t msg;
    zmq_msg_init_size(&msg, len);
    ::memcpy(zmq_msg_data(&msg), cmd, len);
    rc = zmq_msg_send(&msg, sock, 0);
    if (rc < 0) {
        LOG_MM_E(TAG, "[{}] send_cmd2sub fail: [{}] / [{}]", m_addr_pub, rc, zmq_strerror(errno));
        rc = 0;
    }
    // else {
    //     LOG_MM_I(TAG, "[{}] send data: {}/{}", m_addr_pub, rc, len);
    // }
    zmq_msg_close(&msg);
    return rc;
}

size_t socket::recv_cmdfrompub(void *sock) {
    int32_t rc = 0;
    zmq_msg_t msg;
    zmq_msg_init(&msg);
    while (true) {
        errno = 0;
        rc = zmq_msg_recv(&msg, sock, 0);
        if (rc > 0) {
            char *result = strstr((char *)zmq_msg_data(&msg), "create: pub");
            if (nullptr != result) {
                this->create_internal_pub();
                sleep(1);
                break;
            }
        } else {
            LOG_MM_E(TAG, "[{}] zmq_msg_recv fail: [{}] / [{}]", m_addr_sub, rc, zmq_strerror(errno));
            rc = 0;
        }
    }
    zmq_msg_close(&msg);

    return rc;
}

bool socket::create_internal_pub() {
    m_ctx_pub = zmq_ctx_new();
    if (!m_ctx_pub) {
        LOG_MM_E(TAG, "[{}] zmq_ctx_new pub fail: [{}]", m_addr, zmq_strerror(errno));
        return false;
    }

    errno = 0;
    m_pub = zmq_socket(m_ctx_pub, ZMQ_PUB);
    if (!m_pub) {
        this->release();
        LOG_MM_E(TAG, "[{}] zmq_socket pub fail: [{}]", m_addr, zmq_strerror(errno));
        return false;
    }

    errno = 0;
    int32_t rc = zmq_bind(m_pub, m_addr_pub);
    if (0 != rc) {
        this->release();
        LOG_MM_E(TAG, "[{}] zmq_bind pub {} fail: [{}], rc = {}\n", m_addr, m_addr_pub, zmq_strerror(errno), rc);
        return false;
    }

    LOG_MM_I(TAG, "[{}] open: {}", m_addr, m_addr_pub);

    return true;
}

void socket::release() {
    if (m_pub) {
        zmq_close(m_pub);
        m_pub = NULL;
    }

    if (m_ctx_pub) {
        zmq_ctx_destroy(m_ctx_pub);
        m_ctx_pub = NULL;
    }

    if (m_sub) {
        zmq_close(m_sub);
        m_sub = NULL;
    }

    if (m_ctx_sub) {
        zmq_ctx_destroy(m_ctx_sub);
        m_ctx_sub = NULL;
    }
}

bool socket::memcpy_async(void* hd, int flag, int32_t timeout) {
    return true;
}
