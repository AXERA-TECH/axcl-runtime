/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "npu_response.hpp"

#include "axcl_assert.hpp"
#include "axcl_npu_type.h"

#include "protocol.hpp"
#include "enum_cast.hpp"

struct axcl::pkg::device::npu_response::impl {
    [[nodiscard]] bool check_payload() const {
        return true;
    }

    [[nodiscard]] bool select(axcl::native::request::Payload* payload) {
        if (axcl::native::ENGINE != payload->head().module()) {
            AXCL_ASSERT(0, "invalid native module %d", static_cast<int32_t>(payload->head().module()));
            return false;
        }

        m_payload = payload->mutable_body()->mutable_engine();
        AXCL_ASSERT(m_payload, "nil pointer");

        return check_payload();
    }

    [[nodiscard]] void* get_payload() const {
        return m_payload;
    }

    [[nodiscard]] native::ENGINE_API get_api() const {
        return axcl::enum_cast<axcl::native::module::engine::Api, native::ENGINE_API>(m_payload->api());
    }

    [[nodiscard]] bool check_api(const native::ENGINE_API api) const {
        return axcl::enum_equal<axcl::native::module::engine::Api, native::ENGINE_API>(m_payload->api(), api);
    }

    [[nodiscard]] size_t get_data_size(const int index) const {
        return m_payload->parameters(index).size();
    }

    [[nodiscard]] bool get_data(const int index, void* data, const size_t size) const {
        if (index >= m_payload->parameters_size()) {
            AXCL_ASSERT(0, "index %d exceed param count %d", index, static_cast<int32_t>(m_payload->parameters_size()));
            return false;
        }

        if (const auto& param = m_payload->parameters(index); size != param.size()) {
            AXCL_ASSERT(0, "param %d size %zd is not equal to %zd", index, param.size(), size);
            return false;
        }

        ::memcpy(data, m_payload->parameters(index).data(), size);
        return true;
    }

    [[nodiscard]] const std::string& get_data(const int index) const {
        return m_payload->parameters(index);
    }

private:
    axcl::native::module::engine::Request* m_payload = {nullptr};
};

axcl::pkg::device::npu_response::npu_response() : m_impl(new impl()) {
}

axcl::pkg::device::npu_response::npu_response(npu_response&& other) noexcept : m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::device::npu_response& axcl::pkg::device::npu_response::operator=(npu_response&& other) noexcept {
    if (this != &other) {
        delete this->m_impl;
        this->m_impl = std::exchange(other.m_impl, nullptr);
    }

    return *this;
}

axcl::pkg::device::npu_response::~npu_response() {
    delete this->m_impl;
}

[[nodiscard]] bool axcl::pkg::device::npu_response::decode(const void* message, const uint32_t size) const {
    if (!native_response::decode(message, size)) {
        return false;
    }

    return this->m_impl->select(static_cast<axcl::native::request::Payload*>(native_response::get_payload()));
}

void* axcl::pkg::device::npu_response::get_payload() const {
    return this->m_impl->get_payload();
}

axcl::pkg::native::ENGINE_API axcl::pkg::device::npu_response::get_api() const {
    return this->m_impl->get_api();
}

bool axcl::pkg::device::npu_response::check_api(const native::API api) {
    return this->m_impl->check_api(api.engine);
}

size_t axcl::pkg::device::npu_response::get_data_size(const int index) {
    return this->m_impl->get_data_size(index);
}

bool axcl::pkg::device::npu_response::get_data(const int index, void* data, const size_t size) {
    return this->m_impl->get_data(index, data, size);
}

const std::string& axcl::pkg::device::npu_response::get_data(const int index) {
    return this->m_impl->get_data(index);
}
