/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "memcpy_handler.hpp"
#include "axcl_package.hpp"
#include "axcl_logger.hpp"
#include "pcie.hpp"

#define TAG "pcie"

static constexpr size_t MEMCPY_PACKAGE_SIZE = sizeof(struct axcl::pkg::memcpy_package);

memcpy_handler::memcpy_handler(pcie *impl) : m_impl(impl) {
}

size_t memcpy_handler::send(const void *buf, size_t size, int32_t timeout) {

    if (m_impl->is_ep()) {
        LOG_MM_E(TAG, "memcpy handler can only be trigged by host");
        return 0;
    }

    if (size != MEMCPY_PACKAGE_SIZE) {
        LOG_MM_E(TAG, "buff size {} is not equal to {}", size, MEMCPY_PACKAGE_SIZE);
        return 0;
    }

    const int32_t port = m_impl->get_port(DMA_SEND_PORT_IDX);

    const auto *pkg = reinterpret_cast<const struct axcl::pkg::memcpy_package *>(buf);

    if (size_t send_size = m_impl->send_data_by_msg(port, buf, size, timeout); send_size != size) {
        LOG_MM_E(TAG, "send size {} is not equal to {}", send_size, size);
        return 0;
    }

    if (axcl::pkg::memcpy_kind::HOST_VIR_TO_DEVICE == pkg->kind) {
        if (pkg->size != m_impl->send_data_by_dma(port, pkg->data, pkg->size, m_impl->get_dma_buffer(0), timeout)) {
            return 0;
        }
    } else if (axcl::pkg::memcpy_kind::HOST_PHY_TO_DEVICE == pkg->kind) {
        struct dma_mem src;
        src.total_size = pkg->size;
        src.blk_cnt = 1;
        src.blks[0].phy = pkg->src_phy;
        src.blks[0].vir = nullptr;
        src.blks[0].size = pkg->size;
        if (pkg->size != m_impl->send_data_by_dma(port, nullptr, pkg->size, src, timeout)) {
            return 0;
        }
    } else if (axcl::pkg::memcpy_kind::DEVICE_TO_HOST_VIR == pkg->kind) {
        if (pkg->size != m_impl->recv_data_by_dma(port, pkg->data, pkg->size, m_impl->get_dma_buffer(0), timeout)) {
            return 0;
        }
    } else {
        struct dma_mem dst;
        dst.total_size = pkg->size;
        dst.blk_cnt = 1;
        dst.blks[0].phy = pkg->dst_phy;
        dst.blks[0].vir = nullptr;
        dst.blks[0].size = pkg->size;
        if (pkg->size != m_impl->recv_data_by_dma(port, nullptr, pkg->size, dst, timeout)) {
            return 0;
        }
    }

    return MEMCPY_PACKAGE_SIZE;
}

size_t memcpy_handler::recv(void **buf, int32_t timeout) {

    if (!m_impl->is_ep()) {
        LOG_MM_E(TAG, "memcpy handler can only be trigged by host and has no ack from device");
        return 0;
    }

    const int32_t port = m_impl->get_port(DMA_RECV_PORT_IDX);

    struct axcl::pkg::memcpy_package pkg;
    if (size_t recv_size = m_impl->recv_data_by_msg(port, &pkg, MEMCPY_PACKAGE_SIZE, timeout); recv_size != MEMCPY_PACKAGE_SIZE) {
        LOG_MM_E(TAG, "recv size {} is not equal to {}", recv_size, MEMCPY_PACKAGE_SIZE);
        return 0;
    }

    if (axcl::pkg::memcpy_kind::HOST_VIR_TO_DEVICE == pkg.kind) {
        struct dma_mem dst;
        dst.total_size = pkg.size;
        dst.blk_cnt = 1;
        dst.blks[0].phy = pkg.dst_phy;
        dst.blks[0].vir = nullptr;
        dst.blks[0].size = pkg.size;
        if (pkg.size != m_impl->recv_data_by_dma(port, nullptr, pkg.size, dst, timeout)) {
            return 0;
        }
    } else if (axcl::pkg::memcpy_kind::HOST_PHY_TO_DEVICE == pkg.kind) {
        struct dma_mem dst;
        dst.total_size = pkg.size;
        dst.blk_cnt = 1;
        dst.blks[0].phy = pkg.dst_phy;
        dst.blks[0].vir = nullptr;
        dst.blks[0].size = pkg.size;
        if (pkg.size != m_impl->recv_data_by_dma(port, nullptr, pkg.size, dst, timeout)) {
            return 0;
        }
    } else if (axcl::pkg::memcpy_kind::DEVICE_TO_HOST_VIR == pkg.kind) {
        struct dma_mem src;
        src.total_size = pkg.size;
        src.blk_cnt = 1;
        src.blks[0].phy = pkg.src_phy;
        src.blks[0].vir = nullptr;
        src.blks[0].size = pkg.size;
        if (pkg.size != m_impl->send_data_by_dma(port, nullptr, pkg.size, src, timeout)) {
            return 0;
        }
    } else {
        struct dma_mem src;
        src.total_size = pkg.size;
        src.blk_cnt = 1;
        src.blks[0].phy = pkg.src_phy;
        src.blks[0].vir = nullptr;
        src.blks[0].size = pkg.size;
        if (pkg.size != m_impl->send_data_by_dma(port, nullptr, pkg.size, src, timeout)) {
            return 0;
        }
    }

    if (void *mem = m_impl->alloc(MEMCPY_PACKAGE_SIZE); mem) {
        *(reinterpret_cast<struct axcl::pkg::memcpy_package *>(mem)) = pkg;
        *buf = mem;
    }

    return MEMCPY_PACKAGE_SIZE;
}