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

#include "native_request.hpp"
#include "axcl_assert.hpp"
#include "axcl_logger.hpp"
#include "protocol.hpp"

#define TAG "package"

struct axcl::pkg::device::native_request::impl {
    impl() = delete;

    explicit impl(void* payload) : m_payload(static_cast<axcl::native::response::Payload*>(payload)) {
        m_payload->mutable_head();
        m_payload->mutable_body();
    }

    [[nodiscard]] void* select(const axcl::pkg::native::module& module) {
        const auto head = m_payload->mutable_head();
        const auto body = m_payload->mutable_body();

        switch (module) {
            case axcl::pkg::native::module::SYS:
                head->set_module(axcl::native::SYS);
                return body->mutable_sys();
            case axcl::pkg::native::module::VDEC:
                head->set_module(axcl::native::VDEC);
                return body->mutable_vdec();
            case axcl::pkg::native::module::VENC:
                head->set_module(axcl::native::VENC);
                return body->mutable_venc();
            case axcl::pkg::native::module::IVPS:
                head->set_module(axcl::native::IVPS);
                return body->mutable_ivps();
            case axcl::pkg::native::module::IVE:
                head->set_module(axcl::native::IVE);
                return body->mutable_ive();
            case axcl::pkg::native::module::ENGINE:
                head->set_module(axcl::native::ENGINE);
                return body->mutable_engine();
            case axcl::pkg::native::module::DMADIM:
                head->set_module(axcl::native::DMADIM);
                return body->mutable_dmadim();
            default:
                LOG_MM_E(TAG,  "unknown native module {}", static_cast<int32_t>(module));
                AXCL_ASSERT(0, "unknown native module %d", static_cast<int32_t>(module));
                return nullptr;
        }
    }

private:
    axcl::native::response::Payload* m_payload;
};

axcl::pkg::device::native_request::native_request()
    : m_args([this](const void* arg, size_t size) { return add_data(arg, size); }),
      m_impl(new impl(request::select(axcl::pkg::module::NATIVE))) {
}

axcl::pkg::device::native_request::native_request(native_request&& other) noexcept
    : request(std::move(other)),
      m_args([this](const void* arg, size_t size) { return add_data(arg, size); }),
      m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::device::native_request& axcl::pkg::device::native_request::operator=(native_request&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }

    return *this;
}

axcl::pkg::device::native_request::~native_request() {
    delete m_impl;
}

[[nodiscard]] void* axcl::pkg::device::native_request::select(const axcl::pkg::native::module& module) const {
    return m_impl->select(module);
}

bool axcl::pkg::device::native_request::add_data(const void* data, size_t size) {
    return false;
}