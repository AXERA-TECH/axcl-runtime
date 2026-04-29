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

struct axcl::pkg::device::memory_response::impl {
    [[nodiscard]] bool check_payload() {
        if (!m_payload->has_head() || !m_payload->has_body()) {
            LOG_MM_E(TAG, "check runtime memory response payload fail, has head {} has body {}", m_payload->has_head(),
                     m_payload->has_body());
            return false;
        }

        bool status = true;
        const auto& body = m_payload->body();
        switch (m_payload->head().api()) {
            case axcl::runtime::memory::Api::ALLOC:
                m_api = axcl::pkg::runtime::memory::API::ALLOC;
                status = body.has_size() && body.has_cache();
                break;
            case axcl::runtime::memory::Api::FREE:
                m_api = axcl::pkg::runtime::memory::API::FREE;
                break;
            case axcl::runtime::memory::Api::FLUSH:
                m_api = axcl::pkg::runtime::memory::API::FLUSH;
                status = body.has_size();
                break;
            case axcl::runtime::memory::Api::INVALIDATE:
                m_api = axcl::pkg::runtime::memory::API::INVALIDATE;
                status = body.has_size();
                break;
            case axcl::runtime::memory::Api::SET:
                m_api = axcl::pkg::runtime::memory::API::SET;
                status = body.has_size() && body.has_value();
                break;
            case axcl::runtime::memory::Api::WRITE:
                m_api = axcl::pkg::runtime::memory::API::WRITE;
                status = body.has_size();
                break;
            case axcl::runtime::memory::Api::READ:
                m_api = axcl::pkg::runtime::memory::API::READ;
                status = body.has_size();
                break;
            case axcl::runtime::memory::Api::SYNC:
                m_api = axcl::pkg::runtime::memory::API::SYNC;
                status = body.has_size() && body.has_from();
                break;
            case axcl::runtime::memory::Api::CMP:
                m_api = axcl::pkg::runtime::memory::API::CMP;
                status = body.has_size() && body.has_from();
                break;
            default:
                status = false;
                m_api = axcl::pkg::runtime::memory::API::UNKNOWN;
                LOG_MM_E(TAG, "unknown api {} of runtime memory response", static_cast<int32_t>(m_payload->head().api()));
                break;
        }

        return status;
    }

    [[nodiscard]] bool select(axcl::runtime::request::Payload* payload) {
        if (axcl::runtime::MEMORY != payload->head().module()) {
            LOG_MM_E(TAG,  "current module {} is not memory module", static_cast<int32_t>(payload->head().module()));
            AXCL_ASSERT(0, "current module %d is not memory module", static_cast<int32_t>(payload->head().module()));
            return false;
        }

        m_payload = payload->mutable_body()->mutable_memory();
        AXCL_ASSERT(m_payload, "nil pointer");

        return check_payload();
    }

    axcl::pkg::runtime::memory::API get_api() const {
        return m_api;
    }

    uint64_t get_addr() const {
        return m_payload->body().addr();
    }

    uint64_t get_size() const {
        return m_payload->body().size();
    }

    uint64_t get_from() const {
        return m_payload->body().from();
    }

    uint64_t get_phy() const {
        return m_payload->body().phy();
    }


    uint64_t get_kind() const {
        return m_payload->body().kind();
    }

    axcl::pkg::runtime::memory::CHACHE_TYPE get_cache_type() const {
        switch (m_payload->body().cache()) {
            case axcl::runtime::memory::CacheType::CACHED:
                return axcl::pkg::runtime::memory::CHACHE_TYPE::CACHED;
            default:
                return axcl::pkg::runtime::memory::CHACHE_TYPE::NON_CACHED;
        }
    }

    uint32_t get_index() const {
        return m_payload->body().index();
    }

    uint32_t get_value() const {
        return m_payload->body().value();
    }

    uint32_t get_policy() const {
        return m_payload->body().policy();
    }

    uint32_t get_noresp_flag() const {
        return m_payload->body().noresp();
    }


private:
    axcl::runtime::memory::request::Payload* m_payload = nullptr;
    axcl::pkg::runtime::memory::API m_api = axcl::pkg::runtime::memory::API::UNKNOWN;
};

axcl::pkg::device::memory_response::memory_response() : m_impl(new impl()) {
}

axcl::pkg::device::memory_response::memory_response(memory_response&& other) noexcept : m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::device::memory_response& axcl::pkg::device::memory_response::operator=(memory_response&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }

    return *this;
}

axcl::pkg::device::memory_response::~memory_response() {
    delete m_impl;
}

[[nodiscard]] bool axcl::pkg::device::memory_response::decode(const void* message, uint32_t size) const {
    if (!runtime_response::decode(message, size)) {
        return false;
    }

    return m_impl->select(static_cast<axcl::runtime::request::Payload*>(runtime_response::get_payload()));
}

axcl::pkg::runtime::memory::API axcl::pkg::device::memory_response::get_api() const {
    return m_impl->get_api();
}

uint64_t axcl::pkg::device::memory_response::get_addr() const {
    return m_impl->get_addr();
}

uint64_t axcl::pkg::device::memory_response::get_size() const {
    return m_impl->get_size();
}

uint64_t axcl::pkg::device::memory_response::get_from() const {
    return m_impl->get_from();
}

uint64_t axcl::pkg::device::memory_response::get_phy() const {
    return m_impl->get_phy();
}

uint64_t axcl::pkg::device::memory_response::get_kind() const {
    return m_impl->get_kind();
}

axcl::pkg::runtime::memory::CHACHE_TYPE axcl::pkg::device::memory_response::get_cache_type() const {
    return m_impl->get_cache_type();
}

uint32_t axcl::pkg::device::memory_response::get_index() const {
    return m_impl->get_index();
}

uint32_t axcl::pkg::device::memory_response::get_value() const {
    return m_impl->get_value();
}

uint32_t axcl::pkg::device::memory_response::get_policy() const {
    return m_impl->get_policy();
}

uint32_t axcl::pkg::device::memory_response::get_noresp_flag() const {
    return m_impl->get_noresp_flag();
}