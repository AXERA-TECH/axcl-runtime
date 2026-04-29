/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "venc.hpp"
#include "axcl_logger.hpp"
#include "dispatcher.hpp"

#define TAG "venc"

namespace axcl::worker {

venc::venc(dispatcher *dispatcher) : base(dispatcher), m_api(TAG) {
    m_api.append({
        NATIVE_VENC_API_APPEND(AX_VENC_Init),
        NATIVE_VENC_API_APPEND(AX_VENC_Deinit),
        NATIVE_VENC_API_APPEND(AX_VENC_CreateChn),
        NATIVE_VENC_API_APPEND(AX_VENC_CreateChnEx),
        NATIVE_VENC_API_APPEND(AX_VENC_DestroyChn),
        NATIVE_VENC_API_APPEND(AX_VENC_SendFrame),
        NATIVE_VENC_API_APPEND(AX_VENC_SendFrameEx),
        NATIVE_VENC_API_APPEND(AX_VENC_SelectGrp),
        NATIVE_VENC_API_APPEND(AX_VENC_SelectClearGrp),
        NATIVE_VENC_API_APPEND(AX_VENC_SelectGrpAddChn),
        NATIVE_VENC_API_APPEND(AX_VENC_SelectGrpDeleteChn),
        NATIVE_VENC_API_APPEND(AX_VENC_SelectGrpQuery),
        NATIVE_VENC_API_APPEND(AX_VENC_GetFd),
        NATIVE_VENC_API_APPEND(AX_VENC_GetStream),
        NATIVE_VENC_API_APPEND(AX_VENC_ReleaseStream),
        NATIVE_VENC_API_APPEND(AX_VENC_GetStreamBufInfo),
        NATIVE_VENC_API_APPEND(AX_VENC_StartRecvFrame),
        NATIVE_VENC_API_APPEND(AX_VENC_StopRecvFrame),
        NATIVE_VENC_API_APPEND(AX_VENC_ResetChn),
        NATIVE_VENC_API_APPEND(AX_VENC_SetRoiAttr),
        NATIVE_VENC_API_APPEND(AX_VENC_GetRoiAttr),
        NATIVE_VENC_API_APPEND(AX_VENC_SetRcParam),
        NATIVE_VENC_API_APPEND(AX_VENC_GetRcParam),
        NATIVE_VENC_API_APPEND(AX_VENC_SetModParam),
        NATIVE_VENC_API_APPEND(AX_VENC_GetModParam),
        NATIVE_VENC_API_APPEND(AX_VENC_SetVuiParam),
        NATIVE_VENC_API_APPEND(AX_VENC_GetVuiParam),
        NATIVE_VENC_API_APPEND(AX_VENC_SetChnAttr),
        NATIVE_VENC_API_APPEND(AX_VENC_GetChnAttr),
        NATIVE_VENC_API_APPEND(AX_VENC_SetRateJamStrategy),
        NATIVE_VENC_API_APPEND(AX_VENC_GetRateJamStrategy),
        NATIVE_VENC_API_APPEND(AX_VENC_SetSuperFrameStrategy),
        NATIVE_VENC_API_APPEND(AX_VENC_GetSuperFrameStrategy),
        NATIVE_VENC_API_APPEND(AX_VENC_SetIntraRefresh),
        NATIVE_VENC_API_APPEND(AX_VENC_GetIntraRefresh),
        NATIVE_VENC_API_APPEND(AX_VENC_SetUsrData),
        NATIVE_VENC_API_APPEND(AX_VENC_GetUsrData),
        NATIVE_VENC_API_APPEND(AX_VENC_SetSliceSplit),
        NATIVE_VENC_API_APPEND(AX_VENC_GetSliceSplit),
        NATIVE_VENC_API_APPEND(AX_VENC_RequestIDR),
        NATIVE_VENC_API_APPEND(AX_VENC_QueryStatus),
        NATIVE_VENC_API_APPEND(AX_VENC_SetJpegParam),
        NATIVE_VENC_API_APPEND(AX_VENC_GetJpegParam),
        NATIVE_VENC_API_APPEND(AX_VENC_JpegEncodeOneFrame),
        NATIVE_VENC_API_APPEND(AX_VENC_JpegGetThumbnail),
    });
}

void venc::run(const void *data, const size_t &size) {
    m_api.run(get_dispatcher(), data, size);
}

NATIVE_VENC_API_IMPL(AX_VENC_Init) {
    AX_VENC_MOD_ATTR_T stModAttr;

    if (!response->unpack(response->get_api(), &stModAttr)) {
        return false;
    }

    AX_S32 ret = ::AX_VENC_Init(&stModAttr);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_Deinit) {
    if (!response->unpack(response->get_api())) {
        return false;
    }

    AX_S32 ret = ::AX_VENC_Deinit();

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_CreateChn) {
    VENC_CHN VeChn;
    AX_VENC_CHN_ATTR_T stAttr;
    axcl::pkg::uint8_array svc;

    if (!response->unpack(response->get_api(), &VeChn, &stAttr, &svc)) {
        return false;
    }

    std::vector<uint8_t> svc_mem;
    if (AX_VENC_GOPMODE_SVC_T == stAttr.stGopAttr.enGopMode) {
        if (!response->deserialize_gop_svc_t(svc.data, svc.size, stAttr.stGopAttr.stSvcT, svc_mem)) {
            LOG_MM_E(TAG, "veChn {}: deserialize gop svc fail", VeChn);
            return false;
        }
    }

    AX_S32 ret = ::AX_VENC_CreateChn(VeChn, &stAttr);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_CreateChnEx) {
    VENC_CHN VeChn;
    AX_VENC_CHN_ATTR_T stAttr;
    axcl::pkg::uint8_array svc;

    if (!response->unpack(response->get_api(), &stAttr, &svc)) {
        return false;
    }

    std::vector<uint8_t> svc_mem;
    if (AX_VENC_GOPMODE_SVC_T == stAttr.stGopAttr.enGopMode) {
        if (!response->deserialize_gop_svc_t(svc.data, svc.size, stAttr.stGopAttr.stSvcT, svc_mem)) {
            LOG_MM_E(TAG, "deserialize gop svc fail");
            return false;
        }
    }

    AX_S32 ret = ::AX_VENC_CreateChnEx(&VeChn, &stAttr);

    if (!request->pack(response->get_api(), ret, VeChn)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_DestroyChn) {
    VENC_CHN VeChn;

    if (!response->unpack(response->get_api(), &VeChn)) {
        return false;
    }

    AX_S32 ret = ::AX_VENC_DestroyChn(VeChn);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_SendFrame) {
    VENC_CHN VeChn;
    AX_VIDEO_FRAME_INFO_T stFrame;
    AX_S32 s32MilliSec;

    if (!response->unpack(response->get_api(), &VeChn, &stFrame, &s32MilliSec)) {
        return false;
    }

    AX_S32 ret = ::AX_VENC_SendFrame(VeChn, &stFrame, s32MilliSec);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_SendFrameEx) {
    VENC_CHN VeChn;
    AX_USER_FRAME_INFO_T stFrame;
    AX_S32 s32MilliSec;

    if (!response->unpack(response->get_api(), &VeChn, &stFrame, &s32MilliSec)) {
        return false;
    }

    AX_S32 ret = ::AX_VENC_SendFrameEx(VeChn, &stFrame, s32MilliSec);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_SelectGrp) {
    VENC_GRP grpId;
    AX_S32 s32MilliSec;

    if (!response->unpack(response->get_api(), &grpId, &s32MilliSec)) {
        return false;
    }

    AX_CHN_STREAM_STATUS_T stChnStrmState;
    AX_S32 ret = ::AX_VENC_SelectGrp(grpId, &stChnStrmState, s32MilliSec);

    if (!request->pack(response->get_api(), ret, &stChnStrmState)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_SelectClearGrp) {
    VENC_GRP grpId;

    if (!response->unpack(response->get_api(), &grpId)) {
        return false;
    }

    AX_S32 ret = ::AX_VENC_SelectClearGrp(grpId);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_SelectGrpAddChn) {
    VENC_GRP grpId;
    VENC_CHN VeChn;

    if (!response->unpack(response->get_api(), &grpId, &VeChn)) {
        return false;
    }

    AX_S32 ret = ::AX_VENC_SelectGrpAddChn(grpId, VeChn);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_SelectGrpDeleteChn) {
    VENC_GRP grpId;
    VENC_CHN VeChn;

    if (!response->unpack(response->get_api(), &grpId, &VeChn)) {
        return false;
    }

    AX_S32 ret = ::AX_VENC_SelectGrpDeleteChn(grpId, VeChn);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_SelectGrpQuery) {
    VENC_GRP grpId;
    AX_VENC_SELECT_GRP_PARAM_T stGrpInfo;

    if (!response->unpack(response->get_api(), &grpId)) {
        return false;
    }

    AX_S32 ret = ::AX_VENC_SelectGrpQuery(grpId, &stGrpInfo);

    if (!request->pack(response->get_api(), ret, &stGrpInfo)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_GetFd) {
    VENC_CHN VeChn;

    if (!response->unpack(response->get_api(), &VeChn)) {
        return false;
    }

    AX_S32 fd = ::AX_VENC_GetFd(VeChn);

    if (!request->pack(response->get_api(), fd)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_GetStream) {
    VENC_CHN VeChn;
    AX_S32 s32MilliSec;

    if (!response->unpack(response->get_api(), &VeChn, &s32MilliSec)) {
        return false;
    }

    AX_VENC_STREAM_T stStream;
    AX_S32 ret = ::AX_VENC_GetStream(VeChn, &stStream, s32MilliSec);

    if (!request->pack(response->get_api(), ret, &stStream)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_ReleaseStream) {
    VENC_CHN VeChn;
    AX_VENC_STREAM_T stStream;

    if (!response->unpack(response->get_api(), &VeChn, &stStream)) {
        return false;
    }

    AX_S32 ret = ::AX_VENC_ReleaseStream(VeChn, &stStream);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_GetStreamBufInfo) {
    VENC_CHN VeChn;

    if (!response->unpack(response->get_api(), &VeChn)) {
        return false;
    }

    AX_VENC_STREAM_BUF_INFO_T stStreamBufInfo;
    AX_S32 ret = ::AX_VENC_GetStreamBufInfo(VeChn, &stStreamBufInfo);

    if (!request->pack(response->get_api(), ret, &stStreamBufInfo)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_StartRecvFrame) {
    VENC_CHN VeChn;
    AX_VENC_RECV_PIC_PARAM_T stRecvParam;

    if (!response->unpack(response->get_api(), &VeChn, &stRecvParam)) {
        return false;
    }

    AX_S32 ret = ::AX_VENC_StartRecvFrame(VeChn, &stRecvParam);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_StopRecvFrame) {
    VENC_CHN VeChn;

    if (!response->unpack(response->get_api(), &VeChn)) {
        return false;
    }

    AX_S32 ret = ::AX_VENC_StopRecvFrame(VeChn);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_ResetChn) {
    VENC_CHN VeChn;

    if (!response->unpack(response->get_api(), &VeChn)) {
        return false;
    }

    AX_S32 ret = ::AX_VENC_ResetChn(VeChn);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_SetRoiAttr) {
    VENC_CHN VeChn;
    AX_VENC_ROI_ATTR_T stRoiAttr;

    if (!response->unpack(response->get_api(), &VeChn, &stRoiAttr)) {
        return false;
    }

    AX_S32 ret = ::AX_VENC_SetRoiAttr(VeChn, &stRoiAttr);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_GetRoiAttr) {
    VENC_CHN VeChn;
    AX_U32 u32Index;

    if (!response->unpack(response->get_api(), &VeChn, &u32Index)) {
        return false;
    }

    AX_VENC_ROI_ATTR_T stRoiAttr;
    AX_S32 ret = ::AX_VENC_GetRoiAttr(VeChn, u32Index, &stRoiAttr);

    if (!request->pack(response->get_api(), ret, &stRoiAttr)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_SetRcParam) {
    VENC_CHN VeChn;
    AX_VENC_RC_PARAM_T stRcParam;

    if (!response->unpack(response->get_api(), &VeChn, &stRcParam)) {
        return false;
    }

    AX_S32 ret = ::AX_VENC_SetRcParam(VeChn, &stRcParam);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_GetRcParam) {
    VENC_CHN VeChn;

    if (!response->unpack(response->get_api(), &VeChn)) {
        return false;
    }

    AX_VENC_RC_PARAM_T stRcParam;
    AX_S32 ret = ::AX_VENC_GetRcParam(VeChn, &stRcParam);

    if (!request->pack(response->get_api(), ret, &stRcParam)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_SetModParam) {
    AX_VENC_ENCODER_TYPE_E enVencType;
    AX_VENC_MOD_PARAM_T stModParam;

    if (!response->unpack(response->get_api(), &enVencType, &stModParam)) {
        return false;
    }

    AX_S32 ret = ::AX_VENC_SetModParam(enVencType, &stModParam);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_GetModParam) {
    AX_VENC_ENCODER_TYPE_E enVencType;

    if (!response->unpack(response->get_api(), &enVencType)) {
        return false;
    }

    AX_VENC_MOD_PARAM_T stModParam;
    AX_S32 ret = ::AX_VENC_GetModParam(enVencType, &stModParam);

    if (!request->pack(response->get_api(), ret, &stModParam)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_SetVuiParam) {
    VENC_CHN VeChn;
    AX_VENC_VUI_PARAM_T stVuiParam;

    if (!response->unpack(response->get_api(), &VeChn, &stVuiParam)) {
        return false;
    }

    AX_S32 ret = ::AX_VENC_SetVuiParam(VeChn, &stVuiParam);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_GetVuiParam) {
    VENC_CHN VeChn;

    if (!response->unpack(response->get_api(), &VeChn)) {
        return false;
    }

    AX_VENC_VUI_PARAM_T stVuiParam;
    AX_S32 ret = ::AX_VENC_GetVuiParam(VeChn, &stVuiParam);

    if (!request->pack(response->get_api(), ret, &stVuiParam)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_SetChnAttr) {
    VENC_CHN VeChn;
    AX_VENC_CHN_ATTR_T stChnAttr;
    axcl::pkg::uint8_array svc;

    if (!response->unpack(response->get_api(), &VeChn, &stChnAttr, &svc)) {
        return false;
    }

    std::vector<uint8_t> svc_mem;
    if (AX_VENC_GOPMODE_SVC_T == stChnAttr.stGopAttr.enGopMode) {
        if (!response->deserialize_gop_svc_t(svc.data, svc.size, stChnAttr.stGopAttr.stSvcT, svc_mem)) {
            LOG_MM_E(TAG, "deserialize gop svc fail");
            return false;
        }
    }

    AX_S32 ret = ::AX_VENC_SetChnAttr(VeChn, &stChnAttr);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_GetChnAttr) {
    VENC_CHN VeChn;

    if (!response->unpack(response->get_api(), &VeChn)) {
        return false;
    }

    AX_VENC_CHN_ATTR_T stChnAttr;
    AX_S32 ret = ::AX_VENC_GetChnAttr(VeChn, &stChnAttr);

    axcl::pkg::uint8_array svc;
    if (AX_VENC_GOPMODE_SVC_T == stChnAttr.stGopAttr.enGopMode) {
        if (!request->serialize_gop_svc_t(stChnAttr.stGopAttr.stSvcT, svc.data, svc.size)) {
            LOG_MM_E(TAG, "serialize gop svc fail");
            return false;
        }
    }

    if (!request->pack(response->get_api(), ret, &stChnAttr, svc)) {
        if (AX_VENC_GOPMODE_SVC_T == stChnAttr.stGopAttr.enGopMode) {
            ::free(svc.data);
        }

        return false;
    }

    if (AX_VENC_GOPMODE_SVC_T == stChnAttr.stGopAttr.enGopMode) {
        ::free(svc.data);
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_SetRateJamStrategy) {
    VENC_CHN VeChn;
    AX_VENC_RATE_JAM_CFG_T stRateJamParam;

    if (!response->unpack(response->get_api(), &VeChn, &stRateJamParam)) {
        return false;
    }

    AX_S32 ret = ::AX_VENC_SetRateJamStrategy(VeChn, &stRateJamParam);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_GetRateJamStrategy) {
    VENC_CHN VeChn;

    if (!response->unpack(response->get_api(), &VeChn)) {
        return false;
    }

    AX_VENC_RATE_JAM_CFG_T stRateJamParam;
    AX_S32 ret = ::AX_VENC_GetRateJamStrategy(VeChn, &stRateJamParam);

    if (!request->pack(response->get_api(), ret, &stRateJamParam)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_SetSuperFrameStrategy) {
    VENC_CHN VeChn;
    AX_VENC_SUPERFRAME_CFG_T stSuperFrameCfg;

    if (!response->unpack(response->get_api(), &VeChn, &stSuperFrameCfg)) {
        return false;
    }

    AX_S32 ret = ::AX_VENC_SetSuperFrameStrategy(VeChn, &stSuperFrameCfg);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_GetSuperFrameStrategy) {
    VENC_CHN VeChn;

    if (!response->unpack(response->get_api(), &VeChn)) {
        return false;
    }

    AX_VENC_SUPERFRAME_CFG_T stSuperFrameCfg;
    AX_S32 ret = ::AX_VENC_GetSuperFrameStrategy(VeChn, &stSuperFrameCfg);

    if (!request->pack(response->get_api(), ret, &stSuperFrameCfg)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_SetIntraRefresh) {
    VENC_CHN VeChn;
    AX_VENC_INTRA_REFRESH_T stIntraRefresh;

    if (!response->unpack(response->get_api(), &VeChn, &stIntraRefresh)) {
        return false;
    }

    AX_S32 ret = ::AX_VENC_SetIntraRefresh(VeChn, &stIntraRefresh);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_GetIntraRefresh) {
    VENC_CHN VeChn;

    if (!response->unpack(response->get_api(), &VeChn)) {
        return false;
    }

    AX_VENC_INTRA_REFRESH_T stIntraRefresh;
    AX_S32 ret = ::AX_VENC_GetIntraRefresh(VeChn, &stIntraRefresh);

    if (!request->pack(response->get_api(), ret, &stIntraRefresh)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_SetUsrData) {
    VENC_CHN VeChn;
    AX_VENC_USR_DATA_T stUsrData;
    axcl::pkg::uint8_array nalu;

    if (!response->unpack(response->get_api(), &VeChn, &stUsrData, &nalu)) {
        return false;
    }

    stUsrData.pu8UsrData = nalu.data;
    stUsrData.u32DataSize = nalu.size;

    AX_S32 ret = ::AX_VENC_SetUsrData(VeChn, &stUsrData);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_GetUsrData) {
    VENC_CHN VeChn;
    AX_VENC_USR_DATA_T input;
    if (!response->unpack(response->get_api(), &VeChn, &input)) {
        return false;
    }

    AX_VENC_USR_DATA_T stUsrData;
    stUsrData.pu8UsrData = (AX_U8 *)::malloc(input.u32DataSize);
    AX_S32 ret = ::AX_VENC_GetUsrData(VeChn, &stUsrData);

    axcl::pkg::uint8_array nalu;
    nalu.data = stUsrData.pu8UsrData;
    nalu.size = stUsrData.u32DataSize;
    if (!request->pack(response->get_api(), ret, &stUsrData, nalu)) {
        ::free(stUsrData.pu8UsrData);
        return false;
    }

    ::free(stUsrData.pu8UsrData);
    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_SetSliceSplit) {
    VENC_CHN VeChn;
    AX_VENC_SLICE_SPLIT_T stSliceSplit;

    if (!response->unpack(response->get_api(), &VeChn, &stSliceSplit)) {
        return false;
    }

    AX_S32 ret = ::AX_VENC_SetSliceSplit(VeChn, &stSliceSplit);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_GetSliceSplit) {
    VENC_CHN VeChn;

    if (!response->unpack(response->get_api(), &VeChn)) {
        return false;
    }

    AX_VENC_SLICE_SPLIT_T stSliceSplit;
    AX_S32 ret = ::AX_VENC_GetSliceSplit(VeChn, &stSliceSplit);

    if (!request->pack(response->get_api(), ret, &stSliceSplit)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_RequestIDR) {
    VENC_CHN VeChn;
    AX_BOOL bInstant;

    if (!response->unpack(response->get_api(), &VeChn, &bInstant)) {
        return false;
    }

    AX_S32 ret = ::AX_VENC_RequestIDR(VeChn, bInstant);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_QueryStatus) {
    VENC_CHN VeChn;

    if (!response->unpack(response->get_api(), &VeChn)) {
        return false;
    }

    AX_VENC_CHN_STATUS_T stStatus;
    AX_S32 ret = ::AX_VENC_QueryStatus(VeChn, &stStatus);

    if (!request->pack(response->get_api(), ret, stStatus)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_SetJpegParam) {
    VENC_CHN VeChn;
    AX_VENC_JPEG_PARAM_T stJpegParam;

    if (!response->unpack(response->get_api(), &VeChn, &stJpegParam)) {
        return false;
    }

    AX_S32 ret = ::AX_VENC_SetJpegParam(VeChn, &stJpegParam);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_GetJpegParam) {
    VENC_CHN VeChn;

    if (!response->unpack(response->get_api(), &VeChn)) {
        return false;
    }

    AX_VENC_JPEG_PARAM_T stJpegParam;
    AX_S32 ret = ::AX_VENC_GetJpegParam(VeChn, &stJpegParam);

    if (!request->pack(response->get_api(), ret, &stJpegParam)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_JpegEncodeOneFrame) {
    AX_JPEG_ENCODE_ONCE_PARAMS_T stJpegParam;

    if (!response->unpack(response->get_api(), &stJpegParam)) {
        return false;
    }

    AX_S32 ret = ::AX_VENC_JpegEncodeOneFrame(&stJpegParam);

    if (!request->pack(response->get_api(), ret, &stJpegParam)) {
        return false;
    }

    return true;
}

NATIVE_VENC_API_IMPL(AX_VENC_JpegGetThumbnail) {
    return false;
}

}  // namespace axcl::worker
