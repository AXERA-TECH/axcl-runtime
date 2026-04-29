/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#pragma once

#include "usrwork_request.hpp"
#include "protocol.hpp"
#include <vector>

namespace axcl::pkg::host {

struct usrwork_request::impl {
    impl() = delete;
    explicit impl(void* payload) : m_payload(static_cast<axcl::runtime::usrwork::request::Payload*>(payload)) {
        m_payload->mutable_head();
        m_payload->mutable_body();
    }

    void transfer_file(const char* src_path, const char* dst_path, uint32_t policy) {
        set_api(axcl::runtime::usrwork::TRANSFER_FILE);
        const auto body = m_payload->mutable_body();
        body->set_src_path(src_path);
        if (dst_path) {
            body->set_dst_path(dst_path);
        }
        body->set_policy(static_cast<axcl::runtime::usrwork::FileTransferPolicy>(policy));
    }

    void set_data(const void* data, uint32_t data_size) {
        const auto body = m_payload->mutable_body();
        body->set_data(data, data_size);
        body->set_data_size(data_size);
    }

    void exec_worker(const char* path, const std::vector<std::string>& argv,
                     uint32_t heartbeat_interval, uint32_t port_num,
                     const std::vector<uint32_t>& ports) {
        set_api(axcl::runtime::usrwork::EXEC_WORKER);
        const auto body = m_payload->mutable_body();
        body->set_path(path);
        body->clear_argv();
        for (const auto& arg : argv) {
            body->add_argv(arg);
        }

        body->set_heartbeat_interval(heartbeat_interval);
        body->set_port_num(port_num);
        body->clear_ports();
        for (const auto& port : ports) {
            body->add_ports(port);
        }
    }

    void kill_worker(uint32_t pid) {
        set_api(axcl::runtime::usrwork::KILL_WORKER);
        const auto body = m_payload->mutable_body();
        body->set_pid(pid);
    }

    void worker_send(uint32_t pid, const void* buf, uint32_t size, int32_t timeout) {
        set_api(axcl::runtime::usrwork::WORKER_SEND);
        const auto body = m_payload->mutable_body();
        body->set_pid(pid);
        body->set_data_size(size);
        body->set_timeout(timeout);
        if (buf && size > 0) {
            body->set_data(buf, size);
        }
    }

    void worker_recv(uint32_t pid, uint32_t bufsize, int32_t timeout) {
        set_api(axcl::runtime::usrwork::WORKER_RECV);
        const auto body = m_payload->mutable_body();
        body->set_pid(pid);
        body->set_data_size(bufsize);
        body->set_timeout(timeout);
    }

    [[nodiscard]] runtime::usrwork::API get_api() const {
        return static_cast<runtime::usrwork::API>(m_payload->head().api());
    }

protected:
    void set_api(axcl::runtime::usrwork::Api api) {
        m_payload->mutable_head()->set_api(api);
    }

private:
    axcl::runtime::usrwork::request::Payload* m_payload;
};

}  // namespace axcl::pkg::host
