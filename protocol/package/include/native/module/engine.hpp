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

namespace axcl::pkg::native {

enum class ENGINE_API : int32_t {
    AX_ENGINE_GetVersion = 0,
    AX_ENGINE_NPUReset = 1,
    AX_ENGINE_Init = 2,
    AX_ENGINE_GetVNPUAttr = 3,
    AX_ENGINE_Deinit = 4,
    AX_ENGINE_GetModelType = 5,
    AX_ENGINE_CreateHandle = 6,
    AX_ENGINE_CreateHandleV2 = 7,
    AX_ENGINE_DestroyHandle = 8,
    AX_ENGINE_GetIOInfo = 9,
    AX_ENGINE_GetGroupIOInfoCount = 10,
    AX_ENGINE_GetGroupIOInfo = 11,
    AX_ENGINE_GetHandleModelType = 12,
    AX_ENGINE_CreateContext = 13,
    AX_ENGINE_CreateContextV2 = 14,
    AX_ENGINE_RunSync = 15,
    AX_ENGINE_RunSyncV2 = 16,
    AX_ENGINE_RunGroupIOSync = 17,
    AX_ENGINE_SetAffinity = 18,
    AX_ENGINE_GetAffinity = 19,
    AX_ENGINE_GetCMMUsage = 20,
    AX_ENGINE_GetModelToolsVersion = 21,
};

}  // namespace axcl::pkg::native