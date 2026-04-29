/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include <stdlib.h>
#include "axcl_rt_log_internal.h"
#include "axcl_logger.hpp"
#include "system/log.hpp"

#define TAG "log"

AXCL_EXPORT axclError axclrtLog(axclrtLogKind kind, axclrtFlagKind flag, const char* output_path) {
    axcl::rt::system::log api;

    return api.dump(kind, flag, output_path);
}
