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
#include "axcl_module_version.h"
#include "axcl_vdec.h"
#include "axcl_logger.hpp"
#include "vdec.hpp"

#define TAG "vdec"
#define CHECK_NULL_POINTER(p)                            \
    if (!(p)) {                                          \
        LOG_MM_E(TAG, "nil pointer");                    \
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_NULL_POINTER); \
    }

AXCL_EXPORT AX_S32 AXCL_VDEC_Init(const AX_VDEC_MOD_ATTR_T *pstModAttr) {
    /**
     * AX_VDEC_Init(NULL) is allowed, use u32MaxGroupCount to work around
     *  - if pstModAttr is NULL, u32MaxGroupCount is set to AX_VDEC_MAX_GRP_NUM + 1
     *  - if pstModAttr is not NULL, check pstModAttr->u32MaxGroupCount is greater than AX_VDEC_MAX_GRP_NUM, return error
     */
    AX_VDEC_MOD_ATTR_T attr;
    if (!pstModAttr) {
        attr.u32MaxGroupCount = AX_VDEC_MAX_GRP_NUM + 1;
    } else {
        if (pstModAttr->u32MaxGroupCount > AX_VDEC_MAX_GRP_NUM) {
            LOG_MM_E(TAG, "Illegal u32MaxGroupCount: {} > {}", pstModAttr->u32MaxGroupCount, AX_VDEC_MAX_GRP_NUM);
            return AX_ERR_VDEC_ILLEGAL_PARAM;
        }

        attr = *pstModAttr;
    }
    vdec api;
    return api.AX_VDEC_Init(&attr);
}

AXCL_EXPORT AX_S32 AXCL_VDEC_Deinit(AX_VOID) {
    vdec api;
    return api.AX_VDEC_Deinit();
}

AXCL_EXPORT AX_S32 AXCL_VDEC_ExtractStreamHeaderInfo(const AX_VDEC_STREAM_T *pstStreamBuf, AX_PAYLOAD_TYPE_E enVideoType,
                                                     AX_VDEC_BITSTREAM_INFO_T *pstBitStreamInfo) {
    CHECK_NULL_POINTER(pstStreamBuf)
    CHECK_NULL_POINTER(pstBitStreamInfo)

    vdec api;
    return api.AX_VDEC_ExtractStreamHeaderInfo(pstStreamBuf, enVideoType, pstBitStreamInfo);
}

AXCL_EXPORT AX_S32 AXCL_VDEC_CreateGrp(AX_VDEC_GRP VdGrp, const AX_VDEC_GRP_ATTR_T *pstGrpAttr) {
    CHECK_NULL_POINTER(pstGrpAttr)

    vdec api;
    return api.AX_VDEC_CreateGrp(VdGrp, pstGrpAttr);
}

AXCL_EXPORT AX_S32 AXCL_VDEC_CreateGrpEx(AX_VDEC_GRP *VdGrp, const AX_VDEC_GRP_ATTR_T *pstGrpAttr) {
    CHECK_NULL_POINTER(pstGrpAttr)
    CHECK_NULL_POINTER(VdGrp)

    vdec api;
    return api.AX_VDEC_CreateGrpEx(VdGrp, pstGrpAttr);
}

AXCL_EXPORT AX_S32 AXCL_VDEC_DestroyGrp(AX_VDEC_GRP VdGrp) {
    vdec api;
    return api.AX_VDEC_DestroyGrp(VdGrp);
}

AXCL_EXPORT AX_S32 AXCL_VDEC_GetGrpAttr(AX_VDEC_GRP VdGrp, AX_VDEC_GRP_ATTR_T *pstGrpAttr) {
    CHECK_NULL_POINTER(pstGrpAttr)

    vdec api;
    return api.AX_VDEC_GetGrpAttr(VdGrp, pstGrpAttr);
}

AXCL_EXPORT AX_S32 AXCL_VDEC_SetGrpAttr(AX_VDEC_GRP VdGrp, const AX_VDEC_GRP_ATTR_T *pstGrpAttr) {
    CHECK_NULL_POINTER(pstGrpAttr)

    vdec api;
    return api.AX_VDEC_SetGrpAttr(VdGrp, pstGrpAttr);
}

