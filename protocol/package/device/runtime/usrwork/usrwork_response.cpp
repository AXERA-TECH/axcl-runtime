/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "usrwork_response_impl.hpp"

axcl::pkg::device::usrwork_response::usrwork_response() : m_impl(new impl()) {
}

axcl::pkg::device::usrwork_response::usrwork_response(usrwork_response&& other) noexcept : m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::device::usrwork_response& axcl::pkg::device::usrwork_response::operator=(usrwork_response&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }

    return *this;
}

axcl::pkg::device::usrwork_response::~usrwork_response() {
    delete m_impl;
}

[[nodiscard]] bool axcl::pkg::device::usrwork_response::decode(const void* message, uint32_t size) const {
    if (!runtime_response::decode(message, size)) {
        return false;
    }

    return m_impl->select(static_cast<axcl::runtime::request::Payload*>(runtime_response::get_payload()));
}

axcl::pkg::runtime::usrwork::API axcl::pkg::device::usrwork_response::get_api() const {
    return m_impl->get_api();
}

uint32_t axcl::pkg::device::usrwork_response::get_pid() const {
    return m_impl->get_pid();
}

int32_t axcl::pkg::device::usrwork_response::get_timeout() const {
    return m_impl->get_timeout();
}

uint32_t axcl::pkg::device::usrwork_response::get_data_size() const {
    return m_impl->get_data_size();
}

const void* axcl::pkg::device::usrwork_response::get_data(size_t& size) const {
    return m_impl->get_data(size);
}

const std::string& axcl::pkg::device::usrwork_response::get_src_path() const {
    return m_impl->get_src_path();
}

const std::string& axcl::pkg::device::usrwork_response::get_dst_path() const {
    return m_impl->get_dst_path();
}

const std::string& axcl::pkg::device::usrwork_response::get_path() const {
    return m_impl->get_path();
}

std::vector<std::string> axcl::pkg::device::usrwork_response::get_argv() const {
    return m_impl->get_argv();
}

axcl::pkg::runtime::usrwork::FILE_TRANSFER_POLICY axcl::pkg::device::usrwork_response::get_policy() const {
    return m_impl->get_policy();
}

uint32_t axcl::pkg::device::usrwork_response::get_heartbeat_interval() const {
    return m_impl->get_heartbeat_interval();
}

uint32_t axcl::pkg::device::usrwork_response::get_port_num() const {
    return m_impl->get_port_num();
}

std::vector<uint32_t> axcl::pkg::device::usrwork_response::get_ports() const {
    return m_impl->get_ports();
}
