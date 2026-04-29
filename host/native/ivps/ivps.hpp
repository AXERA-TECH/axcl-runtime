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

#include "axcl_ivps_type.h"
#include "native.hpp"
#include "package/host/native/module/ivps_request.hpp"
#include "package/host/native/module/ivps_response.hpp"

#define AXCL_DEF_IVPS_ERR(e) AXCL_DEF_NATIVE_ERR(static_cast<uint8_t>(axcl::pkg::native::module::IVPS), (e))

class ivps : public native<axcl::pkg::host::ivps_request, axcl::pkg::host::ivps_response> {
public:
    ivps() : native<axcl::pkg::host::ivps_request, axcl::pkg::host::ivps_response>(axcl::pkg::native::module::IVPS) {
    }

    AX_S32 AX_IVPS_Init();
    AX_S32 AX_IVPS_Deinit();

    AX_S32 AX_IVPS_CreateGrp(IVPS_GRP IvpsGrp, const AX_IVPS_GRP_ATTR_T *ptGrpAttr);
    AX_S32 AX_IVPS_CreateGrpEx(IVPS_GRP *IvpsGrp, const AX_IVPS_GRP_ATTR_T *ptGrpAttr);
    AX_S32 AX_IVPS_DestoryGrp(IVPS_GRP IvpsGrp);
    AX_S32 AX_IVPS_SetPipelineAttr(IVPS_GRP IvpsGrp, AX_IVPS_PIPELINE_ATTR_T *ptPipelineAttr);
    AX_S32 AX_IVPS_GetPipelineAttr(IVPS_GRP IvpsGrp, AX_IVPS_PIPELINE_ATTR_T *ptPipelineAttr);
    AX_S32 AX_IVPS_StartGrp(IVPS_GRP IvpsGrp);
    AX_S32 AX_IVPS_StopGrp(IVPS_GRP IvpsGrp);
    AX_S32 AX_IVPS_EnableChn(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn);
    AX_S32 AX_IVPS_DisableChn(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn);
    AX_S32 AX_IVPS_SendFrame(IVPS_GRP IvpsGrp, const AX_VIDEO_FRAME_T *ptFrame, AX_S32 nMilliSec);
    AX_S32 AX_IVPS_GetChnFrame(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn, AX_VIDEO_FRAME_T *ptFrame, AX_S32 nMilliSec);
    AX_S32 AX_IVPS_ReleaseChnFrame(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn, AX_VIDEO_FRAME_T *ptFrame);
    AX_S32 AX_IVPS_GetGrpFrame(IVPS_GRP IvpsGrp, AX_VIDEO_FRAME_T *ptFrame, AX_S32 nMilliSec);
    AX_S32 AX_IVPS_ReleaseGrpFrame(IVPS_GRP IvpsGrp, AX_VIDEO_FRAME_T *ptFrame);
    AX_S32 AX_IVPS_GetChnFd(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn);
    AX_S32 AX_IVPS_GetDebugFifoFrame(IVPS_GRP IvpsGrp, AX_VIDEO_FRAME_T *ptFrame);
    AX_S32 AX_IVPS_ReleaseDebugFifoFrame(IVPS_GRP IvpsGrp, AX_VIDEO_FRAME_T *ptFrame);
    AX_S32 AX_IVPS_CloseAllFd();

    AX_S32 AX_IVPS_SetGrpLDCAttr(IVPS_GRP IvpsGrp, IVPS_FILTER IvpsFilter, const AX_IVPS_LDC_ATTR_T *ptLDCAttr);
    AX_S32 AX_IVPS_GetGrpLDCAttr(IVPS_GRP IvpsGrp, IVPS_FILTER IvpsFilter, AX_IVPS_LDC_ATTR_T *ptLDCAttr);
    AX_S32 AX_IVPS_SetChnLDCAttr(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn, IVPS_FILTER IvpsFilter, const AX_IVPS_LDC_ATTR_T *ptLDCAttr);
    AX_S32 AX_IVPS_GetChnLDCAttr(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn, IVPS_FILTER IvpsFilter, AX_IVPS_LDC_ATTR_T *ptLDCAttr);

