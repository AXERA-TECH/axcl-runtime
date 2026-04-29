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

namespace axcl::pkg::device {

struct control_request::impl {
    impl() = delete;
    explicit impl(void* payload) : m_payload(static_cast<axcl::runtime::system::response::Control*>(payload)) {
    }

    void create_context(uint32_t pid, uint32_t tid, uint32_t context, uint32_t stream) {
        m_payload->set_cmd(axcl::runtime::system::Command::CREATE_CONTEXT);
        m_payload->set_pid(pid);
        m_payload->set_tid(tid);
        m_payload->set_context(context);
        m_payload->set_stream(stream);
    }

    void destroy_context(uint32_t pid, uint32_t tid, uint32_t context, uint32_t stream) {
        m_payload->set_cmd(axcl::runtime::system::Command::DESTROY_CONTEXT);
        m_payload->set_pid(pid);
        m_payload->set_tid(tid);
        m_payload->set_context(context);
        m_payload->set_stream(stream);
    }

    void create_stream(uint32_t context, uint32_t stream) {
        m_payload->set_cmd(axcl::runtime::system::Command::CREATE_STREAM);
        m_payload->set_context(context);
        m_payload->set_stream(stream);
    }

    void destroy_stream(uint32_t context, uint32_t stream) {
        m_payload->set_cmd(axcl::runtime::system::Command::DESTROY_STREAM);
        m_payload->set_context(context);
        m_payload->set_stream(stream);
    }

    void sync_stream(uint32_t context, uint32_t stream, int32_t status) {
        m_payload->set_cmd(axcl::runtime::system::Command::SYNC_STREAM);
        m_payload->set_context(context);
        m_payload->set_stream(stream);
        m_payload->set_status(status);
    }

    void create_event(uint32_t context, uint32_t stream, uint32_t event) {
        m_payload->set_cmd(axcl::runtime::system::Command::CREATE_EVENT);
        m_payload->set_context(context);
        m_payload->set_stream(stream);
        m_payload->set_event(event);
    }

    void destroy_event(uint32_t context, uint32_t stream, uint32_t event) {
        m_payload->set_cmd(axcl::runtime::system::Command::DESTROY_EVENT);
        m_payload->set_context(context);
        m_payload->set_stream(stream);
        m_payload->set_event(event);
    }

    void record_event(uint32_t context, uint32_t stream, uint32_t event, int32_t status) {
        m_payload->set_cmd(axcl::runtime::system::Command::RECORD_EVENT);
        m_payload->set_context(context);
        m_payload->set_stream(stream);
        m_payload->set_event(event);
        m_payload->set_status(status);
    }

    void wait_event(uint32_t context, uint32_t stream, uint32_t event, int32_t status) {
        m_payload->set_cmd(axcl::runtime::system::Command::WAIT_EVENT);
        m_payload->set_context(context);
        m_payload->set_stream(stream);
        m_payload->set_event(event);
        m_payload->set_status(status);
    }

    void set_shell_cmd_output(int32_t status, const char* output) {
        m_payload->set_cmd(axcl::runtime::system::Command::SHELL);
        auto shell = m_payload->mutable_shell();
        shell->set_status(status);
        shell->set_output(output ? output : "");
    }

    void set_pcie_sub_id_setup_status(int32_t status) {
        m_payload->set_cmd(axcl::runtime::system::Command::SETUP_PCIE_SUB_ID);
        m_payload->set_status(status);
    }

    axcl::pkg::runtime::system::command get_cmd() const {
        return axcl::enum_cast<axcl::runtime::system::Command, axcl::pkg::runtime::system::command>(m_payload->cmd());
    }

private:
    axcl::runtime::system::response::Control* m_payload;
};

}  // namespace axcl::pkg::device
