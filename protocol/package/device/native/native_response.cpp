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

#include "native_response.hpp"
#include "axcl_assert.hpp"
#include "axcl_logger.hpp"
#include "protocol.hpp"

#define TAG "package"

struct axcl::pkg::device::native_response::impl {
    [[nodiscard]] bool check_payload() const {
        bool status = (m_payload && m_payload->has_head() && m_payload->has_body());
        if (status) {
            const auto& head = m_payload->head();
            const auto& body = m_payload->body();
            if (status) {
                switch (head.module()) {
                    case axcl::native::SYS:
                        status = body.has_sys();
                        break;
                    case axcl::native::VDEC:
                        status = body.has_vdec();
                        break;
                    case axcl::native::VENC:
                        status = body.has_venc();
                        break;
                    case axcl::native::IVPS:
                        status = body.has_ivps();
                        break;
                    case axcl::native::IVE:
                        status = body.has_ive();
                        break;
                    case axcl::native::ENGINE:
                        status = body.has_engine();
                        break;
                    case axcl::native::DMADIM:
                        status = body.has_dmadim();
                        break;
                    default:
                        LOG_MM_E(TAG,  "unknown native module {}", static_cast<int32_t>(head.module()));
                        AXCL_ASSERT(0, "unknown native module %d", static_cast<int32_t>(head.module()));
                        status = false;
                        break;
                }
            }
        } else {
            if (!m_payload) {
                LOG_MM_E(TAG, "native response payload is nil");
            } else {
                LOG_MM_E(TAG, "check native response payload fail, has head {} has body {}", m_payload->has_head(), m_payload->has_body());
            }
        }

        return status;
    }

    [[nodiscard]] bool select(axcl::request::Payload* payload) {
        if (axcl::NATIVE != payload->head().module()) {
            return false;
        }

        m_payload = payload->mutable_body()->mutable_native();
        AXCL_ASSERT(m_payload, "nil pointer");

        return check_payload();
    }

    axcl::pkg::native::module get_module() const {
        AXCL_ASSERT(m_payload, "nil pointer");

        switch (m_payload->head().module()) {
            case axcl::native::SYS:
                return axcl::pkg::native::module::SYS;
            case axcl::native::VDEC:
                return axcl::pkg::native::module::VDEC;
            case axcl::native::VENC:
                return axcl::pkg::native::module::VENC;
            case axcl::native::IVPS:
                return axcl::pkg::native::module::IVPS;
            case axcl::native::IVE:
                return axcl::pkg::native::module::IVE;
            case axcl::native::ENGINE:
                return axcl::pkg::native::module::ENGINE;
            case axcl::native::DMADIM:
                return axcl::pkg::native::module::DMADIM;
            default:
                LOG_MM_E(TAG,  "unknown native module {}", static_cast<int32_t>(m_payload->head().module()));
                AXCL_ASSERT(0, "unknown native module %d", static_cast<int32_t>(m_payload->head().module()));
                return axcl::pkg::native::module::UNKNOWN;
        }
    }

    void* get_payload() {
        return m_payload;
    }

private:
    axcl::native::request::Payload* m_payload = {nullptr};
};

axcl::pkg::device::native_response::native_response() : m_impl(new impl()) {
}

axcl::pkg::device::native_response::native_response(native_response&& other) noexcept : m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::device::native_response& axcl::pkg::device::native_response::operator=(native_response&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }

    return *this;
}

axcl::pkg::device::native_response::~native_response() {
    delete m_impl;
}

[[nodiscard]] bool axcl::pkg::device::native_response::decode(const void* message, uint32_t size) const {
    if (!response::decode(message, size)) {
        return false;
    }

    m_arg_index = 0;
    return m_impl->select(static_cast<axcl::request::Payload*>(response::get_payload()));
}

[[nodiscard]] axcl::pkg::native::module axcl::pkg::device::native_response::get_module() const {
    return m_impl->get_module();
}

void* axcl::pkg::device::native_response::get_payload() const {
    return m_impl->get_payload();
}

bool axcl::pkg::device::native_response::check_api(axcl::pkg::native::API api) {
    return false;
}

size_t axcl::pkg::device::native_response::get_data_size(int index) {
    return 0;
}

bool axcl::pkg::device::native_response::get_data(int index, void* data, size_t size) {
    return false;
}

const std::string& axcl::pkg::device::native_response::get_data(int index) {
    static const std::string empty;
    return empty;
}

template <>
bool axcl::pkg::device::native_response::get_arg<axcl::pkg::uint8_array>(int index, axcl::pkg::uint8_array* arg) {
    const auto& data = get_data(index);
    arg->size = data.size();
    arg->data = (uint8_t*)data.data();
    arg->size = get_data_size(index);

    return true;
}