    AX_S32 AX_IVPS_SetGrpPoolAttr(IVPS_GRP IvpsGrp, const AX_IVPS_POOL_ATTR_T *ptPoolAttr);
    AX_S32 AX_IVPS_SetChnPoolAttr(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn, const AX_IVPS_POOL_ATTR_T *ptPoolAttr);

    AX_S32 AX_IVPS_SetGrpUserFRC(IVPS_GRP IvpsGrp, const AX_IVPS_USER_FRAME_RATE_CTRL_T *ptFrameRateAttr);
    AX_S32 AX_IVPS_SetChnUserFRC(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn, const AX_IVPS_USER_FRAME_RATE_CTRL_T *ptFrameRateAttr);

    AX_S32 AX_IVPS_SetGrpCrop(IVPS_GRP IvpsGrp, const AX_IVPS_CROP_INFO_T *ptCropInfo);
    AX_S32 AX_IVPS_GetGrpCrop(IVPS_GRP IvpsGrp, AX_IVPS_CROP_INFO_T *ptCropInfo);
    AX_S32 AX_IVPS_SetChnAttr(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn, IVPS_FILTER IvpsFilter, const AX_IVPS_CHN_ATTR_T *ptChnAttr);
    AX_S32 AX_IVPS_GetChnAttr(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn, IVPS_FILTER IvpsFilter, AX_IVPS_CHN_ATTR_T *ptChnAttr);

    AX_S32 AX_IVPS_EnableBackupFrame(IVPS_GRP IvpsGrp, AX_U8 nFifoDepth);
    AX_S32 AX_IVPS_DisableBackupFrame(IVPS_GRP IvpsGrp);
    AX_S32 AX_IVPS_ResetGrp(IVPS_GRP IvpsGrp);

    AX_S32 AX_IVPS_GetEngineDutyCycle(AX_IVPS_DUTY_CYCLE_ATTR_T *ptDutyCycle);

    IVPS_RGN_HANDLE AX_IVPS_RGN_Create();
    AX_S32 AX_IVPS_RGN_Destroy(IVPS_RGN_HANDLE hRegion);
    AX_S32 AX_IVPS_RGN_AttachToFilter(IVPS_RGN_HANDLE hRegion, IVPS_GRP IvpsGrp, IVPS_FILTER IvpsFilter);
    AX_S32 AX_IVPS_RGN_DetachFromFilter(IVPS_RGN_HANDLE hRegion, IVPS_GRP IvpsGrp, IVPS_FILTER IvpsFilter);
    AX_S32 AX_IVPS_RGN_Update(IVPS_RGN_HANDLE hRegion, const AX_IVPS_RGN_DISP_GROUP_T *ptDisp);

