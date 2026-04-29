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

#include <memory>
#include <mutex>
#include <atomic>
#include <unordered_map>
#include <map>
#include <condition_variable>
#include "axcl_pcie_dma_api.h"
#include "base.hpp"
#include "dma_buffer.hpp"
#include "handler.hpp"
#include "mem.hpp"
#include "msg.hpp"

enum { SEND_PORT = 0, RECV_PORT = 1, BUTT_PORT = 2 };

enum PORT_INDEX {
    DMA_SEND_PORT_IDX = 0,
    DMA_RECV_PORT_IDX = 1,
    MSG_SEND_PORT_IDX = 2,
    MSG_RECV_PORT_IDX = 3,
    PORT_INDEX_MAX = 4
};

struct MemcpyWaitInfo {
    std::condition_variable cv;
    bool done = false;
    size_t size = 0;
};

struct HeadLess {
    bool operator()(const axcl::pkg::head* a, const axcl::pkg::head* b) const {
        if (a == nullptr || b == nullptr) return a < b;
        return std::tie(a->seq_num, a->device, a->context, a->stream)
             < std::tie(b->seq_num, b->device, b->context, b->stream);
    }
};

class pcie : public base {
public:
    pcie();

    bool open(const axcl::comm::comm_args_t &args) override;
    void close() override;

    void clear() override;

    size_t send(const void *buf, size_t size, int64_t flag, int32_t timeout) override;
    size_t recv(void **buf, int64_t flag, int32_t timeout) override;
    bool memcpy_async(void* hd, int flag, int32_t timeout) override;

    void *alloc(size_t size);
    void free(void *buf) override;

    size_t send_data_by_msg(int32_t port, const void *data, size_t data_size, int32_t timeout);
    size_t recv_data_by_msg(int32_t port, void *data, size_t data_size, int32_t timeout);

    size_t send_data_by_dma(int32_t port, const void *data, size_t data_size, const struct dma_mem &src, int32_t timeout);
    size_t recv_data_by_dma(int32_t port, void *data, size_t data_size, const struct dma_mem &dst, int32_t timeout);

    bool is_ep() const {
        return 0 == m_target;
    }

    const struct dma_mem &get_dma_buffer(int32_t index) const {
        return m_dmabufs[index]->get();
    }

    int32_t get_port(PORT_INDEX index) const {
        if (index >= PORT_INDEX_MAX) {
            return -1;
        }
        return m_ports[index];
    }

private:
    bool handshake_with_ep(int32_t timeout);
    bool handshake_with_rc(int32_t timeout);

    bool send_handshake_sync_msg(int32_t port, uint32_t seq);
    bool recv_handshake_sync_msg(int32_t port, uint32_t &seq, int32_t timeout);
    bool send_handshake_sync_ack_msg(int32_t port, uint32_t seq);
    bool recv_handshake_sync_ack_msg(int32_t port, uint32_t &seq, int32_t timeout);
    bool send_handshake_done_msg(int32_t port, uint32_t seq);
    bool recv_handshake_done_msg(int32_t port, uint32_t &seq, int32_t timeout);

    bool send_transfer_request_msg(int32_t port, uint32_t type, uint32_t size);
    bool recv_transfer_request_msg(int32_t port, uint32_t &type, uint32_t &size, int32_t timeout);

    bool send_dma_request_mem_msg(int32_t port, uint64_t phy, uint32_t size);
    bool recv_dma_request_mem_msg(int32_t port, uint64_t &phy, uint32_t &size, int32_t timeout);

    bool send_dma_sglist_mem_msg(int32_t port, const msg_sglist_mem_t &sglist);
    bool recv_dma_sglist_mem_msg(int32_t port, msg_sglist_mem_t &sglist, int32_t timeout);

    bool send_dma_linked_list_msg(int32_t port, const msg_dma_linked_list_t& ll);
    bool recv_dma_linked_list_msg(int32_t port, msg_dma_linked_list_t& ll, int32_t timeout);

    bool send_dma_data_ready_msg(int32_t port, uint64_t phy, uint32_t size);
    bool recv_dma_data_ready_msg(int32_t port, uint64_t &phy, uint32_t &size, int32_t timeout);
    bool send_dma_task_done_msg(int32_t port, uint32_t size);
    bool recv_dma_task_done_msg(int32_t port, uint32_t &size, int32_t timeout);
    bool send_dma_read_done_msg(int32_t port, uint32_t size);
    bool recv_dma_read_done_msg(int32_t port, uint32_t &size, int32_t timeout);

    bool send_msg(int32_t port, const msg_data_t &msg);
    bool recv_msg(int32_t port, msg_data_t &msg, int32_t timeout);

    size_t send_msg(int32_t port, const void *msg, size_t len);
    size_t recv_msg(int32_t port, void *msg, size_t len, int32_t timeout);

protected:
    mem m_mem;

    std::unique_ptr<dma_buffer> m_dmabufs[BUTT_PORT];
    DMA_HANDLE m_ep_dma_fd;
    int32_t m_target;
    int32_t m_ports[PORT_INDEX_MAX]; /* [0]: dma send port, [1]: dma recv port [2]: msg send port [3]: msg recv port */

    std::mutex m_mtx_send;
    std::mutex m_mtx_recv;

    std::unordered_map<uint32_t, std::unique_ptr<handler>> m_handles[BUTT_PORT];
    std::atomic<bool> m_offline = {true};

    std::condition_variable m_memcpy_cv;
    std::mutex m_memcpy_mtx;
    std::map<axcl::pkg::head*, MemcpyWaitInfo, HeadLess> m_memcpy_wait_map;
};
