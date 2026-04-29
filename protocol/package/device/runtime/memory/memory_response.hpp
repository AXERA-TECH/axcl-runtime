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

#include "package/device/runtime/runtime_response.hpp"
#include "package/include/runtime/memory/type.hpp"

namespace axcl::pkg::device {

class memory_response : public runtime_response {
public:
    explicit memory_response();
    virtual ~memory_response();

    memory_response(memory_response&& other) noexcept;
    memory_response& operator=(memory_response&& other) noexcept;
    memory_response(const memory_response&) = delete;
    memory_response& operator=(const memory_response&) = delete;

    [[nodiscard]] bool decode(const void* message, uint32_t size) const override;

    axcl::pkg::runtime::memory::API get_api() const;
    uint64_t get_addr() const;
    uint64_t get_size() const;
    uint64_t get_from() const;
    uint64_t get_phy() const;
    uint64_t get_kind() const;
    axcl::pkg::runtime::memory::CHACHE_TYPE get_cache_type() const;
    uint32_t get_index() const;
    uint32_t get_value() const;
    uint32_t get_policy() const;
    uint32_t get_noresp_flag() const;

private:
    struct impl;
    impl* m_impl;
};

}  // namespace axcl::pkg::device