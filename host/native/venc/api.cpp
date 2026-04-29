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
#include "axcl_venc.h"
#include "axcl_logger.hpp"
#include "venc.hpp"

#define TAG "venc"
#define CHECK_NULL_POINTER(p)                            \
    if (!(p)) {                                          \
        LOG_MM_E(TAG, "nil pointer");                    \
        return AXCL_DEF_VENC_ERR(AXCL_ERR_NULL_POINTER); \
    }

#define CHECK_SMALLER_AND_EQUAL_UINT(threshold, actual)                                \
    if (actual <= threshold) {                                                         \
        LOG_MM_E(TAG, "invalid value, {}({}) <= ({}).\n", #actual, actual, threshold); \
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ILLEGAL_PARAM);                              \
    }

AXCL_EXPORT AX_S32 AXCL_VENC_Init(const AX_VENC_MOD_ATTR_T *pstModAttr) {
    CHECK_NULL_POINTER(pstModAttr)

    venc api;
    return api.AX_VENC_Init(pstModAttr);
}

AXCL_EXPORT AX_S32 AXCL_VENC_Deinit() {
    venc api;
    return api.AX_VENC_Deinit();
}

AXCL_EXPORT AX_S32 AXCL_VENC_CreateChn(VENC_CHN VeChn, const AX_VENC_CHN_ATTR_T *pstAttr) {
    CHECK_NULL_POINTER(pstAttr)

    venc api;
    return api.AX_VENC_CreateChn(VeChn, pstAttr);
}

AXCL_EXPORT AX_S32 AXCL_VENC_CreateChnEx(VENC_CHN *pVeChn, const AX_VENC_CHN_ATTR_T *pstAttr) {
    CHECK_NULL_POINTER(pVeChn)
    CHECK_NULL_POINTER(pstAttr)

    venc api;
    return api.AX_VENC_CreateChnEx(pVeChn, pstAttr);
}

AXCL_EXPORT AX_S32 AXCL_VENC_DestroyChn(VENC_CHN VeChn) {
    venc api;
    return api.AX_VENC_DestroyChn(VeChn);
}

AXCL_EXPORT AX_S32 AXCL_VENC_SendFrame(VENC_CHN VeChn, const AX_VIDEO_FRAME_INFO_T *pstFrame, AX_S32 s32MilliSec) {
    CHECK_NULL_POINTER(pstFrame)

    venc api;
    return api.AX_VENC_SendFrame(VeChn, pstFrame, s32MilliSec);
}

AXCL_EXPORT AX_S32 AXCL_VENC_SendFrameEx(VENC_CHN VeChn, const AX_USER_FRAME_INFO_T *pstFrame, AX_S32 s32MilliSec) {
    CHECK_NULL_POINTER(pstFrame)

    venc api;
    return api.AX_VENC_SendFrameEx(VeChn, pstFrame, s32MilliSec);
}

AXCL_EXPORT AX_S32 AXCL_VENC_SelectGrp(VENC_GRP grpId, AX_CHN_STREAM_STATUS_T *pstChnStrmState, AX_S32 s32MilliSec) {
    CHECK_NULL_POINTER(pstChnStrmState)

    venc api;
    return api.AX_VENC_SelectGrp(grpId, pstChnStrmState, s32MilliSec);
}

AXCL_EXPORT AX_S32 AXCL_VENC_SelectClearGrp(VENC_GRP grpId) {
    venc api;
    return api.AX_VENC_SelectClearGrp(grpId);
}

AXCL_EXPORT AX_S32 AXCL_VENC_SelectGrpAddChn(VENC_GRP grpId, VENC_CHN VeChn) {
    venc api;
    return api.AX_VENC_SelectGrpAddChn(grpId, VeChn);
}

AXCL_EXPORT AX_S32 AXCL_VENC_SelectGrpDeleteChn(VENC_GRP grpId, VENC_CHN VeChn) {
    venc api;
    return api.AX_VENC_SelectGrpDeleteChn(grpId, VeChn);
}