    AX_S32 AX_IVPS_CmmCopyTdp(AX_U64 nSrcPhyAddr, AX_U64 nDstPhyAddr, AX_U64 nMemSize);
    AX_S32 AX_IVPS_FlipAndRotationTdp(const AX_VIDEO_FRAME_T *ptSrc, AX_IVPS_CHN_FLIP_MODE_E eFlipMode, AX_IVPS_ROTATION_E eRotation,
                                      AX_VIDEO_FRAME_T *ptDst);
    AX_S32 AX_IVPS_CscTdp(const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst);
    AX_S32 AX_IVPS_CropResizeTdp(const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst, const AX_IVPS_ASPECT_RATIO_T *ptAspectRatio);
    AX_S32 AX_IVPS_CropResizeV2Tdp(const AX_VIDEO_FRAME_T *ptSrc, const AX_IVPS_RECT_T tBox[], AX_U32 nCropNum, AX_VIDEO_FRAME_T *ptDst[],
                                   const AX_IVPS_ASPECT_RATIO_T *ptAspectRatio);
    AX_S32 AX_IVPS_AlphaBlendingTdp(const AX_VIDEO_FRAME_T *ptSrc, const AX_VIDEO_FRAME_T *ptOverlay, const AX_IVPS_POINT_T tOffset,
                                    AX_U8 nAlpha, AX_VIDEO_FRAME_T *ptDst);
    AX_S32 AX_IVPS_AlphaBlendingV3Tdp(const AX_VIDEO_FRAME_T *ptSrc, const AX_OVERLAY_T *ptOverlay, AX_VIDEO_FRAME_T *ptDst);
    AX_S32 AX_IVPS_DrawOsdTdp(const AX_VIDEO_FRAME_T *ptFrame, const AX_OSD_BMP_ATTR_T arrBmp[], AX_U32 nNum);
    AX_S32 AX_IVPS_DrawMosaicTdp(const AX_VIDEO_FRAME_T *ptSrc, AX_IVPS_RGN_MOSAIC_T tMosaic[], AX_U32 nNum);

    AX_S32 AX_IVPS_CmmCopyVpp(AX_U64 nSrcPhyAddr, AX_U64 nDstPhyAddr, AX_U64 nMemSize);
    AX_S32 AX_IVPS_CropResizeVpp(const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst, const AX_IVPS_ASPECT_RATIO_T *ptAspectRatio);
    AX_S32 AX_IVPS_CropResizeV2Vpp(const AX_VIDEO_FRAME_T *ptSrc, const AX_IVPS_RECT_T tBox[], AX_U32 nCropNum, AX_VIDEO_FRAME_T *ptDst[],
                                   const AX_IVPS_ASPECT_RATIO_T *ptAspectRatio);
    AX_S32 AX_IVPS_CropResizeV3Vpp(const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst[], AX_U32 nNum,
                                   const AX_IVPS_ASPECT_RATIO_T *ptAspectRatio);
    AX_S32 AX_IVPS_CscVpp(const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst);
    AX_S32 AX_IVPS_DrawMosaicVpp(const AX_VIDEO_FRAME_T *ptSrc, AX_IVPS_RGN_MOSAIC_T tMosaic[], AX_U32 nNum);

    AX_S32 AX_IVPS_SetScaleCoefLevelVpp(const AX_IVPS_SCALE_RANGE_T *ScaleRange, const AX_IVPS_SCALE_COEF_LEVEL_T *CoefLevel);
    AX_S32 AX_IVPS_GetScaleCoefLevelVpp(const AX_IVPS_SCALE_RANGE_T *ScaleRange, AX_IVPS_SCALE_COEF_LEVEL_T *CoefLevel);

