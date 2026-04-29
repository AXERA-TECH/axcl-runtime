/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "sys_request.hpp"
#include "enum_cast.hpp"
#include "protocol.hpp"

struct axcl::pkg::host::sys_request::impl {
    impl() = delete;

    explicit impl(void* payload) : m_payload(static_cast<axcl::native::module::sys::Request*>(payload)) {
    }

    axcl::pkg::native::SYS_API get_api() const {
        return axcl::enum_cast<axcl::native::module::sys::Api, axcl::pkg::native::SYS_API>(m_payload->api());
    }

    bool add_data(const void* data, size_t size) {
        m_payload->add_parameters(data, size);
        return true;
    }

    bool set_api(axcl::pkg::native::SYS_API api) {
        m_payload->set_api(axcl::enum_cast<axcl::pkg::native::SYS_API, axcl::native::module::sys::Api>(api));
        m_payload->clear_parameters();
        return true;
    }

private:
    axcl::native::module::sys::Request* m_payload;
};

axcl::pkg::host::sys_request::sys_request() : m_impl(new impl(native_request::select(axcl::pkg::native::module::SYS))) {
}

axcl::pkg::host::sys_request::sys_request(sys_request&& other) noexcept
    : native_request(std::move(other)), m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::host::sys_request& axcl::pkg::host::sys_request::operator=(sys_request&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }

    return *this;
}

axcl::pkg::host::sys_request::~sys_request() {
    delete m_impl;
}

axcl::pkg::native::SYS_API axcl::pkg::host::sys_request::get_api() const {
    return m_impl->get_api();
}

bool axcl::pkg::host::sys_request::add_data(const void* data, size_t size) {
    return m_impl->add_data(data, size);
}

bool axcl::pkg::host::sys_request::set_api(axcl::pkg::native::SYS_API api) {
    return m_impl->set_api(api);
}
