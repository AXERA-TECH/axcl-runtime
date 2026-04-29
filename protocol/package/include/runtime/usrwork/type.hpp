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

namespace axcl::pkg::runtime::usrwork {

enum class FILE_TRANSFER_POLICY : int {
    FILE_TRANSFER_FROM_HOST_TO_DEVICE   = 0,
    FILE_TRANSFER_FROM_DEVICE_TO_HOST   = 1,
    FILE_TRANSFER_FROM_DEVICE_TO_DEVICE = 2,
    FILE_TRANSFER_REMOVE_DEVICE_FILE    = 3,
};

enum class API : int {
    TRANSFER_FILE = 0,
    EXEC_WORKER   = 1,
    KILL_WORKER   = 2,
    WORKER_SEND   = 3,
    WORKER_RECV   = 4,
    UNKNOWN,
};

}  // namespace axcl::pkg::runtime::usrwork