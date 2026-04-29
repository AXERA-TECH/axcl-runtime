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
#include "package/include/runtime/system/type.hpp"

namespace axcl::pkg::device {

class system_response : public runtime_response {
public:
    explicit system_response();
    virtual ~system_response();

    system_response(system_response&& other) noexcept;
    system_response& operator=(system_response&& other) noexcept;
    system_response(const system_response&) = delete;
    system_response& operator=(const system_response&) = delete;

    [[nodiscard]] bool decode(const void* message, uint32_t size) const override;

    axcl::pkg::runtime::system::module get_module() const;

protected:
    void* get_payload() const override;

private:
    struct impl;
    impl* m_impl;
};

}  // namespace axcl::pkg::device