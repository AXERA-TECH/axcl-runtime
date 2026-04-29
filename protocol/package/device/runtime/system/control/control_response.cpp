/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "control_response.hpp"
#include "control_response_impl.hpp"
#include "axcl_logger.hpp"

#define TAG "package"

axcl::pkg::device::control_response::control_response() : m_impl(new impl()) {
}

axcl::pkg::device::control_response::control_response(control_response&& other) noexcept : m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::device::control_response& axcl::pkg::device::control_response::operator=(control_response&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }

    return *this;
}

axcl::pkg::device::control_response::~control_response() {
    delete m_impl;
}

[[nodiscard]] bool axcl::pkg::device::control_response::decode(const void* message, uint32_t size) const {
    if (!system_response::decode(message, size)) {
        return false;
    }

    return m_impl->select(static_cast<axcl::runtime::system::request::Payload*>(system_response::get_payload()));
}

axcl::pkg::runtime::system::command axcl::pkg::device::control_response::get_cmd() const {
    return m_impl->get_cmd();
}

uint32_t axcl::pkg::device::control_response::get_pid() const {
    return m_impl->get_pid();
}

uint32_t axcl::pkg::device::control_response::get_tid() const {
    return m_impl->get_tid();
}


const std::string& axcl::pkg::device::control_response::get_shell_cmd() const {
    return m_impl->get_shell_cmd();
}

const std::vector<std::string> axcl::pkg::device::control_response::get_shell_args() const {
    return m_impl->get_shell_args();
}

uint32_t axcl::pkg::device::control_response::get_context() const {
    return m_impl->get_context();
}

uint32_t axcl::pkg::device::control_response::get_stream() const {
    return m_impl->get_stream();
}

uint32_t axcl::pkg::device::control_response::get_event() const {
    return m_impl->get_event();
}

int32_t axcl::pkg::device::control_response::get_timeout() const {
    return m_impl->get_timeout();
}

uint32_t axcl::pkg::device::control_response::get_noresp_flag() const {
    return m_impl->get_noresp_flag();
}

uint32_t axcl::pkg::device::control_response::get_pcie_sub_vendor_id() const {
    return m_impl->get_pcie_sub_vendor_id();
}

uint32_t axcl::pkg::device::control_response::get_pcie_sub_device_id() const {
    return m_impl->get_pcie_sub_device_id();
}
