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

#include "package/device/runtime/system/system_request.hpp"

namespace axcl::pkg::device {

class latency_request : public system_request {
public:
    explicit latency_request();
    virtual ~latency_request();

    latency_request(latency_request&& other) noexcept;
    latency_request& operator=(latency_request&& other) noexcept;
    latency_request(const latency_request&) = delete;
    latency_request& operator=(const latency_request&) = delete;

    void set_latency_data(const void* data, uint32_t size);
    void set_latency_data(const std::string& data);

private:
    struct impl;
    impl* m_impl;
};

}  // namespace axcl::pkg::device