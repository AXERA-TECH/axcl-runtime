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

axcl::pkg::host::log_request::log_request() : m_impl(new impl(system_request::select(axcl::pkg::runtime::system::module::LOG))) {
}

axcl::pkg::host::log_request::log_request(log_request&& other) noexcept
    : system_request(std::move(other)), m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::host::log_request& axcl::pkg::host::log_request::operator=(log_request&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }

    return *this;
}

axcl::pkg::host::log_request::~log_request() {
    delete m_impl;
}

void axcl::pkg::host::log_request::set_type(uint32_t type) {
    m_impl->set_type(type);
}

void axcl::pkg::host::log_request::set_flag(uint32_t flag) {
    m_impl->set_flag(flag);
}
