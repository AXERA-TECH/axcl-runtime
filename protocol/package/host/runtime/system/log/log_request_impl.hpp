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

namespace axcl::pkg::host {

struct log_request::impl {
    impl() = delete;
    explicit impl(void* payload) : m_payload(static_cast<axcl::runtime::system::request::Log*>(payload)) {
    }

    void set_type(uint32_t type) {
        m_payload->set_type(type);
    }

    void set_flag(uint32_t flag) {
        m_payload->set_flag(flag);
    }

private:
    axcl::runtime::system::request::Log* m_payload;
};

}  // namespace axcl::pkg::host