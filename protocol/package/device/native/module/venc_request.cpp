/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "venc_request.hpp"
#include <string.h>
#include "enum_cast.hpp"
#include "axcl_logger.hpp"
#include "protocol.hpp"

#define TAG "package"

struct axcl::pkg::device::venc_request::impl {
    impl() = delete;

    explicit impl(void* payload) : m_payload(static_cast<axcl::native::module::venc::Response*>(payload)) {
    }

    bool add_data(const void* data, size_t size) {
        m_payload->add_parameters(data, size);
        return true;
    }

    bool set_api(axcl::pkg::native::VENC_API api) {
        m_payload->set_api(axcl::enum_cast<axcl::pkg::native::VENC_API, axcl::native::module::venc::Api>(api));
        m_payload->clear_parameters();
        return true;
    }

private:
    axcl::native::module::venc::Response* m_payload;
};

axcl::pkg::device::venc_request::venc_request()
    : m_impl(new impl(native_request::select(axcl::pkg::native::module::VENC))) {
}

axcl::pkg::device::venc_request::venc_request(venc_request&& other) noexcept
    : native_request(std::move(other)), m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::device::venc_request& axcl::pkg::device::venc_request::operator=(venc_request&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }

    return *this;
}

axcl::pkg::device::venc_request::~venc_request() {
    delete m_impl;
}

bool axcl::pkg::device::venc_request::add_data(const void* data, size_t size) {
    return m_impl->add_data(data, size);
}

bool axcl::pkg::device::venc_request::set_api(axcl::pkg::native::VENC_API api) {
    return m_impl->set_api(api);
}

bool axcl::pkg::device::venc_request::serialize_gop_svc_t(const AX_VENC_GOP_SVC_T_T& svc, uint8_t*& buf, uint32_t& size) {
    uint32_t count = 0;
    while (svc.s8SvcTCfg[count] != nullptr) {
        count++;
    }

    size = sizeof(uint32_t) /*u32GopSize */ + sizeof(uint32_t); /* svc.u32GopSize */
    for (uint32_t i = 0; i < count; ++i) {
        size += ::strlen(svc.s8SvcTCfg[i]) + 1 /* '\0' */;
    }

    buf = (uint8_t*)::calloc(1, size);
    if (!buf) {
        LOG_MM_E(TAG, "malloc svc serialize memory fail");
        return false;
    }

    uint8_t* ptr = buf;
    ::memcpy(ptr, &svc.u32GopSize, sizeof(svc.u32GopSize));
    ptr += sizeof(uint32_t);
    ::memcpy(ptr, &count, sizeof(uint32_t));
    ptr += sizeof(uint32_t);

    for (uint32_t i = 0; i < count; ++i) {
        ::strcpy(reinterpret_cast<char*>(ptr), svc.s8SvcTCfg[i]);
        ptr += ::strlen(svc.s8SvcTCfg[i]) + 1;
    }

    return true;
}
