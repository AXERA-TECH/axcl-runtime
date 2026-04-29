/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "engine_p2p_task_impl.hpp"

#include "ax_pcie_p2p_user.h"
#include "axcl_logger.hpp"
#include "res_guard.hpp"
#include "timer.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <ax_sys_api.h>

#define TAG "engine"

#define TIMEOUT 5000

task_router::impl::impl() {
    this->timeout_.tv_sec = TIMEOUT / 1000;
    this->timeout_.tv_usec = (TIMEOUT % 1000) * 1000;
}

task_router::impl::~impl() {
    if (this->fd_ > 0) {
        LOG_MM_D(TAG, "close p2p device fd: {}.", this->fd_);
        ::close(this->fd_);
        this->fd_ = -1;
    }
}

bool task_router::impl::init() {
    this->fd_ = ::open(AX_P2P_DEV, O_RDWR);
    if (this->fd_ < 0) {
        LOG_MM_E(TAG, "open p2p dev: {} failed.", AX_P2P_DEV);
        return false;
    }
    LOG_MM_D(TAG, "p2p dev: {}, fd: {} was opened.", AX_P2P_DEV, this->fd_);

    if (!get_local_id() || !get_local_index() || !get_world_size()) {
        return false;
    }

    get_addr_map();
    get_send_map();

    /* pre-bind mailbox outbound address of 1st target ep to warm up for cycle topo */
    if (this->send_map_.size() > 0) {
        auto target_id = this->send_map_[0].dst_target_id;
        for (uint32_t i = 0; i < this->p2p_cfg_.device_num; ++i) {
            if (this->p2p_cfg_.devices[i].target_id == target_id) {
                unsigned long long mbox_pci_addr = this->p2p_cfg_.devices[i].mbox_pci_addr;
                ::ioctl(this->fd_, AX_P2P_IOC_MAILBOX_OUTBOUND, &mbox_pci_addr);
                LOG_MM_I(TAG, "pre-bind mbox outbound to warm up: target id {:#x}, mbox pci addr {:#x}", target_id, mbox_pci_addr);
                break;
            }
        }
    }

    this->recv_items_.resize(this->world_size_ - 1);
    this->rank_size_ = this->send_map_[0].transfer_size;
    return reset_buffer();
}

bool task_router::impl::set_payload(const uint32_t size, const bool block) {
    if (size * this->world_size_ > this->p2p_cfg_.devices[0].cmm_size) {
        LOG_MM_E(TAG, "payload size {} > allowed cmm size {}.",
            size, this->p2p_cfg_.devices[0].cmm_size / this->world_size_);
        return false;
    }
    this->rank_size_ = size;
    for (auto& item : this->send_map_) {
        item.transfer_size = this->rank_size_;
        item.timeout = block ? TIMEOUT : 0;
    }
    dump_send_map();
    return true;
}

bool task_router::impl::verify_buffer() const {
    const auto cmm_phy = this->p2p_cfg_.devices[this->index_].cmm_phy_addr;
    const auto cmm_size = this->p2p_cfg_.devices[this->index_].cmm_size;
    auto cmm_vir = res_guard<uint8_t*> {
        [cmm_phy, cmm_size]() {
            return static_cast<uint8_t *>(::AX_SYS_Mmap(cmm_phy, cmm_size));
        },
        [cmm_size](void* p) {
            if (p) {
                ::AX_SYS_Munmap(p, cmm_size);
            }
        }
    };
    if (nullptr == cmm_vir.get()) {
        LOG_MM_E(TAG, "verify buffer: mmap payload (phy {:#x}, size {:#x}) failed.", cmm_phy, cmm_size);
        return false;
    }

    const auto slice_size = cmm_size / this->world_size_;

    auto verify_slice = [&](uint32_t rank_index) {
        const auto slice_vir = &cmm_vir.get()[rank_index * slice_size];
        if (rank_index == this->index_) {
            for (uint32_t i = 0; i < slice_size; ++i) {
                if (slice_vir[i] != static_cast<uint8_t>(this->id_)) {
                    LOG_MM_E(TAG, "verify buffer: index {} slice {} failed.", this->index_, rank_index);
                    return false;
                }
            }
        } else {
            const auto target_size = this->rank_size_;
            for (uint32_t i = 0; i < target_size; ++i) {
                if (slice_vir[i] != static_cast<uint8_t>(this->p2p_cfg_.devices[rank_index].target_id)) {
                    LOG_MM_E(TAG, "verify buffer: index {} slice {} failed.", this->index_, rank_index);
                    return false;
                }
            }
        }
        return true;
    };

    auto flag = true;
    for (uint32_t i = 0; i < this->world_size_; ++i) {
        flag &= verify_slice(i);
    }

    return flag;
}

