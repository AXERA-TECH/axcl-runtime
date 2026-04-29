/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "dsp_request.hpp"

#include "protocol.hpp"
#include "enum_cast.hpp"

struct axcl::pkg::device::dsp_request::impl {
    impl() = delete;

    explicit impl(void* payload) : m_payload(static_cast<axcl::native::module::engine::Response*>(payload)) {
    }

    [[nodiscard]] bool add_data(const void* data, const size_t size) const {
        m_payload->add_parameters(data, size);
        return true;
    }

    [[nodiscard]] bool set_api(const native::DSP_API api) const {
        m_payload->set_api(axcl::enum_cast<native::DSP_API, axcl::native::module::engine::Api>(api));
        m_payload->clear_parameters();
        return true;
    }

private:
    axcl::native::module::engine::Response* m_payload;
};

axcl::pkg::device::dsp_request::dsp_request() : m_impl(new impl(select(native::module::ENGINE))) {
}

axcl::pkg::device::dsp_request::dsp_request(dsp_request&& other) noexcept
    : native_request(std::move(other)), m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::device::dsp_request& axcl::pkg::device::dsp_request::operator=(dsp_request&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }

    return *this;
}

axcl::pkg::device::dsp_request::~dsp_request() {
    delete m_impl;
}

bool axcl::pkg::device::dsp_request::add_data(const void* data, const size_t size) {
    return m_impl->add_data(data, size);
}

bool axcl::pkg::device::dsp_request::set_api(const native::DSP_API api) {
    return m_impl->set_api(api);
}
