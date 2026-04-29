/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "system_request.hpp"
#include "axcl_logger.hpp"
#include "system_request_impl.hpp"

#define TAG "package"

axcl::pkg::device::system_request::system_request() : m_impl(new impl(runtime_request::select(axcl::pkg::runtime::module::SYSTEM))) {
}

axcl::pkg::device::system_request::system_request(system_request&& other) noexcept
    : runtime_request(std::move(other)), m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::device::system_request& axcl::pkg::device::system_request::operator=(system_request&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }

    return *this;
}

axcl::pkg::device::system_request::~system_request() {
    delete m_impl;
}

[[nodiscard]] void* axcl::pkg::device::system_request::select(const axcl::pkg::runtime::system::module& module) const {
    return m_impl->select(module);
}

axcl::pkg::runtime::system::module axcl::pkg::device::system_request::get_module() const {
    return m_impl->get_module();
}
