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

struct meta {
    uint32_t layout = 0;
    uint32_t memory = 0;
    uint32_t format = 0;
    uint32_t color = 0;
    uint32_t size = 0;
    uint32_t dim = 0;
    uint32_t shape[8] = {};
    uint32_t stride[8] = {};
};
