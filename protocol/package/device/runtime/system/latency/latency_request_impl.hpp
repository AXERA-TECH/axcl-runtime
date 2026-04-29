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

#include "protocol.hpp"

namespace axcl::pkg::device {

struct latency_request::impl {
    impl() = delete;
    explicit impl(void* payload) : m_payload(static_cast<axcl::runtime::system::response::Latency*>(payload)) {
    }

    void set_latency_data(const void* data, uint32_t size) {
        if (data && size > 0) {
            auto s = m_payload->mutable_data();
            s->assign(static_cast<const char*>(data), size);
        }
    }

    void set_latency_data(const std::string& data) {
        m_payload->set_data(data);
    }

private:
    axcl::runtime::system::response::Latency* m_payload;
};

}  // namespace axcl::pkg::device