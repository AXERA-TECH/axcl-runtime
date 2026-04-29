/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "npu_request.hpp"
#include "enum_cast.hpp"
#include "protocol.hpp"

struct axcl::pkg::host::npu_request::impl {
    impl() = delete;

    explicit impl(void* payload) : m_payload(static_cast<axcl::native::module::engine::Request*>(payload)) {
    }

    [[nodiscard]] axcl::pkg::native::ENGINE_API get_api() const {
        return axcl::enum_cast<axcl::native::module::engine::Api, axcl::pkg::native::ENGINE_API>(m_payload->api());
    }

    [[nodiscard]] bool add_data(const void* data, const size_t size) const {
        m_payload->add_parameters(data, size);
        return true;
    }

    [[nodiscard]] bool set_api(const native::ENGINE_API api) const {
        m_payload->set_api(axcl::enum_cast<native::ENGINE_API, axcl::native::module::engine::Api>(api));
        m_payload->clear_parameters();
        return true;
    }

private:
    axcl::native::module::engine::Request* m_payload;
};

axcl::pkg::host::npu_request::npu_request() : m_impl(new impl(select(native::module::ENGINE))) {
}

axcl::pkg::host::npu_request::npu_request(npu_request&& other) noexcept
    : native_request(std::move(other)), m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::host::npu_request& axcl::pkg::host::npu_request::operator=(npu_request&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }

    return *this;
}

axcl::pkg::host::npu_request::~npu_request() {
    delete m_impl;
}

axcl::pkg::native::ENGINE_API axcl::pkg::host::npu_request::get_api() const {
    return m_impl->get_api();
}

bool axcl::pkg::host::npu_request::add_data(const void* data, const size_t size) {
    return m_impl->add_data(data, size);
}

bool axcl::pkg::host::npu_request::set_api(const axcl::pkg::native::ENGINE_API api) {
    return m_impl->set_api(api);
}
