/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "control_request.hpp"
#include "control_request_impl.hpp"
#include "axcl_logger.hpp"

#define TAG "package"

axcl::pkg::device::control_request::control_request()
    : m_impl(new impl(system_request::select(axcl::pkg::runtime::system::module::CONTROL))) {
}

axcl::pkg::device::control_request::control_request(control_request&& other) noexcept
    : system_request(std::move(other)), m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::device::control_request& axcl::pkg::device::control_request::operator=(control_request&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }

    return *this;
}

axcl::pkg::device::control_request::~control_request() {
    delete m_impl;
}

void axcl::pkg::device::control_request::create_context(uint32_t pid, uint32_t tid, uint32_t context, uint32_t stream) const {
    m_impl->create_context(pid, tid, context, stream);
}

void axcl::pkg::device::control_request::destroy_context(uint32_t pid, uint32_t tid, uint32_t context, uint32_t stream) const {
    m_impl->destroy_context(pid, tid, context, stream);
}

void axcl::pkg::device::control_request::create_stream(uint32_t context, uint32_t stream) const {
    m_impl->create_stream(context, stream);
}

void axcl::pkg::device::control_request::destroy_stream(uint32_t context, uint32_t stream) const {
    m_impl->destroy_stream(context, stream);
}

void axcl::pkg::device::control_request::sync_stream(uint32_t context, uint32_t stream, int32_t status) const {
    m_impl->sync_stream(context, stream, status);
}

void axcl::pkg::device::control_request::create_event(uint32_t context, uint32_t stream, uint32_t event) const {
    m_impl->create_event(context, stream, event);
}

void axcl::pkg::device::control_request::destroy_event(uint32_t context, uint32_t stream, uint32_t event) const {
    m_impl->destroy_event(context, stream, event);
}

void axcl::pkg::device::control_request::record_event(uint32_t context, uint32_t stream, uint32_t event, int32_t status) const {
    m_impl->record_event(context, stream, event, status);
}

void axcl::pkg::device::control_request::wait_event(uint32_t context, uint32_t stream, uint32_t event, int32_t status) const {
    m_impl->wait_event(context, stream, event, status);
}

void axcl::pkg::device::control_request::set_shell_cmd_output(int32_t status, const char* output) const {
    m_impl->set_shell_cmd_output(status, output);
}

axcl::pkg::runtime::system::command axcl::pkg::device::control_request::get_cmd() const {
    return m_impl->get_cmd();
}

void axcl::pkg::device::control_request::set_pcie_sub_id_setup_status(int32_t status) const {
    m_impl->set_pcie_sub_id_setup_status(status);
}
