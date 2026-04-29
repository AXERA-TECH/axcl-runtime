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
#include "package/include/runtime/system/type.hpp"

namespace axcl::pkg::device {

class system_request : public runtime_request {
public:
    explicit system_request();
    virtual ~system_request();

    system_request(system_request&& other) noexcept;
    system_request& operator=(system_request&& other) noexcept;
    system_request(const system_request&) = delete;
    system_request& operator=(const system_request&) = delete;

    axcl::pkg::runtime::system::module get_module() const;

protected:
    [[nodiscard]] void* select(const axcl::pkg::runtime::system::module& module) const;

private:
    struct impl;
    impl* m_impl;
};

}  // namespace axcl::pkg::device