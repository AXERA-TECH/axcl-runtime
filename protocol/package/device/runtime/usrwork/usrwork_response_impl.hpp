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

#include <vector>
#include <string>
#include <cstddef>
#include "axcl_assert.hpp"
#include "axcl_logger.hpp"
#include "usrwork_response.hpp"
#include "protocol.hpp"

#define TAG "package"

namespace axcl::pkg::device {

struct usrwork_response::impl {
    [[nodiscard]] bool check_payload() {
        if (!m_payload->has_head() || !m_payload->has_body()) {
            LOG_MM_E(TAG, "check runtime usrwork response payload fail, has head {} has body {}", m_payload->has_head(),
                     m_payload->has_body());
            return false;
        }

        bool status = true;
        const auto& body = m_payload->body();
        switch (m_payload->head().api()) {
            case axcl::runtime::usrwork::Api::TRANSFER_FILE:
                m_api = axcl::pkg::runtime::usrwork::API::TRANSFER_FILE;
                status = body.has_src_path() && body.has_dst_path() && body.has_policy();
                break;
            case axcl::runtime::usrwork::Api::EXEC_WORKER:
                m_api = axcl::pkg::runtime::usrwork::API::EXEC_WORKER;
                status = body.has_path();
                break;
            case axcl::runtime::usrwork::Api::KILL_WORKER:
                m_api = axcl::pkg::runtime::usrwork::API::KILL_WORKER;
                status = body.has_pid();
                break;
            case axcl::runtime::usrwork::Api::WORKER_SEND:
                m_api = axcl::pkg::runtime::usrwork::API::WORKER_SEND;
                status = body.has_pid() && body.has_data_size() && body.has_timeout();
                break;
            case axcl::runtime::usrwork::Api::WORKER_RECV:
                m_api = axcl::pkg::runtime::usrwork::API::WORKER_RECV;
                status = body.has_pid() && body.has_data_size() && body.has_timeout();
                break;
            default:
                status = false;
                m_api = axcl::pkg::runtime::usrwork::API::UNKNOWN;
                LOG_MM_E(TAG, "unknown api {} of runtime usrwork response", static_cast<int32_t>(m_payload->head().api()));
                break;
        }

        return status;
    }

    [[nodiscard]] bool select(axcl::runtime::request::Payload* payload) {
        if (axcl::runtime::USRWORK != payload->head().module()) {
            LOG_MM_E(TAG,  "current module {} is not usrwork module", static_cast<int32_t>(payload->head().module()));
            AXCL_ASSERT(0, "current module %d is not usrwork module", static_cast<int32_t>(payload->head().module()));
            return false;
        }

        m_payload = payload->mutable_body()->mutable_usrwork();
        AXCL_ASSERT(m_payload, "nil pointer");

        return check_payload();
    }

    axcl::pkg::runtime::usrwork::API get_api() const {
        return m_api;
    }

    uint32_t get_pid() const {
        return m_payload->body().pid();
    }

    int32_t get_timeout() const {
        return m_payload->body().timeout();
    }

    const void* get_data(size_t& size) const {
        if (!m_payload->body().has_data()) {
            LOG_MM_E(TAG, "usrwork runtime response get data fail, no data");
            return nullptr;
        }
        const auto& data = m_payload->body().data();
        size = data.size();
        return data.data();
    }

    uint32_t get_data_size() const {
        return m_payload->body().data_size();
    }

    const std::string& get_src_path() const {
        return m_payload->body().src_path();
    }

    const std::string& get_dst_path() const {
        return m_payload->body().dst_path();
    }

    const std::string& get_path() const {
        return m_payload->body().path();
    }

    std::vector<std::string> get_argv() const {
        const auto& argv = m_payload->body().argv();
        std::vector<std::string> result;
        result.reserve(argv.size());
        for (const auto& arg : argv) {
            result.push_back(arg);
        }
        return result;
    }

    axcl::pkg::runtime::usrwork::FILE_TRANSFER_POLICY get_policy() const {
        switch (m_payload->body().policy()) {
            case axcl::runtime::usrwork::FileTransferPolicy::FILE_TRANSFER_FROM_HOST_TO_DEVICE:
                return axcl::pkg::runtime::usrwork::FILE_TRANSFER_POLICY::FILE_TRANSFER_FROM_HOST_TO_DEVICE;
            case axcl::runtime::usrwork::FileTransferPolicy::FILE_TRANSFER_FROM_DEVICE_TO_HOST:
                return axcl::pkg::runtime::usrwork::FILE_TRANSFER_POLICY::FILE_TRANSFER_FROM_DEVICE_TO_HOST;
            case axcl::runtime::usrwork::FileTransferPolicy::FILE_TRANSFER_FROM_DEVICE_TO_DEVICE:
                return axcl::pkg::runtime::usrwork::FILE_TRANSFER_POLICY::FILE_TRANSFER_FROM_DEVICE_TO_DEVICE;
            case axcl::runtime::usrwork::FileTransferPolicy::FILE_TRANSFER_REMOVE_DEVICE_FILE:
                return axcl::pkg::runtime::usrwork::FILE_TRANSFER_POLICY::FILE_TRANSFER_REMOVE_DEVICE_FILE;
            default:
                return axcl::pkg::runtime::usrwork::FILE_TRANSFER_POLICY::FILE_TRANSFER_FROM_HOST_TO_DEVICE;
        }
    }

    uint32_t get_heartbeat_interval() const {
        return m_payload->body().heartbeat_interval();
    }

    uint32_t get_port_num() const {
        return m_payload->body().port_num();
    }

    std::vector<uint32_t> get_ports() const {
        const auto& ports = m_payload->body().ports();
        std::vector<uint32_t> result;
        result.reserve(ports.size());
        for (const auto& port : ports) {
            result.push_back(port);
        }
        return result;
    }

private:
    axcl::pkg::runtime::usrwork::API m_api{};
    axcl::runtime::usrwork::request::Payload* m_payload{nullptr};
};

}  // namespace axcl::pkg::device
