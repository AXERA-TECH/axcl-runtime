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

#include <unordered_map>
#include <vector>
#include "axcl_venc_type.h"
#include "native.hpp"
#include "package/host/native/module/venc_request.hpp"
#include "package/host/native/module/venc_response.hpp"

#define AXCL_DEF_VENC_ERR(e) AXCL_DEF_NATIVE_ERR(static_cast<uint8_t>(axcl::pkg::native::module::VENC), (e))

class venc : public native<axcl::pkg::host::venc_request, axcl::pkg::host::venc_response> {
public:
    venc() : native<axcl::pkg::host::venc_request, axcl::pkg::host::venc_response>(axcl::pkg::native::module::VENC) {
    }

    AX_S32 AX_VENC_Init(const AX_VENC_MOD_ATTR_T *pstModAttr);
    AX_S32 AX_VENC_Deinit();

    AX_S32 AX_VENC_CreateChn(VENC_CHN VeChn, const AX_VENC_CHN_ATTR_T *pstAttr);
    AX_S32 AX_VENC_CreateChnEx(VENC_CHN *pVeChn, const AX_VENC_CHN_ATTR_T *pstAttr);
    AX_S32 AX_VENC_DestroyChn(VENC_CHN VeChn);

    AX_S32 AX_VENC_SendFrame(VENC_CHN VeChn, const AX_VIDEO_FRAME_INFO_T *pstFrame, AX_S32 s32MilliSec);
    AX_S32 AX_VENC_SendFrameEx(VENC_CHN VeChn, const AX_USER_FRAME_INFO_T *pstFrame, AX_S32 s32MilliSec);

    AX_S32 AX_VENC_SelectGrp(VENC_GRP grpId, AX_CHN_STREAM_STATUS_T *pstChnStrmState, AX_S32 s32MilliSec);
    AX_S32 AX_VENC_SelectClearGrp(VENC_GRP grpId);
    AX_S32 AX_VENC_SelectGrpAddChn(VENC_GRP grpId, VENC_CHN VeChn);
    AX_S32 AX_VENC_SelectGrpDeleteChn(VENC_GRP grpId, VENC_CHN VeChn);
    AX_S32 AX_VENC_SelectGrpQuery(VENC_GRP grpId, AX_VENC_SELECT_GRP_PARAM_T *pstGrpInfo);

    AX_S32 AX_VENC_GetFd(VENC_CHN VeChn);

    AX_S32 AX_VENC_GetStream(VENC_CHN VeChn, AX_VENC_STREAM_T *pstStream, AX_S32 s32MilliSec);
    AX_S32 AX_VENC_ReleaseStream(VENC_CHN VeChn, const AX_VENC_STREAM_T *pstStream);
    AX_S32 AX_VENC_GetStreamBufInfo(VENC_CHN VeChn, AX_VENC_STREAM_BUF_INFO_T *pstStreamBufInfo);

    AX_S32 AX_VENC_StartRecvFrame(VENC_CHN VeChn, const AX_VENC_RECV_PIC_PARAM_T *pstRecvParam);
    AX_S32 AX_VENC_StopRecvFrame(VENC_CHN VeChn);
    AX_S32 AX_VENC_ResetChn(VENC_CHN VeChn);

    AX_S32 AX_VENC_SetRoiAttr(VENC_CHN VeChn, const AX_VENC_ROI_ATTR_T *pstRoiAttr);
    AX_S32 AX_VENC_GetRoiAttr(VENC_CHN VeChn, AX_U32 u32Index, AX_VENC_ROI_ATTR_T *pstRoiAttr);

    AX_S32 AX_VENC_SetRcParam(VENC_CHN VeChn, const AX_VENC_RC_PARAM_T *pstRcParam);
    AX_S32 AX_VENC_GetRcParam(VENC_CHN VeChn, AX_VENC_RC_PARAM_T *pstRcParam);

    AX_S32 AX_VENC_SetModParam(AX_VENC_ENCODER_TYPE_E enVencType, const AX_VENC_MOD_PARAM_T *pstModParam);
    AX_S32 AX_VENC_GetModParam(AX_VENC_ENCODER_TYPE_E enVencType, AX_VENC_MOD_PARAM_T *pstModParam);

    AX_S32 AX_VENC_SetVuiParam(VENC_CHN VeChn, const AX_VENC_VUI_PARAM_T *pstVuiParam);
    AX_S32 AX_VENC_GetVuiParam(VENC_CHN VeChn, AX_VENC_VUI_PARAM_T *pstVuiParam);

    AX_S32 AX_VENC_SetChnAttr(VENC_CHN VeChn, const AX_VENC_CHN_ATTR_T *pstChnAttr);
    AX_S32 AX_VENC_GetChnAttr(VENC_CHN VeChn, AX_VENC_CHN_ATTR_T *pstChnAttr);

    AX_S32 AX_VENC_SetRateJamStrategy(VENC_CHN VeChn, const AX_VENC_RATE_JAM_CFG_T *pstRateJamParam);
    AX_S32 AX_VENC_GetRateJamStrategy(VENC_CHN VeChn, AX_VENC_RATE_JAM_CFG_T *pstRateJamParam);

    AX_S32 AX_VENC_SetSuperFrameStrategy(VENC_CHN VeChn, const AX_VENC_SUPERFRAME_CFG_T *pstSuperFrameCfg);
    AX_S32 AX_VENC_GetSuperFrameStrategy(VENC_CHN VeChn, AX_VENC_SUPERFRAME_CFG_T *pstSuperFrameCfg);

    AX_S32 AX_VENC_SetIntraRefresh(VENC_CHN VeChn, const AX_VENC_INTRA_REFRESH_T *pstIntraRefresh);
    AX_S32 AX_VENC_GetIntraRefresh(VENC_CHN VeChn, AX_VENC_INTRA_REFRESH_T *pstIntraRefresh);

    AX_S32 AX_VENC_SetUsrData(VENC_CHN VeChn, const AX_VENC_USR_DATA_T *pstUsrData);
    AX_S32 AX_VENC_GetUsrData(VENC_CHN VeChn, AX_VENC_USR_DATA_T *pstUsrData);

    AX_S32 AX_VENC_SetSliceSplit(VENC_CHN VeChn, const AX_VENC_SLICE_SPLIT_T *pstSliceSplit);
    AX_S32 AX_VENC_GetSliceSplit(VENC_CHN VeChn, AX_VENC_SLICE_SPLIT_T *pstSliceSplit);

    AX_S32 AX_VENC_RequestIDR(VENC_CHN VeChn, AX_BOOL bInstant);

    AX_S32 AX_VENC_QueryStatus(VENC_CHN VeChn, AX_VENC_CHN_STATUS_T *pstStatus);

    AX_S32 AX_VENC_SetJpegParam(VENC_CHN VeChn, const AX_VENC_JPEG_PARAM_T *pstJpegParam);
    AX_S32 AX_VENC_GetJpegParam(VENC_CHN VeChn, AX_VENC_JPEG_PARAM_T *pstJpegParam);

    AX_S32 AX_VENC_JpegEncodeOneFrame(AX_JPEG_ENCODE_ONCE_PARAMS_T *pstJpegParam);

    AX_S32 AX_VENC_JpegGetThumbnail(const AX_VOID *pRawData, AX_VOID *pThumbData, AX_U32 *pThumbSize);

private:
    static std::unordered_map<int32_t /* device */, std::unordered_map<VENC_CHN, std::vector<uint8_t>>> m_svc_mem_map;
};
