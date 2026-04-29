/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "ive_response.hpp"
#include <string.h>
#include "axcl_assert.hpp"
#include "enum_cast.hpp"
#include "protocol.hpp"

struct axcl::pkg::device::ive_response::impl {
    [[nodiscard]] bool check_payload() const {
        return true;
    }

    [[nodiscard]] bool select(axcl::native::request::Payload* payload) {
        if (axcl::native::IVE != payload->head().module()) {
            AXCL_ASSERT(0, "invalid native module %d", static_cast<int32_t>(payload->head().module()));
            return false;
        }

        m_payload = payload->mutable_body()->mutable_ive();
        AXCL_ASSERT(m_payload, "nil pointer");

        return check_payload();
    }

    void* get_payload() {
        return m_payload;
    }

    axcl::pkg::native::IVE_API get_api() const {
        return axcl::enum_cast<axcl::native::module::ive::Api, axcl::pkg::native::IVE_API>(m_payload->api());
    }

    bool check_api(axcl::pkg::native::IVE_API api) const {
        return axcl::enum_equal<axcl::native::module::ive::Api, axcl::pkg::native::IVE_API>(m_payload->api(), api);
    }

    size_t get_data_size(int index) {
        return m_payload->parameters(index).size();
    }

    bool get_data(int index, void* data, size_t size) {
        if (index >= m_payload->parameters_size()) {
            AXCL_ASSERT(0, "index %d exceed param count %d", index, static_cast<int32_t>(m_payload->parameters_size()));
            return false;
        }

        const auto& param = m_payload->parameters(index);
        if (size != param.size()) {
            AXCL_ASSERT(0, "param %d size %zd is not equal to %zd", index, param.size(), size);
            return false;
        }

        memcpy(data, m_payload->parameters(index).data(), size);
        return true;
    }

    const std::string& get_data(int index) {
        return m_payload->parameters(index);
    }

private:
    axcl::native::module::ive::Request* m_payload = {nullptr};
};

axcl::pkg::device::ive_response::ive_response() : m_impl(new impl()) {
}

axcl::pkg::device::ive_response::ive_response(ive_response&& other) noexcept : m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::device::ive_response& axcl::pkg::device::ive_response::operator=(ive_response&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }

    return *this;
}

axcl::pkg::device::ive_response::~ive_response() {
    delete m_impl;
}

[[nodiscard]] bool axcl::pkg::device::ive_response::decode(const void* message, uint32_t size) const {
    if (!native_response::decode(message, size)) {
        return false;
    }

    return m_impl->select(static_cast<axcl::native::request::Payload*>(native_response::get_payload()));
}

void* axcl::pkg::device::ive_response::get_payload() const {
    return m_impl->get_payload();
}

axcl::pkg::native::IVE_API axcl::pkg::device::ive_response::get_api() const {
    return m_impl->get_api();
}

bool axcl::pkg::device::ive_response::check_api(axcl::pkg::native::API api) {
    return m_impl->check_api(api.ive);
}

size_t axcl::pkg::device::ive_response::get_data_size(int index) {
    return m_impl->get_data_size(index);
}

bool axcl::pkg::device::ive_response::get_data(int index, void* data, size_t size) {
    return m_impl->get_data(index, data, size);
}

const std::string& axcl::pkg::device::ive_response::get_data(int index) {
    return m_impl->get_data(index);
}