/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 * Author: wanglusheng@axera-tech.com
 *
 **************************************************************************************************/

#pragma once

#include "package/host/response.hpp"
#include "package/include/runtime/type.hpp"

namespace axcl::pkg::host {

class runtime_response : public response {
public:
    explicit runtime_response();
    virtual ~runtime_response();

    runtime_response(runtime_response&& other) noexcept;
    runtime_response& operator=(runtime_response&& other) noexcept;
    runtime_response(const runtime_response&) = delete;
    runtime_response& operator=(const runtime_response&) = delete;

    [[nodiscard]] bool decode(const void* message, uint32_t size) const override;

    axcl::pkg::runtime::module get_module() const;

protected:
    void* get_payload() const override;

private:
    struct impl;
    impl* m_impl;
};

}  // namespace axcl::pkg::host