bool task_router::impl::get_local_id() {
    int32_t id = 0;
    if (const auto ret = ::ioctl(this->fd_, AX_P2P_IOC_GET_LOCAL_ID, &id); 0 != ret) {
        LOG_MM_E(TAG, "get local id failed: {}", ret);
        return false;
    }
    this->id_ = id;
    LOG_MM_D(TAG, "local device id: {}", this->id_);
    return true;
}

bool task_router::impl::get_local_index() {
    if (const auto ret = ::ioctl(this->fd_, AX_P2P_IOC_CONFIG_GET, &this->p2p_cfg_); ret < 0) {
        LOG_MM_E(TAG, "fd {} ioctl AX_P2P_IOC_CONFIG_GET failed, {}", this->fd_, ::strerror(errno));
        return false;
    }
    if (0 == this->p2p_cfg_.device_num) {
        LOG_MM_E(TAG, "p2p device num is invalid: {}", this->p2p_cfg_.device_num);
        return false;
    }

    this->index_ = this->p2p_cfg_.device_num;
    for (uint32_t i = 0; i < this->p2p_cfg_.device_num; ++i) {
        if (this->p2p_cfg_.devices[i].target_id == this->id_) {
            this->index_ = i;
            break;
        }
    }
    if (this->index_ == this->p2p_cfg_.device_num) {
        LOG_MM_E(TAG, "device id {} get local index failed.", this->id_);
        return false;
    }
    LOG_MM_D(TAG, "device id {} index: {}", this->id_, this->index_);
    return true;
}

bool task_router::impl::get_world_size() {
    if (const auto ret = ::ioctl(this->fd_, AX_P2P_IOC_CONFIG_GET, &this->p2p_cfg_); ret < 0) {
        LOG_MM_E(TAG, "fd {} ioctl AX_P2P_IOC_CONFIG_GET failed, {}", this->fd_, ::strerror(errno));
        return false;
    }
    if (0 == this->p2p_cfg_.device_num) {
        LOG_MM_E(TAG, "p2p device num is invalid: {}", this->p2p_cfg_.device_num);
        return false;
    }
    this->world_size_ = this->p2p_cfg_.device_num;
    return true;
}

void task_router::impl::get_addr_map() {
    const auto cmm_size = this->p2p_cfg_.devices[0].cmm_size;
    const auto cmm_slice_size = cmm_size / this->world_size_;

    this->address_map_.resize(this->world_size_, std::vector<uint64_t>(this->world_size_, 0));
    for (uint32_t dev_idx = 0; dev_idx < this->world_size_; ++dev_idx) {
        const auto dev_base_addr = this->p2p_cfg_.devices[dev_idx].cmm_phy_addr;
        for (uint32_t rank_idx = 0; rank_idx < this->world_size_; ++rank_idx) {
            this->address_map_[dev_idx][rank_idx] = dev_base_addr + rank_idx * cmm_slice_size;
        }
    }
}

void task_router::impl::get_send_map() {
    const auto cmm_size = this->p2p_cfg_.devices[0].cmm_size;
    const auto cmm_slice_size = cmm_size / this->world_size_;

    this->send_map_.resize(this->world_size_ - 1);

    const auto dst_index = (this->index_ + 1) % this->world_size_;
    const auto dst_id = this->p2p_cfg_.devices[dst_index].target_id;

    for (uint32_t send_index = 0; send_index < this->send_map_.size(); ++send_index) {
        const auto send_addr_index = (this->index_ + this->world_size_ - send_index) % this->world_size_;
        auto&[timeout, time_consume,src_addr, dst_addr, send_size, target_id, cd] = this->send_map_[send_index];
        src_addr = this->address_map_[this->index_][send_addr_index];
        dst_addr = this->address_map_[dst_index][send_addr_index];
        timeout = -1;
        send_size = cmm_slice_size;
        target_id = dst_id;
        memset(cd, 0, std::size(cd));
    }
}

void task_router::impl::dump_send_map() const {
    for (const auto& item : this->send_map_) {
        LOG_MM_D(TAG, "p2p {} -> {}: {:#x} -> {:#x}, {:#08x} bytes.",
            this->id_, item.dst_target_id, item.src_phy_addr, item.dst_phy_addr, item.transfer_size);
    }
}

bool task_router::impl::send() {
    const auto rank_size = this->send_map_[0].transfer_size;
    LOG_MM_D(TAG, "start send payload with rank size: {:#08x}", rank_size);

    timer timer;
    for (const auto& item : this->send_map_) {
        if (const auto ret = ::ioctl(this->fd_, AX_P2P_IOC_ITEM_SEND, &item); ret < 0) {
            LOG_MM_E(TAG, "fd {} ioctl AX_P2P_IOC_ITEM_SEND failed, {}", this->fd_, ::strerror(errno));
            return false;
        }
        LOG_MM_D(TAG, "send payload from {} to {} success.", this->id_, item.dst_target_id);
    }
    LOG_MM_D(TAG, "send payload with rank size: {:#08x} success, elapsed: {} ms.", rank_size, timer.elapsed());
    return true;
}