AXCL_EXPORT AX_S32 AXCL_VDEC_StartRecvStream(AX_VDEC_GRP VdGrp, const AX_VDEC_RECV_PIC_PARAM_T *pstRecvParam) {
    CHECK_NULL_POINTER(pstRecvParam)

    vdec api;
    return api.AX_VDEC_StartRecvStream(VdGrp, pstRecvParam);
}

AXCL_EXPORT AX_S32 AXCL_VDEC_StopRecvStream(AX_VDEC_GRP VdGrp) {
    vdec api;
    return api.AX_VDEC_StopRecvStream(VdGrp);
}

AXCL_EXPORT AX_S32 AXCL_VDEC_QueryStatus(AX_VDEC_GRP VdGrp, AX_VDEC_GRP_STATUS_T *pstGrpStatus) {
    CHECK_NULL_POINTER(pstGrpStatus)

    vdec api;
    return api.AX_VDEC_QueryStatus(VdGrp, pstGrpStatus);
}

AXCL_EXPORT AX_S32 AXCL_VDEC_ResetGrp(AX_VDEC_GRP VdGrp) {
    vdec api;
    return api.AX_VDEC_ResetGrp(VdGrp);
}

AXCL_EXPORT AX_S32 AXCL_VDEC_SetGrpParam(AX_VDEC_GRP VdGrp, const AX_VDEC_GRP_PARAM_T *pstGrpParam) {
    CHECK_NULL_POINTER(pstGrpParam)

    vdec api;
    return api.AX_VDEC_SetGrpParam(VdGrp, pstGrpParam);
}

AXCL_EXPORT AX_S32 AXCL_VDEC_GetGrpParam(AX_VDEC_GRP VdGrp, AX_VDEC_GRP_PARAM_T *pstGrpParam) {
    CHECK_NULL_POINTER(pstGrpParam)

    vdec api;
    return api.AX_VDEC_GetGrpParam(VdGrp, pstGrpParam);
}

AXCL_EXPORT AX_S32 AXCL_VDEC_SelectGrp(AX_VDEC_GRP_SET_INFO_T *pstGrpSet, AX_S32 s32MilliSec) {
    CHECK_NULL_POINTER(pstGrpSet)

    vdec api;
    return api.AX_VDEC_SelectGrp(pstGrpSet, s32MilliSec);
}

AXCL_EXPORT AX_S32 AXCL_VDEC_SendStream(AX_VDEC_GRP VdGrp, const AX_VDEC_STREAM_T *pstStream, AX_S32 s32MilliSec) {
    CHECK_NULL_POINTER(pstStream)

    vdec api;
    return api.AX_VDEC_SendStream(VdGrp, pstStream, s32MilliSec);
}

AXCL_EXPORT AX_S32 AXCL_VDEC_GetChnFrame(AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn, AX_VIDEO_FRAME_INFO_T *pstFrameInfo, AX_S32 s32MilliSec) {
    CHECK_NULL_POINTER(pstFrameInfo)

    vdec api;
    return api.AX_VDEC_GetChnFrame(VdGrp, VdChn, pstFrameInfo, s32MilliSec);
}

AXCL_EXPORT AX_S32 AXCL_VDEC_ReleaseChnFrame(AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn, const AX_VIDEO_FRAME_INFO_T *pstFrameInfo) {
    CHECK_NULL_POINTER(pstFrameInfo)

    vdec api;
    return api.AX_VDEC_ReleaseChnFrame(VdGrp, VdChn, pstFrameInfo);
}

AXCL_EXPORT AX_S32 AXCL_VDEC_GetUserData(AX_VDEC_GRP VdGrp, AX_VDEC_USERDATA_T *pstUserData) {
    CHECK_NULL_POINTER(pstUserData)

    vdec api;
    return api.AX_VDEC_GetUserData(VdGrp, pstUserData);
}

AXCL_EXPORT AX_S32 AXCL_VDEC_ReleaseUserData(AX_VDEC_GRP VdGrp, const AX_VDEC_USERDATA_T *pstUserData) {
    CHECK_NULL_POINTER(pstUserData)

    vdec api;
    return api.AX_VDEC_ReleaseUserData(VdGrp, pstUserData);
}

