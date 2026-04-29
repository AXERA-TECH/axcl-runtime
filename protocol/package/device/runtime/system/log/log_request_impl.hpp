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

struct log_request::impl {
    impl() = delete;
    explicit impl(void* payload) : m_payload(static_cast<axcl::runtime::system::response::Log*>(payload)) {
    }

    void set_name(const std::string& name) {
        m_payload->set_name(name);
    }

    void set_content(const char* data, uint32_t size) {
        m_payload->set_content(data, size);
    }

    void set_content(const std::string& data) {
        m_payload->set_content(data);
    }

private:
    axcl::runtime::system::response::Log* m_payload;
};

}  // namespace axcl::pkg::device