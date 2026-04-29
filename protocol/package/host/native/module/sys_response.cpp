/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "sys_response.hpp"
#include <string.h>
#include "axcl_assert.hpp"
#include "enum_cast.hpp"
#include "protocol.hpp"

struct axcl::pkg::host::sys_response::impl {
    [[nodiscard]] bool check_payload() const {
        return true;
    }

    [[nodiscard]] bool select(axcl::native::response::Payload* payload) {
        if (axcl::native::SYS != payload->head().module()) {
            AXCL_ASSERT(0, "invalid native module %d", static_cast<int32_t>(payload->head().module()));
            return false;
        }

        m_payload = payload->mutable_body()->mutable_sys();
        AXCL_ASSERT(m_payload, "nil pointer");

        return check_payload();
    }

    void* get_payload() {
        return m_payload;
    }

    axcl::pkg::native::SYS_API get_api() const {
        return axcl::enum_cast<axcl::native::module::sys::Api, axcl::pkg::native::SYS_API>(m_payload->api());
    }

    bool check_api(axcl::pkg::native::SYS_API api) const {
        return axcl::enum_equal<axcl::native::module::sys::Api, axcl::pkg::native::SYS_API>(m_payload->api(), api);
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
    axcl::native::module::sys::Response* m_payload = {nullptr};
};

axcl::pkg::host::sys_response::sys_response() : m_impl(new impl()) {
}

axcl::pkg::host::sys_response::sys_response(sys_response&& other) noexcept : m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::host::sys_response& axcl::pkg::host::sys_response::operator=(sys_response&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }

    return *this;
}

axcl::pkg::host::sys_response::~sys_response() {
    delete m_impl;
}

[[nodiscard]] bool axcl::pkg::host::sys_response::decode(const void* message, uint32_t size) const {
    if (!native_response::decode(message, size)) {
        return false;
    }

    return m_impl->select(static_cast<axcl::native::response::Payload*>(native_response::get_payload()));
}

void* axcl::pkg::host::sys_response::get_payload() const {
    return m_impl->get_payload();
}

axcl::pkg::native::SYS_API axcl::pkg::host::sys_response::get_api() const {
    return m_impl->get_api();
}

bool axcl::pkg::host::sys_response::check_api(axcl::pkg::native::API api) {
    return m_impl->check_api(api.sys);
}

size_t axcl::pkg::host::sys_response::get_data_size(int index) {
    return m_impl->get_data_size(index);
}

bool axcl::pkg::host::sys_response::get_data(int index, void* data, size_t size) {
    return m_impl->get_data(index, data, size);
}

const std::string& axcl::pkg::host::sys_response::get_data(int index) {
    return m_impl->get_data(index);
}