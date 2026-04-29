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

axcl::pkg::host::usrwork_request::usrwork_request() : m_impl(new impl(runtime_request::select(axcl::pkg::runtime::module::USRWORK))) {
}

axcl::pkg::host::usrwork_request::usrwork_request(usrwork_request&& other) noexcept
    : runtime_request(std::move(other)), m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::host::usrwork_request& axcl::pkg::host::usrwork_request::operator=(usrwork_request&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }
    return *this;
}

axcl::pkg::host::usrwork_request::~usrwork_request() {
    delete m_impl;
}

void axcl::pkg::host::usrwork_request::transfer_file(const char* src_path, const char* dst_path, uint32_t policy) const {
    m_impl->transfer_file(src_path, dst_path, policy);
}

void axcl::pkg::host::usrwork_request::set_data(const void* data, uint32_t data_size) const {
    m_impl->set_data(data, data_size);
}

void axcl::pkg::host::usrwork_request::exec_worker(const char* path, const std::vector<std::string>& argv,
                                                    uint32_t heartbeat_interval, uint32_t port_num,
                                                    const std::vector<uint32_t>& ports) const {
    m_impl->exec_worker(path, argv, heartbeat_interval, port_num, ports);
}

void axcl::pkg::host::usrwork_request::kill_worker(uint32_t pid) const {
    m_impl->kill_worker(pid);
}

void axcl::pkg::host::usrwork_request::worker_send(uint32_t pid, const void* buf, uint32_t size, int32_t timeout) const {
    m_impl->worker_send(pid, buf, size, timeout);
}

void axcl::pkg::host::usrwork_request::worker_recv(uint32_t pid, uint32_t bufsize, int32_t timeout) const {
    m_impl->worker_recv(pid, bufsize, timeout);
}

axcl::pkg::runtime::usrwork::API axcl::pkg::host::usrwork_request::get_api() const {
    return m_impl->get_api();
}