AXCL_EXPORT AX_S32 AXCL_VENC_SelectGrpQuery(VENC_GRP grpId, AX_VENC_SELECT_GRP_PARAM_T *pstGrpInfo) {
    CHECK_NULL_POINTER(pstGrpInfo)

    venc api;
    return api.AX_VENC_SelectGrpQuery(grpId, pstGrpInfo);
}

AXCL_EXPORT AX_S32 AXCL_VENC_GetFd(VENC_CHN VeChn) {
    return AXCL_DEF_VENC_ERR(AXCL_ERR_UNSUPPORT);
}

AXCL_EXPORT AX_S32 AXCL_VENC_GetStream(VENC_CHN VeChn, AX_VENC_STREAM_T *pstStream, AX_S32 s32MilliSec) {
    CHECK_NULL_POINTER(pstStream)

    venc api;
    return api.AX_VENC_GetStream(VeChn, pstStream, s32MilliSec);
}

AXCL_EXPORT AX_S32 AXCL_VENC_ReleaseStream(VENC_CHN VeChn, const AX_VENC_STREAM_T *pstStream) {
    CHECK_NULL_POINTER(pstStream)

    venc api;
    return api.AX_VENC_ReleaseStream(VeChn, pstStream);
}

AXCL_EXPORT AX_S32 AXCL_VENC_GetStreamBufInfo(VENC_CHN VeChn, AX_VENC_STREAM_BUF_INFO_T *pstStreamBufInfo) {
    CHECK_NULL_POINTER(pstStreamBufInfo)

    venc api;
    return api.AX_VENC_GetStreamBufInfo(VeChn, pstStreamBufInfo);
}

AXCL_EXPORT AX_S32 AXCL_VENC_StartRecvFrame(VENC_CHN VeChn, const AX_VENC_RECV_PIC_PARAM_T *pstRecvParam) {
    CHECK_NULL_POINTER(pstRecvParam)

    venc api;
    return api.AX_VENC_StartRecvFrame(VeChn, pstRecvParam);
}

AXCL_EXPORT AX_S32 AXCL_VENC_StopRecvFrame(VENC_CHN VeChn) {
    venc api;
    return api.AX_VENC_StopRecvFrame(VeChn);
}

AXCL_EXPORT AX_S32 AXCL_VENC_ResetChn(VENC_CHN VeChn) {
    venc api;
    return api.AX_VENC_ResetChn(VeChn);
}

AXCL_EXPORT AX_S32 AXCL_VENC_SetRoiAttr(VENC_CHN VeChn, const AX_VENC_ROI_ATTR_T *pstRoiAttr) {
    CHECK_NULL_POINTER(pstRoiAttr)

    venc api;
    return api.AX_VENC_SetRoiAttr(VeChn, pstRoiAttr);
}

AXCL_EXPORT AX_S32 AXCL_VENC_GetRoiAttr(VENC_CHN VeChn, AX_U32 u32Index, AX_VENC_ROI_ATTR_T *pstRoiAttr) {
    CHECK_NULL_POINTER(pstRoiAttr)

    venc api;
    return api.AX_VENC_GetRoiAttr(VeChn, u32Index, pstRoiAttr);
}

AXCL_EXPORT AX_S32 AXCL_VENC_SetRcParam(VENC_CHN VeChn, const AX_VENC_RC_PARAM_T *pstRcParam) {
    CHECK_NULL_POINTER(pstRcParam)

    venc api;
    return api.AX_VENC_SetRcParam(VeChn, pstRcParam);
}

AXCL_EXPORT AX_S32 AXCL_VENC_GetRcParam(VENC_CHN VeChn, AX_VENC_RC_PARAM_T *pstRcParam) {
    CHECK_NULL_POINTER(pstRcParam)

    venc api;
    return api.AX_VENC_GetRcParam(VeChn, pstRcParam);
}

