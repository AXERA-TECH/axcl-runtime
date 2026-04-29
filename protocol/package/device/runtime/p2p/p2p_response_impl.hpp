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

#include <vector>
#include "axcl_logger.hpp"
#include "p2p_response.hpp"
#include "protocol.hpp"

#define TAG "package"

namespace axcl::pkg::device {

struct p2p_response::impl {
    [[nodiscard]] bool check_payload() {
        if (!m_payload->has_head() || !m_payload->has_body()) {
            LOG_MM_E(TAG, "p2p runtime response check payload fail, has head {} has body {}", m_payload->has_head(), m_payload->has_body());
            return false;
        }

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

        return true;
    }

    [[nodiscard]] bool select(axcl::runtime::request::Payload* payload) {
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

    std::vector<struct axcl::pkg::runtime::p2p::p2p_config> get_p2p_config() const {
        const auto& config = m_payload->body().config();
        const size_t num = config.size();
        std::vector<struct axcl::pkg::runtime::p2p::p2p_config> v;
        v.resize(num);
        for (size_t i = 0; i < num; ++i) {
            const auto& item = config[i];
            v[i].target_id = item.target_id();
            v[i].dma_pci_addr = item.dma_pci_addr();
            v[i].mbox_pci_addr = item.mbox_pci_addr();
            v[i].cmm_phy_addr = item.cmm_phy_addr();
            v[i].cmm_size = item.cmm_size();
        }

        return v;
    }

    uint32_t get_target_id() const {
        return m_payload->body().target_id();
    }

    std::vector<struct axcl::pkg::runtime::p2p::bwt_config> get_bandwidth_test_config() const {
        const auto& config = m_payload->body().bwt_config();
        const size_t num = config.size();
        std::vector<struct axcl::pkg::runtime::p2p::bwt_config> v;
        v.resize(num);
        for (size_t i = 0; i < num; ++i) {
            const auto& item = config[i];
            v[i].target_id = item.target_id();
            v[i].chunk_size = item.chunk_size();
            v[i].loop_count = item.loop_count();
            v[i].loop_interval = item.loop_interval();
        }

        return v;
    }

    int32_t get_bandwidth_test_timeout() const {
        return m_payload->body().bwt_timeout();
    }

    void remove_p2p_config() {
        // pass
    }

    axcl::pkg::runtime::p2p::API get_api() const {
        return m_api;
    }

private:
    axcl::pkg::runtime::p2p::API m_api{};
    axcl::runtime::p2p::request::Payload* m_payload = {nullptr};
};  // namespace axcl::pkg::device

}  // namespace axcl::pkg::device
