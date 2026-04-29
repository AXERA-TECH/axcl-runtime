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

axcl::pkg::host::control_response::control_response() : m_impl(new impl()) {
}

axcl::pkg::host::control_response::control_response(control_response&& other) noexcept : m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::host::control_response& axcl::pkg::host::control_response::operator=(control_response&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }

    return *this;
}

axcl::pkg::host::control_response::~control_response() {
    delete m_impl;
}

[[nodiscard]] bool axcl::pkg::host::control_response::decode(const void* message, uint32_t size) const {
    if (!system_response::decode(message, size)) {
        return false;
    }

    return m_impl->select(static_cast<axcl::runtime::system::response::Payload*>(system_response::get_payload()));
}

axcl::pkg::runtime::system::command axcl::pkg::host::control_response::get_cmd() const {
    return m_impl->get_cmd();
}

uint32_t axcl::pkg::host::control_response::get_pid() const {
    return m_impl->get_pid();
}

uint32_t axcl::pkg::host::control_response::get_tid() const {
    return m_impl->get_tid();
}

uint32_t axcl::pkg::host::control_response::get_context() const {
    return m_impl->get_context();
}

uint32_t axcl::pkg::host::control_response::get_stream() const {
    return m_impl->get_stream();
}

uint32_t axcl::pkg::host::control_response::get_event() const {
    return m_impl->get_event();
}

int32_t axcl::pkg::host::control_response::get_status() const {
    return m_impl->get_status();
}

int32_t axcl::pkg::host::control_response::get_shell_cmd_status() const {
    return m_impl->get_shell_cmd_status();
}

const std::string& axcl::pkg::host::control_response::get_shell_cmd_output() const {
    return m_impl->get_shell_cmd_output();
}

int32_t axcl::pkg::host::control_response::get_pcie_sub_id_setup_status() const {
    return m_impl->get_pcie_sub_id_setup_status();
}
