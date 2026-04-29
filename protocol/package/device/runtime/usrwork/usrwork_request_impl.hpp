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

#include "enum_cast.hpp"
#include "axcl_logger.hpp"
#include "usrwork_request.hpp"
#include "protocol.hpp"

#define TAG "package"

namespace axcl::pkg::device {

struct usrwork_request::impl {
    impl() = delete;
    explicit impl(void* payload) : m_payload(static_cast<axcl::runtime::usrwork::response::Payload*>(payload)) {
        m_payload->mutable_head();
        m_payload->mutable_body();
    }

    void set_api(axcl::pkg::runtime::usrwork::API api) const {
        m_payload->mutable_head()->set_api(axcl::enum_cast<axcl::pkg::runtime::usrwork::API, axcl::runtime::usrwork::Api>(api));
    }

    void set_status(int32_t status) const {
        m_payload->mutable_body()->set_status(status);
    }

    void set_pid(uint32_t pid) const {
        m_payload->mutable_body()->set_pid(pid);
    }

    void set_data_size(uint32_t data_size) const {
        m_payload->mutable_body()->set_data_size(data_size);
    }

    void set_data(const void* data, uint32_t data_size) const {
        if (data && data_size > 0) {
            m_payload->mutable_body()->set_data(data, data_size);
        }
    }

private:
    axcl::runtime::usrwork::response::Payload* m_payload;
};

}  // namespace axcl::pkg::device
