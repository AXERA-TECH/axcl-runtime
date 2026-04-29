/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "dsp_response.hpp"
#include "axcl_assert.hpp"
#include "enum_cast.hpp"
#include "protocol.hpp"

#include <cstring>

struct axcl::pkg::host::dsp_response::impl {
    [[nodiscard]] bool check_payload() const {
        return true;
    }

    [[nodiscard]] bool select(axcl::native::response::Payload* payload) {
        if (axcl::native::DSP != payload->head().module()) {
            AXCL_ASSERT(0, "invalid native module %d", static_cast<int32_t>(payload->head().module()));
            return false;
        }

        m_payload = payload->mutable_body()->mutable_dsp();
        AXCL_ASSERT(m_payload, "nil pointer");

        return check_payload();
    }

    [[nodiscard]] void* get_payload() const {
        return m_payload;
    }

    [[nodiscard]] axcl::pkg::native::DSP_API get_api() const {
        return axcl::enum_cast<axcl::native::module::dsp::Api, axcl::pkg::native::DSP_API>(m_payload->api());
    }

    [[nodiscard]] bool check_api(const axcl::pkg::native::DSP_API api) const {
        return axcl::enum_equal<axcl::native::module::dsp::Api, axcl::pkg::native::DSP_API>(m_payload->api(), api);
    }

    [[nodiscard]] size_t get_data_size(const int index) const {
        return m_payload->parameters(index).size();
    }

    bool get_data(const int index, void* data, const size_t size) const {
        if (index >= m_payload->parameters_size()) {
            AXCL_ASSERT(0, "index %d exceed param count %d", index, static_cast<int32_t>(m_payload->parameters_size()));
            return false;
        }

        if (const auto& param = m_payload->parameters(index); size != param.size()) {
            AXCL_ASSERT(0, "param %d size %zd is not equal to %zd", index, param.size(), size);
            return false;
        }

        memcpy(data, m_payload->parameters(index).data(), size);
        return true;
    }

    [[nodiscard]] const std::string& get_data(const int index) const {
        return m_payload->parameters(index);
    }

private:
    axcl::native::module::dsp::Response* m_payload = {nullptr};
};

axcl::pkg::host::dsp_response::dsp_response() : m_impl(new impl()) {
}

axcl::pkg::host::dsp_response::dsp_response(dsp_response&& other) noexcept : m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::host::dsp_response& axcl::pkg::host::dsp_response::operator=(dsp_response&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }

    return *this;
}

axcl::pkg::host::dsp_response::~dsp_response() {
    delete m_impl;
}

[[nodiscard]] bool axcl::pkg::host::dsp_response::decode(const void* message, const uint32_t size) const {
    if (!native_response::decode(message, size)) {
        return false;
    }

    return m_impl->select(static_cast<axcl::native::response::Payload*>(native_response::get_payload()));
}

void* axcl::pkg::host::dsp_response::get_payload() const {
    return m_impl->get_payload();
}

axcl::pkg::native::DSP_API axcl::pkg::host::dsp_response::get_api() const {
    return m_impl->get_api();
}

bool axcl::pkg::host::dsp_response::check_api(const axcl::pkg::native::API api) {
    return m_impl->check_api(api.dsp);
}

size_t axcl::pkg::host::dsp_response::get_data_size(int index) {
    return m_impl->get_data_size(index);
}

bool axcl::pkg::host::dsp_response::get_data(const int index, void* data, const size_t size) {
    return m_impl->get_data(index, data, size);
}

const std::string& axcl::pkg::host::dsp_response::get_data(const int index) {
    return m_impl->get_data(index);
}