AXCL_EXPORT AX_S32 AXCL_VDEC_SetUserPic(AX_VDEC_GRP VdGrp, const AX_VDEC_USRPIC_T *pstUsrPic) {
    CHECK_NULL_POINTER(pstUsrPic)

    vdec api;
    return api.AX_VDEC_SetUserPic(VdGrp, pstUsrPic);
}

AXCL_EXPORT AX_S32 AXCL_VDEC_EnableUserPic(AX_VDEC_GRP VdGrp) {
    vdec api;
    return api.AX_VDEC_EnableUserPic(VdGrp);
}

AXCL_EXPORT AX_S32 AXCL_VDEC_DisableUserPic(AX_VDEC_GRP VdGrp) {
    vdec api;
    return api.AX_VDEC_DisableUserPic(VdGrp);
}

AXCL_EXPORT AX_S32 AXCL_VDEC_SetDisplayMode(AX_VDEC_GRP VdGrp, AX_VDEC_DISPLAY_MODE_E enDisplayMode) {
    vdec api;
    return api.AX_VDEC_SetDisplayMode(VdGrp, enDisplayMode);
}

AXCL_EXPORT AX_S32 AXCL_VDEC_GetDisplayMode(AX_VDEC_GRP VdGrp, AX_VDEC_DISPLAY_MODE_E *penDisplayMode) {
    CHECK_NULL_POINTER(penDisplayMode)

    vdec api;
    return api.AX_VDEC_GetDisplayMode(VdGrp, penDisplayMode);
}

AXCL_EXPORT AX_S32 AXCL_VDEC_AttachPool(AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn, AX_POOL PoolId) {
    vdec api;
    return api.AX_VDEC_AttachPool(VdGrp, VdChn, PoolId);
}

AXCL_EXPORT AX_S32 AXCL_VDEC_DetachPool(AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn) {
    vdec api;
    return api.AX_VDEC_DetachPool(VdGrp, VdChn);
}

AXCL_EXPORT AX_S32 AXCL_VDEC_EnableChn(AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn) {
    vdec api;
    return api.AX_VDEC_EnableChn(VdGrp, VdChn);
}

AXCL_EXPORT AX_S32 AXCL_VDEC_DisableChn(AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn) {
    vdec api;
    return api.AX_VDEC_DisableChn(VdGrp, VdChn);
}

AXCL_EXPORT AX_S32 AXCL_VDEC_SetChnAttr(AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn, const AX_VDEC_CHN_ATTR_T *pstVdChnAttr) {
    CHECK_NULL_POINTER(pstVdChnAttr)

    vdec api;
    return api.AX_VDEC_SetChnAttr(VdGrp, VdChn, pstVdChnAttr);
}

AXCL_EXPORT AX_S32 AXCL_VDEC_GetChnAttr(AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn, AX_VDEC_CHN_ATTR_T *pstVdChnAttr) {
    CHECK_NULL_POINTER(pstVdChnAttr)

    vdec api;
    return api.AX_VDEC_GetChnAttr(VdGrp, VdChn, pstVdChnAttr);
}

AXCL_EXPORT AX_S32 AXCL_VDEC_JpegDecodeOneFrame(AX_VDEC_DEC_ONE_FRM_T *pstParam) {
    CHECK_NULL_POINTER(pstParam)

    vdec api;
    return api.AX_VDEC_JpegDecodeOneFrame(pstParam);
}

AXCL_EXPORT AX_S32 AXCL_VDEC_GetStreamBufInfo(AX_VDEC_GRP VdGrp, AX_VDEC_STREAM_BUF_INFO_T *pstStreamBufInfo) {
    CHECK_NULL_POINTER(pstStreamBufInfo)

    vdec api;
    return api.AX_VDEC_GetStreamBufInfo(VdGrp, pstStreamBufInfo);
}

AXCL_EXPORT AX_S32 AXCL_VDEC_GetVuiParam(AX_VDEC_GRP VdGrp, AX_VDEC_VUI_PARAM_T *pstVuiParam) {
    CHECK_NULL_POINTER(pstVuiParam)

    vdec api;
    return api.AX_VDEC_GetVuiParam(VdGrp, pstVuiParam);
}
