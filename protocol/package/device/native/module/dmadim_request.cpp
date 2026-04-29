/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "dmadim_request.hpp"
#include "enum_cast.hpp"
#include "protocol.hpp"

struct axcl::pkg::device::dmadim_request::impl {
    impl() = delete;

    explicit impl(void* payload) : m_payload(static_cast<axcl::native::module::dmadim::Response*>(payload)) {
    }

    bool add_data(const void* data, size_t size) {
        m_payload->add_parameters(data, size);
        return true;
    }

    bool set_api(axcl::pkg::native::DMADIM_API api) {
        m_payload->set_api(axcl::enum_cast<axcl::pkg::native::DMADIM_API, axcl::native::module::dmadim::Api>(api));
        m_payload->clear_parameters();
        return true;
    }

private:
    axcl::native::module::dmadim::Response* m_payload;
};

axcl::pkg::device::dmadim_request::dmadim_request() : m_impl(new impl(native_request::select(axcl::pkg::native::module::DMADIM))) {
}

axcl::pkg::device::dmadim_request::dmadim_request(dmadim_request&& other) noexcept
    : native_request(std::move(other)), m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::device::dmadim_request& axcl::pkg::device::dmadim_request::operator=(dmadim_request&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }

    return *this;
}

axcl::pkg::device::dmadim_request::~dmadim_request() {
    delete m_impl;
}

bool axcl::pkg::device::dmadim_request::add_data(const void* data, size_t size) {
    return m_impl->add_data(data, size);
}

bool axcl::pkg::device::dmadim_request::set_api(axcl::pkg::native::DMADIM_API api) {
    return m_impl->set_api(api);
}
