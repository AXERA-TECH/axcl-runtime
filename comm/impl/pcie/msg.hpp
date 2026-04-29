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

#include <cstdint>
#include "axcl_logger.hpp"

#define MIN_PCIE_DMA_LEN (1024)
#define MAX_PCIE_MSG_LEN (384 * 1024)  //!< max message length defined in ko

/* add msg below: */
#define MSG_LIST          \
    X(HANDSHAKE_SYNC)     \
    X(HANDSHAKE_SYNC_ACK) \
    X(HANDSHAKE_DONE)     \
    X(TRANSFER_REQUEST)   \
    X(DMA_REQUEST_MEM)    \
    X(DMA_DATA_READY)     \
    X(DMA_TASK_DONE)      \
    X(DMA_READ_DONE)      \
    X(DMA_SGLIST_INFO)    \
    X(DMA_LINKED_LIST)

enum class PCIE_MSG : uint8_t {
#define X(name) name,
    MSG_LIST
#undef X
};

#pragma pack(push, 1)

typedef struct msg_data_t {
    PCIE_MSG msg;
    uint8_t data[16];

    const char *str() const {
        switch (msg) {
#define X(name)     \
    case PCIE_MSG::name: \
        return #name;
            MSG_LIST
#undef X
            default:
                return "UNKNOWN";
        }
    }
} msg_data_t;

typedef struct sgblk_mem_t {
    uint64_t phy;
    uint32_t size;

    sgblk_mem_t() : phy(0), size(0) {
    }
} sgblk_mem_t;

typedef struct msg_sglist_mem_t {
    PCIE_MSG msg;
    uint8_t blk_cnt;
    sgblk_mem_t blks[64];

    msg_sglist_mem_t() : msg(PCIE_MSG::DMA_SGLIST_INFO), blk_cnt(0) {
    }

    size_t get_msg_size() const {
        return sizeof(msg) + sizeof(blk_cnt) + sizeof(sgblk_mem_t) * blk_cnt;
    }

    const char *str() const {
        return "DMA_SGLIST_INFO";
    }

    void print() const {
        LOG_MM_C("sgmem", "blk cnt = {}, msg_size = {}", blk_cnt, get_msg_size());
        for (uint8_t i = 0; i < blk_cnt; ++i) {
            LOG_MM_C("sgmem", "blk[{}] phy {:#x} size {:#x}", i, blks[i].phy, blks[i].size);
        }
    }

} msg_sglist_mem_t;


typedef struct dma_linked_blk_t {
    uint64_t src_phy;
    uint32_t size;

    dma_linked_blk_t() : src_phy(0), size(0) {
    }
} dma_linked_blk_t;

typedef struct msg_dma_linked_list_t {
    PCIE_MSG msg;
    uint8_t blk_cnt;
    dma_linked_blk_t blks[64];

    msg_dma_linked_list_t() : msg(PCIE_MSG::DMA_LINKED_LIST), blk_cnt(0) {
    }

    size_t get_msg_size() const {
        return sizeof(msg) + sizeof(blk_cnt) + sizeof(dma_linked_blk_t) * blk_cnt;
    }

    const char *str() const {
        return "DMA_LINKED_LIST";
    }

    void print() const {
        LOG_MM_C("linked list", "blk cnt = {}, msg_size = {}", blk_cnt, get_msg_size());
        for (uint8_t i = 0; i < blk_cnt; ++i) {
            LOG_MM_C("linked list", "blk[{}] src {:#x} size {:#x}", i, blks[i].src_phy, blks[i].size);
        }
    }

} msg_dma_linked_list_t;


#pragma pack(pop)
