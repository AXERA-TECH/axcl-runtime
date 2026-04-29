/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "token.hpp"
#include "axcl_base.h"
#include "axcl_module_version.h"
#include "axcl_rt_token.h"

static axcl::token g_token;

AXCL_EXPORT uint64_t axclrtGetToken() {
    return g_token.gen();
}
