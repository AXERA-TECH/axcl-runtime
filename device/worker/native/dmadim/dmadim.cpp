/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "dmadim.hpp"
#include "dispatcher.hpp"
#include "axcl_logger.hpp"

#define TAG "dmadim"

namespace axcl::worker {

dmadim::dmadim(dispatcher *dispatcher) : base(dispatcher), m_api(TAG) {
    m_api.append({
        NATIVE_DMADIM_API_APPEND(AX_DMADIM_Open),
        NATIVE_DMADIM_API_APPEND(AX_DMADIM_Cfg),
        NATIVE_DMADIM_API_APPEND(AX_DMADIM_Start),
        NATIVE_DMADIM_API_APPEND(AX_DMADIM_Waitdone),
        NATIVE_DMADIM_API_APPEND(AX_DMADIM_Close),
        NATIVE_DMADIM_API_APPEND(AX_DMA_MemCopy),
        NATIVE_DMADIM_API_APPEND(AX_DMA_MemSet),
        NATIVE_DMADIM_API_APPEND(AX_DMA_MemCopyXD),
        NATIVE_DMADIM_API_APPEND(AX_DMA_CheckSum),
    });
}

void dmadim::run(const void *data, const size_t& size) {
    m_api.run(get_dispatcher(), data, size);
}

NATIVE_DMADIM_API_IMPL(AX_DMADIM_Open) {
    AX_BOOL bSync;

    if (!response->unpack(response->get_api(), &bSync)) {
        return false;
    }

    AX_S32 chn = ::AX_DMADIM_Open(bSync);

    if (!request->pack(response->get_api(), chn)) {
        return false;
    }

    return true;
}

NATIVE_DMADIM_API_IMPL(AX_DMADIM_Cfg) {
    AX_S32 s32DmaChn;
    AX_DMADIM_MSG_T DmaMsg;
    axcl::pkg::uint8_array desc;

    if (!response->unpack(response->get_api(), &s32DmaChn, &DmaMsg, &desc)) {
        return false;
    }

    DmaMsg.pDescBuf = (AX_VOID *)desc.data;

    AX_S32 ret = ::AX_DMADIM_Cfg(s32DmaChn, &DmaMsg);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_DMADIM_API_IMPL(AX_DMADIM_Start) {
    AX_S32 s32DmaChn;
    AX_S32 s32Id;

    if (!response->unpack(response->get_api(), &s32DmaChn, &s32Id)) {
        return false;
    }

    AX_S32 ret = ::AX_DMADIM_Start(s32DmaChn, s32Id);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_DMADIM_API_IMPL(AX_DMADIM_Waitdone) {
    AX_S32 s32DmaChn;
    AX_S32 s32Timeout;

    if (!response->unpack(response->get_api(), &s32DmaChn, &s32Timeout)) {
        return false;
    }

    AX_DMADIM_XFER_STAT_T XferStat;
    AX_S32 ret = ::AX_DMADIM_Waitdone(s32DmaChn, &XferStat, s32Timeout);

    if (!request->pack(response->get_api(), ret, &XferStat)) {
        return false;
    }

    return true;
}

NATIVE_DMADIM_API_IMPL(AX_DMADIM_Close) {
    AX_S32 s32DmaChn;

    if (!response->unpack(response->get_api(), &s32DmaChn)) {
        return false;
    }

    AX_S32 ret = ::AX_DMADIM_Close(s32DmaChn);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_DMADIM_API_IMPL(AX_DMA_MemCopy) {
    AX_U64 u64PhyDst;
    AX_U64 u64PhySrc;
    AX_U64 U64Size;

    if (!response->unpack(response->get_api(), &u64PhyDst, &u64PhySrc, &U64Size)) {
        return false;
    }

    AX_S32 ret = ::AX_DMA_MemCopy(u64PhyDst, u64PhySrc, U64Size);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_DMADIM_API_IMPL(AX_DMA_MemSet) {
    AX_U64 u64PhyDst;
    AX_U8 u8InitVal;
    AX_U64 U64Size;

    if (!response->unpack(response->get_api(), &u64PhyDst, &u8InitVal, &U64Size)) {
        return false;
    }

    AX_S32 ret = ::AX_DMA_MemSet(u64PhyDst, u8InitVal, U64Size);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_DMADIM_API_IMPL(AX_DMA_MemCopyXD) {
    AX_DMADIM_DESC_XD_T tDimDesc;
    AX_DMADIM_XFER_MODE_E eMode;

    if (!response->unpack(response->get_api(), &tDimDesc, &eMode)) {
        return false;
    }

    AX_S32 ret = ::AX_DMA_MemCopyXD(tDimDesc, eMode);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_DMADIM_API_IMPL(AX_DMA_CheckSum) {
    AX_U64 u64PhyDst;
    AX_U64 U64Size;

    if (!response->unpack(response->get_api(), &u64PhyDst, &U64Size)) {
        return false;
    }

    AX_U32 u32Result;
    AX_S32 ret = ::AX_DMA_CheckSum(&u32Result, u64PhyDst, U64Size);

    if (!request->pack(response->get_api(), ret, u32Result)) {
        return false;
    }

    return true;
}

}  // namespace axcl::worker