AXCL_EXPORT AX_S32 AXCL_VENC_SetModParam(AX_VENC_ENCODER_TYPE_E enVencType, const AX_VENC_MOD_PARAM_T *pstModParam) {
    CHECK_NULL_POINTER(pstModParam)

    venc api;
    return api.AX_VENC_SetModParam(enVencType, pstModParam);
}

AXCL_EXPORT AX_S32 AXCL_VENC_GetModParam(AX_VENC_ENCODER_TYPE_E enVencType, AX_VENC_MOD_PARAM_T *pstModParam) {
    CHECK_NULL_POINTER(pstModParam)

    venc api;
    return api.AX_VENC_GetModParam(enVencType, pstModParam);
}

AXCL_EXPORT AX_S32 AXCL_VENC_SetVuiParam(VENC_CHN VeChn, const AX_VENC_VUI_PARAM_T *pstVuiParam) {
    CHECK_NULL_POINTER(pstVuiParam)

    venc api;
    return api.AX_VENC_SetVuiParam(VeChn, pstVuiParam);
}

AXCL_EXPORT AX_S32 AXCL_VENC_GetVuiParam(VENC_CHN VeChn, AX_VENC_VUI_PARAM_T *pstVuiParam) {
    CHECK_NULL_POINTER(pstVuiParam)

    venc api;
    return api.AX_VENC_GetVuiParam(VeChn, pstVuiParam);
}

AXCL_EXPORT AX_S32 AXCL_VENC_SetChnAttr(VENC_CHN VeChn, const AX_VENC_CHN_ATTR_T *pstChnAttr) {
    CHECK_NULL_POINTER(pstChnAttr)

    venc api;
    return api.AX_VENC_SetChnAttr(VeChn, pstChnAttr);
}

AXCL_EXPORT AX_S32 AXCL_VENC_GetChnAttr(VENC_CHN VeChn, AX_VENC_CHN_ATTR_T *pstChnAttr) {
    CHECK_NULL_POINTER(pstChnAttr)

    venc api;
    return api.AX_VENC_GetChnAttr(VeChn, pstChnAttr);
}

AXCL_EXPORT AX_S32 AXCL_VENC_SetRateJamStrategy(VENC_CHN VeChn, const AX_VENC_RATE_JAM_CFG_T *pstRateJamParam) {
    CHECK_NULL_POINTER(pstRateJamParam)

    venc api;
    return api.AX_VENC_SetRateJamStrategy(VeChn, pstRateJamParam);
}

AXCL_EXPORT AX_S32 AXCL_VENC_GetRateJamStrategy(VENC_CHN VeChn, AX_VENC_RATE_JAM_CFG_T *pstRateJamParam) {
    CHECK_NULL_POINTER(pstRateJamParam)

    venc api;
    return api.AX_VENC_GetRateJamStrategy(VeChn, pstRateJamParam);
}

AXCL_EXPORT AX_S32 AXCL_VENC_SetSuperFrameStrategy(VENC_CHN VeChn, const AX_VENC_SUPERFRAME_CFG_T *pstSuperFrameCfg) {
    CHECK_NULL_POINTER(pstSuperFrameCfg)

    venc api;
    return api.AX_VENC_SetSuperFrameStrategy(VeChn, pstSuperFrameCfg);
}

AXCL_EXPORT AX_S32 AXCL_VENC_GetSuperFrameStrategy(VENC_CHN VeChn, AX_VENC_SUPERFRAME_CFG_T *pstSuperFrameCfg) {
    CHECK_NULL_POINTER(pstSuperFrameCfg)

    venc api;
    return api.AX_VENC_GetSuperFrameStrategy(VeChn, pstSuperFrameCfg);
}

AXCL_EXPORT AX_S32 AXCL_VENC_SetIntraRefresh(VENC_CHN VeChn, const AX_VENC_INTRA_REFRESH_T *pstIntraRefresh) {
    CHECK_NULL_POINTER(pstIntraRefresh)

    venc api;
    return api.AX_VENC_SetIntraRefresh(VeChn, pstIntraRefresh);
}

