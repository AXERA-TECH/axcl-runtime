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

#include <vector>
#include "package/host/runtime/system/system_request.hpp"

namespace axcl::pkg::host {

class control_request : public system_request {
public:
    explicit control_request();
    virtual ~control_request();

    control_request(control_request&& other) noexcept;
    control_request& operator=(control_request&& other) noexcept;
    control_request(const control_request&) = delete;
    control_request& operator=(const control_request&) = delete;

    void create_context(uint32_t pid, uint32_t tid, uint32_t context, uint32_t stream) const;
    void destroy_context(uint32_t pid, uint32_t tid, uint32_t context, uint32_t stream) const;

    void create_stream(uint32_t context, uint32_t stream) const;
    void destroy_stream(uint32_t context, uint32_t stream) const;
    void sync_stream(uint32_t context, uint32_t stream, int32_t timeout) const;

    void create_event(uint32_t context, uint32_t stream, uint32_t event) const;
    void destroy_event(uint32_t context, uint32_t stream, uint32_t event) const;
    void record_event(uint32_t context, uint32_t stream, uint32_t event) const;
    void wait_event(uint32_t context, uint32_t stream, uint32_t event, int32_t timeout) const;

    void set_shell_cmd(const char* cmd, const char* const args[], size_t argc) const;

    void set_pcie_sub_id(uint32_t sub_vendor_id, uint32_t sub_device_id) const;

    axcl::pkg::runtime::system::command get_cmd() const;

private:
    struct impl;
    impl* m_impl;
};

}  // namespace axcl::pkg::host
