/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "venc_response.hpp"
#include <string.h>
#include "axcl_assert.hpp"
#include "enum_cast.hpp"
#include "axcl_logger.hpp"
#include "protocol.hpp"

#define TAG "package"

struct axcl::pkg::device::venc_response::impl {
    [[nodiscard]] bool check_payload() const {
        return true;
    }

    [[nodiscard]] bool select(axcl::native::request::Payload* payload) {
        if (axcl::native::VENC != payload->head().module()) {
            AXCL_ASSERT(0, "invalid native module %d", static_cast<int32_t>(payload->head().module()));
            return false;
        }

        m_payload = payload->mutable_body()->mutable_venc();
        AXCL_ASSERT(m_payload, "nil pointer");

        return check_payload();
    }

    void* get_payload() {
        return m_payload;
    }

    axcl::pkg::native::VENC_API get_api() const {
        return axcl::enum_cast<axcl::native::module::venc::Api, axcl::pkg::native::VENC_API>(m_payload->api());
    }

    bool check_api(axcl::pkg::native::VENC_API api) const {
        return axcl::enum_equal<axcl::native::module::venc::Api, axcl::pkg::native::VENC_API>(m_payload->api(), api);
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
    axcl::native::module::venc::Request* m_payload = {nullptr};
};

axcl::pkg::device::venc_response::venc_response() : m_impl(new impl()) {
}

axcl::pkg::device::venc_response::venc_response(venc_response&& other) noexcept : m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::device::venc_response& axcl::pkg::device::venc_response::operator=(venc_response&& other) noexcept {
    if (this != &other) {
        delete m_impl;
        m_impl = std::exchange(other.m_impl, nullptr);
    }

    return *this;
}

axcl::pkg::device::venc_response::~venc_response() {
    delete m_impl;
}

[[nodiscard]] bool axcl::pkg::device::venc_response::decode(const void* message, uint32_t size) const {
    if (!native_response::decode(message, size)) {
        return false;
    }

    return m_impl->select(static_cast<axcl::native::request::Payload*>(native_response::get_payload()));
}

void* axcl::pkg::device::venc_response::get_payload() const {
    return m_impl->get_payload();
}

axcl::pkg::native::VENC_API axcl::pkg::device::venc_response::get_api() const {
    return m_impl->get_api();
}

bool axcl::pkg::device::venc_response::check_api(axcl::pkg::native::API api) {
    return m_impl->check_api(api.venc);
}

size_t axcl::pkg::device::venc_response::get_data_size(int index) {
    return m_impl->get_data_size(index);
}

bool axcl::pkg::device::venc_response::get_data(int index, void* data, size_t size) {
    return m_impl->get_data(index, data, size);
}

const std::string& axcl::pkg::device::venc_response::get_data(int index) {
    return m_impl->get_data(index);
}

bool axcl::pkg::device::venc_response::deserialize_gop_svc_t(const uint8_t* buf, uint32_t size, AX_VENC_GOP_SVC_T_T& svc,
                                                             std::vector<uint8_t>& mem) {
    if (!buf || size == 0) {
        return false;
    }
    /* 1st 4bytes: gop size */
    svc.u32GopSize = *(reinterpret_cast<const uint32_t*>(buf));

    /* 2nd 4bytes: num */
    uint32_t num = *((uint32_t*)&buf[sizeof(uint32_t)]);
    if (0 == num) {
        mem.clear();
        svc.s8SvcTCfg = nullptr;
        return true;
    }

    const uint32_t cnt = num + 1 /* last nullptr */;
    mem.resize(sizeof(AX_CHAR*) * cnt + size);

    /* copy buf to mem */
    uint8_t* ptr = reinterpret_cast<uint8_t*>(mem.data());
    ptr += sizeof(char*) * cnt;
    ::memcpy(ptr, buf, size);

    /* front area of mem is s8SvcTCfg pointer array */
    svc.s8SvcTCfg = reinterpret_cast<char**>(mem.data());

    ptr += sizeof(uint32_t); /* skip gop size */
    ptr += sizeof(uint32_t); /* skip num */

    for (uint32_t i = 0; i < num; ++i) {
        svc.s8SvcTCfg[i] = const_cast<char*>(reinterpret_cast<const char*>(ptr));
        ptr += ::strlen(reinterpret_cast<const char*>(ptr)) + 1;
    }

    svc.s8SvcTCfg[num] = nullptr;
    return true;
}