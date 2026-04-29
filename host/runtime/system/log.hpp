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
#include "package/host/runtime/system/log/log_request.hpp"
#include "package/host/runtime/system/log/log_response.hpp"

namespace axcl::rt::system {

class log {
public:
    log() = default;

    axclError dump(axclrtLogKind kind, axclrtFlagKind flag, const char* output_path);

protected:
    axclError get_runtime_environment(axclrtStream &stream, axclrtEnvironment &env);
    axclError invoke(axclrtStream stream, const axclrtEnvironment &env, int32_t timeout);

private:
    axcl::pkg::host::log_request m_req;
    axcl::pkg::host::log_response m_rsp;
};

}  // namespace axcl::rt::system