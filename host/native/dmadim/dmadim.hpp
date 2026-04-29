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

#include "axcl_dmadim_type.h"
#include "native.hpp"
#include "package/host/native/module/dmadim_request.hpp"
#include "package/host/native/module/dmadim_response.hpp"

#define AXCL_DEF_DMADIM_ERR(e) AXCL_DEF_NATIVE_ERR(static_cast<uint8_t>(axcl::pkg::native::module::DMADIM), (e))

class dmadim : public native<axcl::pkg::host::dmadim_request, axcl::pkg::host::dmadim_response> {
public:
    dmadim() : native<axcl::pkg::host::dmadim_request, axcl::pkg::host::dmadim_response>(axcl::pkg::native::module::DMADIM) {
    }

    AX_S32 AX_DMADIM_Open(AX_BOOL bSync);
    AX_S32 AX_DMADIM_Cfg(AX_S32 s32DmaChn, AX_DMADIM_MSG_T *pDmaMsg);
    AX_S32 AX_DMADIM_Start(AX_S32 s32DmaChn, AX_S32 s32Id);
    AX_S32 AX_DMADIM_Waitdone(AX_S32 s32DmaChn, AX_DMADIM_XFER_STAT_T *pXferStat, AX_S32 s32Timeout);
    AX_S32 AX_DMADIM_Close(AX_S32 s32DmaChn);

    AX_S32 AX_DMA_MemCopy(AX_U64 u64PhyDst, AX_U64 u64PhySrc, AX_U64 U64Size);
    AX_S32 AX_DMA_MemSet(AX_U64 u64PhyDst, AX_U8 u8InitVal, AX_U64 U64Size);
    AX_S32 AX_DMA_MemCopyXD(AX_DMADIM_DESC_XD_T tDimDesc, AX_DMADIM_XFER_MODE_E eMode);
    AX_S32 AX_DMA_CheckSum(AX_U32 *u32Result, AX_U64 u64PhySrc, AX_U64 U64Size);
};
