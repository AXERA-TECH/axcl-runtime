/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "ivps.hpp"
#include "dispatcher.hpp"
#include "axcl_logger.hpp"

#define TAG "ivps"

namespace axcl::worker {

ivps::ivps(dispatcher *dispatcher) : base(dispatcher), m_api(TAG) {
    m_api.append({
        NATIVE_IVPS_API_APPEND(AX_IVPS_Init),
        NATIVE_IVPS_API_APPEND(AX_IVPS_Deinit),
        NATIVE_IVPS_API_APPEND(AX_IVPS_CreateGrp),
        NATIVE_IVPS_API_APPEND(AX_IVPS_CreateGrpEx),
        NATIVE_IVPS_API_APPEND(AX_IVPS_DestoryGrp),
        NATIVE_IVPS_API_APPEND(AX_IVPS_SetPipelineAttr),
        NATIVE_IVPS_API_APPEND(AX_IVPS_GetPipelineAttr),
        NATIVE_IVPS_API_APPEND(AX_IVPS_StartGrp),
        NATIVE_IVPS_API_APPEND(AX_IVPS_StopGrp),
        NATIVE_IVPS_API_APPEND(AX_IVPS_EnableChn),
        NATIVE_IVPS_API_APPEND(AX_IVPS_DisableChn),
        NATIVE_IVPS_API_APPEND(AX_IVPS_SendFrame),
        NATIVE_IVPS_API_APPEND(AX_IVPS_GetChnFrame),
        NATIVE_IVPS_API_APPEND(AX_IVPS_ReleaseChnFrame),
        NATIVE_IVPS_API_APPEND(AX_IVPS_GetGrpFrame),
        NATIVE_IVPS_API_APPEND(AX_IVPS_ReleaseGrpFrame),
        NATIVE_IVPS_API_APPEND(AX_IVPS_GetChnFd),
        NATIVE_IVPS_API_APPEND(AX_IVPS_GetDebugFifoFrame),
        NATIVE_IVPS_API_APPEND(AX_IVPS_ReleaseDebugFifoFrame),
        NATIVE_IVPS_API_APPEND(AX_IVPS_CloseAllFd),
        NATIVE_IVPS_API_APPEND(AX_IVPS_SetGrpLDCAttr),
        NATIVE_IVPS_API_APPEND(AX_IVPS_GetGrpLDCAttr),
        NATIVE_IVPS_API_APPEND(AX_IVPS_SetChnLDCAttr),
        NATIVE_IVPS_API_APPEND(AX_IVPS_GetChnLDCAttr),
        NATIVE_IVPS_API_APPEND(AX_IVPS_SetGrpPoolAttr),
        NATIVE_IVPS_API_APPEND(AX_IVPS_SetChnPoolAttr),
        NATIVE_IVPS_API_APPEND(AX_IVPS_SetGrpUserFRC),
        NATIVE_IVPS_API_APPEND(AX_IVPS_SetChnUserFRC),
        NATIVE_IVPS_API_APPEND(AX_IVPS_SetGrpCrop),
        NATIVE_IVPS_API_APPEND(AX_IVPS_GetGrpCrop),
        NATIVE_IVPS_API_APPEND(AX_IVPS_SetChnAttr),
        NATIVE_IVPS_API_APPEND(AX_IVPS_GetChnAttr),
        NATIVE_IVPS_API_APPEND(AX_IVPS_EnableBackupFrame),
        NATIVE_IVPS_API_APPEND(AX_IVPS_DisableBackupFrame),
        NATIVE_IVPS_API_APPEND(AX_IVPS_ResetGrp),
        NATIVE_IVPS_API_APPEND(AX_IVPS_GetEngineDutyCycle),
        NATIVE_IVPS_API_APPEND(AX_IVPS_RGN_Create),
        NATIVE_IVPS_API_APPEND(AX_IVPS_RGN_Destroy),
        NATIVE_IVPS_API_APPEND(AX_IVPS_RGN_AttachToFilter),
        NATIVE_IVPS_API_APPEND(AX_IVPS_RGN_DetachFromFilter),
        NATIVE_IVPS_API_APPEND(AX_IVPS_RGN_Update),
        NATIVE_IVPS_API_APPEND(AX_IVPS_CmmCopyTdp),
        NATIVE_IVPS_API_APPEND(AX_IVPS_FlipAndRotationTdp),
        NATIVE_IVPS_API_APPEND(AX_IVPS_CscTdp),
        NATIVE_IVPS_API_APPEND(AX_IVPS_CropResizeTdp),
        NATIVE_IVPS_API_APPEND(AX_IVPS_CropResizeV2Tdp),
        NATIVE_IVPS_API_APPEND(AX_IVPS_AlphaBlendingTdp),
        NATIVE_IVPS_API_APPEND(AX_IVPS_AlphaBlendingV3Tdp),
        NATIVE_IVPS_API_APPEND(AX_IVPS_DrawOsdTdp),
        NATIVE_IVPS_API_APPEND(AX_IVPS_DrawMosaicTdp),
        NATIVE_IVPS_API_APPEND(AX_IVPS_CmmCopyVpp),
        NATIVE_IVPS_API_APPEND(AX_IVPS_CropResizeVpp),
        NATIVE_IVPS_API_APPEND(AX_IVPS_CropResizeV2Vpp),
        NATIVE_IVPS_API_APPEND(AX_IVPS_CropResizeV3Vpp),
        NATIVE_IVPS_API_APPEND(AX_IVPS_CscVpp),
        NATIVE_IVPS_API_APPEND(AX_IVPS_DrawMosaicVpp),
        NATIVE_IVPS_API_APPEND(AX_IVPS_SetScaleCoefLevelVpp),
        NATIVE_IVPS_API_APPEND(AX_IVPS_GetScaleCoefLevelVpp),
        NATIVE_IVPS_API_APPEND(AX_IVPS_CmmCopyVgp),
        NATIVE_IVPS_API_APPEND(AX_IVPS_CscVgp),
        NATIVE_IVPS_API_APPEND(AX_IVPS_CropResizeVgp),
        NATIVE_IVPS_API_APPEND(AX_IVPS_CropResizeV2Vgp),
        NATIVE_IVPS_API_APPEND(AX_IVPS_CropResizeV4Vgp),
        NATIVE_IVPS_API_APPEND(AX_IVPS_AlphaBlendingVgp),
        NATIVE_IVPS_API_APPEND(AX_IVPS_AlphaBlendingV2Vgp),
        NATIVE_IVPS_API_APPEND(AX_IVPS_AlphaBlendingV3Vgp),
        NATIVE_IVPS_API_APPEND(AX_IVPS_DrawOsdVgp),
        NATIVE_IVPS_API_APPEND(AX_IVPS_DrawMosaicVgp),
        NATIVE_IVPS_API_APPEND(AX_IVPS_SetScaleCoefLevelVgp),
        NATIVE_IVPS_API_APPEND(AX_IVPS_GetScaleCoefLevelVgp),
        NATIVE_IVPS_API_APPEND(AX_IVPS_DrawLine),
        NATIVE_IVPS_API_APPEND(AX_IVPS_DrawPolygon),
        NATIVE_IVPS_API_APPEND(AX_IVPS_DrawRect),
        NATIVE_IVPS_API_APPEND(AX_IVPS_Dewarp),
        NATIVE_IVPS_API_APPEND(AX_PyraLite_Gen),
        NATIVE_IVPS_API_APPEND(AX_PyraLite_Rcn),
        NATIVE_IVPS_API_APPEND(AX_IVPS_GdcWorkCreate),
        NATIVE_IVPS_API_APPEND(AX_IVPS_GdcWorkAttrSet),
        NATIVE_IVPS_API_APPEND(AX_IVPS_GdcWorkRun),
        NATIVE_IVPS_API_APPEND(AX_IVPS_GdcWorkDestroy),
        NATIVE_IVPS_API_APPEND(AX_IVPS_FisheyePointQueryDst2Src),
        NATIVE_IVPS_API_APPEND(AX_IVPS_FisheyePointQuerySrc2Dst),
    });
}

void ivps::run(const void *data, const size_t& size) {
    m_api.run(get_dispatcher(), data, size);
}

NATIVE_IVPS_API_IMPL(AX_IVPS_Init) {
    if (!response->unpack(response->get_api())) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_Init();

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_Deinit) {
    if (!response->unpack(response->get_api())) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_Deinit();

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_CreateGrp) {
    IVPS_GRP IvpsGrp;
    AX_IVPS_GRP_ATTR_T tGrpAttr;

    if (!response->unpack(response->get_api(), &IvpsGrp, &tGrpAttr)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_CreateGrp(IvpsGrp, &tGrpAttr);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_CreateGrpEx) {
    AX_IVPS_GRP_ATTR_T tGrpAttr;

    if (!response->unpack(response->get_api(), &tGrpAttr)) {
        return false;
    }

    IVPS_GRP IvpsGrp;
    AX_S32 ret = ::AX_IVPS_CreateGrpEx(&IvpsGrp, &tGrpAttr);

    if (!request->pack(response->get_api(), ret, IvpsGrp)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_DestoryGrp) {
    IVPS_GRP IvpsGrp;

    if (!response->unpack(response->get_api(), &IvpsGrp)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_DestoryGrp(IvpsGrp);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_SetPipelineAttr) {
    IVPS_GRP IvpsGrp;
    AX_IVPS_PIPELINE_ATTR_T tPipelineAttr;

    if (!response->unpack(response->get_api(), &IvpsGrp, &tPipelineAttr)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_SetPipelineAttr(IvpsGrp, &tPipelineAttr);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_GetPipelineAttr) {
    IVPS_GRP IvpsGrp;

    if (!response->unpack(response->get_api(), &IvpsGrp)) {
        return false;
    }

    AX_IVPS_PIPELINE_ATTR_T tPipelineAttr;
    AX_S32 ret = ::AX_IVPS_GetPipelineAttr(IvpsGrp, &tPipelineAttr);

    if (!request->pack(response->get_api(), ret, &tPipelineAttr)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_StartGrp) {
    IVPS_GRP IvpsGrp;

    if (!response->unpack(response->get_api(), &IvpsGrp)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_StartGrp(IvpsGrp);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_StopGrp) {
    IVPS_GRP IvpsGrp;

    if (!response->unpack(response->get_api(), &IvpsGrp)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_StopGrp(IvpsGrp);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_EnableChn) {
    IVPS_GRP IvpsGrp;
    IVPS_CHN IvpsChn;

    if (!response->unpack(response->get_api(), &IvpsGrp, &IvpsChn)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_EnableChn(IvpsGrp, IvpsChn);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_DisableChn) {
    IVPS_GRP IvpsGrp;
    IVPS_CHN IvpsChn;

    if (!response->unpack(response->get_api(), &IvpsGrp, &IvpsChn)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_DisableChn(IvpsGrp, IvpsChn);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_SendFrame) {
    IVPS_GRP IvpsGrp;
    AX_VIDEO_FRAME_T tFrame;
    AX_S32 nMilliSec;

    if (!response->unpack(response->get_api(), &IvpsGrp, &tFrame, &nMilliSec)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_SendFrame(IvpsGrp, &tFrame, nMilliSec);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_GetChnFrame) {
    IVPS_GRP IvpsGrp;
    IVPS_CHN IvpsChn;
    AX_S32 nMilliSec;

    if (!response->unpack(response->get_api(), &IvpsGrp, &IvpsChn, &nMilliSec)) {
        return false;
    }

    AX_VIDEO_FRAME_T tFrame;
    AX_S32 ret = ::AX_IVPS_GetChnFrame(IvpsGrp, IvpsChn, &tFrame, nMilliSec);

    if (!request->pack(response->get_api(), ret, &tFrame)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_ReleaseChnFrame) {
    IVPS_GRP IvpsGrp;
    IVPS_CHN IvpsChn;
    AX_VIDEO_FRAME_T tFrame;

    if (!response->unpack(response->get_api(), &IvpsGrp, &IvpsChn, &tFrame)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_ReleaseChnFrame(IvpsGrp, IvpsChn, &tFrame);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_GetGrpFrame) {
    IVPS_GRP IvpsGrp;
    AX_S32 nMilliSec;

    if (!response->unpack(response->get_api(), &IvpsGrp, &nMilliSec)) {
        return false;
    }

    AX_VIDEO_FRAME_T tFrame;
    AX_S32 ret = ::AX_IVPS_GetGrpFrame(IvpsGrp, &tFrame, nMilliSec);

    if (!request->pack(response->get_api(), ret, &tFrame)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_ReleaseGrpFrame) {
    IVPS_GRP IvpsGrp;
    AX_VIDEO_FRAME_T tFrame;

    if (!response->unpack(response->get_api(), &IvpsGrp, &tFrame)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_ReleaseGrpFrame(IvpsGrp, &tFrame);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_GetChnFd) {
    IVPS_GRP IvpsGrp;
    IVPS_CHN IvpsChn;

    if (!response->unpack(response->get_api(), &IvpsGrp, &IvpsChn)) {
        return false;
    }

    AX_S32 fd = ::AX_IVPS_GetChnFd(IvpsGrp, IvpsChn);

    if (!request->pack(response->get_api(), fd)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_GetDebugFifoFrame) {
    IVPS_GRP IvpsGrp;

    if (!response->unpack(response->get_api(), &IvpsGrp)) {
        return false;
    }

    AX_VIDEO_FRAME_T tFrame;
    AX_S32 ret = ::AX_IVPS_GetDebugFifoFrame(IvpsGrp, &tFrame);

    if (!request->pack(response->get_api(), ret, &tFrame)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_ReleaseDebugFifoFrame) {
    IVPS_GRP IvpsGrp;
    AX_VIDEO_FRAME_T tFrame;

    if (!response->unpack(response->get_api(), &IvpsGrp, &tFrame)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_ReleaseDebugFifoFrame(IvpsGrp, &tFrame);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_CloseAllFd) {
    if (!response->unpack(response->get_api())) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_CloseAllFd();

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_SetGrpLDCAttr) {
    IVPS_GRP IvpsGrp;
    IVPS_FILTER IvpsFilter;
    AX_IVPS_LDC_ATTR_T tLDCAttr;

    if (!response->unpack(response->get_api(), &IvpsGrp, &IvpsFilter, &tLDCAttr)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_SetGrpLDCAttr(IvpsGrp, IvpsFilter, &tLDCAttr);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_GetGrpLDCAttr) {
    IVPS_GRP IvpsGrp;
    IVPS_FILTER IvpsFilter;

    if (!response->unpack(response->get_api(), &IvpsGrp, &IvpsFilter)) {
        return false;
    }

    AX_IVPS_LDC_ATTR_T tLDCAttr;
    AX_S32 ret = ::AX_IVPS_GetGrpLDCAttr(IvpsGrp, IvpsFilter, &tLDCAttr);

    if (!request->pack(response->get_api(), ret, &tLDCAttr)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_SetChnLDCAttr) {
    IVPS_GRP IvpsGrp;
    IVPS_CHN IvpsChn;
    IVPS_FILTER IvpsFilter;
    AX_IVPS_LDC_ATTR_T tLDCAttr;

    if (!response->unpack(response->get_api(), &IvpsGrp, &IvpsChn, &IvpsFilter, &tLDCAttr)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_SetChnLDCAttr(IvpsGrp, IvpsChn, IvpsFilter, &tLDCAttr);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_GetChnLDCAttr) {
    IVPS_GRP IvpsGrp;
    IVPS_CHN IvpsChn;
    IVPS_FILTER IvpsFilter;

    if (!response->unpack(response->get_api(), &IvpsGrp, &IvpsChn, &IvpsFilter)) {
        return false;
    }

    AX_IVPS_LDC_ATTR_T tLDCAttr;
    AX_S32 ret = ::AX_IVPS_SetChnLDCAttr(IvpsGrp, IvpsChn, IvpsFilter, &tLDCAttr);

    if (!request->pack(response->get_api(), ret, &tLDCAttr)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_SetGrpPoolAttr) {
    IVPS_GRP IvpsGrp;
    AX_IVPS_POOL_ATTR_T tPoolAttr;

    if (!response->unpack(response->get_api(), &IvpsGrp, &tPoolAttr)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_SetGrpPoolAttr(IvpsGrp, &tPoolAttr);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_SetChnPoolAttr) {
    IVPS_GRP IvpsGrp;
    IVPS_CHN IvpsChn;
    AX_IVPS_POOL_ATTR_T tPoolAttr;

    if (!response->unpack(response->get_api(), &IvpsGrp, &IvpsChn, &tPoolAttr)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_SetChnPoolAttr(IvpsGrp, IvpsChn, &tPoolAttr);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_SetGrpUserFRC) {
    IVPS_GRP IvpsGrp;
    AX_IVPS_USER_FRAME_RATE_CTRL_T tFrameRateAttr;

    if (!response->unpack(response->get_api(), &IvpsGrp, &tFrameRateAttr)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_SetGrpUserFRC(IvpsGrp, &tFrameRateAttr);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_SetChnUserFRC) {
    IVPS_GRP IvpsGrp;
    IVPS_CHN IvpsChn;
    AX_IVPS_USER_FRAME_RATE_CTRL_T tFrameRateAttr;

    if (!response->unpack(response->get_api(), &IvpsGrp, &IvpsChn, &tFrameRateAttr)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_SetChnUserFRC(IvpsGrp, IvpsChn, &tFrameRateAttr);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_SetGrpCrop) {
    IVPS_GRP IvpsGrp;
    AX_IVPS_CROP_INFO_T tCropInfo;

    if (!response->unpack(response->get_api(), &IvpsGrp, &tCropInfo)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_SetGrpCrop(IvpsGrp, &tCropInfo);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_GetGrpCrop) {
    IVPS_GRP IvpsGrp;

    if (!response->unpack(response->get_api(), &IvpsGrp)) {
        return false;
    }

    AX_IVPS_CROP_INFO_T tCropInfo;
    AX_S32 ret = ::AX_IVPS_GetGrpCrop(IvpsGrp, &tCropInfo);

    if (!request->pack(response->get_api(), ret, &tCropInfo)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_SetChnAttr) {
    IVPS_GRP IvpsGrp;
    IVPS_CHN IvpsChn;
    IVPS_FILTER IvpsFilter;
    AX_IVPS_CHN_ATTR_T tChnAttr;

    if (!response->unpack(response->get_api(), &IvpsGrp, &IvpsChn, &IvpsFilter, &tChnAttr)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_SetChnAttr(IvpsGrp, IvpsChn, IvpsFilter, &tChnAttr);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_GetChnAttr) {
    IVPS_GRP IvpsGrp;
    IVPS_CHN IvpsChn;
    IVPS_FILTER IvpsFilter;

    if (!response->unpack(response->get_api(), &IvpsGrp, &IvpsChn, &IvpsFilter)) {
        return false;
    }

    AX_IVPS_CHN_ATTR_T tChnAttr;
    AX_S32 ret = ::AX_IVPS_GetChnAttr(IvpsGrp, IvpsChn, IvpsFilter, &tChnAttr);

    if (!request->pack(response->get_api(), ret, &tChnAttr)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_EnableBackupFrame) {
    IVPS_GRP IvpsGrp;
    AX_U8 nFifoDepth;

    if (!response->unpack(response->get_api(), &IvpsGrp, &nFifoDepth)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_EnableBackupFrame(IvpsGrp, nFifoDepth);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_DisableBackupFrame) {
    IVPS_GRP IvpsGrp;

    if (!response->unpack(response->get_api(), &IvpsGrp)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_DisableBackupFrame(IvpsGrp);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_ResetGrp) {
    IVPS_GRP IvpsGrp;

    if (!response->unpack(response->get_api(), &IvpsGrp)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_ResetGrp(IvpsGrp);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_GetEngineDutyCycle) {
    AX_IVPS_DUTY_CYCLE_ATTR_T tDutyCycle;

    if (!response->unpack(response->get_api(), &tDutyCycle)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_GetEngineDutyCycle(&tDutyCycle);

    if (!request->pack(response->get_api(), ret, &tDutyCycle)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_RGN_Create) {
    if (!response->unpack(response->get_api())) {
        return false;
    }

    IVPS_RGN_HANDLE hRegion = ::AX_IVPS_RGN_Create();

    if (!request->pack(response->get_api(), hRegion)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_RGN_Destroy) {
    IVPS_RGN_HANDLE hRegion;

    if (!response->unpack(response->get_api(), &hRegion)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_RGN_Destroy(hRegion);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_RGN_AttachToFilter) {
    IVPS_RGN_HANDLE hRegion;
    IVPS_GRP IvpsGrp;
    IVPS_FILTER IvpsFilter;

    if (!response->unpack(response->get_api(), &hRegion, &IvpsGrp, &IvpsFilter)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_RGN_AttachToFilter(hRegion, IvpsGrp, IvpsFilter);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_RGN_DetachFromFilter) {
    IVPS_RGN_HANDLE hRegion;
    IVPS_GRP IvpsGrp;
    IVPS_FILTER IvpsFilter;

    if (!response->unpack(response->get_api(), &hRegion, &IvpsGrp, &IvpsFilter)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_RGN_DetachFromFilter(hRegion, IvpsGrp, IvpsFilter);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_RGN_Update) {
    IVPS_RGN_HANDLE hRegion;
    AX_IVPS_RGN_DISP_GROUP_T tDisp;

    if (!response->unpack(response->get_api(), &hRegion, &tDisp)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_RGN_Update(hRegion, &tDisp);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_CmmCopyTdp) {
    AX_U64 nSrcPhyAddr;
    AX_U64 nDstPhyAddr;
    AX_U64 nMemSize;

    if (!response->unpack(response->get_api(), &nSrcPhyAddr, &nDstPhyAddr, &nMemSize)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_CmmCopyTdp(nSrcPhyAddr, nDstPhyAddr, nMemSize);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_FlipAndRotationTdp) {
    AX_VIDEO_FRAME_T tSrc;
    AX_IVPS_CHN_FLIP_MODE_E eFlipMode;
    AX_IVPS_ROTATION_E eRotation;
    AX_VIDEO_FRAME_T tDst;

    if (!response->unpack(response->get_api(), &tSrc, &eFlipMode, &eRotation, &tDst)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_FlipAndRotationTdp(&tSrc, eFlipMode, eRotation, &tDst);

    if (!request->pack(response->get_api(), ret, &tDst)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_CscTdp) {
    AX_VIDEO_FRAME_T tSrc;
    AX_VIDEO_FRAME_T tDst;

    if (!response->unpack(response->get_api(), &tSrc, &tDst)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_CscTdp(&tSrc, &tDst);

    if (!request->pack(response->get_api(), ret, &tDst)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_CropResizeTdp) {
    AX_VIDEO_FRAME_T tSrc;
    AX_VIDEO_FRAME_T tDst;
    AX_IVPS_ASPECT_RATIO_T tAspectRatio;

    if (!response->unpack(response->get_api(), &tSrc, &tDst, &tAspectRatio)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_CropResizeTdp(&tSrc, &tDst, &tAspectRatio);

    if (!request->pack(response->get_api(), ret, &tDst)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_CropResizeV2Tdp) {
    AX_VIDEO_FRAME_T tSrc;
    AX_U32 nCropNum;
    AX_IVPS_ASPECT_RATIO_T tAspectRatio;
    axcl::pkg::uint8_array adst;
    axcl::pkg::uint8_array abox;

    if (!response->unpack(response->get_api(), &tSrc, &abox, &nCropNum, &adst, &tAspectRatio)) {
        return false;
    }

    auto spastDst = unpackArray2PArray<AX_VIDEO_FRAME_T>(adst);
    if (!spastDst) {
        return false;
    }
    AX_IVPS_RECT_T *pastSrcBoxs = reinterpret_cast<AX_IVPS_RECT_T *>(abox.data);

    AX_S32 ret = ::AX_IVPS_CropResizeV2Tdp(&tSrc, (const AX_IVPS_RECT_T *)pastSrcBoxs, nCropNum, spastDst.get(), &tAspectRatio);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_AlphaBlendingTdp) {
    AX_VIDEO_FRAME_T tSrc;
    AX_VIDEO_FRAME_T tOverlay;
    AX_IVPS_POINT_T tOffset;
    AX_U8 nAlpha;
    AX_VIDEO_FRAME_T tDst;

    if (!response->unpack(response->get_api(), &tSrc, &tOverlay, &tOffset, &nAlpha, &tDst)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_AlphaBlendingTdp(&tSrc, &tOverlay, tOffset, nAlpha, &tDst);

    if (!request->pack(response->get_api(), ret, &tDst)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_AlphaBlendingV3Tdp) {
    AX_VIDEO_FRAME_T tSrc;
    AX_OVERLAY_T tOverlay;
    AX_VIDEO_FRAME_T tDst;

    if (!response->unpack(response->get_api(), &tSrc, &tOverlay, &tDst)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_AlphaBlendingV3Tdp(&tSrc, &tOverlay, &tDst);

    if (!request->pack(response->get_api(), ret, &tDst)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_DrawOsdTdp) {
    AX_VIDEO_FRAME_T tFrame;
    AX_U32 nNum;
    axcl::pkg::uint8_array bmps;

    if (!response->unpack(response->get_api(), &tFrame, &bmps, &nNum)) {
        return false;
    }

    AX_OSD_BMP_ATTR_T *paBmps = reinterpret_cast<AX_OSD_BMP_ATTR_T *>(bmps.data);

    AX_S32 ret = ::AX_IVPS_DrawOsdTdp(&tFrame, (const AX_OSD_BMP_ATTR_T *)paBmps, nNum);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_DrawMosaicTdp) {
    AX_VIDEO_FRAME_T tFrame;
    AX_U32 nNum;
    axcl::pkg::uint8_array mosaics;

    if (!response->unpack(response->get_api(), &tFrame, &mosaics, &nNum)) {
        return false;
    }

    AX_IVPS_RGN_MOSAIC_T *paMosaics = reinterpret_cast<AX_IVPS_RGN_MOSAIC_T *>(mosaics.data);

    AX_S32 ret = ::AX_IVPS_DrawMosaicTdp(&tFrame, paMosaics, nNum);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_CmmCopyVpp) {
    AX_U64 nSrcPhyAddr;
    AX_U64 nDstPhyAddr;
    AX_U64 nMemSize;

    if (!response->unpack(response->get_api(), &nSrcPhyAddr, &nDstPhyAddr, &nMemSize)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_CmmCopyVpp(nSrcPhyAddr, nDstPhyAddr, nMemSize);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_CropResizeVpp) {
    AX_VIDEO_FRAME_T tSrc;
    AX_VIDEO_FRAME_T tDst;
    AX_IVPS_ASPECT_RATIO_T tAspectRatio;

    if (!response->unpack(response->get_api(), &tSrc, &tDst, &tAspectRatio)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_CropResizeVpp(&tSrc, &tDst, &tAspectRatio);

    if (!request->pack(response->get_api(), ret, &tDst)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_CropResizeV2Vpp) {
    AX_VIDEO_FRAME_T tSrc;
    AX_U32 nCropNum;
    AX_IVPS_ASPECT_RATIO_T tAspectRatio;
    axcl::pkg::uint8_array adst;
    axcl::pkg::uint8_array abox;

    if (!response->unpack(response->get_api(), &tSrc, &abox, &nCropNum, &adst, &tAspectRatio)) {
        return false;
    }

    auto spastDst = unpackArray2PArray<AX_VIDEO_FRAME_T>(adst);
    if (!spastDst) {
        return false;
    }
    AX_IVPS_RECT_T *pastSrcBoxs = reinterpret_cast<AX_IVPS_RECT_T *>(abox.data);

    AX_S32 ret = ::AX_IVPS_CropResizeV2Vpp(&tSrc, (const AX_IVPS_RECT_T *)pastSrcBoxs, nCropNum, spastDst.get(), &tAspectRatio);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }
    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_CropResizeV3Vpp) {
    AX_VIDEO_FRAME_T tSrc;
    AX_U32 nNum;
    AX_IVPS_ASPECT_RATIO_T tAspectRatio;
    axcl::pkg::uint8_array adst;

    if (!response->unpack(response->get_api(), &tSrc, &adst, &nNum, &tAspectRatio)) {
        return false;
    }

    auto spastDst = unpackArray2PArray<AX_VIDEO_FRAME_T>(adst);
    if (!spastDst) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_CropResizeV3Vpp(&tSrc, spastDst.get(), nNum, &tAspectRatio);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_CscVpp) {
    AX_VIDEO_FRAME_T tSrc;
    AX_VIDEO_FRAME_T tDst;

    if (!response->unpack(response->get_api(), &tSrc, &tDst)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_CscVpp(&tSrc, &tDst);

    if (!request->pack(response->get_api(), ret, &tDst)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_DrawMosaicVpp) {
    AX_VIDEO_FRAME_T tFrame;
    AX_U32 nNum;
    axcl::pkg::uint8_array mosaics;

    if (!response->unpack(response->get_api(), &tFrame, &mosaics, &nNum)) {
        return false;
    }

    AX_IVPS_RGN_MOSAIC_T *paMosaics = reinterpret_cast<AX_IVPS_RGN_MOSAIC_T *>(mosaics.data);

    AX_S32 ret = ::AX_IVPS_DrawMosaicVpp(&tFrame, paMosaics, nNum);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_SetScaleCoefLevelVpp) {
    AX_IVPS_SCALE_RANGE_T ScaleRange;
    AX_IVPS_SCALE_COEF_LEVEL_T CoefLevel;

    if (!response->unpack(response->get_api(), &ScaleRange, &CoefLevel)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_SetScaleCoefLevelVpp(&ScaleRange, &CoefLevel);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_GetScaleCoefLevelVpp) {
    AX_IVPS_SCALE_RANGE_T ScaleRange;

    if (!response->unpack(response->get_api(), &ScaleRange)) {
        return false;
    }

    AX_IVPS_SCALE_COEF_LEVEL_T CoefLevel;
    AX_S32 ret = ::AX_IVPS_GetScaleCoefLevelVpp(&ScaleRange, &CoefLevel);

    if (!request->pack(response->get_api(), ret, &CoefLevel)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_CmmCopyVgp) {
    AX_U64 nSrcPhyAddr;
    AX_U64 nDstPhyAddr;
    AX_U64 nMemSize;

    if (!response->unpack(response->get_api(), &nSrcPhyAddr, &nDstPhyAddr, &nMemSize)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_CmmCopyVgp(nSrcPhyAddr, nDstPhyAddr, nMemSize);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_CscVgp) {
    AX_VIDEO_FRAME_T tSrc;
    AX_VIDEO_FRAME_T tDst;

    if (!response->unpack(response->get_api(), &tSrc, &tDst)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_CscVgp(&tSrc, &tDst);

    if (!request->pack(response->get_api(), ret, &tDst)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_CropResizeVgp) {
    AX_VIDEO_FRAME_T tSrc;
    AX_VIDEO_FRAME_T tDst;
    AX_IVPS_ASPECT_RATIO_T tAspectRatio;

    if (!response->unpack(response->get_api(), &tSrc, &tDst, &tAspectRatio)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_CropResizeVgp(&tSrc, &tDst, &tAspectRatio);

    if (!request->pack(response->get_api(), ret, &tDst)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_CropResizeV2Vgp) {
    AX_VIDEO_FRAME_T tSrc;
    AX_U32 nCropNum;
    AX_IVPS_ASPECT_RATIO_T tAspectRatio;
    axcl::pkg::uint8_array adst;
    axcl::pkg::uint8_array abox;

    if (!response->unpack(response->get_api(), &tSrc, &abox, &nCropNum, &adst, &tAspectRatio)) {
        return false;
    }
    auto spastDst = unpackArray2PArray<AX_VIDEO_FRAME_T>(adst);
    if (!spastDst) {
        return false;
    }
    AX_IVPS_RECT_T *pastSrcBoxs = reinterpret_cast<AX_IVPS_RECT_T *>(abox.data);

    AX_S32 ret = ::AX_IVPS_CropResizeV2Vgp(&tSrc, (const AX_IVPS_RECT_T *)pastSrcBoxs, nCropNum, spastDst.get(), &tAspectRatio);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_CropResizeV4Vgp) {
    AX_VIDEO_FRAME_T tSrc;
    AX_VIDEO_FRAME_T tDst;
    AX_IVPS_ASPECT_RATIO_T tAspectRatio;
    AX_IVPS_SCALE_STEP_T tScaleStep;

    if (!response->unpack(response->get_api(), &tSrc, &tDst, &tAspectRatio, &tScaleStep)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_CropResizeV4Vgp(&tSrc, &tDst, &tAspectRatio, &tScaleStep);

    if (!request->pack(response->get_api(), ret, &tDst)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_AlphaBlendingVgp) {
    AX_VIDEO_FRAME_T tSrc;
    AX_VIDEO_FRAME_T tOverlay;
    AX_IVPS_POINT_T tOffset;
    AX_U8 nAlpha;
    AX_VIDEO_FRAME_T tDst;

    if (!response->unpack(response->get_api(), &tSrc, &tOverlay, &tOffset, &nAlpha, &tDst)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_AlphaBlendingVgp(&tSrc, &tOverlay, tOffset, nAlpha, &tDst);

    if (!request->pack(response->get_api(), ret, &tDst)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_AlphaBlendingV2Vgp) {
    AX_VIDEO_FRAME_T tSrc;
    AX_VIDEO_FRAME_T tOverlay;
    AX_IVPS_POINT_T tOffset;
    AX_IVPS_ALPHA_LUT_T tSpAlpha;
    AX_VIDEO_FRAME_T tDst;

    if (!response->unpack(response->get_api(), &tSrc, &tOverlay, &tOffset, &tSpAlpha, &tDst)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_AlphaBlendingV2Vgp(&tSrc, &tOverlay, tOffset, &tSpAlpha, &tDst);

    if (!request->pack(response->get_api(), ret, &tDst)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_AlphaBlendingV3Vgp) {
    AX_VIDEO_FRAME_T tSrc;
    AX_OVERLAY_T tOverlay;
    AX_VIDEO_FRAME_T tDst;

    if (!response->unpack(response->get_api(), &tSrc, &tOverlay, &tDst)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_AlphaBlendingV3Vgp(&tSrc, &tOverlay, &tDst);

    if (!request->pack(response->get_api(), ret, &tDst)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_DrawOsdVgp) {
    AX_VIDEO_FRAME_T tFrame;
    AX_U32 nNum;
    axcl::pkg::uint8_array bmps;

    if (!response->unpack(response->get_api(), &tFrame, &bmps, &nNum)) {
        return false;
    }

    AX_OSD_BMP_ATTR_T *paBmps = reinterpret_cast<AX_OSD_BMP_ATTR_T *>(bmps.data);

    AX_S32 ret = ::AX_IVPS_DrawOsdVgp(&tFrame, (const AX_OSD_BMP_ATTR_T *)paBmps, nNum);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_DrawMosaicVgp) {
    AX_VIDEO_FRAME_T tFrame;
    AX_U32 nNum;
    axcl::pkg::uint8_array mosaics;

    if (!response->unpack(response->get_api(), &tFrame, &mosaics, &nNum)) {
        return false;
    }

    AX_IVPS_RGN_MOSAIC_T *paMosaics = reinterpret_cast<AX_IVPS_RGN_MOSAIC_T *>(mosaics.data);

    AX_S32 ret = ::AX_IVPS_DrawMosaicVgp(&tFrame, paMosaics, nNum);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_SetScaleCoefLevelVgp) {
    AX_IVPS_SCALE_RANGE_T ScaleRange;
    AX_IVPS_SCALE_COEF_LEVEL_T CoefLevel;

    if (!response->unpack(response->get_api(), &ScaleRange, &CoefLevel)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_SetScaleCoefLevelVgp(&ScaleRange, &CoefLevel);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_GetScaleCoefLevelVgp) {
    AX_IVPS_SCALE_RANGE_T ScaleRange;

    if (!response->unpack(response->get_api(), &ScaleRange)) {
        return false;
    }

    AX_IVPS_SCALE_COEF_LEVEL_T CoefLevel;
    AX_S32 ret = ::AX_IVPS_GetScaleCoefLevelVgp(&ScaleRange, &CoefLevel);

    if (!request->pack(response->get_api(), ret, &CoefLevel)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_DrawLine) {
    AX_IVPS_RGN_CANVAS_INFO_T tCanvas;
    AX_IVPS_GDI_ATTR_T tAttr;
    AX_U32 nPointNum;
    axcl::pkg::uint8_array pts;

    if (!response->unpack(response->get_api(), &tCanvas, &tAttr, &pts, &nPointNum)) {
        return false;
    }

    AX_IVPS_POINT_T *ptPoints = reinterpret_cast<AX_IVPS_POINT_T *>(pts.data);

    AX_S32 ret = ::AX_IVPS_DrawLine(&tCanvas, tAttr, ptPoints, nPointNum);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_DrawPolygon) {
    AX_IVPS_RGN_CANVAS_INFO_T tCanvas;
    AX_IVPS_GDI_ATTR_T tAttr;
    AX_U32 nPointNum;
    axcl::pkg::uint8_array pts;

    if (!response->unpack(response->get_api(), &tCanvas, &tAttr, &pts, &nPointNum)) {
        return false;
    }

    AX_IVPS_POINT_T *ptPoints = reinterpret_cast<AX_IVPS_POINT_T *>(pts.data);

    AX_S32 ret = ::AX_IVPS_DrawPolygon(&tCanvas, tAttr, ptPoints, nPointNum);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_DrawRect) {
    AX_IVPS_RGN_CANVAS_INFO_T tCanvas;
    AX_IVPS_GDI_ATTR_T tAttr;
    AX_IVPS_RECT_T tRect;

    if (!response->unpack(response->get_api(), &tCanvas, &tAttr, &tRect)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_DrawRect(&tCanvas, tAttr, tRect);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_Dewarp) {
    AX_VIDEO_FRAME_T tSrc;
    AX_VIDEO_FRAME_T tDst;
    AX_IVPS_DEWARP_ATTR_T tAttr;

    if (!response->unpack(response->get_api(), &tSrc, &tDst, &tAttr)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_Dewarp(&tSrc, &tDst, &tAttr);

    if (!request->pack(response->get_api(), ret, &tDst)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_PyraLite_Gen) {
    AX_PYRA_FRAME_T tSrcFrame;
    AX_PYRA_FRAME_T tDstFrame;
    AX_BOOL bMaskFlag;

    if (!response->unpack(response->get_api(), &tSrcFrame, &tDstFrame, &bMaskFlag)) {
        return false;
    }

    AX_S32 ret = ::AX_PyraLite_Gen(&tSrcFrame, &tDstFrame, bMaskFlag);

    if (!request->pack(response->get_api(), ret, &tDstFrame)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_PyraLite_Rcn) {
    AX_PYRA_FRAME_T tSrcFrame;
    AX_PYRA_FRAME_T tDstFrame;
    AX_BOOL bBottom;

    if (!response->unpack(response->get_api(), &tSrcFrame, &tDstFrame, &bBottom)) {
        return false;
    }

    AX_S32 ret = ::AX_PyraLite_Rcn(&tSrcFrame, &tDstFrame, bBottom);

    if (!request->pack(response->get_api(), ret, &tDstFrame)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_GdcWorkCreate) {
    GDC_HANDLE GdcHandle;

    if (!response->unpack(response->get_api(), &GdcHandle)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_GdcWorkCreate(&GdcHandle);

    if (!request->pack(response->get_api(), ret, GdcHandle)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_GdcWorkAttrSet) {
    GDC_HANDLE nGdcHandle;
    AX_IVPS_GDC_ATTR_T tGdcAttr;
    axcl::pkg::uint8_array user_map;

    if (!response->unpack(response->get_api(), &nGdcHandle, &tGdcAttr, &user_map)) {
        return false;
    }

    if (AX_IVPS_GDC_MAP_USER == tGdcAttr.eGdcType) {
        tGdcAttr.tMapUserAttr.pUserMap = reinterpret_cast<AX_S32 *>(user_map.data);
    }

    AX_S32 ret = ::AX_IVPS_GdcWorkAttrSet(nGdcHandle, &tGdcAttr);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_GdcWorkRun) {
    GDC_HANDLE nGdcHandle;
    AX_VIDEO_FRAME_T tSrc;
    AX_VIDEO_FRAME_T tDst;

    if (!response->unpack(response->get_api(), &nGdcHandle, &tSrc, &tDst)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_GdcWorkRun(nGdcHandle, &tSrc, &tDst);

    if (!request->pack(response->get_api(), ret, &tDst)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_GdcWorkDestroy) {
    GDC_HANDLE nGdcHandle;

    if (!response->unpack(response->get_api(), &nGdcHandle)) {
        return false;
    }

    AX_S32 ret = ::AX_IVPS_GdcWorkDestroy(nGdcHandle);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_FisheyePointQueryDst2Src) {
    AX_IVPS_POINT_NICE_T tDstPoint;
    AX_U16 nInputW;
    AX_U16 nInputH;
    AX_U8 nRgnIdx;
    AX_IVPS_FISHEYE_ATTR_T tFisheyeAttr;

    if (!response->unpack(response->get_api(), &tDstPoint, &nInputW, &nInputH, &nRgnIdx, &tFisheyeAttr)) {
        return false;
    }

    AX_IVPS_POINT_NICE_T tSrcPoint;
    AX_S32 ret = ::AX_IVPS_FisheyePointQueryDst2Src(&tSrcPoint, &tDstPoint, nInputW, nInputH, nRgnIdx, &tFisheyeAttr);

    if (!request->pack(response->get_api(), ret, &tSrcPoint)) {
        return false;
    }

    return true;
}

NATIVE_IVPS_API_IMPL(AX_IVPS_FisheyePointQuerySrc2Dst) {
    AX_IVPS_POINT_NICE_T tSrcPoint;
    AX_U16 nInputW;
    AX_U16 nInputH;
    AX_U8 nRgnIdx;
    AX_IVPS_FISHEYE_ATTR_T tFisheyeAttr;

    if (!response->unpack(response->get_api(), &tSrcPoint, &nInputW, &nInputH, &nRgnIdx, &tFisheyeAttr)) {
        return false;
    }

    AX_IVPS_POINT_NICE_T tDstPoint;
    AX_S32 ret = ::AX_IVPS_FisheyePointQueryDst2Src(&tSrcPoint, &tDstPoint, nInputW, nInputH, nRgnIdx, &tFisheyeAttr);

    if (!request->pack(response->get_api(), ret, &tDstPoint)) {
        return false;
    }

    return true;
}

}  // namespace axcl::worker
