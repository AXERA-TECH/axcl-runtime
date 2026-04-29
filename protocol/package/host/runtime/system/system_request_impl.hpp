/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#pragma once

#include "axcl_assert.hpp"
#include "enum_cast.hpp"
#include "axcl_logger.hpp"
#include "protocol.hpp"
#include "system_request.hpp"

namespace axcl::pkg::host {

struct system_request::impl {
    impl() = delete;
    explicit impl(void* payload) : m_payload(static_cast<axcl::runtime::system::request::Payload*>(payload)) {
        m_payload->mutable_head();
        m_payload->mutable_body();
    }

    [[nodiscard]] void* select(const axcl::pkg::runtime::system::module& module) {
        const auto head = m_payload->mutable_head();
        const auto body = m_payload->mutable_body();

        switch (module) {
            case axcl::pkg::runtime::system::module::ALIVE:
                head->set_module(axcl::runtime::system::Alive);
                return body->mutable_alive();
            case axcl::pkg::runtime::system::module::LATENCY:
                head->set_module(axcl::runtime::system::Latency);
                return body->mutable_latency();
            case axcl::pkg::runtime::system::module::LOG:
                head->set_module(axcl::runtime::system::Log);
                return body->mutable_log();
            case axcl::pkg::runtime::system::module::CONTROL:
                head->set_module(axcl::runtime::system::Control);
                return body->mutable_control();
            default:
                LOG_MM_E("package", "unknown system module {}", static_cast<int32_t>(module));
                AXCL_ASSERT(0, "unknown system module %d", static_cast<int32_t>(module));
                return nullptr;
        }
    }

    axcl::pkg::runtime::system::module get_module() const {
        return axcl::enum_cast<axcl::runtime::system::Module, axcl::pkg::runtime::system::module>(m_payload->head().module());
    }

private:
    axcl::runtime::system::request::Payload* m_payload;
};

}  // namespace axcl::pkg::host