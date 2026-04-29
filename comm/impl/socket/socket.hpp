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

#include <mutex>
#include "base.hpp"
#include "mem.hpp"

class socket : public base {
public:
    socket();

    bool open(const axcl::comm::comm_args_t &args) override;
    void close() override;

    void clear() override;

    size_t send(const void *buf, size_t size, int64_t flag, int32_t timeout) override;
    size_t recv(void **buf, int64_t flag, int32_t timeout) override;
    bool memcpy_async(void* hd, int flag, int32_t timeout) override;

    void free(void *buf) override;

private:
    size_t send_cmd2sub(void *sock);
    size_t recv_cmdfrompub(void *sock);
    bool create_internal_pub();

    void release();

private:
    mem m_mem;

    void *m_ctx_pub = NULL;
    void *m_ctx_sub = NULL;
    void *m_pub = NULL;
    void *m_sub = NULL;

    char m_addr[64] = "";
    char m_addr_pub[64] = "";
    char m_addr_sub[64] = "";

    std::mutex m_mtx_send;
    std::mutex m_mtx_recv;
};
