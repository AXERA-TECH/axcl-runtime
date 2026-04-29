/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "axcl_base.h"
#include "axcl_dmadim.h"
#include "axcl_module_version.h"
#include "dmadim.hpp"
#include "axcl_logger.hpp"

#define TAG "dmadim"
#define CHECK_NULL_POINTER(p)                              \
    if (!(p)) {                                            \
        LOG_MM_E(TAG, "nil pointer");                      \
        return AXCL_DEF_DMADIM_ERR(AXCL_ERR_NULL_POINTER); \
    }

AXCL_EXPORT AX_S32 AXCL_DMADIM_Open(AX_BOOL bSync) {
    dmadim api;
    return api.AX_DMADIM_Open(bSync);
}

AXCL_EXPORT AX_S32 AXCL_DMADIM_Cfg(AX_S32 s32DmaChn, AX_DMADIM_MSG_T *pDmaMsg) {
    CHECK_NULL_POINTER(pDmaMsg)

    dmadim api;
    return api.AX_DMADIM_Cfg(s32DmaChn, pDmaMsg);
}

AXCL_EXPORT AX_S32 AXCL_DMADIM_Start(AX_S32 s32DmaChn, AX_S32 s32Id) {
    dmadim api;
    return api.AX_DMADIM_Start(s32DmaChn, s32Id);
}

AXCL_EXPORT AX_S32 AXCL_DMADIM_Waitdone(AX_S32 s32DmaChn, AX_DMADIM_XFER_STAT_T *pXferStat, AX_S32 s32Timeout) {
    CHECK_NULL_POINTER(pXferStat)

    dmadim api;
    return api.AX_DMADIM_Waitdone(s32DmaChn, pXferStat, s32Timeout);
}

AXCL_EXPORT AX_S32 AXCL_DMADIM_Close(AX_S32 s32DmaChn) {
    dmadim api;
    return api.AX_DMADIM_Close(s32DmaChn);
}

AXCL_EXPORT AX_S32 AXCL_DMA_MemCopy(AX_U64 u64PhyDst, AX_U64 u64PhySrc, AX_U64 U64Size) {
    dmadim api;
    return api.AX_DMA_MemCopy(u64PhyDst, u64PhySrc, U64Size);
}

AXCL_EXPORT AX_S32 AXCL_DMA_MemSet(AX_U64 u64PhyDst, AX_U8 u8InitVal, AX_U64 U64Size) {
    dmadim api;
    return api.AX_DMA_MemSet(u64PhyDst, u8InitVal, U64Size);
}

AXCL_EXPORT AX_S32 AXCL_DMA_MemCopyXD(AX_DMADIM_DESC_XD_T tDimDesc, AX_DMADIM_XFER_MODE_E eMode) {
    dmadim api;
    return api.AX_DMA_MemCopyXD(tDimDesc, eMode);
}

AXCL_EXPORT AX_S32 AXCL_DMA_CheckSum(AX_U32 *u32Result, AX_U64 u64PhySrc, AX_U64 U64Size) {
    CHECK_NULL_POINTER(u32Result)

    dmadim api;
    return api.AX_DMA_CheckSum(u32Result, u64PhySrc, U64Size);
}
