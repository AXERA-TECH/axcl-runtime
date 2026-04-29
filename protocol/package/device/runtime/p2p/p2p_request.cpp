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

axcl::pkg::device::p2p_request::p2p_request() : m_impl(new impl(runtime_request::select(axcl::pkg::runtime::module::P2P))) {
}

axcl::pkg::device::p2p_request::p2p_request(p2p_request&& other) noexcept
    : runtime_request(std::move(other)), m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::device::p2p_request& axcl::pkg::device::p2p_request::operator=(p2p_request&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }

    return *this;
}

axcl::pkg::device::p2p_request::~p2p_request() {
    delete m_impl;
}

void axcl::pkg::device::p2p_request::set_api(axcl::pkg::runtime::p2p::API api) const {
    m_impl->set_api(api);
}

void axcl::pkg::device::p2p_request::set_status(int32_t status) const {
    m_impl->set_status(status);
}

void axcl::pkg::device::p2p_request::set_bandwidth_test_result(const struct axcl::pkg::runtime::p2p::bwt_result &res) const {
    m_impl->set_bandwidth_test_result(res);
}