    AX_S32 AX_IVPS_CmmCopyVgp(AX_U64 nSrcPhyAddr, AX_U64 nDstPhyAddr, AX_U64 nMemSize);
    AX_S32 AX_IVPS_CscVgp(const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst);
    AX_S32 AX_IVPS_CropResizeVgp(const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst, const AX_IVPS_ASPECT_RATIO_T *ptAspectRatio);
    AX_S32 AX_IVPS_CropResizeV2Vgp(const AX_VIDEO_FRAME_T *ptSrc, const AX_IVPS_RECT_T tBox[], AX_U32 nCropNum, AX_VIDEO_FRAME_T *ptDst[],
                                   const AX_IVPS_ASPECT_RATIO_T *ptAspectRatio);
    AX_S32 AX_IVPS_CropResizeV4Vgp(const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst, const AX_IVPS_ASPECT_RATIO_T *ptAspectRatio,
                                   const AX_IVPS_SCALE_STEP_T *ptScaleStep);
    AX_S32 AX_IVPS_AlphaBlendingVgp(const AX_VIDEO_FRAME_T *ptSrc, const AX_VIDEO_FRAME_T *ptOverlay, const AX_IVPS_POINT_T tOffset,
                                    AX_U8 nAlpha, AX_VIDEO_FRAME_T *ptDst);
    AX_S32 AX_IVPS_AlphaBlendingV2Vgp(const AX_VIDEO_FRAME_T *ptSrc, const AX_VIDEO_FRAME_T *ptOverlay, const AX_IVPS_POINT_T tOffset,
                                      const AX_IVPS_ALPHA_LUT_T *ptSpAlpha, AX_VIDEO_FRAME_T *ptDst);
    AX_S32 AX_IVPS_AlphaBlendingV3Vgp(const AX_VIDEO_FRAME_T *ptSrc, const AX_OVERLAY_T *ptOverlay, AX_VIDEO_FRAME_T *ptDst);
    AX_S32 AX_IVPS_DrawOsdVgp(const AX_VIDEO_FRAME_T *ptFrame, const AX_OSD_BMP_ATTR_T arrBmp[], AX_U32 nNum);
    AX_S32 AX_IVPS_DrawMosaicVgp(const AX_VIDEO_FRAME_T *ptSrc, AX_IVPS_RGN_MOSAIC_T tMosaic[], AX_U32 nNum);
    AX_S32 AX_IVPS_SetScaleCoefLevelVgp(const AX_IVPS_SCALE_RANGE_T *ScaleRange, const AX_IVPS_SCALE_COEF_LEVEL_T *CoefLevel);
    AX_S32 AX_IVPS_GetScaleCoefLevelVgp(const AX_IVPS_SCALE_RANGE_T *ScaleRange, AX_IVPS_SCALE_COEF_LEVEL_T *CoefLevel);

    AX_S32 AX_IVPS_DrawLine(const AX_IVPS_RGN_CANVAS_INFO_T *ptCanvas, AX_IVPS_GDI_ATTR_T tAttr, const AX_IVPS_POINT_T tPoint[],
                            AX_U32 nPointNum);
    AX_S32 AX_IVPS_DrawPolygon(const AX_IVPS_RGN_CANVAS_INFO_T *ptCanvas, AX_IVPS_GDI_ATTR_T tAttr, const AX_IVPS_POINT_T tPoint[],
                               AX_U32 nPointNum);
    AX_S32 AX_IVPS_DrawRect(const AX_IVPS_RGN_CANVAS_INFO_T *ptCanvas, AX_IVPS_GDI_ATTR_T tAttr, AX_IVPS_RECT_T tRect);

    AX_S32 AX_IVPS_Dewarp(const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst, const AX_IVPS_DEWARP_ATTR_T *ptAttr);

    AX_S32 AX_PyraLite_Gen(const AX_PYRA_FRAME_T *tSrcFrame, AX_PYRA_FRAME_T *tDstFrame, AX_BOOL bMaskFlag);
    AX_S32 AX_PyraLite_Rcn(const AX_PYRA_FRAME_T *tSrcFrame, AX_PYRA_FRAME_T *tDstFrame, AX_BOOL bBottom);

    AX_S32 AX_IVPS_GdcWorkCreate(GDC_HANDLE *pGdcHandle);
    AX_S32 AX_IVPS_GdcWorkAttrSet(GDC_HANDLE nGdcHandle, const AX_IVPS_GDC_ATTR_T *ptGdcAttr);
    AX_S32 AX_IVPS_GdcWorkRun(GDC_HANDLE nGdcHandle, const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst);
    AX_S32 AX_IVPS_GdcWorkDestroy(GDC_HANDLE nGdcHandle);

