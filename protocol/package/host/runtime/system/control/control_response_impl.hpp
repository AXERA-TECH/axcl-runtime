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
#include "package/include/runtime/system/type.hpp"
#include "protocol.hpp"

namespace axcl::pkg::host {

struct control_response::impl {
    [[nodiscard]] bool check_payload() {
        if (m_payload) {
            bool status = false;
            switch (m_payload->cmd()) {
                case axcl::runtime::system::Command::CREATE_CONTEXT:
                case axcl::runtime::system::Command::DESTROY_CONTEXT:
                    status = (m_payload->has_context() && m_payload->has_stream() && m_payload->has_pid() && m_payload->has_tid());
                    break;
                case axcl::runtime::system::Command::CREATE_STREAM:
                case axcl::runtime::system::Command::DESTROY_STREAM:
                    status = (m_payload->has_context() && m_payload->has_stream());
                    break;
                case axcl::runtime::system::Command::SYNC_STREAM:
                    status = (m_payload->has_context() && m_payload->has_stream() && m_payload->has_status());
                    break;
                case axcl::runtime::system::Command::CREATE_EVENT:
                case axcl::runtime::system::Command::DESTROY_EVENT:
                    status = (m_payload->has_context() && m_payload->has_stream() && m_payload->has_event());
                    break;
                case axcl::runtime::system::Command::RECORD_EVENT:
                case axcl::runtime::system::Command::WAIT_EVENT:
                    status = (m_payload->has_context() && m_payload->has_stream() && m_payload->has_event() && m_payload->has_status());
                    break;
                case axcl::runtime::system::Command::SHELL:
                    status = m_payload->has_shell();
                    break;
                case axcl::runtime::system::Command::SETUP_PCIE_SUB_ID:
                    status = m_payload->has_status();
                    break;
                default:
                    LOG_MM_E("package", "unknown system control command {}", static_cast<int32_t>(m_payload->cmd()));
                    AXCL_ASSERT(0, "unknown system control command %d", static_cast<int32_t>(m_payload->cmd()));
                    break;
            }

            if (!status) {
                LOG_MM_E("package", "check system control payload fail");
            }

            return status;

        } else {
            LOG_MM_E("package", "system control response payload is nil");
            return false;
        }
    }

    [[nodiscard]] bool select(axcl::runtime::system::response::Payload *payload) {
        if (axcl::runtime::system::Module::Control != payload->head().module()) {
            LOG_MM_E("package", "current module {} is not control module", static_cast<int32_t>(payload->head().module()));
            AXCL_ASSERT(0, "current module %d is not control module", static_cast<int32_t>(payload->head().module()));
            return false;
        }

        m_payload = payload->mutable_body()->mutable_control();
        AXCL_ASSERT(m_payload, "nil pointer");

        return check_payload();
    }

    axcl::pkg::runtime::system::command get_cmd() const {
        return axcl::enum_cast<axcl::runtime::system::Command, axcl::pkg::runtime::system::command>(m_payload->cmd());
    }

    uint32_t get_pid() const {
        return m_payload->pid();
    }

    uint32_t get_tid() const {
        return m_payload->tid();
    }

    uint32_t get_context() const {
        return m_payload->context();
    }

    uint32_t get_stream() const {
        return m_payload->stream();
    }

    uint32_t get_event() const {
        return m_payload->event();
    }

    int32_t get_status() const {
        return m_payload->status();
    }

    int32_t get_shell_cmd_status() const {
        return m_payload->shell().status();
    }

    int32_t get_pcie_sub_id_setup_status() const {
        return m_payload->status();
    }

    const std::string &get_shell_cmd_output() const {
        return m_payload->shell().output();
    }

private:
    axcl::runtime::system::response::Control *m_payload = {nullptr};
};

}  // namespace axcl::pkg::host
