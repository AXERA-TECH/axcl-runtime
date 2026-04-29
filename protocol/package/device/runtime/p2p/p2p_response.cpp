/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "p2p_response_impl.hpp"

axcl::pkg::device::p2p_response::p2p_response() : m_impl(new impl()) {
}

axcl::pkg::device::p2p_response::p2p_response(p2p_response&& other) noexcept : m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::device::p2p_response& axcl::pkg::device::p2p_response::operator=(p2p_response&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }

    return *this;
}

axcl::pkg::device::p2p_response::~p2p_response() {
    delete m_impl;
}

bool axcl::pkg::device::p2p_response::decode(const void* message, uint32_t size) const {
    if (!runtime_response::decode(message, size)) {
        return false;
    }

    return m_impl->select(static_cast<axcl::runtime::request::Payload*>(runtime_response::get_payload()));
}

axcl::pkg::runtime::p2p::API axcl::pkg::device::p2p_response::get_api() const {
    return m_impl->get_api();
}

std::vector<struct axcl::pkg::runtime::p2p::p2p_config> axcl::pkg::device::p2p_response::get_p2p_config() const {
    return m_impl->get_p2p_config();
}

std::vector<struct axcl::pkg::runtime::p2p::bwt_config> axcl::pkg::device::p2p_response::get_bandwidth_test_config() const {
    return m_impl->get_bandwidth_test_config();
}

int32_t axcl::pkg::device::p2p_response::get_bandwidth_test_timeout() const {
    return m_impl->get_bandwidth_test_timeout();
}

uint32_t axcl::pkg::device::p2p_response::get_target_id() const {
    return m_impl->get_target_id();
}

void axcl::pkg::device::p2p_response::remove_p2p_config() {
    return m_impl->remove_p2p_config();
}
