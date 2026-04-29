/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "system_response.hpp"
#include "axcl_logger.hpp"
#include "system_response_impl.hpp"

#define TAG "package"

axcl::pkg::host::system_response::system_response() : m_impl(new impl()) {
}

axcl::pkg::host::system_response::system_response(system_response&& other) noexcept : m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::host::system_response& axcl::pkg::host::system_response::operator=(system_response&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }

    return *this;
}

axcl::pkg::host::system_response::~system_response() {
    delete m_impl;
}

[[nodiscard]] bool axcl::pkg::host::system_response::decode(const void* message, uint32_t size) const {
    if (!runtime_response::decode(message, size)) {
        return false;
    }

    return m_impl->select(static_cast<axcl::runtime::response::Payload*>(runtime_response::get_payload()));
}

axcl::pkg::runtime::system::module axcl::pkg::host::system_response::get_module() const {
    return m_impl->get_module();
}

void* axcl::pkg::host::system_response::get_payload() const {
    return m_impl->get_payload();
}
