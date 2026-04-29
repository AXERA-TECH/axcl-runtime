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
#include "protocol.hpp"

struct axcl::pkg::device::memory_request::impl {
    impl() = delete;

    explicit impl(void* payload) : m_payload(static_cast<axcl::runtime::memory::response::Payload*>(payload)) {
        m_payload->mutable_head();
        m_payload->mutable_body();
    }

    void set_api(axcl::pkg::runtime::memory::API api) const {
        m_payload->mutable_head()->set_api(axcl::enum_cast<axcl::pkg::runtime::memory::API, axcl::runtime::memory::Api>(api));
    }

    void set_status(int32_t status) const {
        m_payload->mutable_body()->set_status(status);
    }

    void set_addr(const void* addr) const {
        m_payload->mutable_body()->set_addr(reinterpret_cast<uint64_t>(addr));
    }

private:
    axcl::runtime::memory::response::Payload* m_payload;
};

axcl::pkg::device::memory_request::memory_request() : m_impl(new impl(runtime_request::select(axcl::pkg::runtime::module::MEMORY))) {
}

axcl::pkg::device::memory_request::memory_request(memory_request&& other) noexcept
    : runtime_request(std::move(other)), m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::device::memory_request& axcl::pkg::device::memory_request::operator=(memory_request&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }

    return *this;
}

axcl::pkg::device::memory_request::~memory_request() {
    delete m_impl;
}

void axcl::pkg::device::memory_request::set_api(axcl::pkg::runtime::memory::API api) const {
    m_impl->set_api(api);
}

void axcl::pkg::device::memory_request::set_status(int32_t status) const {
    m_impl->set_status(status);
}

void axcl::pkg::device::memory_request::set_addr(const void* addr) const {
    m_impl->set_addr(addr);
}