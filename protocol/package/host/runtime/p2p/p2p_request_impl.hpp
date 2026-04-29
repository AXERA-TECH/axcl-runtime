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

#include "p2p_request.hpp"
#include "protocol.hpp"

namespace axcl::pkg::host {

struct p2p_request::impl {
    impl() = delete;
    explicit impl(void* payload) : m_payload(static_cast<axcl::runtime::p2p::request::Payload*>(payload)) {
        m_payload->mutable_head();
        m_payload->mutable_body();
    }

    void setup_p2p_config(const struct axcl::pkg::runtime::p2p::p2p_config* config, uint32_t num) {
        set_api(axcl::runtime::p2p::Api::P2P_SETUP_CONFIG);
        const auto body = m_payload->mutable_body();
        body->clear_config();
        body->mutable_config()->Reserve(num);
        for (uint32_t i = 0; i < num; ++i) {
            auto item = body->add_config();
            item->set_target_id(config[i].target_id);
            item->set_dma_pci_addr(config[i].dma_pci_addr);
            item->set_mbox_pci_addr(config[i].mbox_pci_addr);
            item->set_cmm_phy_addr(config[i].cmm_phy_addr);
            item->set_cmm_size(config[i].cmm_size);
        }
    }

    void start_bandwidth_test(uint32_t target_id, const struct axcl::pkg::runtime::p2p::bwt_config* config, uint32_t num, int32_t timeout) {
        set_api(axcl::runtime::p2p::Api::P2P_START_BANDWIDTH_TEST);
        const auto body = m_payload->mutable_body();
        body->set_target_id(target_id); /* src target id */
        body->set_bwt_timeout(timeout);
        body->clear_bwt_config();
        body->mutable_bwt_config()->Reserve(num);
        for (uint32_t i = 0; i < num; ++i) {
            auto item = body->add_bwt_config();
            item->set_target_id(config[i].target_id); /* dst target id */
            item->set_loop_count(config[i].loop_count);
            item->set_loop_interval(config[i].loop_interval);
            item->set_chunk_size(config[i].chunk_size);
        }
    }

    void remove_p2p_config(void) {
        set_api(axcl::runtime::p2p::Api::P2P_REMOVE_CONFIG);
    }

protected:
    void set_api(axcl::runtime::p2p::Api api) {
        m_payload->mutable_head()->set_api(api);
    }

private:
    axcl::runtime::p2p::request::Payload* m_payload;
};

}  // namespace axcl::pkg::host
