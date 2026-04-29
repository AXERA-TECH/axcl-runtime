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

#include "package/device/runtime/system/system_request.hpp"

namespace axcl::pkg::device {

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
    void sync_stream(uint32_t context, uint32_t stream, int32_t status) const;

    void create_event(uint32_t context, uint32_t stream, uint32_t event) const;
    void destroy_event(uint32_t context, uint32_t stream, uint32_t event) const;
    void record_event(uint32_t context, uint32_t stream, uint32_t event, int32_t status) const;
    void wait_event(uint32_t context, uint32_t stream, uint32_t event, int32_t status) const;

    void set_shell_cmd_output(int32_t status, const char* output) const;

    void set_pcie_sub_id_setup_status(int32_t status) const;

    axcl::pkg::runtime::system::command get_cmd() const;

private:
    struct impl;
    impl* m_impl;
};

}  // namespace axcl::pkg::device
