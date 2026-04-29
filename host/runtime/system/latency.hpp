/**************************************************************************************************
 *
 * Copyright (c) 2019-2025 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#pragma once

#include "axcl_rt_type.h"
#include "axcl_rt_type_internal.h"
#include "package/host/runtime/system/latency/latency_request.hpp"
#include "package/host/runtime/system/latency/latency_response.hpp"

namespace axcl::rt::system {

class latency {
public:
    latency() = default;

    axclError loopback(const void* data, uint32_t size, bool check, int comm_mode = -1);

protected:
    axclError invoke(int32_t timeout, int comm_mode = -1);

private:
    axcl::pkg::host::latency_request m_req;
    axcl::pkg::host::latency_response m_rsp;
};

}