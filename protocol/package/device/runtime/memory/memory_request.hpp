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

#include "package/device/runtime/runtime_request.hpp"
#include "package/include/runtime/memory/type.hpp"

namespace axcl::pkg::device {

class memory_request : public runtime_request {
public:
    explicit memory_request();
    virtual ~memory_request();

    memory_request(memory_request&& other) noexcept;
    memory_request& operator=(memory_request&& other) noexcept;
    memory_request(const memory_request&) = delete;
    memory_request& operator=(const memory_request&) = delete;

    void set_api(axcl::pkg::runtime::memory::API api) const;
    void set_status(int32_t status) const;
    void set_addr(const void* addr) const;

private:
    struct impl;
    impl* m_impl;
};

}  // namespace axcl::pkg::device