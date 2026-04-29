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
#include <mutex>
#include <unordered_map>

namespace axcl::rt {

class device_memory {
public:
    device_memory() = default;
    device_memory(const device_memory &) = delete;
    device_memory &operator=(const device_memory &) = delete;

    void insert(uint64_t addr, size_t size) {
        std::lock_guard<std::mutex> lck(m_mtx);
        m_mem[addr] = size;
    }

    void remove(uint64_t addr) {
        std::lock_guard<std::mutex> lck(m_mtx);
        m_mem.erase(addr);
    }

    bool find(uint64_t addr) {
        std::lock_guard<std::mutex> lck(m_mtx);
        auto it = m_mem.find(addr);
        if (it != m_mem.end()) {
            return true;
        }

        return false;
    }

    void print();

private:
    std::mutex m_mtx;
    std::unordered_map<uint64_t /* addr */, int32_t /* size*/> m_mem;
};

}  // namespace axcl::rt