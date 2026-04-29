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

#include <cstdint>

namespace axcl::pkg::runtime::system {

enum class module : int32_t {
    ALIVE = 0,
    LATENCY = 1,
    LOG = 2,
    CONTROL = 3,
    UNKNOWN
};

enum class command : int32_t {
    CREATE_CONTEXT = 0,
    DESTROY_CONTEXT = 1,
    SHELL = 2,
    SETUP_PCIE_SUB_ID = 3,
    CREATE_STREAM = 4,
    DESTROY_STREAM = 5,
    SYNC_STREAM = 6,
    CREATE_EVENT = 7,
    DESTROY_EVENT = 8,
    RECORD_EVENT = 9,
    WAIT_EVENT = 10,
    UNKNOWN
};

}  // namespace axcl::pkg::runtime