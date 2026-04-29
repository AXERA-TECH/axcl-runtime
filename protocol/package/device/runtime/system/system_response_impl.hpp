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
#include "axcl_logger.hpp"
#include "protocol.hpp"
#include "system_response.hpp"

namespace axcl::pkg::device {

struct system_response::impl {
    [[nodiscard]] bool check_payload() {
        bool status = (m_payload && m_payload->has_head() && m_payload->has_body());
        if (status) {
            const auto &head = m_payload->head();
            const auto &body = m_payload->body();

            switch (head.module()) {
                case axcl::runtime::system::Module::Alive:
                    m_module = axcl::pkg::runtime::system::module::ALIVE;
                    break;
                case axcl::runtime::system::Module::Latency:
                    m_module = axcl::pkg::runtime::system::module::LATENCY;
                    status = body.has_latency();
                    break;
                case axcl::runtime::system::Module::Log:
                    m_module = axcl::pkg::runtime::system::module::LOG;
                    status = body.has_log();
                    break;
                case axcl::runtime::system::Module::Control:
                    m_module = axcl::pkg::runtime::system::module::CONTROL;
                    status = body.has_control();
                    break;
                default:
                    LOG_MM_E("package", "unknown system module {}", static_cast<int32_t>(head.module()));
                    AXCL_ASSERT(0, "unknown system module %d", static_cast<int32_t>(head.module()));
                    status = false;
                    break;
            }
        }

        return status;
    }

    [[nodiscard]] bool select(axcl::runtime::request::Payload *payload) {
        if (axcl::runtime::SYSTEM != payload->head().module()) {
            LOG_MM_E("package", "current module {} is not system module", static_cast<int32_t>(payload->head().module()));
            AXCL_ASSERT(0, "current module %d is not system module", static_cast<int32_t>(payload->head().module()));
            return false;
        }

        m_payload = payload->mutable_body()->mutable_system();
        AXCL_ASSERT(m_payload, "nil pointer");

        return check_payload();
    }

    axcl::pkg::runtime::system::module get_module() const {
        return m_module;
    }

    void *get_payload() {
        return m_payload;
    }

private:
    axcl::runtime::system::request::Payload *m_payload;
    axcl::pkg::runtime::system::module m_module = axcl::pkg::runtime::system::module::UNKNOWN;
};

}  // namespace axcl::pkg::device