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

enum class DMADIM_API : int32_t {
    AX_DMADIM_Open = 0,
    AX_DMADIM_Cfg = 1,
    AX_DMADIM_Start = 2,
    AX_DMADIM_Waitdone = 3,
    AX_DMADIM_Close = 4,
    AX_DMA_MemCopy = 5,
    AX_DMA_MemSet = 6,
    AX_DMA_MemCopyXD = 7,
    AX_DMA_CheckSum = 8
};

}  // namespace axcl::pkg::native