AXCL_EXPORT AX_S32 AXCL_VENC_GetIntraRefresh(VENC_CHN VeChn, AX_VENC_INTRA_REFRESH_T *pstIntraRefresh) {
    CHECK_NULL_POINTER(pstIntraRefresh)

    venc api;
    return api.AX_VENC_GetIntraRefresh(VeChn, pstIntraRefresh);
}

AXCL_EXPORT AX_S32 AXCL_VENC_SetUsrData(VENC_CHN VeChn, const AX_VENC_USR_DATA_T *pstUsrData) {
    CHECK_NULL_POINTER(pstUsrData)

    venc api;
    return api.AX_VENC_SetUsrData(VeChn, pstUsrData);
}

AXCL_EXPORT AX_S32 AXCL_VENC_GetUsrData(VENC_CHN VeChn, AX_VENC_USR_DATA_T *pstUsrData) {
    CHECK_NULL_POINTER(pstUsrData)
    CHECK_NULL_POINTER(pstUsrData->pu8UsrData)
    CHECK_SMALLER_AND_EQUAL_UINT(0, pstUsrData->u32DataSize)

    venc api;
    return api.AX_VENC_GetUsrData(VeChn, pstUsrData);
}

AXCL_EXPORT AX_S32 AXCL_VENC_SetSliceSplit(VENC_CHN VeChn, const AX_VENC_SLICE_SPLIT_T *pstSliceSplit) {
    CHECK_NULL_POINTER(pstSliceSplit)

    venc api;
    return api.AX_VENC_SetSliceSplit(VeChn, pstSliceSplit);
}

AXCL_EXPORT AX_S32 AXCL_VENC_GetSliceSplit(VENC_CHN VeChn, AX_VENC_SLICE_SPLIT_T *pstSliceSplit) {
    CHECK_NULL_POINTER(pstSliceSplit)

    venc api;
    return api.AX_VENC_GetSliceSplit(VeChn, pstSliceSplit);
}

AXCL_EXPORT AX_S32 AXCL_VENC_RequestIDR(VENC_CHN VeChn, AX_BOOL bInstant) {
    venc api;
    return api.AX_VENC_RequestIDR(VeChn, bInstant);
}

AXCL_EXPORT AX_S32 AXCL_VENC_QueryStatus(VENC_CHN VeChn, AX_VENC_CHN_STATUS_T *pstStatus) {
    CHECK_NULL_POINTER(pstStatus)

    venc api;
    return api.AX_VENC_QueryStatus(VeChn, pstStatus);
}

AXCL_EXPORT AX_S32 AXCL_VENC_SetJpegParam(VENC_CHN VeChn, const AX_VENC_JPEG_PARAM_T *pstJpegParam) {
    CHECK_NULL_POINTER(pstJpegParam)

    venc api;
    return api.AX_VENC_SetJpegParam(VeChn, pstJpegParam);
}

AXCL_EXPORT AX_S32 AXCL_VENC_GetJpegParam(VENC_CHN VeChn, AX_VENC_JPEG_PARAM_T *pstJpegParam) {
    CHECK_NULL_POINTER(pstJpegParam)

    venc api;
    return api.AX_VENC_GetJpegParam(VeChn, pstJpegParam);
}

AXCL_EXPORT AX_S32 AXCL_VENC_JpegEncodeOneFrame(AX_JPEG_ENCODE_ONCE_PARAMS_T *pstJpegParam) {
    CHECK_NULL_POINTER(pstJpegParam)

    venc api;
    return api.AX_VENC_JpegEncodeOneFrame(pstJpegParam);
}

AXCL_EXPORT AX_S32 AXCL_VENC_JpegGetThumbnail(const AX_VOID *pRawData, AX_VOID *pThumbData, AX_U32 *pThumbSize) {
    CHECK_NULL_POINTER(pRawData)
    CHECK_NULL_POINTER(pThumbData)
    CHECK_NULL_POINTER(pThumbSize)

    venc api;
    return api.AX_VENC_JpegGetThumbnail(pRawData, pThumbData, pThumbSize);
}
