/**
 * @file axcl_rt_engine.h
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
**/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <cstdint>

#define MAX_P2P_TEST_COUNT 16u

typedef struct axclrtP2PTestRes {
    uint32_t count;
    float elapsed[MAX_P2P_TEST_COUNT];
} axclrtEngineP2pBwRes;

AXCL_EXPORT axclError axclrtEngineP2pBwTest(axclrtEngineP2pBwRes* bw, uint32_t mode, uint64_t size);

#ifdef __cplusplus
}
#endif
