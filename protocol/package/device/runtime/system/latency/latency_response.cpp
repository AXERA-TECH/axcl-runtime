/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "latency_response.hpp"
#include "latency_response_impl.hpp"
#include "axcl_logger.hpp"

#define TAG "package"

axcl::pkg::device::latency_response::latency_response() : m_impl(new impl()) {
}

axcl::pkg::device::latency_response::latency_response(latency_response&& other) noexcept : m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::device::latency_response& axcl::pkg::device::latency_response::operator=(latency_response&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }

    return *this;
}

axcl::pkg::device::latency_response::~latency_response() {
    delete m_impl;
}

[[nodiscard]] bool axcl::pkg::device::latency_response::decode(const void* message, uint32_t size) const {
    if (!system_response::decode(message, size)) {
        return false;
    }

    return m_impl->select(static_cast<axcl::runtime::system::request::Payload*>(system_response::get_payload()));
}

const void* axcl::pkg::device::latency_response::get_latency_data(size_t& size) const {
    return m_impl->get_latency_data(size);
}

const std::string& axcl::pkg::device::latency_response::get_latency_data() const {
    return m_impl->get_latency_data();
}