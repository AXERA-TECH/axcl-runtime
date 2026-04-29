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
#include <unordered_map>
#include "package/device/runtime/memory/memory_request.hpp"
#include "package/device/runtime/memory/memory_response.hpp"
#include "ax_sys_api.h"
#include "base.hpp"
#include "runtime_api.hpp"

#define RUNTIME_MEMORY_API_DECL(_API_) \
            RUNTIME_API_DECL(_API_, RUNTIME_API_PARAM(memory))

#define RUNTIME_MEMORY_API_IMPL(_API_) \
            RUNTIME_API_IMPL(memory, _API_, RUNTIME_API_PARAM(memory))

#define RUNTIME_MEMORY_API_APPEND(_API_) \
            RUNTIME_API_APPEND(memory, _API_, RUNTIME_API_PARAM(memory))

namespace axcl::worker {

template<>
bool axcl::worker::runtime_api<axcl::pkg::runtime::memory::API,
                axcl::pkg::device::memory_response,
                axcl::pkg::device::memory_request>::handle_ack_err(
    const axcl::pkg::device::memory_response& decoder, int32_t ret);

class dispatcher;
class memory : public base {
public:
    memory(dispatcher *dispatcher);
    ~memory() = default;

    virtual void run(const void *data, const size_t& size) override;

private:
    RUNTIME_MEMORY_API_DECL(ALLOC);
    RUNTIME_MEMORY_API_DECL(FREE);
    RUNTIME_MEMORY_API_DECL(FLUSH);
    RUNTIME_MEMORY_API_DECL(INVALIDATE);
    RUNTIME_MEMORY_API_DECL(SET);
    RUNTIME_MEMORY_API_DECL(WRITE);
    RUNTIME_MEMORY_API_DECL(READ);
    RUNTIME_MEMORY_API_DECL(SYNC);
    RUNTIME_MEMORY_API_DECL(CMP);

protected:
    void insert(const uint64_t& phyaddr, void* pviraddr);
    int32_t free(const uint64_t& phyaddr);
    void* find(const uint64_t& phyaddr);

private:
    std::mutex m_mutex;
    std::unordered_map<uint64_t, void *> m_map;
    runtime_api<axcl::pkg::runtime::memory::API, axcl::pkg::device::memory_response, axcl::pkg::device::memory_request> m_api;
    axcl::pkg::memcpy_package m_memcpy_package;
};

}  // namespace axcl::worker
