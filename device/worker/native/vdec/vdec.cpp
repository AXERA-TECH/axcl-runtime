/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "vdec.hpp"
#include "dispatcher.hpp"
#include "axcl_logger.hpp"

#define TAG "vdec"

namespace axcl::worker {

vdec::vdec(dispatcher *dispatcher) : base(dispatcher), m_api(TAG) {
    m_api.append({
            NATIVE_VDEC_API_APPEND(AX_VDEC_Init),
            NATIVE_VDEC_API_APPEND(AX_VDEC_Deinit),
            NATIVE_VDEC_API_APPEND(AX_VDEC_ExtractStreamHeaderInfo),
            NATIVE_VDEC_API_APPEND(AX_VDEC_CreateGrp),
            NATIVE_VDEC_API_APPEND(AX_VDEC_CreateGrpEx),
            NATIVE_VDEC_API_APPEND(AX_VDEC_DestroyGrp),
            NATIVE_VDEC_API_APPEND(AX_VDEC_GetGrpAttr),
            NATIVE_VDEC_API_APPEND(AX_VDEC_SetGrpAttr),
            NATIVE_VDEC_API_APPEND(AX_VDEC_StartRecvStream),
            NATIVE_VDEC_API_APPEND(AX_VDEC_StopRecvStream),
            NATIVE_VDEC_API_APPEND(AX_VDEC_QueryStatus),
            NATIVE_VDEC_API_APPEND(AX_VDEC_ResetGrp),
            NATIVE_VDEC_API_APPEND(AX_VDEC_SetGrpParam),
            NATIVE_VDEC_API_APPEND(AX_VDEC_GetGrpParam),
            NATIVE_VDEC_API_APPEND(AX_VDEC_SelectGrp),
            NATIVE_VDEC_API_APPEND(AX_VDEC_SendStream),
            NATIVE_VDEC_API_APPEND(AX_VDEC_GetChnFrame),
            NATIVE_VDEC_API_APPEND(AX_VDEC_ReleaseChnFrame),
            NATIVE_VDEC_API_APPEND(AX_VDEC_GetUserData),
            NATIVE_VDEC_API_APPEND(AX_VDEC_ReleaseUserData),
            NATIVE_VDEC_API_APPEND(AX_VDEC_SetUserPic),
            NATIVE_VDEC_API_APPEND(AX_VDEC_EnableUserPic),
            NATIVE_VDEC_API_APPEND(AX_VDEC_DisableUserPic),
            NATIVE_VDEC_API_APPEND(AX_VDEC_SetDisplayMode),
            NATIVE_VDEC_API_APPEND(AX_VDEC_GetDisplayMode),
            NATIVE_VDEC_API_APPEND(AX_VDEC_AttachPool),
            NATIVE_VDEC_API_APPEND(AX_VDEC_DetachPool),
            NATIVE_VDEC_API_APPEND(AX_VDEC_EnableChn),
            NATIVE_VDEC_API_APPEND(AX_VDEC_DisableChn),
            NATIVE_VDEC_API_APPEND(AX_VDEC_SetChnAttr),
            NATIVE_VDEC_API_APPEND(AX_VDEC_GetChnAttr),
            NATIVE_VDEC_API_APPEND(AX_VDEC_JpegDecodeOneFrame),
            NATIVE_VDEC_API_APPEND(AX_VDEC_GetStreamBufInfo),
            NATIVE_VDEC_API_APPEND(AX_VDEC_GetVuiParam),
    });
}

void vdec::run(const void *data, const size_t& size) {
    m_api.run(get_dispatcher(), data, size);
}

NATIVE_VDEC_API_IMPL(AX_VDEC_Init) {
    AX_VDEC_MOD_ATTR_T stModAttr;

    if (!response->unpack(response->get_api(), &stModAttr)) {
        return false;
    }

    /* refer to host/native/vdec/api.cpp:AX_VDEC_Init() */
    AX_S32 ret = ::AX_VDEC_Init(stModAttr.u32MaxGroupCount > AX_VDEC_MAX_GRP_NUM ? NULL : &stModAttr);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VDEC_API_IMPL(AX_VDEC_Deinit) {
    if (!response->unpack(response->get_api())) {
        return false;
    }

    AX_S32 ret = ::AX_VDEC_Deinit();

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VDEC_API_IMPL(AX_VDEC_ExtractStreamHeaderInfo) {
    AX_VDEC_STREAM_T stStreamBuf;
    AX_PAYLOAD_TYPE_E enVideoType;
    AX_VDEC_BITSTREAM_INFO_T stBitStreamInfo;
    axcl::pkg::uint8_array nalu;

    if (!response->unpack(response->get_api(), &stStreamBuf, &enVideoType)) {
        return false;
    }

    AX_S32 ret = ::AX_VDEC_ExtractStreamHeaderInfo(&stStreamBuf, enVideoType, &stBitStreamInfo);

    if (!request->pack(response->get_api(), ret, &stBitStreamInfo)) {
        return false;
    }

    return true;
}

NATIVE_VDEC_API_IMPL(AX_VDEC_CreateGrp) {
    AX_VDEC_GRP VdGrp;
    AX_VDEC_GRP_ATTR_T stGrpAttr;

    if (!response->unpack(response->get_api(), &VdGrp, &stGrpAttr)) {
        return false;
    }

    AX_S32 ret = ::AX_VDEC_CreateGrp(VdGrp, &stGrpAttr);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VDEC_API_IMPL(AX_VDEC_CreateGrpEx) {
    AX_VDEC_GRP VdGrp;
    AX_VDEC_GRP_ATTR_T stGrpAttr;

    if (!response->unpack(response->get_api(), &stGrpAttr)) {
        return false;
    }

    AX_S32 ret = ::AX_VDEC_CreateGrpEx(&VdGrp, &stGrpAttr);

    if (!request->pack(response->get_api(), ret, VdGrp)) {
        return false;
    }

    return true;
}

NATIVE_VDEC_API_IMPL(AX_VDEC_DestroyGrp) {
    AX_VDEC_GRP VdGrp;

    if (!response->unpack(response->get_api(), &VdGrp)) {
        return false;
    }

    AX_S32 ret = ::AX_VDEC_DestroyGrp(VdGrp);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VDEC_API_IMPL(AX_VDEC_GetGrpAttr) {
    AX_VDEC_GRP VdGrp;
    AX_VDEC_GRP_ATTR_T stGrpAttr;

    if (!response->unpack(response->get_api(), &VdGrp)) {
        return false;
    }

    AX_S32 ret = ::AX_VDEC_GetGrpAttr(VdGrp, &stGrpAttr);

    if (!request->pack(response->get_api(), ret, &stGrpAttr)) {
        return false;
    }

    return true;
}

NATIVE_VDEC_API_IMPL(AX_VDEC_SetGrpAttr) {
    AX_VDEC_GRP VdGrp;
    AX_VDEC_GRP_ATTR_T stGrpAttr;

    if (!response->unpack(response->get_api(), &VdGrp, &stGrpAttr)) {
        return false;
    }

    AX_S32 ret = ::AX_VDEC_SetGrpAttr(VdGrp, &stGrpAttr);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VDEC_API_IMPL(AX_VDEC_StartRecvStream) {
    AX_VDEC_GRP VdGrp;
    AX_VDEC_RECV_PIC_PARAM_T stRecvParam;

    if (!response->unpack(response->get_api(), &VdGrp, &stRecvParam)) {
        return false;
    }

    AX_S32 ret = ::AX_VDEC_StartRecvStream(VdGrp, &stRecvParam);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VDEC_API_IMPL(AX_VDEC_StopRecvStream) {
    AX_VDEC_GRP VdGrp;

    if (!response->unpack(response->get_api(), &VdGrp)) {
        return false;
    }

    AX_S32 ret = ::AX_VDEC_StopRecvStream(VdGrp);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VDEC_API_IMPL(AX_VDEC_QueryStatus) {
    AX_VDEC_GRP VdGrp;
    AX_VDEC_GRP_STATUS_T stGrpStatus;

    if (!response->unpack(response->get_api(), &VdGrp)) {
        return false;
    }

    AX_S32 ret = ::AX_VDEC_QueryStatus(VdGrp, &stGrpStatus);

    if (!request->pack(response->get_api(), ret, &stGrpStatus)) {
        return false;
    }

    return true;
}

NATIVE_VDEC_API_IMPL(AX_VDEC_ResetGrp) {
    AX_VDEC_GRP VdGrp;

    if (!response->unpack(response->get_api(), &VdGrp)) {
        return false;
    }

    AX_S32 ret = ::AX_VDEC_ResetGrp(VdGrp);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VDEC_API_IMPL(AX_VDEC_SetGrpParam) {
    AX_VDEC_GRP VdGrp;
    AX_VDEC_GRP_PARAM_T stGrpParam;

    if (!response->unpack(response->get_api(), &VdGrp, &stGrpParam)) {
        return false;
    }

    AX_S32 ret = ::AX_VDEC_SetGrpParam(VdGrp, &stGrpParam);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VDEC_API_IMPL(AX_VDEC_GetGrpParam) {
    AX_VDEC_GRP VdGrp;
    AX_VDEC_GRP_PARAM_T stGrpParam;

    if (!response->unpack(response->get_api(), &VdGrp)) {
        return false;
    }

    AX_S32 ret = ::AX_VDEC_GetGrpParam(VdGrp, &stGrpParam);

    if (!request->pack(response->get_api(), ret, &stGrpParam)) {
        return false;
    }

    return true;
}

NATIVE_VDEC_API_IMPL(AX_VDEC_SelectGrp) {
    AX_VDEC_GRP_SET_INFO_T stGrpSet;
    AX_S32 s32MilliSec;

    if (!response->unpack(response->get_api(), &s32MilliSec)) {
        return false;
    }

    AX_S32 ret = ::AX_VDEC_SelectGrp(&stGrpSet, s32MilliSec);

    if (!request->pack(response->get_api(), ret, &stGrpSet)) {
        return false;
    }

    return true;
}

NATIVE_VDEC_API_IMPL(AX_VDEC_SendStream) {
    AX_VDEC_GRP VdGrp;
    AX_VDEC_STREAM_T stStream;
    AX_S32 s32MilliSec;
    axcl::pkg::uint8_array nalu;

    if (!response->unpack(response->get_api(), &VdGrp, &stStream, &s32MilliSec, &nalu)) {
        return false;
    }

    stStream.pu8Addr = (0 == nalu.size) ? NULL : nalu.data;
    stStream.u32StreamPackLen = nalu.size;

    AX_S32 ret = ::AX_VDEC_SendStream(VdGrp, &stStream, s32MilliSec);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VDEC_API_IMPL(AX_VDEC_GetChnFrame) {
    AX_VDEC_GRP VdGrp;
    AX_VDEC_CHN VdChn;
    AX_VIDEO_FRAME_INFO_T stFrameInfo;
    AX_S32 s32MilliSec;

    if (!response->unpack(response->get_api(), &VdGrp, &VdChn, &s32MilliSec)) {
        return false;
    }

    AX_S32 ret = ::AX_VDEC_GetChnFrame(VdGrp, VdChn, &stFrameInfo, s32MilliSec);

    if (!request->pack(response->get_api(), ret, &stFrameInfo)) {
        return false;
    }

    return true;
}

NATIVE_VDEC_API_IMPL(AX_VDEC_ReleaseChnFrame) {
    AX_VDEC_GRP VdGrp;
    AX_VDEC_CHN VdChn;
    AX_VIDEO_FRAME_INFO_T stFrameInfo;

    if (!response->unpack(response->get_api(), &VdGrp, &VdChn, &stFrameInfo)) {
        return false;
    }

    AX_S32 ret = ::AX_VDEC_ReleaseChnFrame(VdGrp, VdChn, &stFrameInfo);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VDEC_API_IMPL(AX_VDEC_GetUserData) {
    AX_VDEC_GRP VdGrp;
    AX_VDEC_USERDATA_T stUserData;

    if (!response->unpack(response->get_api(), &VdGrp)) {
        return false;
    }

    AX_S32 ret = ::AX_VDEC_GetUserData(VdGrp, &stUserData);

    axcl::pkg::uint8_array nalu;
    if (0 == ret && stUserData.u32Len > 0) {
        nalu.data = static_cast<uint8_t *>(stUserData.pu8Addr);
        nalu.size = stUserData.u32Len;
    }

    if (!request->pack(response->get_api(), ret, &stUserData, nalu)) {
        return false;
    }

    return true;
}

NATIVE_VDEC_API_IMPL(AX_VDEC_ReleaseUserData) {
    AX_VDEC_GRP VdGrp;
    AX_VDEC_USERDATA_T stUserData;

    if (!response->unpack(response->get_api(), &VdGrp, &stUserData)) {
        return false;
    }

    AX_S32 ret = ::AX_VDEC_ReleaseUserData(VdGrp, &stUserData);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VDEC_API_IMPL(AX_VDEC_SetUserPic) {
    AX_VDEC_GRP VdGrp;
    AX_VDEC_USRPIC_T stUsrPic;

    if (!response->unpack(response->get_api(), &VdGrp, &stUsrPic)) {
        return false;
    }

    AX_S32 ret = ::AX_VDEC_SetUserPic(VdGrp, &stUsrPic);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VDEC_API_IMPL(AX_VDEC_EnableUserPic) {
    AX_VDEC_GRP VdGrp;

    if (!response->unpack(response->get_api(), &VdGrp)) {
        return false;
    }

    AX_S32 ret = ::AX_VDEC_EnableUserPic(VdGrp);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VDEC_API_IMPL(AX_VDEC_DisableUserPic) {
    AX_VDEC_GRP VdGrp;

    if (!response->unpack(response->get_api(), &VdGrp)) {
        return false;
    }

    AX_S32 ret = ::AX_VDEC_DisableUserPic(VdGrp);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VDEC_API_IMPL(AX_VDEC_SetDisplayMode) {
    AX_VDEC_GRP VdGrp;
    AX_VDEC_DISPLAY_MODE_E enDisplayMode;

    if (!response->unpack(response->get_api(), &VdGrp, &enDisplayMode)) {
        return false;
    }

    AX_S32 ret = ::AX_VDEC_SetDisplayMode(VdGrp, enDisplayMode);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VDEC_API_IMPL(AX_VDEC_GetDisplayMode) {
    AX_VDEC_GRP VdGrp;
    AX_VDEC_DISPLAY_MODE_E enDisplayMode;

    if (!response->unpack(response->get_api(), &VdGrp)) {
        return false;
    }

    AX_S32 ret = ::AX_VDEC_GetDisplayMode(VdGrp, &enDisplayMode);

    if (!request->pack(response->get_api(), ret, enDisplayMode)) {
        return false;
    }

    return true;
}

NATIVE_VDEC_API_IMPL(AX_VDEC_AttachPool) {
    AX_VDEC_GRP VdGrp;
    AX_VDEC_CHN VdChn;
    AX_POOL PoolId;

    if (!response->unpack(response->get_api(), &VdGrp, &VdChn, &PoolId)) {
        return false;
    }

    AX_S32 ret = ::AX_VDEC_AttachPool(VdGrp, VdChn, PoolId);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VDEC_API_IMPL(AX_VDEC_DetachPool) {
    AX_VDEC_GRP VdGrp;
    AX_VDEC_CHN VdChn;

    if (!response->unpack(response->get_api(), &VdGrp, &VdChn)) {
        return false;
    }

    AX_S32 ret = ::AX_VDEC_DetachPool(VdGrp, VdChn);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VDEC_API_IMPL(AX_VDEC_EnableChn) {
    AX_VDEC_GRP VdGrp;
    AX_VDEC_CHN VdChn;

    if (!response->unpack(response->get_api(), &VdGrp, &VdChn)) {
        return false;
    }

    AX_S32 ret = ::AX_VDEC_EnableChn(VdGrp, VdChn);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VDEC_API_IMPL(AX_VDEC_DisableChn) {
    AX_VDEC_GRP VdGrp;
    AX_VDEC_CHN VdChn;

    if (!response->unpack(response->get_api(), &VdGrp, &VdChn)) {
        return false;
    }

    AX_S32 ret = ::AX_VDEC_DisableChn(VdGrp, VdChn);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VDEC_API_IMPL(AX_VDEC_SetChnAttr) {
    AX_VDEC_GRP VdGrp;
    AX_VDEC_CHN VdChn;
    AX_VDEC_CHN_ATTR_T stVdChnAttr;

    if (!response->unpack(response->get_api(), &VdGrp, &VdChn, &stVdChnAttr)) {
        return false;
    }

    AX_S32 ret = ::AX_VDEC_SetChnAttr(VdGrp, VdChn, &stVdChnAttr);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_VDEC_API_IMPL(AX_VDEC_GetChnAttr) {
    AX_VDEC_GRP VdGrp;
    AX_VDEC_CHN VdChn;
    AX_VDEC_CHN_ATTR_T stVdChnAttr;

    if (!response->unpack(response->get_api(), &VdGrp, &VdChn)) {
        return false;
    }

    AX_S32 ret = ::AX_VDEC_GetChnAttr(VdGrp, VdChn, &stVdChnAttr);

    if (!request->pack(response->get_api(), ret, &stVdChnAttr)) {
        return false;
    }

    return true;
}

NATIVE_VDEC_API_IMPL(AX_VDEC_JpegDecodeOneFrame) {
    AX_VDEC_DEC_ONE_FRM_T stParam;

    if (!response->unpack(response->get_api(), &stParam)) {
        return false;
    }

    AX_S32 ret = ::AX_VDEC_JpegDecodeOneFrame(&stParam);

    if (!request->pack(response->get_api(), ret, &stParam)) {
        return false;
    }

    return true;
}

NATIVE_VDEC_API_IMPL(AX_VDEC_GetStreamBufInfo) {
    AX_VDEC_GRP VdGrp;
    AX_VDEC_STREAM_BUF_INFO_T stStreamBufInfo;

    if (!response->unpack(response->get_api(), &VdGrp)) {
        return false;
    }

    AX_S32 ret = ::AX_VDEC_GetStreamBufInfo(VdGrp, &stStreamBufInfo);

    if (!request->pack(response->get_api(), ret, &stStreamBufInfo)) {
        return false;
    }

    return true;
}

NATIVE_VDEC_API_IMPL(AX_VDEC_GetVuiParam) {
    AX_VDEC_GRP VdGrp;
    AX_VDEC_VUI_PARAM_T stVuiParam;

    if (!response->unpack(response->get_api(), &VdGrp)) {
        return false;
    }

    AX_S32 ret = ::AX_VDEC_GetVuiParam(VdGrp, &stVuiParam);

    if (!request->pack(response->get_api(), ret, &stVuiParam)) {
        return false;
    }

    return true;
}
}  // namespace axcl::worker
