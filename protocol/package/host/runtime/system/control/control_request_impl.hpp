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

#include "enum_cast.hpp"
#include "protocol.hpp"

namespace axcl::pkg::host {

struct control_request::impl {
    impl() = delete;
    explicit impl(void* payload) : m_payload(static_cast<axcl::runtime::system::request::Control*>(payload)) {
    }

    void create_context(uint32_t pid, uint32_t tid, uint32_t context, uint32_t stream) {
        m_payload->set_cmd(axcl::runtime::system::Command::CREATE_CONTEXT);
        m_payload->set_pid(pid);
        m_payload->set_tid(tid);
        m_payload->set_context(context);
        m_payload->set_stream(stream);
        m_payload->set_noresp(0);
    }

    void destroy_context(uint32_t pid, uint32_t tid, uint32_t context, uint32_t stream) {
        m_payload->set_cmd(axcl::runtime::system::Command::DESTROY_CONTEXT);
        m_payload->set_pid(pid);
        m_payload->set_tid(tid);
        m_payload->set_context(context);
        m_payload->set_stream(stream);
        m_payload->set_noresp(0);
    }

    void create_stream(uint32_t context, uint32_t stream) {
        m_payload->set_cmd(axcl::runtime::system::Command::CREATE_STREAM);
        m_payload->set_context(context);
        m_payload->set_stream(stream);
        m_payload->set_noresp(0);
    }

    void destroy_stream(uint32_t context, uint32_t stream) {
        m_payload->set_cmd(axcl::runtime::system::Command::DESTROY_STREAM);
        m_payload->set_context(context);
        m_payload->set_stream(stream);
        m_payload->set_noresp(0);
    }

    void sync_stream(uint32_t context, uint32_t stream, int32_t timeout) {
        m_payload->set_cmd(axcl::runtime::system::Command::SYNC_STREAM);
        m_payload->set_context(context);
        m_payload->set_stream(stream);
        m_payload->set_timeout(timeout);
        m_payload->set_noresp(0);
    }

    void create_event(uint32_t context, uint32_t stream, uint32_t event) {
        m_payload->set_cmd(axcl::runtime::system::Command::CREATE_EVENT);
        m_payload->set_context(context);
        m_payload->set_stream(stream);
        m_payload->set_event(event);
        m_payload->set_noresp(0);
    }

    void destroy_event(uint32_t context, uint32_t stream, uint32_t event) {
        m_payload->set_cmd(axcl::runtime::system::Command::DESTROY_EVENT);
        m_payload->set_context(context);
        m_payload->set_stream(stream);
        m_payload->set_event(event);
        m_payload->set_noresp(0);
    }

    void record_event(uint32_t context, uint32_t stream, uint32_t event) {
        m_payload->set_cmd(axcl::runtime::system::Command::RECORD_EVENT);
        m_payload->set_context(context);
        m_payload->set_stream(stream);
        m_payload->set_event(event);
        m_payload->set_noresp(1);
    }

    void wait_event(uint32_t context, uint32_t stream, uint32_t event, int32_t timeout) {
        m_payload->set_cmd(axcl::runtime::system::Command::WAIT_EVENT);
        m_payload->set_context(context);
        m_payload->set_stream(stream);
        m_payload->set_event(event);
        m_payload->set_timeout(timeout);
        m_payload->set_noresp(1);
    }

    void set_shell_cmd(const char* cmd, const char* const args[], size_t argc) {
        m_payload->set_cmd(axcl::runtime::system::Command::SHELL);
        m_payload->set_noresp(0);
        auto shell = m_payload->mutable_shell();
        shell->set_cmd(cmd);

        shell->clear_args();
        for (size_t i = 0; i < argc; ++i) {
            shell->add_args(args[i]);
        }
    }

    void set_pcie_sub_id(uint32_t sub_vendor_id, uint32_t sub_device_id) {
        m_payload->set_cmd(axcl::runtime::system::Command::SETUP_PCIE_SUB_ID);
        m_payload->set_noresp(0);
        auto sub = m_payload->mutable_pcie_sub_id();
        sub->set_vendor_id(sub_vendor_id);
        sub->set_device_id(sub_device_id);
    }

    axcl::pkg::runtime::system::command get_cmd() const {
        return axcl::enum_cast<axcl::runtime::system::Command, axcl::pkg::runtime::system::command>(m_payload->cmd());
    }

private:
    axcl::runtime::system::request::Control* m_payload;
};

}  // namespace axcl::pkg::host
