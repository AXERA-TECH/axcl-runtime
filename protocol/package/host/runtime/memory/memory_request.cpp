/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "memory_request.hpp"
#include "enum_cast.hpp"
#include "axcl_logger.hpp"
#include "protocol.hpp"

#define TAG "package"

struct axcl::pkg::host::memory_request::impl {
    impl() = delete;

    explicit impl(void* payload) : m_payload(static_cast<axcl::runtime::memory::request::Payload*>(payload)) {
        m_payload->mutable_head();
        m_payload->mutable_body();
    }

    void alloc(uint32_t size, uint32_t policy, bool cached) const {
        m_payload->mutable_head()->set_api(axcl::runtime::memory::Api::ALLOC);
        const auto& body = m_payload->mutable_body();
        body->set_size(size);
        body->set_cache(cached ? axcl::runtime::memory::CACHED : axcl::runtime::memory::NON_CACHED);
        body->set_policy(policy);
    }

    void free(const void* addr) const {
        m_payload->mutable_head()->set_api(axcl::runtime::memory::Api::FREE);
        const auto& body = m_payload->mutable_body();
        body->set_addr(reinterpret_cast<uint64_t>(addr));
    }

    void read(const void* from, const void* to, uint32_t size, axcl::pkg::memcpy_kind kind, uint32_t noresp) const {
        m_payload->mutable_head()->set_api(axcl::runtime::memory::Api::READ);
        const auto& body = m_payload->mutable_body();
        body->set_from(reinterpret_cast<uint64_t>(from));
        body->set_addr(reinterpret_cast<uint64_t>(to));
        body->set_phy(m_memcpy_package.dst_phy);
        body->set_size(size);
        body->set_kind(static_cast<uint32_t>(kind));
        body->set_noresp(noresp);
    }

    void write(const void* from, const void* to, uint32_t size, axcl::pkg::memcpy_kind kind, uint32_t noresp) const {
        m_payload->mutable_head()->set_api(axcl::runtime::memory::Api::WRITE);
        const auto& body = m_payload->mutable_body();
        body->set_from(reinterpret_cast<uint64_t>(from));
        body->set_addr(reinterpret_cast<uint64_t>(to));
        body->set_phy(m_memcpy_package.src_phy);
        body->set_size(size);
        body->set_kind(static_cast<uint32_t>(kind));
        body->set_noresp(noresp);
    }

    void set(const void* addr, uint8_t value, uint32_t size, uint32_t noresp) const {
        m_payload->mutable_head()->set_api(axcl::runtime::memory::Api::SET);
        const auto& body = m_payload->mutable_body();
        body->set_addr(reinterpret_cast<uint64_t>(addr));
        body->set_value(value);
        body->set_size(size);
        body->set_noresp(noresp);
    }

    void flush(const void* addr, uint32_t size) const {
        m_payload->mutable_head()->set_api(axcl::runtime::memory::Api::FLUSH);
        const auto& body = m_payload->mutable_body();
        body->set_addr(reinterpret_cast<uint64_t>(addr));
        body->set_size(size);
    }

    void invalidate(const void* addr, uint32_t size) const {
        m_payload->mutable_head()->set_api(axcl::runtime::memory::Api::INVALIDATE);
        const auto& body = m_payload->mutable_body();
        body->set_addr(reinterpret_cast<uint64_t>(addr));
        body->set_size(size);
    }

    void sync(const void* from, const void* to, uint32_t size, uint32_t noresp) const {
        m_payload->mutable_head()->set_api(axcl::runtime::memory::Api::SYNC);
        const auto& body = m_payload->mutable_body();
        body->set_from(reinterpret_cast<uint64_t>(from));
        body->set_addr(reinterpret_cast<uint64_t>(to));
        body->set_size(size);
        body->set_noresp(noresp);
    }

    void cmp(const void* s1, const void* s2, uint32_t size, uint32_t noresp) const {
        m_payload->mutable_head()->set_api(axcl::runtime::memory::Api::CMP);
        const auto& body = m_payload->mutable_body();
        body->set_from(reinterpret_cast<uint64_t>(s1));
        body->set_addr(reinterpret_cast<uint64_t>(s2));
        body->set_size(size);
        body->set_noresp(noresp);
    }

    axcl::pkg::runtime::memory::API get_api() const {
        return axcl::enum_cast<axcl::runtime::memory::Api, axcl::pkg::runtime::memory::API>(m_payload->head().api());
    }

