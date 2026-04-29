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

#include "axcl_logger.hpp"
#include "usrwork_response.hpp"
#include "protocol.hpp"

#define TAG "package"

namespace axcl::pkg::host {

struct usrwork_response::impl {
    [[nodiscard]] bool check_payload() {
        if (!m_payload->has_head() || !m_payload->has_body()) {
            LOG_MM_E(TAG, "usrwork runtime response check payload fail, has head {} has body {}",
                m_payload->has_head(), m_payload->has_body());
            return false;
        }

        bool status = true;
        switch (m_payload->head().api()) {
            case axcl::runtime::usrwork::TRANSFER_FILE:
                m_api = axcl::pkg::runtime::usrwork::API::TRANSFER_FILE;
                break;
            case axcl::runtime::usrwork::EXEC_WORKER:
                m_api = axcl::pkg::runtime::usrwork::API::EXEC_WORKER;
                break;
            case axcl::runtime::usrwork::KILL_WORKER:
                m_api = axcl::pkg::runtime::usrwork::API::KILL_WORKER;
                break;
            case axcl::runtime::usrwork::WORKER_SEND:
                m_api = axcl::pkg::runtime::usrwork::API::WORKER_SEND;
                break;
            case axcl::runtime::usrwork::WORKER_RECV:
                m_api = axcl::pkg::runtime::usrwork::API::WORKER_RECV;
                break;
            default:
                LOG_MM_E(TAG, "unknown api {} of usrwork runtime response", static_cast<int32_t>(m_payload->head().api()));
                return false;
        }

        return status;
    }

    [[nodiscard]] bool select(axcl::runtime::response::Payload *payload) {
        if (axcl::runtime::USRWORK != payload->head().module()) {
            LOG_MM_E(TAG, "current module {} is not usrwork module", static_cast<int32_t>(payload->head().module()));
            return false;
        }

        m_payload = payload->mutable_body()->mutable_usrwork();
        if (!m_payload) {
            LOG_MM_E(TAG, "usrwork runtime response select payload fail, nil pointer");
            return false;
        }

        return check_payload();
    }

    int32_t get_status() const {
        return m_payload->body().status();
    }

    uint32_t get_pid() const {
        return m_payload->body().pid();
    }

    uint32_t get_recvlen() const {
        return m_payload->body().data_size();
    }

    const void* get_data(size_t &size) const {
        if (!m_payload->body().has_data()) {
            LOG_MM_E(TAG, "usrwork runtime response get data fail, no data");
            return nullptr;
        }
        const auto &data = m_payload->body().data();
        size = data.size();
        return data.data();
    }

    axcl::pkg::runtime::usrwork::API get_api() const {
        return m_api;
    }

private:
    axcl::pkg::runtime::usrwork::API m_api{};
    axcl::runtime::usrwork::response::Payload *m_payload = {nullptr};
};

}  // namespace axcl::pkg::host
