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

#include "axcl_rt_type.h"
#include "axcl_rt_type_internal.h"
#include "package/host/runtime/system/control/control_request.hpp"
#include "package/host/runtime/system/control/control_response.hpp"

namespace axcl::rt::system {

class control {
public:
    control() = default;

    axclError create_context(int32_t context_id);
    axclError destroy_context(int32_t context_id);

    axclError create_stream(int32_t context_id, axclrtStream stream);
    axclError destroy_stream(int32_t context_id, axclrtStream stream);
    axclError sync_stream(int32_t context_id, axclrtStream stream, int32_t timeout_ms);

    axclError create_event(int32_t event_id);
    axclError destroy_event(int32_t event_id);
    axclError record_event(axclrtStream stream, int32_t event_id);
    axclError stream_wait_event(axclrtStream stream, int32_t event_id, int32_t timeout_ms);

    axclError execute_shell_cmd(const char *cmd, const char *const args[], size_t argc, const char **output, int32_t timeout);

    axclError change_pcie_sub_id(uint32_t sub_vendor_id, uint32_t sub_device_id);

protected:
    axclError get_runtime_environment(axclrtStream &stream, axclrtEnvironment &env);
    axclError invoke(axclrtStream stream, const axclrtEnvironment &env, uint32_t control_cmd, int32_t timeout, bool has_resp=true);

private:
    axcl::pkg::host::control_request m_req;
    axcl::pkg::host::control_response m_rsp;
};

}  // namespace axcl::rt::system