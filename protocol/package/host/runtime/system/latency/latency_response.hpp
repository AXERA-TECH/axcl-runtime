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

#include "package/host/runtime/system/system_response.hpp"

namespace axcl::pkg::host {

class latency_response : public system_response {
public:
    explicit latency_response();
    virtual ~latency_response();

    latency_response(latency_response&& other) noexcept;
    latency_response& operator=(latency_response&& other) noexcept;
    latency_response(const latency_response&) = delete;
    latency_response& operator=(const latency_response&) = delete;

    const void* get_latency_data(size_t& size) const;
    const std::string& get_latency_data() const;

    [[nodiscard]] bool decode(const void* message, uint32_t size) const override;

private:
    struct impl;
    impl* m_impl;
};

}  // namespace axcl::pkg::host