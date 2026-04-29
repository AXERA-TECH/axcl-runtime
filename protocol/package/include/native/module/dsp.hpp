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

enum class DSP_API : int32_t {
    AX_DSP_PowerOn = 0,
    AX_DSP_PowerOff = 1,
    AX_DSP_LoadBin = 2,
    AX_DSP_EnableCore = 3,
    AX_DSP_DisableCore = 4,
    AX_DSP_PRC = 5,
    AX_DSP_Query = 6,
};

}  // namespace axcl::pkg::native
