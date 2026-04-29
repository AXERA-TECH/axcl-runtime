/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "latency_request.hpp"
#include "latency_request_impl.hpp"
#include "axcl_logger.hpp"

#define TAG "package"

axcl::pkg::device::latency_request::latency_request()
    : m_impl(new impl(system_request::select(axcl::pkg::runtime::system::module::LATENCY))) {
}

axcl::pkg::device::latency_request::latency_request(latency_request&& other) noexcept
    : system_request(std::move(other)), m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::device::latency_request& axcl::pkg::device::latency_request::operator=(latency_request&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }

    return *this;
}

axcl::pkg::device::latency_request::~latency_request() {
    delete m_impl;
}

void axcl::pkg::device::latency_request::set_latency_data(const void* data, uint32_t size) {
    m_impl->set_latency_data(data, size);
}

void axcl::pkg::device::latency_request::set_latency_data(const std::string& data) {
    m_impl->set_latency_data(data);
}