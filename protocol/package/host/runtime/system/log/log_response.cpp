/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "log_response.hpp"
#include "axcl_logger.hpp"
#include "log_response_impl.hpp"

#define TAG "package"

axcl::pkg::host::log_response::log_response() : m_impl(new impl()) {
}

axcl::pkg::host::log_response::log_response(log_response&& other) noexcept : m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::host::log_response& axcl::pkg::host::log_response::operator=(log_response&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }

    return *this;
}

axcl::pkg::host::log_response::~log_response() {
    delete m_impl;
}

[[nodiscard]] bool axcl::pkg::host::log_response::decode(const void* message, uint32_t size) const {
    if (!system_response::decode(message, size)) {
        return false;
    }

    return m_impl->select(static_cast<axcl::runtime::system::response::Payload*>(system_response::get_payload()));
}

const std::string& axcl::pkg::host::log_response::get_name() const {
    return m_impl->get_name();
}

const std::string& axcl::pkg::host::log_response::get_content() const {
    return m_impl->get_content();
}

const char* axcl::pkg::host::log_response::get_content(size_t& size) const {
    return m_impl->get_content(size);
}