bool task_router::impl::send_slice(const uint32_t rank_index) {
    const auto rank_size = this->send_map_[0].transfer_size;
    LOG_MM_D(TAG, "start send device (index: {}) payload with rank size: {:#08x}", rank_index, rank_size);

    const auto& item = this->send_map_[rank_index];
    timer timer;
    if (const auto ret = ::ioctl(this->fd_, AX_P2P_IOC_ITEM_SEND, &item); ret < 0) {
        LOG_MM_E(TAG, "fd {} ioctl AX_P2P_IOC_ITEM_SEND failed, {}", this->fd_, ::strerror(errno));
        return false;
    }
    LOG_MM_D(TAG, "send (index: {}) payload with rank size: {:#08x} success, elapsed: {} ms.", rank_index, rank_size, timer.elapsed());
    return true;
}

bool task_router::impl::recv() {
    timer timer;
    for (auto & recv_item : this->recv_items_) {
        fd_set set;
        FD_ZERO(&set);
        FD_SET(this->fd_, &set);

        const int32_t select_fd{this->fd_ + 1};
        const auto select_return = ::select(select_fd, &set, nullptr, nullptr, &this->timeout_);
        if (select_return < 0) {
            LOG_MM_E(TAG, "select fd {} error.", this->fd_);
            return false;
        }

        if (0 == select_return) {
            LOG_MM_E(TAG, "select fd {} timeout.", this->fd_);
            return false;
        }

        if (FD_ISSET(this->fd_, &set)) {
            if (const auto ret = ::ioctl(this->fd_, AX_P2P_IOC_ITEM_GET, &recv_item); ret < 0) {
                LOG_MM_E(TAG, "fd {} ioctl AX_P2P_IOC_ITEM_GET failed, {}", this->fd_, ::strerror(errno));
                return false;
            }
        }
    }
    const auto elapsed = timer.elapsed();

    std::string id_group;
    for (size_t i = 0; i < this->recv_items_.size() - 1; ++i) {
        id_group += std::to_string(this->recv_items_[i].msg.src_target_id) + ", ";
    }
    id_group += std::to_string(this->recv_items_[this->recv_items_.size() - 1].msg.src_target_id);

    LOG_MM_D(TAG, "recv payload from ({}) success, elapsed: {} ms.", id_group, elapsed);
    return true;
}

bool task_router::impl::recv_slice(const uint32_t rank_index) {
    timer timer;
    fd_set set;
    FD_ZERO(&set);
    FD_SET(this->fd_, &set);

    const int32_t select_fd{this->fd_ + 1};
    const auto select_return = ::select(select_fd, &set, nullptr, nullptr, &this->timeout_);
    if (select_return < 0) {
        LOG_MM_E(TAG, "select fd {} error.", this->fd_);
        return false;
    }

    if (0 == select_return) {
        LOG_MM_E(TAG, "select fd {} timeout.", this->fd_);
        return false;
    }

    if (FD_ISSET(this->fd_, &set)) {
        if (const auto ret = ::ioctl(this->fd_, AX_P2P_IOC_ITEM_GET, &this->recv_items_[rank_index]); ret < 0) {
            LOG_MM_E(TAG, "fd {} ioctl AX_P2P_IOC_ITEM_GET failed, {}", this->fd_, ::strerror(errno));
            return false;
        }
    }

    LOG_MM_D(TAG, "recv payload from {} success, elapsed: {} ms.", this->recv_items_[rank_index].msg.src_target_id, timer.elapsed());
    return true;
}

bool task_router::impl::reset_buffer() const {
    const auto cmm_phy = this->p2p_cfg_.devices[this->index_].cmm_phy_addr;
    const auto cmm_size = this->p2p_cfg_.devices[this->index_].cmm_size;
    auto cmm_vir = res_guard<uint8_t*> {
        [cmm_phy, cmm_size]() {
            return static_cast<uint8_t *>(::AX_SYS_Mmap(cmm_phy, cmm_size));
        },
        [cmm_size](void* p) {
            if (p) {
                ::AX_SYS_Munmap(p, cmm_size);
            }
        }
    };
    if (nullptr == cmm_vir.get()) {
        LOG_MM_E(TAG, "verify buffer: mmap payload (phy {:#x}, size {:#x}) failed.", cmm_phy, cmm_size);
        return false;
    }

    // clear all
    ::memset(cmm_vir.get(), 0, cmm_size);

    // fill self slice
    const auto slice_size = cmm_size / this->world_size_;
    const auto slice_vir = &cmm_vir.get()[this->index_ * slice_size];
    ::memset(slice_vir, static_cast<int>(this->id_), slice_size);

    return true;
}
