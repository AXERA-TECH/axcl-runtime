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

axcl::pkg::host::p2p_response::p2p_response() : m_impl(new impl()) {
}

axcl::pkg::host::p2p_response::p2p_response(p2p_response&& other) noexcept : m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::host::p2p_response& axcl::pkg::host::p2p_response::operator=(p2p_response&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }

    return *this;
}

axcl::pkg::host::p2p_response::~p2p_response() {
    delete m_impl;
}

bool axcl::pkg::host::p2p_response::decode(const void* message, uint32_t size) const {
    if (!runtime_response::decode(message, size)) {
        return false;
    }

    return m_impl->select(static_cast<axcl::runtime::response::Payload*>(runtime_response::get_payload()));
}

int32_t axcl::pkg::host::p2p_response::get_status() const {
    return m_impl->get_status();
}

axcl::pkg::runtime::p2p::API axcl::pkg::host::p2p_response::get_api() const {
    return m_impl->get_api();
}

struct axcl::pkg::runtime::p2p::bwt_result axcl::pkg::host::p2p_response::get_bandwidth_test_result() const {
    return m_impl->get_bandwidth_test_result();
}