/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "memory_response.hpp"
#include "axcl_assert.hpp"
#include "axcl_logger.hpp"
#include "protocol.hpp"

#define TAG "package"

struct axcl::pkg::host::memory_response::impl {
    [[nodiscard]] bool check_payload() {
        if (!m_payload->has_head() || !m_payload->has_body()) {
            LOG_MM_E(TAG, "check runtime memory response payload fail, has head {} has body {}", m_payload->has_head(),
                     m_payload->has_body());
            return false;
        }

        bool status = true;
        const auto& head = m_payload->head();
        const auto& body = m_payload->body();
        switch (head.api()) {
            case axcl::runtime::memory::Api::ALLOC:
                status = body.has_addr();
                m_api = axcl::pkg::runtime::memory::API::ALLOC;
                break;
            case axcl::runtime::memory::Api::FREE:
                m_api = axcl::pkg::runtime::memory::API::FREE;
                break;
            case axcl::runtime::memory::Api::FLUSH:
                m_api = axcl::pkg::runtime::memory::API::FLUSH;
                break;
            case axcl::runtime::memory::Api::INVALIDATE:
                m_api = axcl::pkg::runtime::memory::API::INVALIDATE;
                break;
            case axcl::runtime::memory::Api::SET:
                m_api = axcl::pkg::runtime::memory::API::SET;
                break;
            case axcl::runtime::memory::Api::WRITE:
                m_api = axcl::pkg::runtime::memory::API::WRITE;
                break;
            case axcl::runtime::memory::Api::READ:
                m_api = axcl::pkg::runtime::memory::API::READ;
                break;
            case axcl::runtime::memory::Api::SYNC:
                m_api = axcl::pkg::runtime::memory::API::SYNC;
                break;
            case axcl::runtime::memory::Api::CMP:
                m_api = axcl::pkg::runtime::memory::API::CMP;
                break;
            default:
                status = false;
                m_api = axcl::pkg::runtime::memory::API::UNKNOWN;
                LOG_MM_E(TAG, "unknown api {} of runtime memory response", static_cast<int32_t>(m_payload->head().api()));
                break;
        }

        return status;
    }

    [[nodiscard]] bool select(axcl::runtime::response::Payload* payload) {
        if (axcl::runtime::MEMORY != payload->head().module()) {
            LOG_MM_E(TAG,  "current module {} is not memory module", static_cast<int32_t>(payload->head().module()));
            AXCL_ASSERT(0, "current module %d is not memory module", static_cast<int32_t>(payload->head().module()));
            return false;
        }

        m_payload = payload->mutable_body()->mutable_memory();
        AXCL_ASSERT(m_payload, "nil pointer");

        return check_payload();
    }

    int32_t get_status() const {
        return m_payload->body().status();
    }

    void* get_addr() const {
        return reinterpret_cast<void*>(m_payload->body().addr());
    }

    axcl::pkg::runtime::memory::API get_api() const {
        return m_api;
    }

private:
    axcl::runtime::memory::response::Payload* m_payload = nullptr;
    axcl::pkg::runtime::memory::API m_api = axcl::pkg::runtime::memory::API::UNKNOWN;
};

axcl::pkg::host::memory_response::memory_response() : m_impl(new impl()) {
}

axcl::pkg::host::memory_response::memory_response(memory_response&& other) noexcept : m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::host::memory_response& axcl::pkg::host::memory_response::operator=(memory_response&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }

    return *this;
}

axcl::pkg::host::memory_response::~memory_response() {
    delete m_impl;
}

[[nodiscard]] bool axcl::pkg::host::memory_response::decode(const void* message, uint32_t size) const {
    if (!runtime_response::decode(message, size)) {
        return false;
    }

    return m_impl->select(static_cast<axcl::runtime::response::Payload*>(runtime_response::get_payload()));
}

[[nodiscard]] int32_t axcl::pkg::host::memory_response::get_status() const {
    return m_impl->get_status();
}

[[nodiscard]] void* axcl::pkg::host::memory_response::get_addr() const {
    return m_impl->get_addr();
}

[[nodiscard]] axcl::pkg::runtime::memory::API axcl::pkg::host::memory_response::get_api() const {
    return m_impl->get_api();
}