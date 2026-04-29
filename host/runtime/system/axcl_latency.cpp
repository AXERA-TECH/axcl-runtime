/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "axcl_rt_latency_internal.h"
#include "latency.hpp"

AXCL_EXPORT axclError axclrtLatencyLoopback(const void *data, uint32_t size, bool check, int comm_mode) {
    axcl::rt::system::latency api;
    return api.loopback(data, size, check, comm_mode);
}
