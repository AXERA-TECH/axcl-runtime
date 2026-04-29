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

#include <cstddef>
#include <cstdint>
#include <memory>
#include "axcl_comm.hpp"
#include "axcl_rt_type.h"
#include "lock_queue.hpp"
#include "threadx.hpp"

struct RecvBuffer {
    void* data = nullptr;
    std::size_t size = 0;
};

class usrworkchannel {
public:
    usrworkchannel();
    ~usrworkchannel();

    int32_t open(const axcl::comm::comm_args_t& args);
    void close();

    int32_t send(const void* buf, std::size_t size, int32_t timeout);
    int32_t recv(void* buf, uint32_t bufsize, uint32_t* recvlen, int32_t timeout);

    std::size_t get_capacity() const;
    void set_capacity(std::size_t capacity);

    uint32_t get_pid() const {
        return m_pid;
    }
    void set_pid(uint32_t pid) {
        m_pid = pid;
    }

protected:
    usrworkchannel(const usrworkchannel&) = delete;
    usrworkchannel& operator=(const usrworkchannel&) = delete;

private:
    void recv_thread();
    void clear();

private:
    uint32_t m_pid = 0;
    std::shared_ptr<axcl::comm::icomm> m_comm;
    axcl::threadx m_thread;
    axcl::lock_queue<RecvBuffer> m_queue;
};
