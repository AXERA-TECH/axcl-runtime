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

#include "request.hpp"
#include <atomic>
#include <chrono>
#include "axcl_assert.hpp"
#include "axcl_logger.hpp"
#include "protocol.hpp"
#include "version.hpp"

#define TAG "package"

#ifndef MIN_PKG_HOLD_SIZE
#define MIN_PKG_HOLD_SIZE (1024) /* 1K */
#endif

#ifndef MAX_PKG_HOLD_SIZE
#define MAX_PKG_HOLD_SIZE (0x10000) /* 64K */
#endif

struct axcl::pkg::host::request::impl {
    explicit impl() {
        m_payload.mutable_head();

        set_version({AXCL_PROTOCOL_MAJOR_VERSION, AXCL_PROTOCOL_MINOR_VERSION, AXCL_PROTOCOL_PATCH_VERSION});
        set_timestamp(0);

        m_payload.mutable_body();
        m_payload.mutable_tail();
    }

    ~impl() {
        if (m_pkg) {
            delete[] m_pkg;
        }
    }

    void set_timestamp(uint64_t timestamp) {
        if (0 == timestamp) {
            timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        }

        m_payload.mutable_head()->set_timestamp(timestamp);
    }

    void set_seq_num(uint64_t seq_num) {
        m_payload.mutable_head()->set_seq_num(seq_num);
    }

    void set_version(const axcl::pkg::version& v) {
        const auto version = m_payload.mutable_head()->mutable_version();
        version->set_major_v(v.major_v);
        version->set_minor_v(v.minor_v);
        version->set_patch_v(v.patch_v);
    }

    void set_env(const axcl::pkg::environment& env) {
        const auto envir = m_payload.mutable_head()->mutable_env();
        envir->set_pid(env.pid);
        envir->set_tid(env.tid);
    }

    uint64_t get_seq_num() const {
        return m_payload.head().seq_num();
    }

    axcl::pkg::version get_version() const {
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

    [[nodiscard]] void* select(const axcl::pkg::module& module) {
        const auto head = m_payload.mutable_head();
        const auto body = m_payload.mutable_body();

        switch (module) {
            case axcl::pkg::module::RUNTIME:
                head->set_module(axcl::RUNTIME);
                return body->mutable_runtime();
            case axcl::pkg::module::NATIVE:
                head->set_module(axcl::NATIVE);
                return body->mutable_native();
            default:
                LOG_MM_E(TAG,  "unknown package module {}", static_cast<int32_t>(module));
                AXCL_ASSERT(0, "unknown package module %d", static_cast<int32_t>(module));
                return nullptr;
        }
    }

    [[nodiscard]] void* encode(const axcl::pkg::head& hd, uint32_t& size) {
        if (0 == hd.seq_num) {
            LOG_MM_E(TAG, "encode package fail, seq num is 0");
            size = 0;
            return nullptr;
        }

        set_seq_num(hd.seq_num);

        // TODO: add crc works
        m_payload.mutable_tail()->set_crc(0);

        int bytes = static_cast<int>(m_payload.ByteSizeLong());
        size = bytes + PACKAGE_HEAD_SIZE;

        uint8_t* pkg;
        if (size <= MIN_PKG_HOLD_SIZE) {
            pkg = m_min_pkg;
        } else {
            if (!m_pkg) {
                m_pkg_size = size;
                m_pkg = new uint8_t[m_pkg_size];
            } else {
                if (size > m_pkg_size || (size <= MAX_PKG_HOLD_SIZE && m_pkg_size > MAX_PKG_HOLD_SIZE)) {
                    /* keep hold size less than MAX_PKG_HOLD_SIZE */
                    delete[] m_pkg;
                    m_pkg = new uint8_t[size];
                    m_pkg_size = size;
                }
            }

            pkg = m_pkg;
        }

        *((axcl::pkg::head*)pkg) = hd;

        if (!m_payload.SerializeToArray(static_cast<void*>(&pkg[PACKAGE_HEAD_SIZE]), bytes)) {
            LOG_MM_E(TAG, "encode request module {} message fail", static_cast<int>(m_payload.mutable_head()->module()));
            size = 0;
            return nullptr;
        }

        return pkg;
    }

private:
    axcl::request::Payload m_payload;
    uint8_t* m_pkg = nullptr;
    size_t m_pkg_size = 0;
    uint8_t m_min_pkg[MIN_PKG_HOLD_SIZE];
};

axcl::pkg::host::request::request() : m_impl(new impl()) {
}

axcl::pkg::host::request::request(request&& other) noexcept : m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::host::request& axcl::pkg::host::request::operator=(request&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }

    return *this;
}

axcl::pkg::host::request::~request() {
    delete m_impl;
}

void axcl::pkg::host::request::set_env(const axcl::pkg::environment& env) const {
    m_impl->set_env(env);
}

void axcl::pkg::host::request::set_timestamp(uint64_t timestamp) const {
    m_impl->set_timestamp(timestamp);
}

void axcl::pkg::host::request::set_version(const axcl::pkg::version& v) const {
    m_impl->set_version(v);
}

uint64_t axcl::pkg::host::request::get_timestamp() const {
    return m_impl->get_timestamp();
}

uint64_t axcl::pkg::host::request::get_seq_num() const {
    return m_impl->get_seq_num();
}

axcl::pkg::version axcl::pkg::host::request::get_version() const {
    return m_impl->get_version();
}

axcl::pkg::environment axcl::pkg::host::request::get_env() const {
    return m_impl->get_env();
}

[[nodiscard]] void* axcl::pkg::host::request::select(const axcl::pkg::module& module) const {
    return m_impl->select(module);
}

[[nodiscard]] void* axcl::pkg::host::request::encode(const axcl::pkg::head& hd, uint32_t& size) const {
    return m_impl->encode(hd, size);
}
