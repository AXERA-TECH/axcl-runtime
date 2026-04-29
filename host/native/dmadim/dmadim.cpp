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
#include "axcl_logger.hpp"

#define TAG "dmadim"

AX_S32 dmadim::AX_DMADIM_Open(AX_BOOL bSync) {
    axcl::pkg::native::DMADIM_API api(axcl::pkg::native::DMADIM_API::AX_DMADIM_Open);
    AX_S32 ret;

    if (!m_req.pack(api, bSync)) {
        return AXCL_DEF_DMADIM_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_DMADIM_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 dmadim::AX_DMADIM_Cfg(AX_S32 s32DmaChn, AX_DMADIM_MSG_T *pDmaMsg) {
    axcl::pkg::native::DMADIM_API api(axcl::pkg::native::DMADIM_API::AX_DMADIM_Cfg);
    AX_S32 ret;

    if (pDmaMsg->pfnCallBack) {
        LOG_MM_E(TAG, "callback is not supported yet");
        return AXCL_DEF_DMADIM_ERR(AXCL_ERR_UNSUPPORT);
    }

    axcl::pkg::uint8_array desc;
    desc.data = reinterpret_cast<uint8_t *>(pDmaMsg->pDescBuf);
    if (AX_DMADIM_1D == pDmaMsg->eDmaMode || AX_DMADIM_MEMORY_INIT == pDmaMsg->eDmaMode || AX_DMADIM_CHECKSUM == pDmaMsg->eDmaMode) {
        desc.size = sizeof(AX_DMADIM_DESC_T);
    } else {
        desc.size = sizeof(AX_DMADIM_DESC_XD_T);
    }

    if (!m_req.pack(api, s32DmaChn, pDmaMsg, desc)) {
        return AXCL_DEF_DMADIM_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_DMADIM_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 dmadim::AX_DMADIM_Start(AX_S32 s32DmaChn, AX_S32 s32Id) {
    axcl::pkg::native::DMADIM_API api(axcl::pkg::native::DMADIM_API::AX_DMADIM_Start);
    AX_S32 ret;

    if (!m_req.pack(api, s32DmaChn, s32Id)) {
        return AXCL_DEF_DMADIM_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_DMADIM_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 dmadim::AX_DMADIM_Waitdone(AX_S32 s32DmaChn, AX_DMADIM_XFER_STAT_T *pXferStat, AX_S32 s32Timeout) {
    axcl::pkg::native::DMADIM_API api(axcl::pkg::native::DMADIM_API::AX_DMADIM_Waitdone);
    AX_S32 ret;

    if (!m_req.pack(api, s32DmaChn, s32Timeout)) {
        return AXCL_DEF_DMADIM_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(s32Timeout); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pXferStat)) {
        return AXCL_DEF_DMADIM_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 dmadim::AX_DMADIM_Close(AX_S32 s32DmaChn) {
    axcl::pkg::native::DMADIM_API api(axcl::pkg::native::DMADIM_API::AX_DMADIM_Close);
    AX_S32 ret;

    if (!m_req.pack(api, s32DmaChn)) {
        return AXCL_DEF_DMADIM_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_DMADIM_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 dmadim::AX_DMA_MemCopy(AX_U64 u64PhyDst, AX_U64 u64PhySrc, AX_U64 U64Size) {
    axcl::pkg::native::DMADIM_API api(axcl::pkg::native::DMADIM_API::AX_DMA_MemCopy);
    AX_S32 ret;

    if (!m_req.pack(api, u64PhyDst, u64PhySrc, U64Size)) {
        return AXCL_DEF_DMADIM_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_DMADIM_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 dmadim::AX_DMA_MemSet(AX_U64 u64PhyDst, AX_U8 u8InitVal, AX_U64 U64Size) {
    axcl::pkg::native::DMADIM_API api(axcl::pkg::native::DMADIM_API::AX_DMA_MemSet);
    AX_S32 ret;

    if (!m_req.pack(api, u64PhyDst, u8InitVal, U64Size)) {
        return AXCL_DEF_DMADIM_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_DMADIM_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 dmadim::AX_DMA_MemCopyXD(AX_DMADIM_DESC_XD_T tDimDesc, AX_DMADIM_XFER_MODE_E eMode) {
    axcl::pkg::native::DMADIM_API api(axcl::pkg::native::DMADIM_API::AX_DMA_MemCopyXD);
    AX_S32 ret;

    if (!m_req.pack(api, tDimDesc, eMode)) {
        return AXCL_DEF_DMADIM_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_DMADIM_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 dmadim::AX_DMA_CheckSum(AX_U32 *u32Result, AX_U64 u64PhySrc, AX_U64 U64Size) {
    axcl::pkg::native::DMADIM_API api(axcl::pkg::native::DMADIM_API::AX_DMA_CheckSum);
    AX_S32 ret;

    if (!m_req.pack(api, u64PhySrc, U64Size)) {
        return AXCL_DEF_DMADIM_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, u32Result)) {
        return AXCL_DEF_DMADIM_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}
