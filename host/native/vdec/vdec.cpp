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

std::unordered_map<int32_t /* device */, std::unordered_map<AX_VDEC_GRP, std::vector<uint8_t>>> vdec::m_usrdata_mem_map;

AX_S32 vdec::AX_VDEC_Init(const AX_VDEC_MOD_ATTR_T *pstModAttr) {
    axcl::pkg::native::VDEC_API api(axcl::pkg::native::VDEC_API::AX_VDEC_Init);
    AX_S32 ret;

    if (!m_req.pack(api, pstModAttr)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 vdec::AX_VDEC_Deinit() {
    axcl::pkg::native::VDEC_API api(axcl::pkg::native::VDEC_API::AX_VDEC_Deinit);
    AX_S32 ret;

    if (!m_req.pack(api)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 vdec::AX_VDEC_ExtractStreamHeaderInfo(const AX_VDEC_STREAM_T *pstStreamBuf, AX_PAYLOAD_TYPE_E enVideoType,
                                             AX_VDEC_BITSTREAM_INFO_T *pstBitStreamInfo) {
    axcl::pkg::native::VDEC_API api(axcl::pkg::native::VDEC_API::AX_VDEC_ExtractStreamHeaderInfo);
    AX_S32 ret;

    if (!m_req.pack(api, pstStreamBuf, enVideoType)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pstBitStreamInfo)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 vdec::AX_VDEC_CreateGrp(AX_VDEC_GRP VdGrp, const AX_VDEC_GRP_ATTR_T *pstGrpAttr) {
    axcl::pkg::native::VDEC_API api(axcl::pkg::native::VDEC_API::AX_VDEC_CreateGrp);
    AX_S32 ret;

    if (!m_req.pack(api, VdGrp, pstGrpAttr)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 vdec::AX_VDEC_CreateGrpEx(AX_VDEC_GRP *VdGrp, const AX_VDEC_GRP_ATTR_T *pstGrpAttr) {
    axcl::pkg::native::VDEC_API api(axcl::pkg::native::VDEC_API::AX_VDEC_CreateGrpEx);
    AX_S32 ret;

    if (!m_req.pack(api, pstGrpAttr)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, VdGrp)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 vdec::AX_VDEC_DestroyGrp(AX_VDEC_GRP VdGrp) {
    axcl::pkg::native::VDEC_API api(axcl::pkg::native::VDEC_API::AX_VDEC_DestroyGrp);
    AX_S32 ret;

    if (!m_req.pack(api, VdGrp)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 vdec::AX_VDEC_GetGrpAttr(AX_VDEC_GRP VdGrp, AX_VDEC_GRP_ATTR_T *pstGrpAttr) {
    axcl::pkg::native::VDEC_API api(axcl::pkg::native::VDEC_API::AX_VDEC_GetGrpAttr);
    AX_S32 ret;

    if (!m_req.pack(api, VdGrp)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pstGrpAttr)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 vdec::AX_VDEC_SetGrpAttr(AX_VDEC_GRP VdGrp, const AX_VDEC_GRP_ATTR_T *pstGrpAttr) {
    axcl::pkg::native::VDEC_API api(axcl::pkg::native::VDEC_API::AX_VDEC_SetGrpAttr);
    AX_S32 ret;

    if (!m_req.pack(api, VdGrp, pstGrpAttr)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 vdec::AX_VDEC_StartRecvStream(AX_VDEC_GRP VdGrp, const AX_VDEC_RECV_PIC_PARAM_T *pstRecvParam) {
    axcl::pkg::native::VDEC_API api(axcl::pkg::native::VDEC_API::AX_VDEC_StartRecvStream);
    AX_S32 ret;

    if (!m_req.pack(api, VdGrp, pstRecvParam)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 vdec::AX_VDEC_StopRecvStream(AX_VDEC_GRP VdGrp) {
    axcl::pkg::native::VDEC_API api(axcl::pkg::native::VDEC_API::AX_VDEC_StopRecvStream);
    AX_S32 ret;

    if (!m_req.pack(api, VdGrp)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 vdec::AX_VDEC_QueryStatus(AX_VDEC_GRP VdGrp, AX_VDEC_GRP_STATUS_T *pstGrpStatus) {
    axcl::pkg::native::VDEC_API api(axcl::pkg::native::VDEC_API::AX_VDEC_QueryStatus);
    AX_S32 ret;

    if (!m_req.pack(api, VdGrp)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pstGrpStatus)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 vdec::AX_VDEC_ResetGrp(AX_VDEC_GRP VdGrp) {
    axcl::pkg::native::VDEC_API api(axcl::pkg::native::VDEC_API::AX_VDEC_ResetGrp);
    AX_S32 ret;

    if (!m_req.pack(api, VdGrp)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 vdec::AX_VDEC_SetGrpParam(AX_VDEC_GRP VdGrp, const AX_VDEC_GRP_PARAM_T *pstGrpParam) {
    axcl::pkg::native::VDEC_API api(axcl::pkg::native::VDEC_API::AX_VDEC_SetGrpParam);
    AX_S32 ret;

    if (!m_req.pack(api, VdGrp, pstGrpParam)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 vdec::AX_VDEC_GetGrpParam(AX_VDEC_GRP VdGrp, AX_VDEC_GRP_PARAM_T *pstGrpParam) {
    axcl::pkg::native::VDEC_API api(axcl::pkg::native::VDEC_API::AX_VDEC_GetGrpParam);
    AX_S32 ret;

    if (!m_req.pack(api, VdGrp)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pstGrpParam)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 vdec::AX_VDEC_SelectGrp(AX_VDEC_GRP_SET_INFO_T *pstGrpSet, AX_S32 s32MilliSec) {
    axcl::pkg::native::VDEC_API api(axcl::pkg::native::VDEC_API::AX_VDEC_SelectGrp);
    AX_S32 ret;

    if (!m_req.pack(api, s32MilliSec)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(s32MilliSec); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pstGrpSet)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 vdec::AX_VDEC_SendStream(AX_VDEC_GRP VdGrp, const AX_VDEC_STREAM_T *pstStream, AX_S32 s32MilliSec) {
    axcl::pkg::native::VDEC_API api(axcl::pkg::native::VDEC_API::AX_VDEC_SendStream);
    AX_S32 ret;

    axcl::pkg::uint8_array nalu;
    nalu.data = pstStream->pu8Addr;
    nalu.size = pstStream->u32StreamPackLen;
    if (!m_req.pack(api, VdGrp, pstStream, s32MilliSec, nalu)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(s32MilliSec); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 vdec::AX_VDEC_GetChnFrame(AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn, AX_VIDEO_FRAME_INFO_T *pstFrameInfo, AX_S32 s32MilliSec) {
    axcl::pkg::native::VDEC_API api(axcl::pkg::native::VDEC_API::AX_VDEC_GetChnFrame);
    AX_S32 ret;

    if (!m_req.pack(api, VdGrp, VdChn, s32MilliSec)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(s32MilliSec); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pstFrameInfo)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 vdec::AX_VDEC_ReleaseChnFrame(AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn, const AX_VIDEO_FRAME_INFO_T *pstFrameInfo) {
    axcl::pkg::native::VDEC_API api(axcl::pkg::native::VDEC_API::AX_VDEC_ReleaseChnFrame);
    AX_S32 ret;

    if (!m_req.pack(api, VdGrp, VdChn, pstFrameInfo)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 vdec::AX_VDEC_GetUserData(AX_VDEC_GRP VdGrp, AX_VDEC_USERDATA_T *pstUserData) {
    axcl::pkg::native::VDEC_API api(axcl::pkg::native::VDEC_API::AX_VDEC_GetUserData);
    AX_S32 ret;

    if (!m_req.pack(api, VdGrp)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_ENCODE);
    }

    axclrtEnvironment env;
    if (ret = invoke(NO_TIMEOUT, &env); AXCL_SUCC != ret) {
        return ret;
    }

    axcl::pkg::uint8_array data;
    if (!m_rsp.unpack(api, &ret, pstUserData, &data)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_DECODE);
    }

    pstUserData->u64PhyAddr = 0;
    if (0 == ret && data.size > 0) {
        auto &mem = m_usrdata_mem_map[env.device][VdGrp];
        mem.assign(data.data, data.data + data.size);
        pstUserData->pu8Addr = mem.data();
    }

    return ret;
}

AX_S32 vdec::AX_VDEC_ReleaseUserData(AX_VDEC_GRP VdGrp, const AX_VDEC_USERDATA_T *pstUserData) {
    axcl::pkg::native::VDEC_API api(axcl::pkg::native::VDEC_API::AX_VDEC_ReleaseUserData);
    AX_S32 ret;

    if (!m_req.pack(api, VdGrp, pstUserData)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 vdec::AX_VDEC_SetUserPic(AX_VDEC_GRP VdGrp, const AX_VDEC_USRPIC_T *pstUsrPic) {
    axcl::pkg::native::VDEC_API api(axcl::pkg::native::VDEC_API::AX_VDEC_SetUserPic);
    AX_S32 ret;

    if (!m_req.pack(api, VdGrp, pstUsrPic)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 vdec::AX_VDEC_EnableUserPic(AX_VDEC_GRP VdGrp) {
    axcl::pkg::native::VDEC_API api(axcl::pkg::native::VDEC_API::AX_VDEC_EnableUserPic);
    AX_S32 ret;

    if (!m_req.pack(api, VdGrp)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 vdec::AX_VDEC_DisableUserPic(AX_VDEC_GRP VdGrp) {
    axcl::pkg::native::VDEC_API api(axcl::pkg::native::VDEC_API::AX_VDEC_DisableUserPic);
    AX_S32 ret;

    if (!m_req.pack(api, VdGrp)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 vdec::AX_VDEC_SetDisplayMode(AX_VDEC_GRP VdGrp, AX_VDEC_DISPLAY_MODE_E enDisplayMode) {
    axcl::pkg::native::VDEC_API api(axcl::pkg::native::VDEC_API::AX_VDEC_SetDisplayMode);
    AX_S32 ret;

    if (!m_req.pack(api, VdGrp, enDisplayMode)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 vdec::AX_VDEC_GetDisplayMode(AX_VDEC_GRP VdGrp, AX_VDEC_DISPLAY_MODE_E *penDisplayMode) {
    axcl::pkg::native::VDEC_API api(axcl::pkg::native::VDEC_API::AX_VDEC_GetDisplayMode);
    AX_S32 ret;

    if (!m_req.pack(api, VdGrp)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, penDisplayMode)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 vdec::AX_VDEC_AttachPool(AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn, AX_POOL PoolId) {
    axcl::pkg::native::VDEC_API api(axcl::pkg::native::VDEC_API::AX_VDEC_AttachPool);
    AX_S32 ret;

    if (!m_req.pack(api, VdGrp, VdChn, PoolId)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 vdec::AX_VDEC_DetachPool(AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn) {
    axcl::pkg::native::VDEC_API api(axcl::pkg::native::VDEC_API::AX_VDEC_DetachPool);
    AX_S32 ret;

    if (!m_req.pack(api, VdGrp, VdChn)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 vdec::AX_VDEC_EnableChn(AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn) {
    axcl::pkg::native::VDEC_API api(axcl::pkg::native::VDEC_API::AX_VDEC_EnableChn);
    AX_S32 ret;

    if (!m_req.pack(api, VdGrp, VdChn)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 vdec::AX_VDEC_DisableChn(AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn) {
    axcl::pkg::native::VDEC_API api(axcl::pkg::native::VDEC_API::AX_VDEC_DisableChn);
    AX_S32 ret;

    if (!m_req.pack(api, VdGrp, VdChn)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 vdec::AX_VDEC_SetChnAttr(AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn, const AX_VDEC_CHN_ATTR_T *pstVdChnAttr) {
    axcl::pkg::native::VDEC_API api(axcl::pkg::native::VDEC_API::AX_VDEC_SetChnAttr);
    AX_S32 ret;

    if (!m_req.pack(api, VdGrp, VdChn, pstVdChnAttr)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 vdec::AX_VDEC_GetChnAttr(AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn, AX_VDEC_CHN_ATTR_T *pstVdChnAttr) {
    axcl::pkg::native::VDEC_API api(axcl::pkg::native::VDEC_API::AX_VDEC_GetChnAttr);
    AX_S32 ret;

    if (!m_req.pack(api, VdGrp, VdChn)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pstVdChnAttr)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 vdec::AX_VDEC_JpegDecodeOneFrame(AX_VDEC_DEC_ONE_FRM_T *pstParam) {
    axcl::pkg::native::VDEC_API api(axcl::pkg::native::VDEC_API::AX_VDEC_JpegDecodeOneFrame);
    AX_S32 ret;

    if (!m_req.pack(api, pstParam)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pstParam)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 vdec::AX_VDEC_GetStreamBufInfo(AX_VDEC_GRP VdGrp, AX_VDEC_STREAM_BUF_INFO_T *pstStreamBufInfo) {
    axcl::pkg::native::VDEC_API api(axcl::pkg::native::VDEC_API::AX_VDEC_GetStreamBufInfo);
    AX_S32 ret;

    if (!m_req.pack(api, VdGrp)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pstStreamBufInfo)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 vdec::AX_VDEC_GetVuiParam(AX_VDEC_GRP VdGrp, AX_VDEC_VUI_PARAM_T *pstVuiParam) {
    axcl::pkg::native::VDEC_API api(axcl::pkg::native::VDEC_API::AX_VDEC_GetVuiParam);
    AX_S32 ret;

    if (!m_req.pack(api, VdGrp)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pstVuiParam)) {
        return AXCL_DEF_VDEC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}
