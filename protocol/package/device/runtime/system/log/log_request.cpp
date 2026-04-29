/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "log_request.hpp"
#include "axcl_logger.hpp"
#include "log_request_impl.hpp"

#define TAG "package"

axcl::pkg::device::log_request::log_request() : m_impl(new impl(system_request::select(axcl::pkg::runtime::system::module::LOG))) {
}

axcl::pkg::device::log_request::log_request(log_request&& other) noexcept
    : system_request(std::move(other)), m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::device::log_request& axcl::pkg::device::log_request::operator=(log_request&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }

    return *this;
}

axcl::pkg::device::log_request::~log_request() {
    delete m_impl;
}

void axcl::pkg::device::log_request::set_name(const std::string& name) {
    m_impl->set_name(name);
}

void axcl::pkg::device::log_request::set_content(const char* data, uint32_t size) {
    m_impl->set_content(data, size);
}

void axcl::pkg::device::log_request::set_content(const std::string& data) {
    m_impl->set_content(data);
}
