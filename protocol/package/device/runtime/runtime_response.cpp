/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 * Author: wanglusheng@axera-tech.com
 *
 **************************************************************************************************/

#include "runtime_response.hpp"
#include "axcl_assert.hpp"
#include "axcl_logger.hpp"
#include "protocol.hpp"

#define TAG "package"

struct axcl::pkg::device::runtime_response::impl {
    [[nodiscard]] bool check_payload() const {
        bool status = (m_payload && m_payload->has_head() && m_payload->has_body());
        if (status) {
            const auto& head = m_payload->head();
            const auto& body = m_payload->body();
            if (status) {
                switch (head.module()) {
                    case axcl::runtime::SYSTEM:
                        status = body.has_system();
                        break;
                    case axcl::runtime::MEMORY:
                        status = body.has_memory();
                        break;
                    case axcl::runtime::ENGINE:
                        status = body.has_engine();
                        break;
                    case axcl::runtime::P2P:
                        status = body.has_p2p();
                        break;
                    case axcl::runtime::USRWORK:
                        status = body.has_usrwork();
                        break;
                    default:
                        LOG_MM_E(TAG,  "unknown runtime module {}", static_cast<int32_t>(head.module()));
                        AXCL_ASSERT(0, "unknown runtime module %d", static_cast<int32_t>(head.module()));
                        status = false;
                        break;
                }
            }
        } else {
            if (!m_payload) {
                LOG_MM_E(TAG, "runtime response payload is nil");
            } else {
                LOG_MM_E(TAG, "check runtime response payload fail, has head {} has body {}", m_payload->has_head(), m_payload->has_body());
            }
        }

        return status;
    }

    [[nodiscard]] bool select(axcl::request::Payload* payload) {
        m_payload = payload->mutable_body()->mutable_runtime();
        AXCL_ASSERT(m_payload, "nil pointer");

        return check_payload();
    }

    axcl::pkg::runtime::module get_module() const {
        AXCL_ASSERT(m_payload, "nil pointer");

        switch (m_payload->head().module()) {
            case axcl::runtime::SYSTEM:
                return axcl::pkg::runtime::module::SYSTEM;
            case axcl::runtime::MEMORY:
                return axcl::pkg::runtime::module::MEMORY;
            case axcl::runtime::ENGINE:
                return axcl::pkg::runtime::module::ENGINE;
            case axcl::runtime::P2P:
                return axcl::pkg::runtime::module::P2P;
            case axcl::runtime::USRWORK:
                return axcl::pkg::runtime::module::USRWORK;
            default:
                LOG_MM_E(TAG,  "unknown runtime module {}", static_cast<int32_t>(m_payload->head().module()));
                AXCL_ASSERT(0, "unknown runtime module %d", static_cast<int32_t>(m_payload->head().module()));
                return axcl::pkg::runtime::module::UNKNOWN;
        }
    }

    void* get_payload() {
        return m_payload;
    }

private:
    axcl::runtime::request::Payload* m_payload = {nullptr};
};

axcl::pkg::device::runtime_response::runtime_response() : m_impl(new impl()) {
}

axcl::pkg::device::runtime_response::runtime_response(runtime_response&& other) noexcept : m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::device::runtime_response& axcl::pkg::device::runtime_response::operator=(runtime_response&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }

    return *this;
}

axcl::pkg::device::runtime_response::~runtime_response() {
    delete m_impl;
}

[[nodiscard]] bool axcl::pkg::device::runtime_response::decode(const void* message, uint32_t size) const {
    if (!response::decode(message, size)) {
        return false;
    }

    return m_impl->select(static_cast<axcl::request::Payload*>(response::get_payload()));
}

axcl::pkg::runtime::module axcl::pkg::device::runtime_response::get_module() const {
    return m_impl->get_module();
}

void* axcl::pkg::device::runtime_response::get_payload() const {
    return m_impl->get_payload();
}