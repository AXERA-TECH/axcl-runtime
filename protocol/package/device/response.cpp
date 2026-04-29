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

#include "response.hpp"
#include "axcl_assert.hpp"
#include "axcl_logger.hpp"
#include "protocol.hpp"

#define TAG "package"

struct axcl::pkg::device::response::impl {
    [[nodiscard]] bool check_payload() const {
        bool status = (m_payload.has_head() && m_payload.has_body() && m_payload.has_tail());
        if (status) {
            const auto& head = m_payload.head();
            const auto& body = m_payload.body();
            switch (head.module()) {
                case axcl::RUNTIME:
                    status = body.has_runtime();
                    break;
                case axcl::NATIVE:
                    status = body.has_native();
                    break;
                default:
                    LOG_MM_E(TAG,  "unknown package module {}", static_cast<int32_t>(head.module()));
                    AXCL_ASSERT(0, "unknown package module %d", static_cast<int32_t>(head.module()));
                    status = false;
                    break;
            }
        }

        if (!status) {
            LOG_MM_E(TAG, "invalid response message payload");
        }

        return status;
    }

    [[nodiscard]] bool decode(const void* message, uint32_t size) {
        if (!message || size <= PACKAGE_HEAD_SIZE) {
            LOG_MM_E(TAG, "invalid response message, size: {}", size);
            return false;
        }

        if (!m_payload.ParseFromArray((void*)((uint8_t*)message + PACKAGE_HEAD_SIZE), (int)(size - PACKAGE_HEAD_SIZE))) {
            LOG_MM_E(TAG, "device decode response message fail");
            return false;
        }

        return check_payload();
    }

    uint64_t get_seq_num() const {
        return m_payload.head().seq_num();
    }

    axcl::pkg::version get_version() {
        const auto& v = m_payload.head().version();
        axcl::pkg::version ret;
        ret.major_v = v.major_v();
        ret.minor_v = v.minor_v();
        ret.patch_v = v.patch_v();
        return ret;
    };

    axcl::pkg::environment get_env() const {
        const auto& env = m_payload.head().env();
        axcl::pkg::environment ret;
        ret.pid = env.pid();
        ret.tid = env.tid();
        return ret;
    }

    uint64_t get_timestamp() const {
        return m_payload.head().timestamp();
    }

    axcl::pkg::module get_module() const {
        switch (m_payload.head().module()) {
            case axcl::RUNTIME:
                return axcl::pkg::module::RUNTIME;
            case axcl::NATIVE:
                return axcl::pkg::module::NATIVE;
            default:
                LOG_MM_E(TAG,  "unknown package module {}", static_cast<int32_t>(m_payload.head().module()));
                AXCL_ASSERT(0, "unknown package module %d", static_cast<int32_t>(m_payload.head().module()));
                return axcl::pkg::module::UNKNOWN;
        }
    }

    void* get_payload() {
        return &m_payload;
    }

private:
    axcl::request::Payload m_payload;
};

axcl::pkg::device::response::response() : m_impl(new impl()) {
}

axcl::pkg::device::response::response(response&& other) noexcept : m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::device::response& axcl::pkg::device::response::operator=(response&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }

    return *this;
}

axcl::pkg::device::response::~response() {
    delete m_impl;
}

[[nodiscard]] bool axcl::pkg::device::response::decode(const void* message, uint32_t size) const {
    return m_impl->decode(message, size);
}

uint64_t axcl::pkg::device::response::get_seq_num() const {
    return m_impl->get_seq_num();
}

axcl::pkg::version axcl::pkg::device::response::get_version() const {
    return m_impl->get_version();
}

axcl::pkg::environment axcl::pkg::device::response::get_env() const {
    return m_impl->get_env();
}

uint64_t axcl::pkg::device::response::get_timestamp() const {
    return m_impl->get_timestamp();
}

axcl::pkg::module axcl::pkg::device::response::get_module() const {
    return m_impl->get_module();
}

void* axcl::pkg::device::response::get_payload() const {
    return m_impl->get_payload();
}
