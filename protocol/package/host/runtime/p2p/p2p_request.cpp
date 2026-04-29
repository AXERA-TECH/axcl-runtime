/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "p2p_request_impl.hpp"

axcl::pkg::host::p2p_request::p2p_request() : m_impl(new impl(runtime_request::select(axcl::pkg::runtime::module::P2P))) {
}

axcl::pkg::host::p2p_request::p2p_request(p2p_request&& other) noexcept
    : runtime_request(std::move(other)), m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::host::p2p_request& axcl::pkg::host::p2p_request::operator=(p2p_request&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }

    return *this;
}

axcl::pkg::host::p2p_request::~p2p_request() {
    delete m_impl;
}

void axcl::pkg::host::p2p_request::setup_p2p_config(const struct axcl::pkg::runtime::p2p::p2p_config* config, uint32_t num) const {
    m_impl->setup_p2p_config(config, num);
}

void axcl::pkg::host::p2p_request::start_bandwidth_test(uint32_t target_id, const struct axcl::pkg::runtime::p2p::bwt_config* config, uint32_t num, int32_t timeout) const {
    m_impl->start_bandwidth_test(target_id, config, num, timeout);
}

void axcl::pkg::host::p2p_request::remove_p2p_config(void) const {
    m_impl->remove_p2p_config();
}
