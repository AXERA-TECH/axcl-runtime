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

axcl::pkg::host::usrwork_response::usrwork_response() : m_impl(new impl()) {
}

axcl::pkg::host::usrwork_response::usrwork_response(usrwork_response&& other) noexcept : m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::host::usrwork_response& axcl::pkg::host::usrwork_response::operator=(usrwork_response&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }
    return *this;
}

axcl::pkg::host::usrwork_response::~usrwork_response() {
    delete m_impl;
}

bool axcl::pkg::host::usrwork_response::decode(const void* message, uint32_t size) const {

    if (!runtime_response::decode(message, size)) {
        return false;
    }

    return m_impl->select(static_cast<axcl::runtime::response::Payload*>(runtime_response::get_payload()));
}

int32_t axcl::pkg::host::usrwork_response::get_status() const {
    return m_impl->get_status();
}

uint32_t axcl::pkg::host::usrwork_response::get_pid() const {
    return m_impl->get_pid();
}

uint32_t axcl::pkg::host::usrwork_response::get_recvlen() const {
    return m_impl->get_recvlen();
}

const void* axcl::pkg::host::usrwork_response::get_data(size_t& size) const {
    return m_impl->get_data(size);
}

axcl::pkg::runtime::usrwork::API axcl::pkg::host::usrwork_response::get_api() const {
    return m_impl->get_api();
}