    [[nodiscard]] void* encode(const axcl::pkg::head& hd, uint32_t& size) {
        const auto& head = m_payload->head();
        const auto& body = m_payload->body();
        switch (head.api()) {
            case axcl::runtime::memory::Api::WRITE: /* host -> device */

                m_memcpy_package.hd = hd;
                m_memcpy_package.kind = static_cast<axcl::pkg::memcpy_kind>(body.kind());
                m_memcpy_package.dst_phy = body.addr();
                m_memcpy_package.size = body.size();
                if (axcl::pkg::memcpy_kind::HOST_VIR_TO_DEVICE == m_memcpy_package.kind) {
                    m_memcpy_package.data = reinterpret_cast<void*>(body.from());
                } else {
                    m_memcpy_package.src_phy = body.from();
                }
                break;
            case axcl::runtime::memory::Api::READ: /* host <- device */
                m_memcpy_package.hd = hd;
                m_memcpy_package.kind = static_cast<axcl::pkg::memcpy_kind>(body.kind());
                m_memcpy_package.src_phy = body.from();
                m_memcpy_package.size = body.size();
                if (axcl::pkg::memcpy_kind::DEVICE_TO_HOST_VIR == m_memcpy_package.kind) {
                    m_memcpy_package.data = reinterpret_cast<void*>(body.addr());
                } else {
                    m_memcpy_package.dst_phy = body.addr();
                }
                break;
            default:
                LOG_MM_E(TAG, "unsupport api {}", static_cast<uint32_t>(head.api()));
                size = 0;
                return nullptr;
        }

        /* raw package for memcpy between host and device */
        size = sizeof(m_memcpy_package);
        return reinterpret_cast<void*>(&m_memcpy_package);
    }
private:
    axcl::runtime::memory::request::Payload* m_payload;
    axcl::pkg::memcpy_package m_memcpy_package;
};

axcl::pkg::host::memory_request::memory_request() : m_impl(new impl(runtime_request::select(axcl::pkg::runtime::module::MEMORY))) {
}

axcl::pkg::host::memory_request::memory_request(memory_request&& other) noexcept
    : runtime_request(std::move(other)), m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::host::memory_request& axcl::pkg::host::memory_request::operator=(memory_request&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }

    return *this;
}

axcl::pkg::host::memory_request::~memory_request() {
    delete m_impl;
}

void* axcl::pkg::host::memory_request::encode(const axcl::pkg::head& hd, uint32_t& size) const {
    if (RUNTIME_MEMCPY_HANDLER_TYPE == hd.type) {
        return m_impl->encode(hd, size);
    } else {
        return axcl::pkg::host::runtime_request::encode(hd, size);
    }
}

void axcl::pkg::host::memory_request::alloc(uint32_t size, uint32_t policy, bool cached /* = false */) const {
    m_impl->alloc(size, policy, cached);
}

void axcl::pkg::host::memory_request::free(const void* addr) const {
    m_impl->free(addr);
}

void axcl::pkg::host::memory_request::read(const void* from, const void* to, uint32_t size, axcl::pkg::memcpy_kind kind, uint32_t noresp) const {
    m_impl->read(from, to, size, kind, noresp);
}

void axcl::pkg::host::memory_request::write(const void* from, const void* to, uint32_t size, axcl::pkg::memcpy_kind kind, uint32_t noresp) const {
    m_impl->write(from, to, size, kind, noresp);
}

void axcl::pkg::host::memory_request::set(const void* addr, uint8_t value, uint32_t size, uint32_t noresp) const {
    m_impl->set(addr, value, size, noresp);
}

void axcl::pkg::host::memory_request::flush(const void* addr, uint32_t size) const {
    m_impl->flush(addr, size);
}

void axcl::pkg::host::memory_request::invalidate(const void* addr, uint32_t size) const {
    m_impl->invalidate(addr, size);
}

void axcl::pkg::host::memory_request::sync(const void* from, const void* to, uint32_t size, uint32_t noresp) const {
    m_impl->sync(from, to, size, noresp);
}

void axcl::pkg::host::memory_request::cmp(const void* s1, const void* s2, uint32_t size, uint32_t noresp) const {
    m_impl->cmp(s1, s2, size, noresp);
}

axcl::pkg::runtime::memory::API axcl::pkg::host::memory_request::get_api() const {
    return m_impl->get_api();
}