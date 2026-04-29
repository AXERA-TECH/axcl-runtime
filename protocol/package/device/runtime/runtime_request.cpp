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

#include "runtime_request.hpp"
#include "axcl_assert.hpp"
#include "axcl_logger.hpp"
#include "protocol.hpp"

#define TAG "package"

struct axcl::pkg::device::runtime_request::impl {
    impl() = delete;

    explicit impl(void* payload) : m_payload(static_cast<axcl::runtime::response::Payload*>(payload)) {
        m_payload->mutable_head();
        m_payload->mutable_body();
    }

    [[nodiscard]] void* select(const axcl::pkg::runtime::module& module) {
        const auto head = m_payload->mutable_head();
        const auto body = m_payload->mutable_body();

        switch (module) {
            case axcl::pkg::runtime::module::SYSTEM:
                head->set_module(axcl::runtime::SYSTEM);
                return body->mutable_system();
            case axcl::pkg::runtime::module::MEMORY:
                head->set_module(axcl::runtime::MEMORY);
                return body->mutable_memory();
            case axcl::pkg::runtime::module::ENGINE:
                head->set_module(axcl::runtime::ENGINE);
                return body->mutable_engine();
            case axcl::pkg::runtime::module::P2P:
                head->set_module(axcl::runtime::P2P);
                return body->mutable_p2p();
            case axcl::pkg::runtime::module::USRWORK:
                head->set_module(axcl::runtime::USRWORK);
                return body->mutable_usrwork();
            default:
                LOG_MM_E(TAG,  "unknown runtime module {}", static_cast<int32_t>(module));
                AXCL_ASSERT(0, "unknown runtime module %d", static_cast<int32_t>(module));
                return nullptr;
        }
    }

private:
    axcl::runtime::response::Payload* m_payload;
};

axcl::pkg::device::runtime_request::runtime_request() : m_impl(new impl(request::select(axcl::pkg::module::RUNTIME))) {
}

axcl::pkg::device::runtime_request::runtime_request(runtime_request&& other) noexcept
    : request(std::move(other)), m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::device::runtime_request& axcl::pkg::device::runtime_request::operator=(runtime_request&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }

    return *this;
}

axcl::pkg::device::runtime_request::~runtime_request() {
    delete m_impl;
}

[[nodiscard]] void* axcl::pkg::device::runtime_request::select(const axcl::pkg::runtime::module& module) const {
    return m_impl->select(module);
}
