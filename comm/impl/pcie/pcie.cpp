/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "pcie.hpp"
#include <string.h>
#include <algorithm>
#include <random>
#include "axcl_pcie_dma_api.h"
#include "axcl_pcie_msg_api.h"

#include "axcl_logger.hpp"
#include "memcpy_handler.hpp"
#include <condition_variable>

#define TAG "pcie"

#if defined(WINDOWS)
#else
#define INFINITE (-1)
#endif
#define HANDSHAKE_TIMEOUT (5000)

/* PCIe RC/EP index definition */
#define PCIE_RC_IDX (0)
#define PCIE_EP_IDX (1)

#define USE_HANDLE_ASYNC (0)

static constexpr uint32_t MIN_DMA_BUF_SIZE = 0x100000;
static constexpr size_t MEMCPY_PACKAGE_SIZE = sizeof(struct axcl::pkg::memcpy_package);

pcie::pcie() : base(axcl::comm::COMM_TYPE::COMM_TYPE_PCIE), m_ep_dma_fd(-1), m_target(-1) {
    for (auto &&m : m_ports) {
        m = -1;
    }
}

bool pcie::open(const axcl::comm::comm_args_t &args) {
    AX_S32 ret;

    m_target = args.pcie.target < 0 ? 0 : args.pcie.target;
    if (is_ep()) {
        m_ports[DMA_SEND_PORT_IDX] = args.pcie.port[1];
        m_ports[DMA_RECV_PORT_IDX] = args.pcie.port[0];
        m_ports[MSG_SEND_PORT_IDX] = args.pcie.port[3];
        m_ports[MSG_RECV_PORT_IDX] = args.pcie.port[2];
    } else {
        m_ports[DMA_SEND_PORT_IDX] = args.pcie.port[0];
        m_ports[DMA_RECV_PORT_IDX] = args.pcie.port[1];
        m_ports[MSG_SEND_PORT_IDX] = args.pcie.port[2];
        m_ports[MSG_RECV_PORT_IDX] = args.pcie.port[3];
    }

    LOG_MM_I(TAG, "open target {} ports [{},{} - {},{}] +++", m_target,
             m_ports[DMA_SEND_PORT_IDX], m_ports[DMA_RECV_PORT_IDX],
             m_ports[MSG_SEND_PORT_IDX], m_ports[MSG_RECV_PORT_IDX]);

    for (size_t i = 0; i < sizeof(m_ports) / sizeof(m_ports[0]); ++i) {
        if (ret = AX_PCIe_OpenMsgPort(m_target, m_ports[i]); ret < 0) {
            LOG_MM_E(TAG, "AX_PCIe_OpenMsgPort(target: {} port: {}) fail, ret = {:#x}", m_target, m_ports[i], static_cast<uint32_t>(ret));
            for (size_t j = 0; j < i; ++j) {
                AX_PCIe_CloseMsgPort(m_target, m_ports[j]);
            }

            m_target = -1;
            return false;
        }
    }

    m_offline = false;

    if (args.pcie.handshake) {
        if (bool ret = is_ep() ? handshake_with_rc(HANDSHAKE_TIMEOUT) : handshake_with_ep(HANDSHAKE_TIMEOUT); !ret) {
            LOG_MM_E(TAG, "handshake fail");
            for (auto &&m : m_ports) {
                AX_PCIe_CloseMsgPort(m_target, m);
            }

            m_offline = true;
            m_target = -1;
            return false;
        }
    }

    DMA_HANDLE ep_dma_fd = -1;
    if (is_ep()) {
        if (ep_dma_fd = AX_PCIe_OpenDmaDev(); ep_dma_fd < 0) {
            LOG_MM_E(TAG, "AX_PCIe_OpenDmaDev(target: {}) fail", m_target);
            for (auto &&m : m_ports) {
                AX_PCIe_CloseMsgPort(m_target, m);
            }

            m_offline = true;
            m_target = -1;
            return false;
        }
    }

    size_t dma_buf_size = PCIE_DMA_BUFFER_SIZE;
    axcl::comm::attr dma_buf_size_value;
    if (get_attr(axcl::comm::icomm::DMA_BUF_SIZE_ATTR, dma_buf_size_value)) {
        if (auto result = std::get_if<uint32_t>(&dma_buf_size_value)) {
            dma_buf_size = static_cast<size_t>(*result);
            if (dma_buf_size < MIN_DMA_BUF_SIZE) {
                LOG_MM_W(TAG, "dma buf size {} is too small, adjust to min. size {}", dma_buf_size, MIN_DMA_BUF_SIZE);
                dma_buf_size = MIN_DMA_BUF_SIZE;
            }
        }
    }
    LOG_MM_I(TAG, "dma buf size = {:#x}", dma_buf_size);

    for (size_t i = 0; i < sizeof(m_dmabufs) / sizeof(m_dmabufs[0]); ++i) {
        /* ep always allocates continious memory */
        m_dmabufs[i] = std::make_unique<dma_buffer>(m_target);

#ifdef AXCL_DISABLE_SGLIST
        if (!m_dmabufs[i]->alloc(dma_buf_size, true, false)) {
#else
        if (!m_dmabufs[i]->alloc(dma_buf_size, true, !is_ep())) {
#endif
            if (ep_dma_fd > 0) {
                AX_PCIe_CloseDmaDev(ep_dma_fd);
                ep_dma_fd = -1;
            }

            for (auto &&m : m_ports) {
                AX_PCIe_CloseMsgPort(m_target, m);
            }

            for (size_t j = 0; j < i; ++j) {
                m_dmabufs[j]->free();
                m_dmabufs[j] = nullptr;
            }

            m_offline = true;
            m_target = -1;
            return false;
        }

        const auto &mem = m_dmabufs[i]->get();
        if (mem.scattered) {
            LOG_MM_I(TAG, "target: {}, dma sg buffers[{}] num {}, cached {}", m_target, i, mem.blk_cnt, mem.cached);
            for (uint32_t j = 0; j < mem.blk_cnt; ++j) {
                LOG_MM_I(TAG, " sg[{}]: phy {:#x}, vir {:#x}, size {}", j, mem.blks[j].phy, reinterpret_cast<uint64_t>(mem.blks[j].vir),
                         mem.blks[j].size);
            }
        } else {
            LOG_MM_I(TAG, "target: {}, dma buffer[{}]: phy {:#x}, vir {:#x}, cached {}", m_target, i, mem.blks[0].phy,
                     reinterpret_cast<uint64_t>(mem.blks[0].vir), mem.cached);
        }
    }

    /* register handlers */
    if (is_ep()) {
        m_handles[PCIE_EP_IDX][RUNTIME_MEMCPY_HANDLER_TYPE] = std::make_unique<memcpy_handler>(this);
    } else {
        m_handles[PCIE_RC_IDX][RUNTIME_MEMCPY_HANDLER_TYPE] = std::make_unique<memcpy_handler>(this);
    }

    m_ep_dma_fd = ep_dma_fd;
    LOG_MM_I(TAG, "open target {} ports [{},{} - {},{}] ---", m_target,
             m_ports[DMA_SEND_PORT_IDX], m_ports[DMA_RECV_PORT_IDX],
             m_ports[MSG_SEND_PORT_IDX], m_ports[MSG_RECV_PORT_IDX]);
    return true;
}

void pcie::close() {
    if (m_target < 0) {
        LOG_MM_W(TAG, "invalid target id {}", m_target);
        return;
    }

    LOG_MM_I(TAG, "close target {} ports [{},{} - {},{}] +++", m_target, m_ports[DMA_SEND_PORT_IDX], m_ports[DMA_RECV_PORT_IDX],
             m_ports[MSG_SEND_PORT_IDX], m_ports[MSG_RECV_PORT_IDX]);

    m_offline = true;

    for (auto &&m : m_dmabufs) {
        if (m) {
            m->free();
        }
    }

    for (auto &&m : m_ports) {
        AX_PCIe_CloseMsgPort(m_target, m);
    }

    if (m_ep_dma_fd > 0) {
        AX_PCIe_CloseDmaDev(m_ep_dma_fd);
        m_ep_dma_fd = -1;
    }

    m_handles[0].clear();
    m_handles[1].clear();

    LOG_MM_I(TAG, "close target {} ports [{},{} - {},{}] ---", m_target, m_ports[DMA_SEND_PORT_IDX], m_ports[DMA_RECV_PORT_IDX],
             m_ports[MSG_SEND_PORT_IDX], m_ports[MSG_RECV_PORT_IDX]);
    m_target = -1;

    for (auto &&m : m_ports) {
        m = -1;
    }
}

void pcie::clear() {
}

size_t pcie::send_data_by_msg(int32_t port, const void *data, size_t data_size, int32_t timeout) {
    return send_msg(port, data, data_size);
}

size_t pcie::recv_data_by_msg(int32_t port, void *data, size_t data_size, int32_t timeout) {
    return recv_msg(port, data, data_size, timeout);
}

size_t pcie::send_data_by_dma(int32_t port, const void *data, size_t data_size, const struct dma_mem &src, int32_t timeout) {

    if (!is_ep()) {
        /**
         * fixme: Currently, there is no such requirement. If there is, it will be supported later.
         */
        if (!data && src.blk_cnt > 1) {
            LOG_MM_E(TAG, "[{}-{}] Copy from host sg phy memory to device is unsupported", m_target, port);
            return 0;
        }
    }

    msg_sglist_mem_t dst_mem_info;
    if (!recv_dma_sglist_mem_msg(port, dst_mem_info, timeout)) {
        return 0;
    }

    struct dma_mem dst = {};
    dst.blk_cnt = dst_mem_info.blk_cnt;
    dst.total_size = 0;
    for (uint8_t i = 0; i < dst_mem_info.blk_cnt; ++i) {
        dst.blks[i].phy = dst_mem_info.blks[i].phy;
        dst.blks[i].size = dst_mem_info.blks[i].size;
        dst.total_size += dst.blks[i].size;
    }

    const uint32_t max_size = static_cast<uint32_t>(std::min(src.total_size, dst.total_size));
    const uint32_t partition = static_cast<uint32_t>((data_size + max_size - 1) / max_size);
    uint8_t *src_buf = (uint8_t *)data;
    const bool copy_to_phy = data ? true : false;
    uint64_t src_phy = src.blks[0].phy;
    size_t left_size = data_size;
    size_t send_size = 0;

    for (uint32_t i = 0; i < partition; ++i) {

        const uint32_t dma_size = (left_size > max_size) ? max_size : static_cast<uint32_t>(left_size);

        if (copy_to_phy) {
            ::memcpy(src.blks[0].vir, src_buf, dma_size);

            if (src.cached) {
                src.ops.flush(src.blks[0].phy, src.blks[0].vir, dma_size);
            }
        }

        if (is_ep()) {
            /**
             * ep -> rc
             * ep always continous memory, not scattered to simple dma linked list configuration.
             */
            AX_PCIE_DMA_BLOCK_T dma_blks[MAX_DMA_MEM_BLOCK_NUM];
            uint32_t dma_blk_cnt = 0;

            AX_U64 ep_phy = src_phy;
            uint32_t left_dma_size = dma_size;
            for (uint32_t j = 0; j < dst.blk_cnt; ++j) {
                if (0 == left_dma_size) {
                    break;
                }

                const uint32_t blk_size = (left_dma_size > dst.blks[j].size) ? dst.blks[j].size : left_dma_size;
                dma_blks[j].u64SrcAddr = ep_phy;
                dma_blks[j].u64DstAddr = dst.blks[j].phy;
                dma_blks[j].u32BlkSize = blk_size;

                left_dma_size -= blk_size;
                ep_phy += blk_size;

                ++dma_blk_cnt;
            }

            if (AX_S32 ret = AX_PCIe_CreatDmaLlpTask(m_ep_dma_fd, dma_blks, dma_blk_cnt, AX_FALSE /* DMA WRITE */); ret < 0) {
                LOG_MM_E(TAG, "[{}-{}] create dma task (size {}) fail, ret = {:#x}", m_target, port, dma_size, static_cast<int32_t>(ret));

                /* let rc known */
                send_dma_task_done_msg(port, 0);
                return 0;
            }

            /* 04: send dma transfer done msg to rc */
            if (!send_dma_task_done_msg(port, dma_size)) {
                LOG_MM_E(TAG, "[{}-{}] send dma task done msg fail, size: {}", m_target, port, dma_size);
                return 0;
            }
        } else {
            /**
             * rc -> ep: send dma linked list to ep
             */
            msg_dma_linked_list_t ll;
            if (copy_to_phy) {
                uint32_t dma_left_size = dma_size;
                for (uint32_t j = 0; j < src.blk_cnt; ++j) {
                    if (0 == dma_left_size) {
                        break;
                    }

                    const uint32_t blk_size = (dma_left_size > src.blks[j].size) ? src.blks[j].size : dma_left_size;
                    ll.blks[j].src_phy = src.blks[j].phy;
                    ll.blks[j].size = blk_size;

                    dma_left_size -= blk_size;
                    ++ll.blk_cnt;
                }
            } else {
                ll.blk_cnt = 1;
                ll.blks[0].src_phy = src_phy;
                ll.blks[0].size = dma_size;
            }

            if (!send_dma_linked_list_msg(port, ll)) {
                LOG_MM_E(TAG, "[{}-{}] send dma linked list msg fail", m_target, port);
                return 0;
            }
        }

        /**
         * rc: wait ep dma task finish -> wait read done msg from ep.
         * ep: ep launch dma task -> send dma done msg to rc -> wait rc read done msg from rc
         */
        uint32_t read_size;
        if (!recv_dma_read_done_msg(port, read_size, timeout) || read_size != dma_size) {
            LOG_MM_E(TAG, "[{}-{}] recv dma size {} is not equal to {}", m_target, port, read_size, dma_size);
            return 0;
        }

        if (copy_to_phy) {
            src_buf += dma_size;
        } else {
            src_phy += dma_size;
        }

        left_size -= dma_size;
        send_size += dma_size;
    }

    return send_size;
}

size_t pcie::send(const void *buf, size_t size, int64_t flag, int32_t timeout) {
    if (axcl::comm::PCIE_MSG == flag) {
        return send_data_by_msg(m_ports[MSG_SEND_PORT_IDX], buf, size, timeout);
    }

    std::lock_guard<std::mutex> lck(m_mtx_send);

    const int32_t port = m_ports[DMA_SEND_PORT_IDX];

    uint32_t type;
    if (axcl::comm::icomm::FLAG_RAWDATA == flag) {
        type = static_cast<uint32_t>(-1);
    } else {
        if (size < sizeof(uint32_t)) {
            LOG_MM_E(TAG, "[{}-{}] send size: {} is invalid", m_target, port, size);
            return 0;
        }

        /* 1st uint32_t is type of axcl::pkg::head */
        type = *((uint32_t *)buf);
    }

    if (!send_transfer_request_msg(port, type, size)) {
        LOG_MM_E(TAG, "[{}-{}] send transfer size msg fail, size: {}", m_target, port, size);
        return 0;
    }

    if (RUNTIME_MEMCPY_ASYNC_HANDLER_TYPE == type) {
        return this->send_data_by_msg(port, buf, size, timeout);
    } else {

        if (m_handles[PCIE_RC_IDX].end() != m_handles[PCIE_RC_IDX].find(type)) {
            return m_handles[PCIE_RC_IDX][type]->send(buf, size, timeout);
        } else {
            /* if no handler, using normal routeline */
            if (size < MIN_PCIE_DMA_LEN) {
                /* transfer by message */
                return send_data_by_msg(port, buf, size, timeout);
            }

            /* transfer by DMA */
            return send_data_by_dma(port, buf, size, m_dmabufs[0]->get(), timeout);
        }
    }
}

void *pcie::alloc(size_t size) {
    return m_mem.malloc(size);
}

void pcie::free(void *buf) {
    m_mem.free(buf);
}

size_t pcie::recv_data_by_dma(int32_t port, void *data, size_t data_size, const struct dma_mem &dst, int32_t timeout) {
    /**
     * send target memory info of receiver to sender
     */
    msg_sglist_mem_t dst_mem_info;
    dst_mem_info.blk_cnt = static_cast<uint8_t>(dst.blk_cnt);
    for (uint32_t i = 0; i < dst.blk_cnt; ++i) {
        dst_mem_info.blks[i].phy = dst.blks[i].phy;
        dst_mem_info.blks[i].size = dst.blks[i].size;
    }

    if (!send_dma_sglist_mem_msg(port, dst_mem_info)) {
        return 0;
    }

    // dst_mem_info.print();

    uint8_t *dst_buf = (uint8_t *)data;
    const bool copy_to_buf = data ? true : false;
    uint64_t dst_phy = dst.blks[0].phy;
    size_t recv_size = 0;
    size_t left_size = data_size;

    do {
        uint32_t dma_size = 0;

        if (is_ep()) {
            /**
             * copy from rc -> ep
             *
             * recv dma linked list msg -> start dma task -> send read done to rc
             */
            msg_dma_linked_list_t ll;
            if (!recv_dma_linked_list_msg(port, ll, timeout)) {
                return 0;
            }

            uint64_t ep_phy = dst_phy;
            AX_PCIE_DMA_BLOCK_T dma_blks[MAX_DMA_MEM_BLOCK_NUM];
            for (uint8_t i = 0; i < ll.blk_cnt; ++i) {
                dma_blks[i].u64SrcAddr = ll.blks[i].src_phy;
                dma_blks[i].u64DstAddr = ep_phy;
                dma_blks[i].u32BlkSize = ll.blks[i].size;
                dma_size += ll.blks[i].size;
                ep_phy += ll.blks[i].size;
            }

            if (AX_S32 ret = AX_PCIe_CreatDmaLlpTask(m_ep_dma_fd, dma_blks, ll.blk_cnt, AX_TRUE /* DMA READ */); ret < 0) {
                LOG_MM_E(TAG, "[{}-{}] create dma task (size {}) fail, ret = {:#x}", m_target, port, dma_size, static_cast<int32_t>(ret));

                /* let rc known */
                send_dma_read_done_msg(port, 0);
                return 0;
            }

        } else {
            /**
             * copy from ep to rc
             *
             * ep start dma task -> ep send dma task done msg -> rc wait dma task done msg of ep
             */
            if (!recv_dma_task_done_msg(port, dma_size, timeout) || (0 == dma_size)) {
                LOG_MM_E(TAG, "[{}-{}] recv dma task done msg fail, size: {}", m_target, port, dma_size);
                return 0;
            }
        }

        /**
         * memory copy
         */
        if (copy_to_buf) {
            if (dst.cached) {
                dst.ops.invalidate(dst.blks[0].phy, dst.blks[0].vir, dma_size);
            }

            ::memcpy(dst_buf, dst.blks[0].vir, dma_size);
            dst_buf += dma_size;
        } else {
            dst_phy += dma_size;
        }

        left_size -= dma_size;
        recv_size += dma_size;

        if (!send_dma_read_done_msg(port, dma_size)) {
            LOG_MM_E(TAG, "[{}-{}] send read done msg fail, size: {}", m_target, port, dma_size);
            return 0;
        }

    } while (recv_size < data_size);

    return recv_size;
}

size_t pcie::recv(void **buf, int64_t flag, int32_t timeout) {

    if (axcl::comm::PCIE_MSG == flag) {
        void *mem = m_mem.malloc(MAX_PCIE_MSG_SIZE);
        if (!mem) {
            LOG_MM_E(TAG, "[{}-{}] allocate recv msg buffer fail", m_target, m_ports[MSG_RECV_PORT_IDX]);
            return 0;
        }

        size_t recv_size = recv_data_by_msg(m_ports[MSG_RECV_PORT_IDX], mem, MAX_PCIE_MSG_SIZE, timeout);
        if (0 == recv_size) {
            m_mem.free(mem);
            return 0;
        }

        *buf = mem;
        return recv_size;
    }

    std::lock_guard<std::mutex> lck(m_mtx_recv);
    const int32_t port = m_ports[DMA_RECV_PORT_IDX];

    if (!buf) {
        LOG_MM_E(TAG, "[{}-{}] nil parameter", m_target, port);
        return 0;
    }

    *buf = nullptr;

    uint32_t size;
    uint32_t type;
    if (!recv_transfer_request_msg(port, type, size, timeout) || 0 == size) {
        if (!m_offline.load()) {
            LOG_MM_E(TAG, "[{}-{}] recv transfer size msg fail, type: {:#x}, size: {}", m_target, port, type, size);
        }
        return 0;
    }

#if USE_HANDLE_ASYNC
    if (RUNTIME_MEMCPY_ASYNC_HANDLER_TYPE == type && is_ep()) {
#else
    if (RUNTIME_MEMCPY_ASYNC_HANDLER_TYPE == type) {
#endif
        if (size < MIN_PCIE_DMA_LEN) {

            void *mem = m_mem.malloc(size);
            if (!mem) {
                LOG_MM_E(TAG, "[{}-{}] malloc (size: {}) fail", m_target, port, size);
                return 0;
            }

            auto recv_size = recv_data_by_msg(port, mem, size, timeout);
            if (0 == recv_size) {
                m_mem.free(mem);
                return 0;
            }

#if !USE_HANDLE_ASYNC
            auto *pkg = reinterpret_cast<struct axcl::pkg::memcpy_package *>(mem);
            pkg->hd.type = RUNTIME_MEMCPY_HANDLER_TYPE;
#endif
            size_t send_size = this->send(mem, size, axcl::comm::PCIE_DMA, timeout);

            *buf = mem;
            return send_size;
        }

        return 0;

    }
#if USE_HANDLE_ASYNC
    else if (RUNTIME_MEMCPY_ASYNC_HANDLER_TYPE == type && is_ep()) {

        type = RUNTIME_MEMCPY_HANDLER_TYPE;
        if (m_handles[PCIE_EP_IDX].end() != m_handles[PCIE_EP_IDX].find(type)) {

            size_t send_size = m_handles[PCIE_EP_IDX][type]->recv(buf, timeout);
            auto* pkg = reinterpret_cast<axcl::pkg::memcpy_package*>(*buf);

            LOG_MM_D(TAG, "recv end pkg->hd: type={:#x}, seq_num={}, device={}, context={}, stream={}, command={}",
                     pkg->hd.type,
                     pkg->hd.seq_num,
                     pkg->hd.device,
                     pkg->hd.context,
                     pkg->hd.stream,
                     pkg->hd.command);
            {
                std::lock_guard<std::mutex> lk(m_memcpy_mtx);
                auto it = m_memcpy_wait_map.find(&pkg->hd);
                if (it != m_memcpy_wait_map.end()) {
                    it->second.done = true;
                    it->second.size = send_size;
                    it->second.cv.notify_one();
                }
            }

            return send_size;
        }

        return 0;
    }
#endif
    else {

        if (m_handles[PCIE_EP_IDX].end() != m_handles[PCIE_EP_IDX].find(type)) {
#if USE_HANDLE_ASYNC
            return m_handles[PCIE_EP_IDX][type]->recv(buf, timeout);
#else
            size_t send_size = m_handles[PCIE_EP_IDX][type]->recv(buf, timeout);
            auto* pkg = reinterpret_cast<axcl::pkg::memcpy_package*>(*buf);
            LOG_MM_D(TAG, "recv end pkg->hd: type={:#x}, seq_num={}, device={}, context={}, stream={}, command={}",
                        pkg->hd.type,
                        pkg->hd.seq_num,
                        pkg->hd.device,
                        pkg->hd.context,
                        pkg->hd.stream,
                        pkg->hd.command);
            {
                std::lock_guard<std::mutex> lk(m_memcpy_mtx);
                auto it = m_memcpy_wait_map.find(&pkg->hd);
                if (it != m_memcpy_wait_map.end()) {
                    it->second.done = true;
                    it->second.size = send_size;
                    it->second.cv.notify_one();
                }
            }

            return send_size;
#endif
        } else {
            /* if no handler, using normal routeline */
            void *mem = m_mem.malloc(size);
            if (!mem) {
                LOG_MM_E(TAG, "[{}-{}] malloc (size: {}) fail", m_target, port, size);
                return 0;
            }

            if (size < MIN_PCIE_DMA_LEN) {
                auto recv_size = recv_data_by_msg(port, mem, size, timeout);
                if (0 == recv_size) {
                    m_mem.free(mem);
                    return 0;
                } else {
                    *buf = mem;
                    return recv_size;
                }
            }

            if (size_t recv_size = recv_data_by_dma(port, mem, size, m_dmabufs[1]->get(), timeout); recv_size != size) {
                m_mem.free(mem);
                return recv_size;
            }

            *buf = mem;
            return size;
        }
    }
}

size_t pcie::send_msg(int32_t port, const void *msg, size_t len) {
    if (m_target < 0 || port < 0) {
        return 0;
    }

    AX_S32 ret = AX_PCIe_WriteMsg(m_target, port, const_cast<AX_VOID *>(msg), len);
    if (ret < 0) {
        if (!m_offline.load()) {
            LOG_MM_E(TAG, "[{}-{}] AX_PCIe_WriteMsg(len {}) fail, ret = {}", m_target, port, len, ret);
        }
        return 0;
    }

    return len;
}

size_t pcie::recv_msg(int32_t port, void *msg, size_t len, int32_t timeout) {
    if (m_offline.load()) {
        return 0;
    }

    AX_S32 size = AX_PCIe_ReadMsg(m_target, port, msg, len, timeout);
    if (size < 0) {
        if (AX_PCIE_POLL_HUP == size) {
            m_offline = true;
            LOG_MM_I(TAG, "poll hung up");
        } else if (AX_PCIE_POLL_NVAL == size) {
            m_offline = true;
            LOG_MM_I(TAG, "poll nval");
        } else {
            if (!m_offline.load()) {
                LOG_MM_E(TAG, "[{}-{}] AX_PCIe_ReadMsg(len {} timeout {}) fail, size = {}", m_target, port, len, timeout, size);
            }
        }
        return 0;
    }

    return static_cast<size_t>(size);
}

bool pcie::send_msg(int32_t port, const msg_data_t &msg) {
    size_t size = send_msg(port, (const void *)&msg, sizeof(msg_data_t));
    if (size == static_cast<size_t>(sizeof(msg_data_t))) {
        return true;
    }

    LOG_MM_E(TAG, "[{}-{}] send msg {} fail, send size = {}", m_target, port, msg.str(), size);
    return false;
}

bool pcie::recv_msg(int32_t port, msg_data_t &msg, int32_t timeout) {
    size_t size = recv_msg(port, (void *)&msg, sizeof(msg_data_t), timeout);
    if (size == static_cast<size_t>(sizeof(msg_data_t))) {
        return true;
    }

    if (!m_offline.load()) {
        LOG_MM_E(TAG, "[{}-{}] recv msg {} fail, recv size = {}", m_target, port, msg.str(), size);
    }
    return false;
}

bool pcie::send_transfer_request_msg(int32_t port, uint32_t type, uint32_t size) {
    msg_data_t msg;
    msg.msg = PCIE_MSG::TRANSFER_REQUEST;
    uint8_t *p = (uint8_t *)msg.data;
    *((uint32_t *)p) = type;
    p += sizeof(uint32_t);
    *((uint32_t *)p) = size;

    return send_msg(port, msg);
}

bool pcie::recv_transfer_request_msg(int32_t port, uint32_t &type, uint32_t &size, int32_t timeout) {
    msg_data_t msg;
    if (!recv_msg(port, msg, timeout)) {
        type = 0;
        size = 0;
        return false;
    }

    if (msg.msg != PCIE_MSG::TRANSFER_REQUEST) {
        LOG_MM_E(TAG, "[{}-{}] recv msg {} is not TRANSFER_REQUEST", m_target, port, msg.str());
        type = 0;
        size = 0;
        return false;
    }

    uint8_t *p = (uint8_t *)msg.data;
    type = *((uint32_t *)p);
    p += sizeof(uint32_t);
    size = *((uint32_t *)p);

    return true;
}

bool pcie::send_dma_request_mem_msg(int32_t port, uint64_t phy, uint32_t size) {
    msg_data_t msg;
    msg.msg = PCIE_MSG::DMA_REQUEST_MEM;
    uint8_t *p = (uint8_t *)msg.data;
    *((uint64_t *)p) = phy;
    p += sizeof(uint64_t);
    *((uint32_t *)p) = size;

    return send_msg(port, msg);
}

bool pcie::recv_dma_request_mem_msg(int32_t port, uint64_t &phy, uint32_t &size, int32_t timeout) {
    msg_data_t msg;
    if (!recv_msg(port, msg, timeout)) {
        phy = 0;
        size = 0;
        return false;
    }

    if (msg.msg != PCIE_MSG::DMA_REQUEST_MEM) {
        LOG_MM_E(TAG, "[{}-{}] recv msg {} is not DMA_REQUEST_MEM", m_target, port, msg.str());
        phy = 0;
        size = 0;
        return false;
    }

    uint8_t *p = (uint8_t *)msg.data;
    phy = *((uint64_t *)p);
    p += sizeof(uint64_t);
    size = *((uint32_t *)p);

    return true;
}

bool pcie::send_dma_data_ready_msg(int32_t port, uint64_t phy, uint32_t size) {
    msg_data_t msg;
    msg.msg = PCIE_MSG::DMA_DATA_READY;

    uint8_t *p = (uint8_t *)msg.data;
    *((uint64_t *)p) = phy;
    p += sizeof(uint64_t);
    *((uint32_t *)p) = size;

    return send_msg(port, msg);
}

bool pcie::recv_dma_data_ready_msg(int32_t port, uint64_t &phy, uint32_t &size, int32_t timeout) {
    msg_data_t msg;
    if (!recv_msg(port, msg, timeout)) {
        phy = 0;
        size = 0;
        return false;
    }

    if (msg.msg != PCIE_MSG::DMA_DATA_READY) {
        phy = 0;
        size = 0;
        LOG_MM_E(TAG, "[{}-{}] recv msg {} is not DMA_DATA_READY", m_target, port, msg.str());
        return false;
    }

    uint8_t *p = (uint8_t *)msg.data;
    phy = *((uint64_t *)p);
    p += sizeof(uint64_t);
    size = *((uint32_t *)p);
    return true;
}

bool pcie::send_dma_task_done_msg(int32_t port, uint32_t size) {
    msg_data_t msg;
    msg.msg = PCIE_MSG::DMA_TASK_DONE;

    uint8_t *p = (uint8_t *)msg.data;
    *((uint32_t *)p) = size;

    return send_msg(port, msg);
}

bool pcie::recv_dma_task_done_msg(int32_t port, uint32_t &size, int32_t timeout) {
    msg_data_t msg;
    if (!recv_msg(port, msg, timeout)) {
        size = 0;
        return false;
    }

    if (msg.msg != PCIE_MSG::DMA_TASK_DONE) {
        size = 0;
        LOG_MM_E(TAG, "[{}-{}] recv msg {} is not DMA_TASK_DONE", m_target, port, msg.str());
        return false;
    }

    uint8_t *p = (uint8_t *)msg.data;
    size = *((uint32_t *)p);
    return true;
}

bool pcie::send_dma_read_done_msg(int32_t port, uint32_t size) {
    msg_data_t msg;
    msg.msg = PCIE_MSG::DMA_READ_DONE;
    *((uint32_t *)msg.data) = size;
    return send_msg(port, msg);
}

bool pcie::recv_dma_read_done_msg(int32_t port, uint32_t &size, int32_t timeout) {
    msg_data_t msg;
    if (!recv_msg(port, msg, timeout)) {
        size = 0;
        return false;
    }

    if (msg.msg != PCIE_MSG::DMA_READ_DONE) {
        size = 0;
        LOG_MM_E(TAG, "[{}-{}] recv msg {} is not DMA_READ_DONE", m_target, port, msg.str());
        return false;
    }

    size = *((uint32_t *)msg.data);
    return true;
}

bool pcie::send_handshake_sync_msg(int32_t port, uint32_t seq) {
    msg_data_t msg;
    msg.msg = PCIE_MSG::HANDSHAKE_SYNC;
    *((uint32_t *)msg.data) = seq;
    return send_msg(port, msg);
}

bool pcie::recv_handshake_sync_msg(int32_t port, uint32_t &seq, int32_t timeout) {
    msg_data_t msg;
    if (!recv_msg(port, msg, timeout)) {
        seq = 0;
        return false;
    }

    if (msg.msg != PCIE_MSG::HANDSHAKE_SYNC) {
        seq = 0;
        LOG_MM_E(TAG, "[{}-{}] recv msg {} is not HANDSHAKE_SYNC", m_target, port, msg.str());
        return false;
    }

    seq = *((uint32_t *)msg.data);
    return true;
}

bool pcie::send_handshake_sync_ack_msg(int32_t port, uint32_t seq) {
    msg_data_t msg;
    msg.msg = PCIE_MSG::HANDSHAKE_SYNC_ACK;
    *((uint32_t *)msg.data) = seq;

    return send_msg(port, msg);
}

bool pcie::recv_handshake_sync_ack_msg(int32_t port, uint32_t &seq, int32_t timeout) {
    msg_data_t msg;
    if (!recv_msg(port, msg, timeout)) {
        seq = 0;
        return false;
    }

    if (msg.msg != PCIE_MSG::HANDSHAKE_SYNC_ACK) {
        seq = 0;
        LOG_MM_E(TAG, "[{}-{}] recv msg {} is not HANDSHAKE_SYNC_ACK", m_target, port, msg.str());
        return false;
    }

    seq = *((uint32_t *)msg.data);
    return true;
}

bool pcie::send_handshake_done_msg(int32_t port, uint32_t seq) {
    msg_data_t msg;
    msg.msg = PCIE_MSG::HANDSHAKE_DONE;
    *((uint32_t *)msg.data) = seq;

    return send_msg(port, msg);
}

bool pcie::recv_handshake_done_msg(int32_t port, uint32_t &seq, int32_t timeout) {
    msg_data_t msg;
    if (!recv_msg(port, msg, timeout)) {
        seq = 0;
        return false;
    }

    if (msg.msg != PCIE_MSG::HANDSHAKE_DONE) {
        seq = 0;
        LOG_MM_E(TAG, "[{}-{}] recv msg {} is not HANDSHAKE_DONE", m_target, port, msg.str());
        return false;
    }

    seq = *((uint32_t *)msg.data);
    return true;
}

bool pcie::send_dma_sglist_mem_msg(int32_t port, const msg_sglist_mem_t &sglist) {
    if (0 == sglist.blk_cnt) {
        LOG_MM_E(TAG, "[{}-{}] sg blk cnt == 0", m_target, port);
        return false;
    }

    for (uint8_t i = 0; i < sglist.blk_cnt; ++i) {
        if (0 == sglist.blks[i].phy || 0 == sglist.blks[i].size) {
            LOG_MM_E(TAG, "[{}-{}] invalid sg[{}], phy {} size {}", m_target, port, i, sglist.blks[i].phy, sglist.blks[i].size);
            return false;
        }
    }

    const auto msg_size = sglist.get_msg_size();
    size_t size = send_msg(port, reinterpret_cast<const void *>(&sglist), msg_size);
    if (size == msg_size) {
        return true;
    }

    LOG_MM_E(TAG, "[{}-{}] send msg {} fail, sent size {} != {}", m_target, port, sglist.str(), size, msg_size);
    return false;
}

bool pcie::recv_dma_sglist_mem_msg(int32_t port, msg_sglist_mem_t &sglist, int32_t timeout) {
    size_t size = recv_msg(port, reinterpret_cast<void *>(&sglist), sizeof(msg_sglist_mem_t), timeout);
    const auto msg_size = sglist.get_msg_size();
    if (size == msg_size && sglist.msg == PCIE_MSG::DMA_SGLIST_INFO && sglist.blk_cnt > 0) {
        for (uint8_t i = 0; i < sglist.blk_cnt; ++i) {
            if (0 == sglist.blks[i].phy || 0 == sglist.blks[i].size) {
                LOG_MM_E(TAG, "[{}-{}] recv invalid sg[{}], phy {} size {}", m_target, port, i, sglist.blks[i].phy, sglist.blks[i].size);
                return false;
            }
        }

        return true;
    }

    if (!m_offline.load()) {
        if (size != msg_size) {
            LOG_MM_E(TAG, "[{}-{}] recv msg {} fail, recv size {} != {}", m_target, port, sglist.str(), size, msg_size);
        } else if (sglist.msg != PCIE_MSG::DMA_SGLIST_INFO) {
            LOG_MM_E(TAG, "[{}-{}] recv msg {} is not {}", m_target, port, static_cast<int32_t>(sglist.msg), sglist.str());
        } else if (0 == sglist.blk_cnt) {
            LOG_MM_E(TAG, "[{}-{}] recv sg blk cnt is 0", m_target, port);
        }
    }

    return false;
}

bool pcie::send_dma_linked_list_msg(int32_t port, const msg_dma_linked_list_t &ll) {
    if (0 == ll.blk_cnt) {
        LOG_MM_E(TAG, "[{}-{}] dma linked list blk cnt == 0", m_target, port);
        return false;
    }

    for (uint8_t i = 0; i < ll.blk_cnt; ++i) {
        if (0 == ll.blks[i].src_phy || 0 == ll.blks[i].size) {
            LOG_MM_E(TAG, "[{}-{}] invalid dma linked list[{}], src phy {} size {}", m_target, port, i, ll.blks[i].src_phy,
                     ll.blks[i].size);
            return false;
        }
    }

    const auto msg_size = ll.get_msg_size();
    size_t size = send_msg(port, reinterpret_cast<const void *>(&ll), msg_size);
    if (size == msg_size) {
        return true;
    }

    LOG_MM_E(TAG, "[{}-{}] send msg {} fail, sent size {} != {}", m_target, port, ll.str(), size, msg_size);
    return false;
}

bool pcie::recv_dma_linked_list_msg(int32_t port, msg_dma_linked_list_t &ll, int32_t timeout) {
    size_t size = recv_msg(port, reinterpret_cast<void *>(&ll), sizeof(msg_dma_linked_list_t), timeout);
    const auto msg_size = ll.get_msg_size();
    if (size == msg_size && ll.msg == PCIE_MSG::DMA_LINKED_LIST && ll.blk_cnt > 0) {
        for (uint8_t i = 0; i < ll.blk_cnt; ++i) {
            if (0 == ll.blks[i].src_phy || 0 == ll.blks[i].size) {
                LOG_MM_E(TAG, "[{}-{}] recv invalid dma linked list[{}], src phy {} dst phy {} size {}", m_target, port, i,
                         ll.blks[i].src_phy, ll.blks[i].size);
                return false;
            }
        }
        return true;
    }

    if (!m_offline.load()) {
        if (size != msg_size) {
            LOG_MM_E(TAG, "[{}-{}] recv msg {} fail, recv size {} != {}", m_target, port, ll.str(), size, msg_size);
        } else if (ll.msg != PCIE_MSG::DMA_LINKED_LIST) {
            LOG_MM_E(TAG, "[{}-{}] recv msg {} is not {}", m_target, port, static_cast<int32_t>(ll.msg), ll.str());
        } else if (0 == ll.blk_cnt) {
            LOG_MM_E(TAG, "[{}-{}] recv dma linked list blk cnt is 0", m_target, port);
        }
    }
    return false;
}

bool pcie::handshake_with_ep(int32_t timeout) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint32_t> dis(1, 0x00FFFFFF);
    uint32_t sync_seq = dis(gen);
    uint32_t done_seq = sync_seq + 2;

    uint64_t now = get_tick();

    /* WR: sleep for ep is ready */
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

RETRY:
    while (timeout < 0 || get_tick() - now < static_cast<uint64_t>(timeout)) {
        if (!send_handshake_sync_msg(m_ports[DMA_SEND_PORT_IDX], sync_seq)) {
            goto RETRY;
        } else {
            LOG_MM_I(TAG, "handshake to {}: send sync, seq {}", m_target, sync_seq);
        }

        uint32_t ack0_seq = 0;
        if (!recv_handshake_sync_ack_msg(m_ports[DMA_RECV_PORT_IDX], ack0_seq, 200)) {
            goto RETRY;
        } else {
            LOG_MM_I(TAG, "handshake to {}: recv ack0, seq {}", m_target, ack0_seq);
        }

        if (ack0_seq != (sync_seq + 1)) {
            LOG_MM_E(TAG, "handshake to {}: recv ack0 seq {} is unexpected {}", m_target, ack0_seq, sync_seq + 1);
            goto RETRY;
        }

        if (!send_handshake_done_msg(m_ports[DMA_SEND_PORT_IDX], done_seq)) {
            goto RETRY;
        } else {
            LOG_MM_I(TAG, "handshake to {}: send done, seq {}", m_target, done_seq);
        }

        return true;
    };

    LOG_MM_E(TAG, "handshake to {} timeout", m_target);
    return false;
}

bool pcie::handshake_with_rc(int32_t timeout) {
    const uint64_t now = get_tick();
RETRY:
    while (timeout < 0 || get_tick() - now < static_cast<uint64_t>(timeout)) {
        uint32_t sync_seq = 0;
        if (!recv_handshake_sync_msg(m_ports[DMA_RECV_PORT_IDX], sync_seq, 1000)) {
            goto RETRY;
        } else {
            LOG_MM_I(TAG, "handshake to {}: recv sync, seq {}", m_target, sync_seq);
        }

        uint32_t ack0_seq = sync_seq + 1;
        if (!send_handshake_sync_ack_msg(m_ports[DMA_SEND_PORT_IDX], ack0_seq)) {
            goto RETRY;
        } else {
            LOG_MM_I(TAG, "handshake to {}: send ack0, seq {}", m_target, ack0_seq);
        }

        uint32_t done_seq = 0;
        if (!recv_handshake_done_msg(m_ports[DMA_RECV_PORT_IDX], done_seq, 200)) {
            goto RETRY;
        } else {
            LOG_MM_I(TAG, "handshake to {}: recv done, seq {}", m_target, done_seq);
        }

        if (done_seq != ack0_seq + 1) {
            LOG_MM_E(TAG, "handshake to {}: recv done seq {} is unexpected {}", m_target, done_seq, ack0_seq + 1);
            return false;
        }

        return true;
    };

    LOG_MM_E(TAG, "handshake to {} timeout", m_target);
    return false;
}

bool pcie::memcpy_async(void* hd, int flag, int32_t timeout) {

    axcl::pkg::head* phead = static_cast<axcl::pkg::head*>(hd);

    LOG_MM_I(TAG, "[{}] memcpy_async pkg->hd: type={:#x}, seq_num={}, device={}, context={}, stream={}, command={}",
                flag,
                phead->type,
                phead->seq_num,
                phead->device,
                phead->context,
                phead->stream,
                phead->command);

    if (flag == 0) {

        std::lock_guard<std::mutex> lk(m_memcpy_mtx);
        auto& info = m_memcpy_wait_map[phead];
        info.done = false;
        info.size = 0;
        return true;

    } else if (flag == 1) {

        std::unique_lock<std::mutex> lk(m_memcpy_mtx);
        auto it = m_memcpy_wait_map.find(phead);
        if (it == m_memcpy_wait_map.end()) {
            return false;
        }
        auto& info = it->second;
        if (timeout < 0) {
            info.cv.wait(lk, [&]{ return info.done; });
        } else {
            if (!info.cv.wait_for(lk, std::chrono::milliseconds(timeout), [&]{ return info.done; })) {
                m_memcpy_wait_map.erase(phead);
                return false;
            }
        }
        size_t memcpy_size = info.size;
        m_memcpy_wait_map.erase(phead);
        return memcpy_size == MEMCPY_PACKAGE_SIZE;
    }
    return false;
}