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

#include "package/host/runtime/system/system_response.hpp"
#include "package/include/runtime/system/type.hpp"

namespace axcl::pkg::host {

class control_response : public system_response {
public:
    explicit control_response();
    virtual ~control_response();

    control_response(control_response&& other) noexcept;
    control_response& operator=(control_response&& other) noexcept;
    control_response(const control_response&) = delete;
    control_response& operator=(const control_response&) = delete;

    axcl::pkg::runtime::system::command get_cmd() const;
    uint32_t get_pid() const;
    uint32_t get_tid() const;
    uint32_t get_context() const;
    uint32_t get_stream() const;
    uint32_t get_event() const;
    int32_t get_status() const;

    int32_t get_pcie_sub_id_setup_status() const;

    int32_t get_shell_cmd_status() const;
    const std::string& get_shell_cmd_output() const;

    [[nodiscard]] bool decode(const void* message, uint32_t size) const override;

private:
    struct impl;
    impl* m_impl;
};

}  // namespace axcl::pkg::host