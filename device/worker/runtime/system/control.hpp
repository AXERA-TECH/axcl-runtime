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

#include <string>
#include <mutex>
#include "package/device/runtime/system/control/control_request.hpp"
#include "package/device/runtime/system/control/control_response.hpp"
#include "../base/base.hpp"
#include "system_api.hpp"

#define SYSTEM_CONTROL_API_DECL(_API_) \
            SYSTEM_API_DECL(_API_, SYSTEM_API_PARAM(control))

#define SYSTEM_CONTROL_API_IMPL(_API_) \
            SYSTEM_API_IMPL(control, _API_, SYSTEM_API_PARAM(control))

#define SYSTEM_CONTROL_API_APPEND(_API_) \
            SYSTEM_API_APPEND(control, _API_, SYSTEM_API_PARAM(control))

namespace axcl::worker {

class stream;
class dispatcher;
class control : public base {
public:
    control(dispatcher *dispatcher);
    ~control() = default;

    virtual void run(const void *data, const size_t& size) override;

    bool on_context_create(uint32_t context_id, uint32_t stream_id);
    bool on_context_destroy(uint32_t context_id, uint32_t stream_id);
    bool on_stream_create(uint32_t context_id, uint32_t stream_id);
    bool on_stream_destroy(uint32_t context_id, uint32_t stream_id);
    bool on_event_create(uint32_t context_id, uint32_t stream_id, uint32_t event_id);
    bool on_event_destroy(uint32_t context_id, uint32_t stream_id, uint32_t event_id);
    stream* get_stream(uint32_t context_id, uint32_t stream_id, int& error_code);

private:
    SYSTEM_CONTROL_API_DECL(CREATE_CONTEXT);
    SYSTEM_CONTROL_API_DECL(DESTROY_CONTEXT);

    SYSTEM_CONTROL_API_DECL(CREATE_STREAM);
    SYSTEM_CONTROL_API_DECL(DESTROY_STREAM);
    SYSTEM_CONTROL_API_DECL(SYNC_STREAM);

    SYSTEM_CONTROL_API_DECL(CREATE_EVENT);
    SYSTEM_CONTROL_API_DECL(DESTROY_EVENT);
    SYSTEM_CONTROL_API_DECL(RECORD_EVENT);
    SYSTEM_CONTROL_API_DECL(WAIT_EVENT);

    SYSTEM_CONTROL_API_DECL(SHELL);
    SYSTEM_CONTROL_API_DECL(SETUP_PCIE_SUB_ID);

private:
    int32_t execute_shell(const char* cmd, std::string& output);

    /* save ep vendor id and device id to nor flash */
    int32_t write_pcie_sub_id_to_flash(uint32_t sub_vendor_id, uint32_t sub_device_id);
    int32_t get_storage_type(uint32_t &storage);

private:
    system_api<axcl::pkg::runtime::system::command, axcl::pkg::device::control_response, axcl::pkg::device::control_request> m_api;
    std::mutex m_mtx;
};

}  // namespace axcl::worker