    AX_S32 AX_IVPS_FisheyePointQueryDst2Src(AX_IVPS_POINT_NICE_T *ptSrcPoint, const AX_IVPS_POINT_NICE_T *ptDstPoint, AX_U16 nInputW,
                                            AX_U16 nInputH, AX_U8 nRgnIdx, const AX_IVPS_FISHEYE_ATTR_T *ptFisheyeAttr);
    AX_S32 AX_IVPS_FisheyePointQuerySrc2Dst(AX_IVPS_POINT_NICE_T *ptDstPoint, const AX_IVPS_POINT_NICE_T *ptSrcPoint, AX_U16 nInputW,
                                            AX_U16 nInputH, AX_U8 nRgnIdx, const AX_IVPS_FISHEYE_ATTR_T *ptFisheyeAttr);

private:
    AX_S32 CmmCopy(axcl::pkg::native::IVPS_API api, AX_U64 nSrcPhyAddr, AX_U64 nDstPhyAddr, AX_U64 nMemSize);

    AX_S32 FlipAndRotation(axcl::pkg::native::IVPS_API api, const AX_VIDEO_FRAME_T *ptSrc, AX_IVPS_CHN_FLIP_MODE_E eFlipMode,
                           AX_IVPS_ROTATION_E eRotation, AX_VIDEO_FRAME_T *ptDst);

    AX_S32 Csc(axcl::pkg::native::IVPS_API api, const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst);

    AX_S32 CropResize(axcl::pkg::native::IVPS_API api, const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst,
                      const AX_IVPS_ASPECT_RATIO_T *ptAspectRatio);
    AX_S32 CropResizeV2(axcl::pkg::native::IVPS_API api, const AX_VIDEO_FRAME_T *ptSrc, const AX_IVPS_RECT_T tBox[], AX_U32 nCropNum,
                        AX_VIDEO_FRAME_T *ptDst[], const AX_IVPS_ASPECT_RATIO_T *ptAspectRatio);
    AX_S32 CropResizeV3(axcl::pkg::native::IVPS_API api, const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst[], AX_U32 nNum,
                        const AX_IVPS_ASPECT_RATIO_T *ptAspectRatio);
    AX_S32 CropResizeV4(axcl::pkg::native::IVPS_API api, const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst,
                        const AX_IVPS_ASPECT_RATIO_T *ptAspectRatio, const AX_IVPS_SCALE_STEP_T *ptScaleStep);

    AX_S32 SetScaleCoefLevel(axcl::pkg::native::IVPS_API api, const AX_IVPS_SCALE_RANGE_T *ScaleRange,
                             const AX_IVPS_SCALE_COEF_LEVEL_T *CoefLevel);
    AX_S32 GetScaleCoefLevel(axcl::pkg::native::IVPS_API api, const AX_IVPS_SCALE_RANGE_T *ScaleRange,
                             AX_IVPS_SCALE_COEF_LEVEL_T *CoefLevel);

    AX_S32 AlphaBlending(axcl::pkg::native::IVPS_API api, const AX_VIDEO_FRAME_T *ptSrc, const AX_VIDEO_FRAME_T *ptOverlay,
                         const AX_IVPS_POINT_T tOffset, AX_U8 nAlpha, AX_VIDEO_FRAME_T *ptDst);
    AX_S32 AlphaBlendingV2(axcl::pkg::native::IVPS_API api, const AX_VIDEO_FRAME_T *ptSrc, const AX_VIDEO_FRAME_T *ptOverlay,
                           const AX_IVPS_POINT_T tOffset, const AX_IVPS_ALPHA_LUT_T *ptSpAlpha, AX_VIDEO_FRAME_T *ptDst);
    AX_S32 AlphaBlendingV3(axcl::pkg::native::IVPS_API api, const AX_VIDEO_FRAME_T *ptSrc, const AX_OVERLAY_T *ptOverlay,
                           AX_VIDEO_FRAME_T *ptDst);

    AX_S32 DrawOsd(axcl::pkg::native::IVPS_API api, const AX_VIDEO_FRAME_T *ptFrame, const AX_OSD_BMP_ATTR_T arrBmp[], AX_U32 nNum);
    AX_S32 DrawMosaic(axcl::pkg::native::IVPS_API api, const AX_VIDEO_FRAME_T *ptSrc, AX_IVPS_RGN_MOSAIC_T tMosaic[], AX_U32 nNum);
};
