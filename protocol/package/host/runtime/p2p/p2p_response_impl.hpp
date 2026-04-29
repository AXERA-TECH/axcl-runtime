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

#include "axcl_logger.hpp"
#include "p2p_response.hpp"
#include "protocol.hpp"

#define TAG "package"

namespace axcl::pkg::host {

struct p2p_response::impl {
    [[nodiscard]] bool check_payload() {
        if (!m_payload->has_head() || !m_payload->has_body()) {
            LOG_MM_E(TAG, "p2p runtime response check payload fail, has head {} has body {}", m_payload->has_head(), m_payload->has_body());
            return false;
        }

        bool status = true;
        switch (m_payload->head().api()) {
            case axcl::runtime::p2p::Api::P2P_SETUP_CONFIG:
                m_api = axcl::pkg::runtime::p2p::API::P2P_SETUP_CONFIG;
                break;
            case axcl::runtime::p2p::Api::P2P_START_BANDWIDTH_TEST:
                m_api = axcl::pkg::runtime::p2p::API::P2P_START_BANDWIDTH_TEST;
                break;
            case axcl::runtime::p2p::Api::P2P_REMOVE_CONFIG:
                m_api = axcl::pkg::runtime::p2p::API::P2P_REMOVE_CONFIG;
                break;
            default:
                LOG_MM_E(TAG, "unknown api {} of p2p runtime response", static_cast<int32_t>(m_payload->head().api()));
                return false;
        }

        return status;
    }

    [[nodiscard]] bool select(axcl::runtime::response::Payload *payload) {
        if (axcl::runtime::P2P != payload->head().module()) {
            LOG_MM_E(TAG, "current module {} is not p2p module", static_cast<int32_t>(payload->head().module()));
            return false;
        }

        m_payload = payload->mutable_body()->mutable_p2p();
        if (!m_payload) {
            LOG_MM_E(TAG, "p2p runtime response select payload fail, nil pointer");
            return false;
        }

        return check_payload();
    }

    int32_t get_status() const {
        return m_payload->body().status();
    }

    struct axcl::pkg::runtime::p2p::bwt_result get_bandwidth_test_result() const {
        struct axcl::pkg::runtime::p2p::bwt_result result;
        const auto& res = m_payload->body().bwt_res();
        result.target_id = res.target_id();
        result.avg = res.avg();
        result.min = res.min();
        result.max = res.max();
        return result;
    }

    axcl::pkg::runtime::p2p::API get_api() const {
        return m_api;
    }

private:
    axcl::pkg::runtime::p2p::API m_api{};
    axcl::runtime::p2p::response::Payload *m_payload = {nullptr};
};  // namespace axcl::pkg::host

}  // namespace axcl::pkg::host
