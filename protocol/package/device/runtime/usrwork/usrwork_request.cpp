/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "usrwork_request_impl.hpp"

axcl::pkg::device::usrwork_request::usrwork_request() : m_impl(new impl(runtime_request::select(axcl::pkg::runtime::module::USRWORK))) {
}

axcl::pkg::device::usrwork_request::usrwork_request(usrwork_request&& other) noexcept
    : runtime_request(std::move(other)), m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::device::usrwork_request& axcl::pkg::device::usrwork_request::operator=(usrwork_request&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }

    return *this;
}

axcl::pkg::device::usrwork_request::~usrwork_request() {
    delete m_impl;
}

void axcl::pkg::device::usrwork_request::set_api(axcl::pkg::runtime::usrwork::API api) const {
    m_impl->set_api(api);
}

void axcl::pkg::device::usrwork_request::set_status(int32_t status) const {
    m_impl->set_status(status);
}

void axcl::pkg::device::usrwork_request::set_pid(uint32_t pid) const {
    m_impl->set_pid(pid);
}

void axcl::pkg::device::usrwork_request::set_data_size(uint32_t data_size) const {
    m_impl->set_data_size(data_size);
}

void axcl::pkg::device::usrwork_request::set_data(const void* data, uint32_t data_size) const {
    m_impl->set_data(data, data_size);
}
