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
#include "p2p_request.hpp"
#include "protocol.hpp"

#define TAG "package"

namespace axcl::pkg::device {

struct p2p_request::impl {
    impl() = delete;
    explicit impl(void* payload) : m_payload(static_cast<axcl::runtime::p2p::response::Payload*>(payload)) {
        m_payload->mutable_head();
        m_payload->mutable_body();
    }

    void set_api(axcl::pkg::runtime::p2p::API api) const {
        m_payload->mutable_head()->set_api(axcl::enum_cast<axcl::pkg::runtime::p2p::API, axcl::runtime::p2p::Api>(api));
    }

    void set_status(int32_t status) const {
        m_payload->mutable_body()->set_status(status);
    }

    void set_bandwidth_test_result(const struct axcl::pkg::runtime::p2p::bwt_result &res) {
        auto result = m_payload->mutable_body()->mutable_bwt_res();
        result->set_target_id(res.target_id);
        result->set_avg(res.avg);
        result->set_min(res.min);
        result->set_max(res.max);
    }

private:
    axcl::runtime::p2p::response::Payload* m_payload;
};

}  // namespace axcl::pkg::device
