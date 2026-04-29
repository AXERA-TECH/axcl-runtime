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
#include "axcl_ivps.h"
#include "axcl_module_version.h"
#include "ivps.hpp"
#include "axcl_logger.hpp"

#define TAG "ivps"
#define CHECK_NULL_POINTER(p)                            \
    if (!(p)) {                                          \
        LOG_MM_E(TAG, "nil pointer");                    \
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_NULL_POINTER); \
    }

AXCL_EXPORT AX_S32 AXCL_IVPS_Init(AX_VOID) {
    ivps api;
    return api.AX_IVPS_Init();
}

AXCL_EXPORT AX_S32 AXCL_IVPS_Deinit(AX_VOID) {
    ivps api;
    return api.AX_IVPS_Deinit();
}

AXCL_EXPORT AX_S32 AXCL_IVPS_CreateGrp(IVPS_GRP IvpsGrp, const AX_IVPS_GRP_ATTR_T *ptGrpAttr) {
    CHECK_NULL_POINTER(ptGrpAttr)

    ivps api;
    return api.AX_IVPS_CreateGrp(IvpsGrp, ptGrpAttr);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_CreateGrpEx(IVPS_GRP *IvpsGrp, const AX_IVPS_GRP_ATTR_T *ptGrpAttr) {
    CHECK_NULL_POINTER(IvpsGrp)
    CHECK_NULL_POINTER(ptGrpAttr)

    ivps api;
    return api.AX_IVPS_CreateGrpEx(IvpsGrp, ptGrpAttr);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_DestoryGrp(IVPS_GRP IvpsGrp) {
    ivps api;
    return api.AX_IVPS_DestoryGrp(IvpsGrp);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_SetPipelineAttr(IVPS_GRP IvpsGrp, AX_IVPS_PIPELINE_ATTR_T *ptPipelineAttr) {
    CHECK_NULL_POINTER(ptPipelineAttr)

    ivps api;
    return api.AX_IVPS_SetPipelineAttr(IvpsGrp, ptPipelineAttr);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_GetPipelineAttr(IVPS_GRP IvpsGrp, AX_IVPS_PIPELINE_ATTR_T *ptPipelineAttr) {
    CHECK_NULL_POINTER(ptPipelineAttr)

    ivps api;
    return api.AX_IVPS_GetPipelineAttr(IvpsGrp, ptPipelineAttr);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_StartGrp(IVPS_GRP IvpsGrp) {
    ivps api;
    return api.AX_IVPS_StartGrp(IvpsGrp);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_StopGrp(IVPS_GRP IvpsGrp) {
    ivps api;
    return api.AX_IVPS_StopGrp(IvpsGrp);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_EnableChn(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn) {
    ivps api;
    return api.AX_IVPS_EnableChn(IvpsGrp, IvpsChn);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_DisableChn(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn) {
    ivps api;
    return api.AX_IVPS_DisableChn(IvpsGrp, IvpsChn);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_SendFrame(IVPS_GRP IvpsGrp, const AX_VIDEO_FRAME_T *ptFrame, AX_S32 nMilliSec) {
    CHECK_NULL_POINTER(ptFrame)

    ivps api;
    return api.AX_IVPS_SendFrame(IvpsGrp, ptFrame, nMilliSec);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_GetChnFrame(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn, AX_VIDEO_FRAME_T *ptFrame, AX_S32 nMilliSec) {
    CHECK_NULL_POINTER(ptFrame)

    ivps api;
    return api.AX_IVPS_GetChnFrame(IvpsGrp, IvpsChn, ptFrame, nMilliSec);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_ReleaseChnFrame(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn, AX_VIDEO_FRAME_T *ptFrame) {
    CHECK_NULL_POINTER(ptFrame)

    ivps api;
    return api.AX_IVPS_ReleaseChnFrame(IvpsGrp, IvpsChn, ptFrame);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_GetGrpFrame(IVPS_GRP IvpsGrp, AX_VIDEO_FRAME_T *ptFrame, AX_S32 nMilliSec) {
    CHECK_NULL_POINTER(ptFrame)

    ivps api;
    return api.AX_IVPS_GetGrpFrame(IvpsGrp, ptFrame, nMilliSec);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_ReleaseGrpFrame(IVPS_GRP IvpsGrp, AX_VIDEO_FRAME_T *ptFrame) {
    CHECK_NULL_POINTER(ptFrame)

    ivps api;
    return api.AX_IVPS_ReleaseGrpFrame(IvpsGrp, ptFrame);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_GetChnFd(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn) {
    ivps api;
    return api.AX_IVPS_GetChnFd(IvpsGrp, IvpsChn);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_GetDebugFifoFrame(IVPS_GRP IvpsGrp, AX_VIDEO_FRAME_T *ptFrame) {
    CHECK_NULL_POINTER(ptFrame)

    ivps api;
    return api.AX_IVPS_GetDebugFifoFrame(IvpsGrp, ptFrame);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_ReleaseDebugFifoFrame(IVPS_GRP IvpsGrp, AX_VIDEO_FRAME_T *ptFrame) {
    CHECK_NULL_POINTER(ptFrame)

    ivps api;
    return api.AX_IVPS_ReleaseDebugFifoFrame(IvpsGrp, ptFrame);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_CloseAllFd(AX_VOID) {
    ivps api;
    return api.AX_IVPS_CloseAllFd();
}

AXCL_EXPORT AX_S32 AXCL_IVPS_SetGrpLDCAttr(IVPS_GRP IvpsGrp, IVPS_FILTER IvpsFilter, const AX_IVPS_LDC_ATTR_T *ptLDCAttr) {
    CHECK_NULL_POINTER(ptLDCAttr)

    ivps api;
    return api.AX_IVPS_SetGrpLDCAttr(IvpsGrp, IvpsFilter, ptLDCAttr);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_GetGrpLDCAttr(IVPS_GRP IvpsGrp, IVPS_FILTER IvpsFilter, AX_IVPS_LDC_ATTR_T *ptLDCAttr) {
    CHECK_NULL_POINTER(ptLDCAttr)

    ivps api;
    return api.AX_IVPS_GetGrpLDCAttr(IvpsGrp, IvpsFilter, ptLDCAttr);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_SetChnLDCAttr(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn, IVPS_FILTER IvpsFilter,
                                           const AX_IVPS_LDC_ATTR_T *ptLDCAttr) {
    CHECK_NULL_POINTER(ptLDCAttr)

    ivps api;
    return api.AX_IVPS_SetChnLDCAttr(IvpsGrp, IvpsChn, IvpsFilter, ptLDCAttr);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_GetChnLDCAttr(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn, IVPS_FILTER IvpsFilter, AX_IVPS_LDC_ATTR_T *ptLDCAttr) {
    CHECK_NULL_POINTER(ptLDCAttr)

    ivps api;
    return api.AX_IVPS_GetChnLDCAttr(IvpsGrp, IvpsChn, IvpsFilter, ptLDCAttr);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_SetGrpPoolAttr(IVPS_GRP IvpsGrp, const AX_IVPS_POOL_ATTR_T *ptPoolAttr) {
    CHECK_NULL_POINTER(ptPoolAttr)

    ivps api;
    return api.AX_IVPS_SetGrpPoolAttr(IvpsGrp, ptPoolAttr);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_SetChnPoolAttr(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn, const AX_IVPS_POOL_ATTR_T *ptPoolAttr) {
    CHECK_NULL_POINTER(ptPoolAttr)

    ivps api;
    return api.AX_IVPS_SetChnPoolAttr(IvpsGrp, IvpsChn, ptPoolAttr);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_SetGrpUserFRC(IVPS_GRP IvpsGrp, const AX_IVPS_USER_FRAME_RATE_CTRL_T *ptFrameRateAttr) {
    CHECK_NULL_POINTER(ptFrameRateAttr)

    ivps api;
    return api.AX_IVPS_SetGrpUserFRC(IvpsGrp, ptFrameRateAttr);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_SetChnUserFRC(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn, const AX_IVPS_USER_FRAME_RATE_CTRL_T *ptFrameRateAttr) {
    CHECK_NULL_POINTER(ptFrameRateAttr)

    ivps api;
    return api.AX_IVPS_SetChnUserFRC(IvpsGrp, IvpsChn, ptFrameRateAttr);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_SetGrpCrop(IVPS_GRP IvpsGrp, const AX_IVPS_CROP_INFO_T *ptCropInfo) {
    CHECK_NULL_POINTER(ptCropInfo)

    ivps api;
    return api.AX_IVPS_SetGrpCrop(IvpsGrp, ptCropInfo);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_GetGrpCrop(IVPS_GRP IvpsGrp, AX_IVPS_CROP_INFO_T *ptCropInfo) {
    CHECK_NULL_POINTER(ptCropInfo)

    ivps api;
    return api.AX_IVPS_GetGrpCrop(IvpsGrp, ptCropInfo);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_SetChnAttr(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn, IVPS_FILTER IvpsFilter, const AX_IVPS_CHN_ATTR_T *ptChnAttr) {
    CHECK_NULL_POINTER(ptChnAttr)

    ivps api;
    return api.AX_IVPS_SetChnAttr(IvpsGrp, IvpsChn, IvpsFilter, ptChnAttr);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_GetChnAttr(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn, IVPS_FILTER IvpsFilter, AX_IVPS_CHN_ATTR_T *ptChnAttr) {
    CHECK_NULL_POINTER(ptChnAttr)

    ivps api;
    return api.AX_IVPS_GetChnAttr(IvpsGrp, IvpsChn, IvpsFilter, ptChnAttr);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_EnableBackupFrame(IVPS_GRP IvpsGrp, AX_U8 nFifoDepth) {
    ivps api;
    return api.AX_IVPS_EnableBackupFrame(IvpsGrp, nFifoDepth);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_DisableBackupFrame(IVPS_GRP IvpsGrp) {
    ivps api;
    return api.AX_IVPS_DisableBackupFrame(IvpsGrp);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_ResetGrp(IVPS_GRP IvpsGrp) {
    ivps api;
    return api.AX_IVPS_ResetGrp(IvpsGrp);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_GetEngineDutyCycle(AX_IVPS_DUTY_CYCLE_ATTR_T *ptDutyCycle) {
    CHECK_NULL_POINTER(ptDutyCycle)

    ivps api;
    return api.AX_IVPS_GetEngineDutyCycle(ptDutyCycle);
}

AXCL_EXPORT IVPS_RGN_HANDLE AXCL_IVPS_RGN_Create(AX_VOID) {
    ivps api;
    return api.AX_IVPS_RGN_Create();
}

AXCL_EXPORT AX_S32 AXCL_IVPS_RGN_Destroy(IVPS_RGN_HANDLE hRegion) {
    ivps api;
    return api.AX_IVPS_RGN_Destroy(hRegion);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_RGN_AttachToFilter(IVPS_RGN_HANDLE hRegion, IVPS_GRP IvpsGrp, IVPS_FILTER IvpsFilter) {
    ivps api;
    return api.AX_IVPS_RGN_AttachToFilter(hRegion, IvpsGrp, IvpsFilter);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_RGN_DetachFromFilter(IVPS_RGN_HANDLE hRegion, IVPS_GRP IvpsGrp, IVPS_FILTER IvpsFilter) {
    ivps api;
    return api.AX_IVPS_RGN_DetachFromFilter(hRegion, IvpsGrp, IvpsFilter);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_RGN_Update(IVPS_RGN_HANDLE hRegion, const AX_IVPS_RGN_DISP_GROUP_T *ptDisp) {
    CHECK_NULL_POINTER(ptDisp)

    ivps api;
    return api.AX_IVPS_RGN_Update(hRegion, ptDisp);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_CmmCopyTdp(AX_U64 nSrcPhyAddr, AX_U64 nDstPhyAddr, AX_U64 nMemSize) {
    ivps api;
    return api.AX_IVPS_CmmCopyTdp(nSrcPhyAddr, nDstPhyAddr, nMemSize);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_FlipAndRotationTdp(const AX_VIDEO_FRAME_T *ptSrc, AX_IVPS_CHN_FLIP_MODE_E eFlipMode,
                                                AX_IVPS_ROTATION_E eRotation, AX_VIDEO_FRAME_T *ptDst) {
    CHECK_NULL_POINTER(ptSrc)
    CHECK_NULL_POINTER(ptDst)

    ivps api;
    return api.AX_IVPS_FlipAndRotationTdp(ptSrc, eFlipMode, eRotation, ptDst);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_CscTdp(const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst) {
    CHECK_NULL_POINTER(ptSrc)
    CHECK_NULL_POINTER(ptDst)

    ivps api;
    return api.AX_IVPS_CscTdp(ptSrc, ptDst);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_CropResizeTdp(const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst,
                                           const AX_IVPS_ASPECT_RATIO_T *ptAspectRatio) {
    CHECK_NULL_POINTER(ptSrc)
    CHECK_NULL_POINTER(ptDst)
    CHECK_NULL_POINTER(ptAspectRatio)

    ivps api;
    return api.AX_IVPS_CropResizeTdp(ptSrc, ptDst, ptAspectRatio);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_CropResizeV2Tdp(const AX_VIDEO_FRAME_T *ptSrc, const AX_IVPS_RECT_T tBox[], AX_U32 nCropNum,
                                             AX_VIDEO_FRAME_T *ptDst[], const AX_IVPS_ASPECT_RATIO_T *ptAspectRatio) {
    CHECK_NULL_POINTER(ptSrc)
    CHECK_NULL_POINTER(tBox)
    CHECK_NULL_POINTER(ptDst)
    CHECK_NULL_POINTER(ptAspectRatio)

    ivps api;
    return api.AX_IVPS_CropResizeV2Tdp(ptSrc, tBox, nCropNum, ptDst, ptAspectRatio);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_AlphaBlendingTdp(const AX_VIDEO_FRAME_T *ptSrc, const AX_VIDEO_FRAME_T *ptOverlay,
                                              const AX_IVPS_POINT_T tOffset, AX_U8 nAlpha, AX_VIDEO_FRAME_T *ptDst) {
    CHECK_NULL_POINTER(ptSrc)
    CHECK_NULL_POINTER(ptOverlay)
    CHECK_NULL_POINTER(ptDst)

    ivps api;
    return api.AX_IVPS_AlphaBlendingTdp(ptSrc, ptOverlay, tOffset, nAlpha, ptDst);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_AlphaBlendingV3Tdp(const AX_VIDEO_FRAME_T *ptSrc, const AX_OVERLAY_T *ptOverlay, AX_VIDEO_FRAME_T *ptDst) {
    CHECK_NULL_POINTER(ptSrc)
    CHECK_NULL_POINTER(ptOverlay)
    CHECK_NULL_POINTER(ptDst)

    ivps api;
    return api.AX_IVPS_AlphaBlendingV3Tdp(ptSrc, ptOverlay, ptDst);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_DrawOsdTdp(const AX_VIDEO_FRAME_T *ptFrame, const AX_OSD_BMP_ATTR_T arrBmp[], AX_U32 nNum) {
    CHECK_NULL_POINTER(ptFrame)

    ivps api;
    return api.AX_IVPS_DrawOsdTdp(ptFrame, arrBmp, nNum);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_DrawMosaicTdp(const AX_VIDEO_FRAME_T *ptSrc, AX_IVPS_RGN_MOSAIC_T tMosaic[], AX_U32 nNum) {
    CHECK_NULL_POINTER(ptSrc)

    ivps api;
    return api.AX_IVPS_DrawMosaicTdp(ptSrc, tMosaic, nNum);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_CmmCopyVpp(AX_U64 nSrcPhyAddr, AX_U64 nDstPhyAddr, AX_U64 nMemSize) {
    ivps api;
    return api.AX_IVPS_CmmCopyVpp(nSrcPhyAddr, nDstPhyAddr, nMemSize);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_CropResizeVpp(const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst,
                                           const AX_IVPS_ASPECT_RATIO_T *ptAspectRatio) {
    CHECK_NULL_POINTER(ptSrc)
    CHECK_NULL_POINTER(ptDst)
    CHECK_NULL_POINTER(ptAspectRatio)

    ivps api;
    return api.AX_IVPS_CropResizeVpp(ptSrc, ptDst, ptAspectRatio);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_CropResizeV2Vpp(const AX_VIDEO_FRAME_T *ptSrc, const AX_IVPS_RECT_T tBox[], AX_U32 nCropNum,
                                             AX_VIDEO_FRAME_T *ptDst[], const AX_IVPS_ASPECT_RATIO_T *ptAspectRatio) {
    CHECK_NULL_POINTER(ptSrc)
    CHECK_NULL_POINTER(ptDst)
    CHECK_NULL_POINTER(ptAspectRatio)

    ivps api;
    return api.AX_IVPS_CropResizeV2Vpp(ptSrc, tBox, nCropNum, ptDst, ptAspectRatio);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_CropResizeV3Vpp(const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst[], AX_U32 nNum,
                                             const AX_IVPS_ASPECT_RATIO_T *ptAspectRatio) {
    CHECK_NULL_POINTER(ptSrc)
    CHECK_NULL_POINTER(ptDst)
    CHECK_NULL_POINTER(ptAspectRatio)

    ivps api;
    return api.AX_IVPS_CropResizeV3Vpp(ptSrc, ptDst, nNum, ptAspectRatio);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_CscVpp(const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst) {
    CHECK_NULL_POINTER(ptSrc)
    CHECK_NULL_POINTER(ptDst)

    ivps api;
    return api.AX_IVPS_CscVpp(ptSrc, ptDst);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_DrawMosaicVpp(const AX_VIDEO_FRAME_T *ptSrc, AX_IVPS_RGN_MOSAIC_T tMosaic[], AX_U32 nNum) {
    CHECK_NULL_POINTER(ptSrc)

    ivps api;
    return api.AX_IVPS_DrawMosaicVpp(ptSrc, tMosaic, nNum);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_SetScaleCoefLevelVpp(const AX_IVPS_SCALE_RANGE_T *ScaleRange, const AX_IVPS_SCALE_COEF_LEVEL_T *CoefLevel) {
    CHECK_NULL_POINTER(ScaleRange)
    CHECK_NULL_POINTER(CoefLevel)

    ivps api;
    return api.AX_IVPS_SetScaleCoefLevelVpp(ScaleRange, CoefLevel);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_GetScaleCoefLevelVpp(const AX_IVPS_SCALE_RANGE_T *ScaleRange, AX_IVPS_SCALE_COEF_LEVEL_T *CoefLevel) {
    CHECK_NULL_POINTER(ScaleRange)
    CHECK_NULL_POINTER(CoefLevel)

    ivps api;
    return api.AX_IVPS_GetScaleCoefLevelVpp(ScaleRange, CoefLevel);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_CmmCopyVgp(AX_U64 nSrcPhyAddr, AX_U64 nDstPhyAddr, AX_U64 nMemSize) {
    ivps api;
    return api.AX_IVPS_CmmCopyVgp(nSrcPhyAddr, nDstPhyAddr, nMemSize);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_CscVgp(const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst) {
    CHECK_NULL_POINTER(ptSrc)
    CHECK_NULL_POINTER(ptDst)

    ivps api;
    return api.AX_IVPS_CscVgp(ptSrc, ptDst);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_CropResizeVgp(const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst,
                                           const AX_IVPS_ASPECT_RATIO_T *ptAspectRatio) {
    CHECK_NULL_POINTER(ptSrc)
    CHECK_NULL_POINTER(ptDst)
    CHECK_NULL_POINTER(ptAspectRatio)

    ivps api;
    return api.AX_IVPS_CropResizeVgp(ptSrc, ptDst, ptAspectRatio);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_CropResizeV2Vgp(const AX_VIDEO_FRAME_T *ptSrc, const AX_IVPS_RECT_T tBox[], AX_U32 nCropNum,
                                             AX_VIDEO_FRAME_T *ptDst[], const AX_IVPS_ASPECT_RATIO_T *ptAspectRatio) {
    CHECK_NULL_POINTER(ptSrc)
    CHECK_NULL_POINTER(ptDst)
    CHECK_NULL_POINTER(ptAspectRatio)

    ivps api;
    return api.AX_IVPS_CropResizeV2Vgp(ptSrc, tBox, nCropNum, ptDst, ptAspectRatio);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_CropResizeV4Vgp(const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst,
                                             const AX_IVPS_ASPECT_RATIO_T *ptAspectRatio, const AX_IVPS_SCALE_STEP_T *ptScaleStep) {
    CHECK_NULL_POINTER(ptSrc)
    CHECK_NULL_POINTER(ptDst)
    CHECK_NULL_POINTER(ptAspectRatio)
    CHECK_NULL_POINTER(ptScaleStep)

    ivps api;
    return api.AX_IVPS_CropResizeV4Vgp(ptSrc, ptDst, ptAspectRatio, ptScaleStep);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_AlphaBlendingVgp(const AX_VIDEO_FRAME_T *ptSrc, const AX_VIDEO_FRAME_T *ptOverlay,
                                              const AX_IVPS_POINT_T tOffset, AX_U8 nAlpha, AX_VIDEO_FRAME_T *ptDst) {
    CHECK_NULL_POINTER(ptSrc)
    CHECK_NULL_POINTER(ptOverlay)
    CHECK_NULL_POINTER(ptDst)

    ivps api;
    return api.AX_IVPS_AlphaBlendingVgp(ptSrc, ptOverlay, tOffset, nAlpha, ptDst);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_AlphaBlendingV2Vgp(const AX_VIDEO_FRAME_T *ptSrc, const AX_VIDEO_FRAME_T *ptOverlay,
                                                const AX_IVPS_POINT_T tOffset, const AX_IVPS_ALPHA_LUT_T *ptSpAlpha,
                                                AX_VIDEO_FRAME_T *ptDst) {
    CHECK_NULL_POINTER(ptSrc)
    CHECK_NULL_POINTER(ptOverlay)
    CHECK_NULL_POINTER(ptSpAlpha)
    CHECK_NULL_POINTER(ptDst)

    ivps api;
    return api.AX_IVPS_AlphaBlendingV2Vgp(ptSrc, ptOverlay, tOffset, ptSpAlpha, ptDst);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_AlphaBlendingV3Vgp(const AX_VIDEO_FRAME_T *ptSrc, const AX_OVERLAY_T *ptOverlay, AX_VIDEO_FRAME_T *ptDst) {
    CHECK_NULL_POINTER(ptSrc)
    CHECK_NULL_POINTER(ptOverlay)
    CHECK_NULL_POINTER(ptDst)

    ivps api;
    return api.AX_IVPS_AlphaBlendingV3Vgp(ptSrc, ptOverlay, ptDst);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_DrawOsdVgp(const AX_VIDEO_FRAME_T *ptFrame, const AX_OSD_BMP_ATTR_T arrBmp[], AX_U32 nNum) {
    CHECK_NULL_POINTER(ptFrame)

    ivps api;
    return api.AX_IVPS_DrawOsdVgp(ptFrame, arrBmp, nNum);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_DrawMosaicVgp(const AX_VIDEO_FRAME_T *ptSrc, AX_IVPS_RGN_MOSAIC_T tMosaic[], AX_U32 nNum) {
    CHECK_NULL_POINTER(ptSrc)

    ivps api;
    return api.AX_IVPS_DrawMosaicVgp(ptSrc, tMosaic, nNum);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_SetScaleCoefLevelVgp(const AX_IVPS_SCALE_RANGE_T *ScaleRange, const AX_IVPS_SCALE_COEF_LEVEL_T *CoefLevel) {
    CHECK_NULL_POINTER(ScaleRange)
    CHECK_NULL_POINTER(CoefLevel)

    ivps api;
    return api.AX_IVPS_SetScaleCoefLevelVgp(ScaleRange, CoefLevel);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_GetScaleCoefLevelVgp(const AX_IVPS_SCALE_RANGE_T *ScaleRange, AX_IVPS_SCALE_COEF_LEVEL_T *CoefLevel) {
    CHECK_NULL_POINTER(ScaleRange)
    CHECK_NULL_POINTER(CoefLevel)

    ivps api;
    return api.AX_IVPS_GetScaleCoefLevelVgp(ScaleRange, CoefLevel);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_DrawLine(const AX_IVPS_RGN_CANVAS_INFO_T *ptCanvas, AX_IVPS_GDI_ATTR_T tAttr, const AX_IVPS_POINT_T tPoint[],
                                      AX_U32 nPointNum) {
    CHECK_NULL_POINTER(ptCanvas)

    ivps api;
    return api.AX_IVPS_DrawLine(ptCanvas, tAttr, tPoint, nPointNum);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_DrawPolygon(const AX_IVPS_RGN_CANVAS_INFO_T *ptCanvas, AX_IVPS_GDI_ATTR_T tAttr,
                                         const AX_IVPS_POINT_T tPoint[], AX_U32 nPointNum) {
    CHECK_NULL_POINTER(ptCanvas)

    ivps api;
    return api.AX_IVPS_DrawPolygon(ptCanvas, tAttr, tPoint, nPointNum);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_DrawRect(const AX_IVPS_RGN_CANVAS_INFO_T *ptCanvas, AX_IVPS_GDI_ATTR_T tAttr, AX_IVPS_RECT_T tRect) {
    CHECK_NULL_POINTER(ptCanvas)

    ivps api;
    return api.AX_IVPS_DrawRect(ptCanvas, tAttr, tRect);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_Dewarp(const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst, const AX_IVPS_DEWARP_ATTR_T *ptAttr) {
    CHECK_NULL_POINTER(ptSrc)
    CHECK_NULL_POINTER(ptDst)
    CHECK_NULL_POINTER(ptAttr)

    ivps api;
    return api.AX_IVPS_Dewarp(ptSrc, ptDst, ptAttr);
}

AXCL_EXPORT AX_S32 AXCL_PyraLite_Gen(const AX_PYRA_FRAME_T *tSrcFrame, AX_PYRA_FRAME_T *tDstFrame, AX_BOOL bMaskFlag) {
    CHECK_NULL_POINTER(tSrcFrame)
    CHECK_NULL_POINTER(tDstFrame)

    ivps api;
    return api.AX_PyraLite_Gen(tSrcFrame, tDstFrame, bMaskFlag);
}

AXCL_EXPORT AX_S32 AXCL_PyraLite_Rcn(const AX_PYRA_FRAME_T *tSrcFrame, AX_PYRA_FRAME_T *tDstFrame, AX_BOOL bBottom) {
    CHECK_NULL_POINTER(tSrcFrame)
    CHECK_NULL_POINTER(tDstFrame)

    ivps api;
    return api.AX_PyraLite_Rcn(tSrcFrame, tDstFrame, bBottom);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_GdcWorkCreate(GDC_HANDLE *pGdcHandle) {
    CHECK_NULL_POINTER(pGdcHandle)

    ivps api;
    return api.AX_IVPS_GdcWorkCreate(pGdcHandle);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_GdcWorkAttrSet(GDC_HANDLE nGdcHandle, const AX_IVPS_GDC_ATTR_T *ptGdcAttr) {
    CHECK_NULL_POINTER(ptGdcAttr)

    ivps api;
    return api.AX_IVPS_GdcWorkAttrSet(nGdcHandle, ptGdcAttr);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_GdcWorkRun(GDC_HANDLE nGdcHandle, const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst) {
    CHECK_NULL_POINTER(ptSrc)
    CHECK_NULL_POINTER(ptDst)

    ivps api;
    return api.AX_IVPS_GdcWorkRun(nGdcHandle, ptSrc, ptDst);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_GdcWorkDestroy(GDC_HANDLE nGdcHandle) {
    ivps api;
    return api.AX_IVPS_GdcWorkDestroy(nGdcHandle);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_FisheyePointQueryDst2Src(AX_IVPS_POINT_NICE_T *ptSrcPoint, const AX_IVPS_POINT_NICE_T *ptDstPoint,
                                                      AX_U16 nInputW, AX_U16 nInputH, AX_U8 nRgnIdx,
                                                      const AX_IVPS_FISHEYE_ATTR_T *ptFisheyeAttr) {
    CHECK_NULL_POINTER(ptSrcPoint)
    CHECK_NULL_POINTER(ptDstPoint)
    CHECK_NULL_POINTER(ptFisheyeAttr)

    ivps api;
    return api.AX_IVPS_FisheyePointQueryDst2Src(ptSrcPoint, ptDstPoint, nInputW, nInputH, nRgnIdx, ptFisheyeAttr);
}

AXCL_EXPORT AX_S32 AXCL_IVPS_FisheyePointQuerySrc2Dst(AX_IVPS_POINT_NICE_T *ptDstPoint, const AX_IVPS_POINT_NICE_T *ptSrcPoint,
                                                      AX_U16 nInputW, AX_U16 nInputH, AX_U8 nRgnIdx,
                                                      const AX_IVPS_FISHEYE_ATTR_T *ptFisheyeAttr) {
    CHECK_NULL_POINTER(ptDstPoint)
    CHECK_NULL_POINTER(ptSrcPoint)
    CHECK_NULL_POINTER(ptFisheyeAttr)

    ivps api;
    return api.AX_IVPS_FisheyePointQuerySrc2Dst(ptDstPoint, ptSrcPoint, nInputW, nInputH, nRgnIdx, ptFisheyeAttr);
}
