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

#include "axcl_package.hpp"
#include "package/host/runtime/runtime_request.hpp"
#include "package/include/runtime/memory/type.hpp"

namespace axcl::pkg::host {

class memory_request : public runtime_request {
public:
    explicit memory_request();
    virtual ~memory_request();

    memory_request(memory_request&& other) noexcept;
    memory_request& operator=(memory_request&& other) noexcept;
    memory_request(const memory_request&) = delete;
    memory_request& operator=(const memory_request&) = delete;

    void alloc(uint32_t size, uint32_t policy, bool cached = false) const;
    void free(const void* addr) const;
    void read(const void* from, const void* to, uint32_t size, axcl::pkg::memcpy_kind kind, uint32_t noresp=0) const;
    void write(const void* from, const void* to, uint32_t size, axcl::pkg::memcpy_kind kind, uint32_t noresp=0) const;
    void set(const void* addr, uint8_t value, uint32_t size, uint32_t noresp=0) const;
    void flush(const void* addr, uint32_t size) const;
    void invalidate(const void* addr, uint32_t size) const;
    void sync(const void* from, const void* to, uint32_t size, uint32_t noresp=0) const;
    void cmp(const void* s1, const void* s2, uint32_t size, uint32_t noresp=0) const;

    axcl::pkg::runtime::memory::API get_api() const;

    [[nodiscard]] void* encode(const axcl::pkg::head& hd, uint32_t& size) const override;

private:
    struct impl;
    impl* m_impl;
};

}  // namespace axcl::pkg::host