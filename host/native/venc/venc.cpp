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
#include <string.h>
#include "axcl_logger.hpp"

#define TAG "venc"

std::unordered_map<int32_t /* device */, std::unordered_map<VENC_CHN, std::vector<uint8_t>>> venc::m_svc_mem_map;

AX_S32 venc::AX_VENC_Init(const AX_VENC_MOD_ATTR_T *pstModAttr) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_Init);
    AX_S32 ret;

    if (!m_req.pack(api, pstModAttr)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_Deinit() {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_Deinit);
    AX_S32 ret;

    if (!m_req.pack(api)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_CreateChn(VENC_CHN VeChn, const AX_VENC_CHN_ATTR_T *pstAttr) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_CreateChn);
    AX_S32 ret;

    axcl::pkg::uint8_array svc = {nullptr, 0};
    if (AX_VENC_GOPMODE_SVC_T == pstAttr->stGopAttr.enGopMode) {
        if (!m_req.serialize_gop_svc_t(pstAttr->stGopAttr.stSvcT, svc.data, svc.size)) {
            return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
        }
    }

    if (!m_req.pack(api, VeChn, pstAttr, svc)) {
        if (AX_VENC_GOPMODE_SVC_T == pstAttr->stGopAttr.enGopMode) {
            ::free(svc.data);
        }

        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (AX_VENC_GOPMODE_SVC_T == pstAttr->stGopAttr.enGopMode) {
        ::free(svc.data);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_CreateChnEx(VENC_CHN *pVeChn, const AX_VENC_CHN_ATTR_T *pstAttr) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_CreateChnEx);
    AX_S32 ret;

    axcl::pkg::uint8_array svc = {nullptr, 0};
    if (AX_VENC_GOPMODE_SVC_T == pstAttr->stGopAttr.enGopMode) {
        if (!m_req.serialize_gop_svc_t(pstAttr->stGopAttr.stSvcT, svc.data, svc.size)) {
            return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
        }
    }

    if (!m_req.pack(api, pstAttr, svc)) {
        if (AX_VENC_GOPMODE_SVC_T == pstAttr->stGopAttr.enGopMode) {
            ::free(svc.data);
        }

        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (AX_VENC_GOPMODE_SVC_T == pstAttr->stGopAttr.enGopMode) {
        ::free(svc.data);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pVeChn)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_DestroyChn(VENC_CHN VeChn) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_DestroyChn);
    AX_S32 ret;

    if (!m_req.pack(api, VeChn)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_SendFrame(VENC_CHN VeChn, const AX_VIDEO_FRAME_INFO_T *pstFrame, AX_S32 s32MilliSec) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_SendFrame);
    AX_S32 ret;

    if (!m_req.pack(api, VeChn, pstFrame, s32MilliSec)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(s32MilliSec); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_SendFrameEx(VENC_CHN VeChn, const AX_USER_FRAME_INFO_T *pstFrame, AX_S32 s32MilliSec) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_SendFrameEx);
    AX_S32 ret;

    if (!m_req.pack(api, VeChn, pstFrame, s32MilliSec)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(s32MilliSec); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_SelectGrp(VENC_GRP grpId, AX_CHN_STREAM_STATUS_T *pstChnStrmState, AX_S32 s32MilliSec) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_SelectGrp);
    AX_S32 ret;

    if (!m_req.pack(api, grpId, s32MilliSec)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(s32MilliSec); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pstChnStrmState)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_SelectClearGrp(VENC_GRP grpId) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_SelectClearGrp);
    AX_S32 ret;

    if (!m_req.pack(api, grpId)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_SelectGrpAddChn(VENC_GRP grpId, VENC_CHN VeChn) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_SelectGrpAddChn);
    AX_S32 ret;

    if (!m_req.pack(api, grpId, VeChn)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_SelectGrpDeleteChn(VENC_GRP grpId, VENC_CHN VeChn) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_SelectGrpDeleteChn);
    AX_S32 ret;

    if (!m_req.pack(api, grpId, VeChn)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_SelectGrpQuery(VENC_GRP grpId, AX_VENC_SELECT_GRP_PARAM_T *pstGrpInfo) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_SelectGrpQuery);
    AX_S32 ret;

    if (!m_req.pack(api, grpId)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pstGrpInfo)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_GetFd(VENC_CHN VeChn) {
    LOG_MM_E(TAG, "unsupported");
    return AXCL_DEF_VENC_ERR(AXCL_ERR_UNSUPPORT);
}

AX_S32 venc::AX_VENC_GetStream(VENC_CHN VeChn, AX_VENC_STREAM_T *pstStream, AX_S32 s32MilliSec) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_GetStream);
    AX_S32 ret;

    if (!m_req.pack(api, VeChn, s32MilliSec)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(s32MilliSec); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pstStream)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_ReleaseStream(VENC_CHN VeChn, const AX_VENC_STREAM_T *pstStream) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_ReleaseStream);
    AX_S32 ret;

    if (!m_req.pack(api, VeChn, pstStream)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_GetStreamBufInfo(VENC_CHN VeChn, AX_VENC_STREAM_BUF_INFO_T *pstStreamBufInfo) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_GetStreamBufInfo);
    AX_S32 ret;

    if (!m_req.pack(api, VeChn)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pstStreamBufInfo)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_StartRecvFrame(VENC_CHN VeChn, const AX_VENC_RECV_PIC_PARAM_T *pstRecvParam) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_StartRecvFrame);
    AX_S32 ret;

    if (!m_req.pack(api, VeChn, pstRecvParam)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_StopRecvFrame(VENC_CHN VeChn) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_StopRecvFrame);
    AX_S32 ret;

    if (!m_req.pack(api, VeChn)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_ResetChn(VENC_CHN VeChn) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_ResetChn);
    AX_S32 ret;

    if (!m_req.pack(api, VeChn)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_SetRoiAttr(VENC_CHN VeChn, const AX_VENC_ROI_ATTR_T *pstRoiAttr) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_SetRoiAttr);
    AX_S32 ret;

    if (!m_req.pack(api, VeChn, pstRoiAttr)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_GetRoiAttr(VENC_CHN VeChn, AX_U32 u32Index, AX_VENC_ROI_ATTR_T *pstRoiAttr) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_GetRoiAttr);
    AX_S32 ret;

    if (!m_req.pack(api, VeChn, u32Index)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pstRoiAttr)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_SetRcParam(VENC_CHN VeChn, const AX_VENC_RC_PARAM_T *pstRcParam) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_SetRcParam);
    AX_S32 ret;

    if (!m_req.pack(api, VeChn, pstRcParam)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_GetRcParam(VENC_CHN VeChn, AX_VENC_RC_PARAM_T *pstRcParam) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_GetRcParam);
    AX_S32 ret;

    if (!m_req.pack(api, VeChn)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pstRcParam)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_SetModParam(AX_VENC_ENCODER_TYPE_E enVencType, const AX_VENC_MOD_PARAM_T *pstModParam) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_SetModParam);
    AX_S32 ret;

    if (!m_req.pack(api, enVencType, pstModParam)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_GetModParam(AX_VENC_ENCODER_TYPE_E enVencType, AX_VENC_MOD_PARAM_T *pstModParam) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_GetModParam);
    AX_S32 ret;

    if (!m_req.pack(api, enVencType)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pstModParam)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_SetVuiParam(VENC_CHN VeChn, const AX_VENC_VUI_PARAM_T *pstVuiParam) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_SetVuiParam);
    AX_S32 ret;

    if (!m_req.pack(api, VeChn, pstVuiParam)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_GetVuiParam(VENC_CHN VeChn, AX_VENC_VUI_PARAM_T *pstVuiParam) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_GetVuiParam);
    AX_S32 ret;

    if (!m_req.pack(api, VeChn)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pstVuiParam)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_SetChnAttr(VENC_CHN VeChn, const AX_VENC_CHN_ATTR_T *pstChnAttr) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_SetChnAttr);
    AX_S32 ret;

    axcl::pkg::uint8_array svc = {nullptr, 0};
    if (AX_VENC_GOPMODE_SVC_T == pstChnAttr->stGopAttr.enGopMode) {
        if (!m_req.serialize_gop_svc_t(pstChnAttr->stGopAttr.stSvcT, svc.data, svc.size)) {
            return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
        }
    }

    if (!m_req.pack(api, VeChn, pstChnAttr, svc)) {
        if (AX_VENC_GOPMODE_SVC_T == pstChnAttr->stGopAttr.enGopMode) {
            ::free(svc.data);
        }

        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (AX_VENC_GOPMODE_SVC_T == pstChnAttr->stGopAttr.enGopMode) {
        ::free(svc.data);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_GetChnAttr(VENC_CHN VeChn, AX_VENC_CHN_ATTR_T *pstChnAttr) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_GetChnAttr);
    AX_S32 ret;

    if (!m_req.pack(api, VeChn)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    axclrtEnvironment env;
    if (ret = invoke(NO_TIMEOUT, &env); AXCL_SUCC != ret) {
        return ret;
    }

    axcl::pkg::uint8_array svc;
    if (!m_rsp.unpack(api, &ret, pstChnAttr, &svc)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    if (AX_VENC_GOPMODE_SVC_T == pstChnAttr->stGopAttr.enGopMode) {
        if (0 == svc.size) {
            pstChnAttr->stGopAttr.stSvcT.s8SvcTCfg = nullptr;
            LOG_MM_W(TAG, "svc size is 0");
            return AXCL_SUCC;
        }

        std::vector<uint8_t> &svc_mem = m_svc_mem_map[env.device][VeChn];
        if (!m_rsp.deserialize_gop_svc_t(svc.data, svc.size, pstChnAttr->stGopAttr.stSvcT, svc_mem)) {
            return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
        }
    }

    return ret;
}

AX_S32 venc::AX_VENC_SetRateJamStrategy(VENC_CHN VeChn, const AX_VENC_RATE_JAM_CFG_T *pstRateJamParam) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_SetRateJamStrategy);
    AX_S32 ret;

    if (!m_req.pack(api, VeChn, pstRateJamParam)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_GetRateJamStrategy(VENC_CHN VeChn, AX_VENC_RATE_JAM_CFG_T *pstRateJamParam) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_GetRateJamStrategy);
    AX_S32 ret;

    if (!m_req.pack(api, VeChn)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pstRateJamParam)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_SetSuperFrameStrategy(VENC_CHN VeChn, const AX_VENC_SUPERFRAME_CFG_T *pstSuperFrameCfg) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_SetSuperFrameStrategy);
    AX_S32 ret;

    if (!m_req.pack(api, VeChn, pstSuperFrameCfg)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_GetSuperFrameStrategy(VENC_CHN VeChn, AX_VENC_SUPERFRAME_CFG_T *pstSuperFrameCfg) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_GetSuperFrameStrategy);
    AX_S32 ret;

    if (!m_req.pack(api, VeChn)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pstSuperFrameCfg)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_SetIntraRefresh(VENC_CHN VeChn, const AX_VENC_INTRA_REFRESH_T *pstIntraRefresh) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_SetIntraRefresh);
    AX_S32 ret;

    if (!m_req.pack(api, VeChn, pstIntraRefresh)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_GetIntraRefresh(VENC_CHN VeChn, AX_VENC_INTRA_REFRESH_T *pstIntraRefresh) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_GetIntraRefresh);
    AX_S32 ret;

    if (!m_req.pack(api, VeChn)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pstIntraRefresh)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_SetUsrData(VENC_CHN VeChn, const AX_VENC_USR_DATA_T *pstUsrData) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_SetUsrData);
    AX_S32 ret;

    axcl::pkg::uint8_array data;
    data.data = pstUsrData->pu8UsrData;
    data.size = pstUsrData->u32DataSize;
    if (!m_req.pack(api, VeChn, pstUsrData, data)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_GetUsrData(VENC_CHN VeChn, AX_VENC_USR_DATA_T *pstUsrData) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_GetUsrData);
    AX_S32 ret;

    if (!m_req.pack(api, VeChn, pstUsrData)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    axcl::pkg::uint8_array data;
    AX_VENC_USR_DATA_T usrdata;
    if (!m_rsp.unpack(api, &ret, &usrdata, &data)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    if (0 != ret) {
        return ret;
    }

    pstUsrData->bEnable = usrdata.bEnable;
    pstUsrData->u32DataSize = usrdata.u32DataSize;
    if (usrdata.u32DataSize <= pstUsrData->u32DataSize) {
        ::memcpy(pstUsrData->pu8UsrData, data.data, data.size);
    } else {
        LOG_MM_E(TAG, "usrdata size from device is too big {}", usrdata.u32DataSize);
        return AXCL_DEF_VENC_ERR(AXCL_ERR_NO_MEMORY);
    }

    return 0;
}

AX_S32 venc::AX_VENC_SetSliceSplit(VENC_CHN VeChn, const AX_VENC_SLICE_SPLIT_T *pstSliceSplit) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_SetSliceSplit);
    AX_S32 ret;

    if (!m_req.pack(api, VeChn, pstSliceSplit)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_GetSliceSplit(VENC_CHN VeChn, AX_VENC_SLICE_SPLIT_T *pstSliceSplit) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_GetSliceSplit);
    AX_S32 ret;

    if (!m_req.pack(api, VeChn)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pstSliceSplit)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_RequestIDR(VENC_CHN VeChn, AX_BOOL bInstant) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_RequestIDR);
    AX_S32 ret;

    if (!m_req.pack(api, VeChn, bInstant)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_QueryStatus(VENC_CHN VeChn, AX_VENC_CHN_STATUS_T *pstStatus) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_QueryStatus);
    AX_S32 ret;

    if (!m_req.pack(api, VeChn)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pstStatus)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_SetJpegParam(VENC_CHN VeChn, const AX_VENC_JPEG_PARAM_T *pstJpegParam) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_SetJpegParam);
    AX_S32 ret;

    if (!m_req.pack(api, VeChn, pstJpegParam)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_GetJpegParam(VENC_CHN VeChn, AX_VENC_JPEG_PARAM_T *pstJpegParam) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_GetJpegParam);
    AX_S32 ret;

    if (!m_req.pack(api, VeChn)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pstJpegParam)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_JpegEncodeOneFrame(AX_JPEG_ENCODE_ONCE_PARAMS_T *pstJpegParam) {
    axcl::pkg::native::VENC_API api(axcl::pkg::native::VENC_API::AX_VENC_JpegEncodeOneFrame);
    AX_S32 ret;

    if (!m_req.pack(api, pstJpegParam)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pstJpegParam)) {
        return AXCL_DEF_VENC_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 venc::AX_VENC_JpegGetThumbnail(const AX_VOID *pRawData, AX_VOID *pThumbData, AX_U32 *pThumbSize) {
    LOG_MM_E(TAG, "unsupported");
    return AXCL_DEF_VENC_ERR(AXCL_ERR_UNSUPPORT);
}
