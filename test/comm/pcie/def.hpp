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

#define PCIE_PORT (15)
#define TOTAL_MSG_NUM (100)

#define NORM_DMA_BUF_SIZE (8192)
#define NORM_DMA_LOOP_CNT (100)

#define HUGE_DMA_BUF_SIZE (1024 * 1024 * 8 + 1010)
#define HUGE_DMA_LOOP_CNT (10)

#define LATENCY_LOOP_CNT (100)
#define MAX_LATENCY_MSG_LEN (1024 * 1024)


typedef enum {
    MSG_DEVICE_CMM_MEMORY = 1,
    MSG_HOST_OS_MEMORY = 2,
} MSG;

#pragma pack(push, 1)
typedef struct msg_data_t {
    MSG msg;
    uint8_t data[16];
} msg_data_t;
#pragma pack(pop)
