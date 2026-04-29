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
#include <string.h>
#include "axcl_logger.hpp"

#define TAG "ivps"

AX_S32 ivps::AX_IVPS_Init() {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_Init);
    int32_t ret;

    if (!m_req.pack(api)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_Deinit() {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_Deinit);
    int32_t ret;

    if (!m_req.pack(api)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_CreateGrp(IVPS_GRP IvpsGrp, const AX_IVPS_GRP_ATTR_T *ptGrpAttr) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_CreateGrp);
    int32_t ret;

    if (!m_req.pack(api, IvpsGrp, ptGrpAttr)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_CreateGrpEx(IVPS_GRP *IvpsGrp, const AX_IVPS_GRP_ATTR_T *ptGrpAttr) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_CreateGrpEx);
    int32_t ret;

    if (!m_req.pack(api, ptGrpAttr)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, IvpsGrp)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_DestoryGrp(IVPS_GRP IvpsGrp) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_DestoryGrp);
    int32_t ret;

    if (!m_req.pack(api, IvpsGrp)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_SetPipelineAttr(IVPS_GRP IvpsGrp, AX_IVPS_PIPELINE_ATTR_T *ptPipelineAttr) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_SetPipelineAttr);
    int32_t ret;

    if (!m_req.pack(api, IvpsGrp, ptPipelineAttr)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_GetPipelineAttr(IVPS_GRP IvpsGrp, AX_IVPS_PIPELINE_ATTR_T *ptPipelineAttr) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_GetPipelineAttr);
    int32_t ret;

    if (!m_req.pack(api, IvpsGrp)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, ptPipelineAttr)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_StartGrp(IVPS_GRP IvpsGrp) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_StartGrp);
    int32_t ret;

    if (!m_req.pack(api, IvpsGrp)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_StopGrp(IVPS_GRP IvpsGrp) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_StopGrp);
    int32_t ret;

    if (!m_req.pack(api, IvpsGrp)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_EnableChn(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_EnableChn);
    int32_t ret;

    if (!m_req.pack(api, IvpsGrp, IvpsChn)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_DisableChn(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_DisableChn);
    int32_t ret;

    if (!m_req.pack(api, IvpsGrp, IvpsChn)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_SendFrame(IVPS_GRP IvpsGrp, const AX_VIDEO_FRAME_T *ptFrame, AX_S32 nMilliSec) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_SendFrame);
    int32_t ret;

    if (!m_req.pack(api, IvpsGrp, ptFrame, nMilliSec)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(nMilliSec); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_GetChnFrame(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn, AX_VIDEO_FRAME_T *ptFrame, AX_S32 nMilliSec) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_GetChnFrame);
    int32_t ret;

    if (!m_req.pack(api, IvpsGrp, IvpsChn, nMilliSec)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(nMilliSec); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, ptFrame)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_ReleaseChnFrame(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn, AX_VIDEO_FRAME_T *ptFrame) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_ReleaseChnFrame);
    int32_t ret;

    if (!m_req.pack(api, IvpsGrp, IvpsChn, ptFrame)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_GetGrpFrame(IVPS_GRP IvpsGrp, AX_VIDEO_FRAME_T *ptFrame, AX_S32 nMilliSec) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_GetGrpFrame);
    int32_t ret;

    if (!m_req.pack(api, IvpsGrp, nMilliSec)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(nMilliSec); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, ptFrame)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_ReleaseGrpFrame(IVPS_GRP IvpsGrp, AX_VIDEO_FRAME_T *ptFrame) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_ReleaseGrpFrame);
    int32_t ret;

    if (!m_req.pack(api, IvpsGrp, ptFrame)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_GetChnFd(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn) {
    LOG_MM_E(TAG, "unsupported");
    return AXCL_DEF_IVPS_ERR(AXCL_ERR_UNSUPPORT);
}

AX_S32 ivps::AX_IVPS_GetDebugFifoFrame(IVPS_GRP IvpsGrp, AX_VIDEO_FRAME_T *ptFrame) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_GetDebugFifoFrame);
    int32_t ret;

    if (!m_req.pack(api, IvpsGrp)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, ptFrame)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_ReleaseDebugFifoFrame(IVPS_GRP IvpsGrp, AX_VIDEO_FRAME_T *ptFrame) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_ReleaseDebugFifoFrame);
    int32_t ret;

    if (!m_req.pack(api, IvpsGrp, ptFrame)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_CloseAllFd() {
    LOG_MM_E(TAG, "unsupported");
    return AXCL_DEF_IVPS_ERR(AXCL_ERR_UNSUPPORT);
}

AX_S32 ivps::AX_IVPS_SetGrpLDCAttr(IVPS_GRP IvpsGrp, IVPS_FILTER IvpsFilter, const AX_IVPS_LDC_ATTR_T *ptLDCAttr) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_SetGrpLDCAttr);
    int32_t ret;

    if (!m_req.pack(api, IvpsGrp, IvpsFilter, ptLDCAttr)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_GetGrpLDCAttr(IVPS_GRP IvpsGrp, IVPS_FILTER IvpsFilter, AX_IVPS_LDC_ATTR_T *ptLDCAttr) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_GetGrpLDCAttr);
    int32_t ret;

    if (!m_req.pack(api, IvpsGrp, IvpsFilter)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, ptLDCAttr)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_SetChnLDCAttr(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn, IVPS_FILTER IvpsFilter, const AX_IVPS_LDC_ATTR_T *ptLDCAttr) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_SetChnLDCAttr);
    int32_t ret;

    if (!m_req.pack(api, IvpsGrp, IvpsChn, IvpsFilter, ptLDCAttr)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_GetChnLDCAttr(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn, IVPS_FILTER IvpsFilter, AX_IVPS_LDC_ATTR_T *ptLDCAttr) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_GetChnLDCAttr);
    int32_t ret;

    if (!m_req.pack(api, IvpsGrp, IvpsChn, IvpsFilter)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, ptLDCAttr)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_SetGrpPoolAttr(IVPS_GRP IvpsGrp, const AX_IVPS_POOL_ATTR_T *ptPoolAttr) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_SetGrpPoolAttr);
    int32_t ret;

    if (!m_req.pack(api, IvpsGrp, ptPoolAttr)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_SetChnPoolAttr(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn, const AX_IVPS_POOL_ATTR_T *ptPoolAttr) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_SetChnPoolAttr);
    int32_t ret;

    if (!m_req.pack(api, IvpsGrp, IvpsChn, ptPoolAttr)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_SetGrpUserFRC(IVPS_GRP IvpsGrp, const AX_IVPS_USER_FRAME_RATE_CTRL_T *ptFrameRateAttr) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_SetGrpUserFRC);
    int32_t ret;

    if (!m_req.pack(api, IvpsGrp, ptFrameRateAttr)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_SetChnUserFRC(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn, const AX_IVPS_USER_FRAME_RATE_CTRL_T *ptFrameRateAttr) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_SetChnUserFRC);
    int32_t ret;

    if (!m_req.pack(api, IvpsGrp, IvpsChn, ptFrameRateAttr)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_SetGrpCrop(IVPS_GRP IvpsGrp, const AX_IVPS_CROP_INFO_T *ptCropInfo) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_SetGrpCrop);
    int32_t ret;

    if (!m_req.pack(api, IvpsGrp, ptCropInfo)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_GetGrpCrop(IVPS_GRP IvpsGrp, AX_IVPS_CROP_INFO_T *ptCropInfo) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_GetGrpCrop);
    int32_t ret;

    if (!m_req.pack(api, IvpsGrp)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, ptCropInfo)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_SetChnAttr(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn, IVPS_FILTER IvpsFilter, const AX_IVPS_CHN_ATTR_T *ptChnAttr) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_SetChnAttr);
    int32_t ret;

    if (!m_req.pack(api, IvpsGrp, IvpsChn, IvpsFilter, ptChnAttr)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_GetChnAttr(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn, IVPS_FILTER IvpsFilter, AX_IVPS_CHN_ATTR_T *ptChnAttr) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_GetChnAttr);
    int32_t ret;

    if (!m_req.pack(api, IvpsGrp, IvpsChn, IvpsFilter)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, ptChnAttr)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_EnableBackupFrame(IVPS_GRP IvpsGrp, AX_U8 nFifoDepth) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_EnableBackupFrame);
    int32_t ret;

    if (!m_req.pack(api, IvpsGrp, nFifoDepth)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_DisableBackupFrame(IVPS_GRP IvpsGrp) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_DisableBackupFrame);
    int32_t ret;

    if (!m_req.pack(api, IvpsGrp)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_ResetGrp(IVPS_GRP IvpsGrp) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_ResetGrp);
    int32_t ret;

    if (!m_req.pack(api, IvpsGrp)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_GetEngineDutyCycle(AX_IVPS_DUTY_CYCLE_ATTR_T *ptDutyCycle) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_GetEngineDutyCycle);
    int32_t ret;

    if (!m_req.pack(api, ptDutyCycle /* I|O */)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, ptDutyCycle)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

IVPS_RGN_HANDLE ivps::AX_IVPS_RGN_Create() {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_RGN_Create);
    int32_t ret;

    if (!m_req.pack(api)) {
        return AX_IVPS_INVALID_REGION_HANDLE;
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return AX_IVPS_INVALID_REGION_HANDLE;
    }

    IVPS_RGN_HANDLE handle;
    if (!m_rsp.unpack(api, &handle)) {
        return AX_IVPS_INVALID_REGION_HANDLE;
    }

    return handle;
}

AX_S32 ivps::AX_IVPS_RGN_Destroy(IVPS_RGN_HANDLE hRegion) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_RGN_Destroy);
    int32_t ret;

    if (!m_req.pack(api, hRegion)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_RGN_AttachToFilter(IVPS_RGN_HANDLE hRegion, IVPS_GRP IvpsGrp, IVPS_FILTER IvpsFilter) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_RGN_AttachToFilter);
    int32_t ret;

    if (!m_req.pack(api, hRegion, IvpsGrp, IvpsFilter)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_RGN_DetachFromFilter(IVPS_RGN_HANDLE hRegion, IVPS_GRP IvpsGrp, IVPS_FILTER IvpsFilter) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_RGN_DetachFromFilter);
    int32_t ret;

    if (!m_req.pack(api, hRegion, IvpsGrp, IvpsFilter)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_RGN_Update(IVPS_RGN_HANDLE hRegion, const AX_IVPS_RGN_DISP_GROUP_T *ptDisp) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_RGN_Update);
    int32_t ret;

    if (!m_req.pack(api, hRegion, ptDisp)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_CmmCopyTdp(AX_U64 nSrcPhyAddr, AX_U64 nDstPhyAddr, AX_U64 nMemSize) {
    return CmmCopy(axcl::pkg::native::IVPS_API::AX_IVPS_CmmCopyTdp, nSrcPhyAddr, nDstPhyAddr, nMemSize);
}

AX_S32 ivps::AX_IVPS_FlipAndRotationTdp(const AX_VIDEO_FRAME_T *ptSrc, AX_IVPS_CHN_FLIP_MODE_E eFlipMode, AX_IVPS_ROTATION_E eRotation,
                                        AX_VIDEO_FRAME_T *ptDst) {
    return FlipAndRotation(axcl::pkg::native::IVPS_API::AX_IVPS_FlipAndRotationTdp, ptSrc, eFlipMode, eRotation, ptDst);
}

AX_S32 ivps::AX_IVPS_CscTdp(const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst) {
    return Csc(axcl::pkg::native::IVPS_API::AX_IVPS_CscTdp, ptSrc, ptDst);
}

AX_S32 ivps::AX_IVPS_CropResizeTdp(const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst, const AX_IVPS_ASPECT_RATIO_T *ptAspectRatio) {
    return CropResize(axcl::pkg::native::IVPS_API::AX_IVPS_CropResizeTdp, ptSrc, ptDst, ptAspectRatio);
}

AX_S32 ivps::AX_IVPS_CropResizeV2Tdp(const AX_VIDEO_FRAME_T *ptSrc, const AX_IVPS_RECT_T tBox[], AX_U32 nCropNum, AX_VIDEO_FRAME_T *ptDst[],
                                     const AX_IVPS_ASPECT_RATIO_T *ptAspectRatio) {
    return CropResizeV2(axcl::pkg::native::IVPS_API::AX_IVPS_CropResizeV2Tdp, ptSrc, tBox, nCropNum, ptDst, ptAspectRatio);
}

AX_S32 ivps::AX_IVPS_AlphaBlendingTdp(const AX_VIDEO_FRAME_T *ptSrc, const AX_VIDEO_FRAME_T *ptOverlay, const AX_IVPS_POINT_T tOffset,
                                      AX_U8 nAlpha, AX_VIDEO_FRAME_T *ptDst) {
    return AlphaBlending(axcl::pkg::native::IVPS_API::AX_IVPS_AlphaBlendingTdp, ptSrc, ptOverlay, tOffset, nAlpha, ptDst);
}

AX_S32 ivps::AX_IVPS_AlphaBlendingV3Tdp(const AX_VIDEO_FRAME_T *ptSrc, const AX_OVERLAY_T *ptOverlay, AX_VIDEO_FRAME_T *ptDst) {
    return AlphaBlendingV3(axcl::pkg::native::IVPS_API::AX_IVPS_AlphaBlendingV3Tdp, ptSrc, ptOverlay, ptDst);
}

AX_S32 ivps::AX_IVPS_DrawOsdTdp(const AX_VIDEO_FRAME_T *ptFrame, const AX_OSD_BMP_ATTR_T arrBmp[], AX_U32 nNum) {
    return DrawOsd(axcl::pkg::native::IVPS_API::AX_IVPS_DrawOsdTdp, ptFrame, arrBmp, nNum);
}

AX_S32 ivps::AX_IVPS_DrawMosaicTdp(const AX_VIDEO_FRAME_T *ptSrc, AX_IVPS_RGN_MOSAIC_T tMosaic[], AX_U32 nNum) {
    return DrawMosaic(axcl::pkg::native::IVPS_API::AX_IVPS_DrawMosaicTdp, ptSrc, tMosaic, nNum);
}

AX_S32 ivps::AX_IVPS_CmmCopyVpp(AX_U64 nSrcPhyAddr, AX_U64 nDstPhyAddr, AX_U64 nMemSize) {
    return CmmCopy(axcl::pkg::native::IVPS_API::AX_IVPS_CmmCopyVpp, nSrcPhyAddr, nDstPhyAddr, nMemSize);
}

AX_S32 ivps::AX_IVPS_CropResizeVpp(const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst, const AX_IVPS_ASPECT_RATIO_T *ptAspectRatio) {
    return CropResize(axcl::pkg::native::IVPS_API::AX_IVPS_CropResizeVpp, ptSrc, ptDst, ptAspectRatio);
}

AX_S32 ivps::AX_IVPS_CropResizeV2Vpp(const AX_VIDEO_FRAME_T *ptSrc, const AX_IVPS_RECT_T tBox[], AX_U32 nCropNum, AX_VIDEO_FRAME_T *ptDst[],
                                     const AX_IVPS_ASPECT_RATIO_T *ptAspectRatio) {
    return CropResizeV2(axcl::pkg::native::IVPS_API::AX_IVPS_CropResizeV2Vpp, ptSrc, tBox, nCropNum, ptDst, ptAspectRatio);
}

AX_S32 ivps::AX_IVPS_CropResizeV3Vpp(const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst[], AX_U32 nNum,
                                     const AX_IVPS_ASPECT_RATIO_T *ptAspectRatio) {
    return CropResizeV3(axcl::pkg::native::IVPS_API::AX_IVPS_CropResizeV3Vpp, ptSrc, ptDst, nNum, ptAspectRatio);
}

AX_S32 ivps::AX_IVPS_CscVpp(const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst) {
    return Csc(axcl::pkg::native::IVPS_API::AX_IVPS_CscVpp, ptSrc, ptDst);
}

AX_S32 ivps::AX_IVPS_DrawMosaicVpp(const AX_VIDEO_FRAME_T *ptSrc, AX_IVPS_RGN_MOSAIC_T tMosaic[], AX_U32 nNum) {
    return DrawMosaic(axcl::pkg::native::IVPS_API::AX_IVPS_DrawMosaicVpp, ptSrc, tMosaic, nNum);
}

AX_S32 ivps::AX_IVPS_SetScaleCoefLevelVpp(const AX_IVPS_SCALE_RANGE_T *ScaleRange, const AX_IVPS_SCALE_COEF_LEVEL_T *CoefLevel) {
    return SetScaleCoefLevel(axcl::pkg::native::IVPS_API::AX_IVPS_SetScaleCoefLevelVpp, ScaleRange, CoefLevel);
}

AX_S32 ivps::AX_IVPS_GetScaleCoefLevelVpp(const AX_IVPS_SCALE_RANGE_T *ScaleRange, AX_IVPS_SCALE_COEF_LEVEL_T *CoefLevel) {
    return GetScaleCoefLevel(axcl::pkg::native::IVPS_API::AX_IVPS_GetScaleCoefLevelVpp, ScaleRange, CoefLevel);
}

AX_S32 ivps::AX_IVPS_CmmCopyVgp(AX_U64 nSrcPhyAddr, AX_U64 nDstPhyAddr, AX_U64 nMemSize) {
    return CmmCopy(axcl::pkg::native::IVPS_API::AX_IVPS_CmmCopyVgp, nSrcPhyAddr, nDstPhyAddr, nMemSize);
}

AX_S32 ivps::AX_IVPS_CscVgp(const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst) {
    return Csc(axcl::pkg::native::IVPS_API::AX_IVPS_CscVgp, ptSrc, ptDst);
}

AX_S32 ivps::AX_IVPS_CropResizeVgp(const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst, const AX_IVPS_ASPECT_RATIO_T *ptAspectRatio) {
    return CropResize(axcl::pkg::native::IVPS_API::AX_IVPS_CropResizeVgp, ptSrc, ptDst, ptAspectRatio);
}

AX_S32 ivps::AX_IVPS_CropResizeV2Vgp(const AX_VIDEO_FRAME_T *ptSrc, const AX_IVPS_RECT_T tBox[], AX_U32 nCropNum, AX_VIDEO_FRAME_T *ptDst[],
                                     const AX_IVPS_ASPECT_RATIO_T *ptAspectRatio) {
    return CropResizeV2(axcl::pkg::native::IVPS_API::AX_IVPS_CropResizeV2Vgp, ptSrc, tBox, nCropNum, ptDst, ptAspectRatio);
}

AX_S32 ivps::AX_IVPS_CropResizeV4Vgp(const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst, const AX_IVPS_ASPECT_RATIO_T *ptAspectRatio,
                                     const AX_IVPS_SCALE_STEP_T *ptScaleStep) {
    return CropResizeV4(axcl::pkg::native::IVPS_API::AX_IVPS_CropResizeV4Vgp, ptSrc, ptDst, ptAspectRatio, ptScaleStep);
}

AX_S32 ivps::AX_IVPS_AlphaBlendingVgp(const AX_VIDEO_FRAME_T *ptSrc, const AX_VIDEO_FRAME_T *ptOverlay, const AX_IVPS_POINT_T tOffset,
                                      AX_U8 nAlpha, AX_VIDEO_FRAME_T *ptDst) {
    return AlphaBlending(axcl::pkg::native::IVPS_API::AX_IVPS_AlphaBlendingVgp, ptSrc, ptOverlay, tOffset, nAlpha, ptDst);
}

AX_S32 ivps::AX_IVPS_AlphaBlendingV2Vgp(const AX_VIDEO_FRAME_T *ptSrc, const AX_VIDEO_FRAME_T *ptOverlay, const AX_IVPS_POINT_T tOffset,
                                        const AX_IVPS_ALPHA_LUT_T *ptSpAlpha, AX_VIDEO_FRAME_T *ptDst) {
    return AlphaBlendingV2(axcl::pkg::native::IVPS_API::AX_IVPS_AlphaBlendingV2Vgp, ptSrc, ptOverlay, tOffset, ptSpAlpha, ptDst);
}

AX_S32 ivps::AX_IVPS_AlphaBlendingV3Vgp(const AX_VIDEO_FRAME_T *ptSrc, const AX_OVERLAY_T *ptOverlay, AX_VIDEO_FRAME_T *ptDst) {
    return AlphaBlendingV3(axcl::pkg::native::IVPS_API::AX_IVPS_AlphaBlendingV3Vgp, ptSrc, ptOverlay, ptDst);
}

AX_S32 ivps::AX_IVPS_DrawOsdVgp(const AX_VIDEO_FRAME_T *ptFrame, const AX_OSD_BMP_ATTR_T arrBmp[], AX_U32 nNum) {
    return DrawOsd(axcl::pkg::native::IVPS_API::AX_IVPS_DrawOsdVgp, ptFrame, arrBmp, nNum);
}

AX_S32 ivps::AX_IVPS_DrawMosaicVgp(const AX_VIDEO_FRAME_T *ptSrc, AX_IVPS_RGN_MOSAIC_T tMosaic[], AX_U32 nNum) {
    return DrawMosaic(axcl::pkg::native::IVPS_API::AX_IVPS_DrawMosaicVgp, ptSrc, tMosaic, nNum);
}

AX_S32 ivps::AX_IVPS_SetScaleCoefLevelVgp(const AX_IVPS_SCALE_RANGE_T *ScaleRange, const AX_IVPS_SCALE_COEF_LEVEL_T *CoefLevel) {
    return SetScaleCoefLevel(axcl::pkg::native::IVPS_API::AX_IVPS_SetScaleCoefLevelVgp, ScaleRange, CoefLevel);
}

AX_S32 ivps::AX_IVPS_GetScaleCoefLevelVgp(const AX_IVPS_SCALE_RANGE_T *ScaleRange, AX_IVPS_SCALE_COEF_LEVEL_T *CoefLevel) {
    return GetScaleCoefLevel(axcl::pkg::native::IVPS_API::AX_IVPS_GetScaleCoefLevelVgp, ScaleRange, CoefLevel);
}

AX_S32 ivps::AX_IVPS_DrawLine(const AX_IVPS_RGN_CANVAS_INFO_T *ptCanvas, AX_IVPS_GDI_ATTR_T tAttr, const AX_IVPS_POINT_T tPoint[],
                              AX_U32 nPointNum) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_DrawLine);
    int32_t ret;

    /* tPoint is already continuos memory */
    axcl::pkg::uint8_array pts;
    pts.size = sizeof(AX_IVPS_POINT_T) * nPointNum;
    pts.data = const_cast<AX_U8 *>(reinterpret_cast<const AX_U8 *>(&tPoint[0]));
    if (!m_req.pack(api, ptCanvas, tAttr, pts, nPointNum)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_DrawPolygon(const AX_IVPS_RGN_CANVAS_INFO_T *ptCanvas, AX_IVPS_GDI_ATTR_T tAttr, const AX_IVPS_POINT_T tPoint[],
                                 AX_U32 nPointNum) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_DrawPolygon);
    int32_t ret;

    /* tPoint is already continuos memory */
    axcl::pkg::uint8_array pts;
    pts.size = sizeof(AX_IVPS_POINT_T) * nPointNum;
    pts.data = const_cast<AX_U8 *>(reinterpret_cast<const AX_U8 *>(&tPoint[0]));
    if (!m_req.pack(api, ptCanvas, tAttr, pts, nPointNum)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_DrawRect(const AX_IVPS_RGN_CANVAS_INFO_T *ptCanvas, AX_IVPS_GDI_ATTR_T tAttr, AX_IVPS_RECT_T tRect) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_DrawRect);
    int32_t ret;

    if (!m_req.pack(api, ptCanvas, tAttr, tRect)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_Dewarp(const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst, const AX_IVPS_DEWARP_ATTR_T *ptAttr) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_Dewarp);
    int32_t ret;

    if (!m_req.pack(api, ptSrc, ptDst /* I|O */, ptAttr)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, ptDst)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_PyraLite_Gen(const AX_PYRA_FRAME_T *tSrcFrame, AX_PYRA_FRAME_T *tDstFrame, AX_BOOL bMaskFlag) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_PyraLite_Gen);
    int32_t ret;

    if (!m_req.pack(api, tSrcFrame, tDstFrame /* I|O */, bMaskFlag)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, tDstFrame)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_PyraLite_Rcn(const AX_PYRA_FRAME_T *tSrcFrame, AX_PYRA_FRAME_T *tDstFrame, AX_BOOL bBottom) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_PyraLite_Rcn);
    int32_t ret;

    if (!m_req.pack(api, tSrcFrame, tDstFrame /* I|O */, bBottom)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, tDstFrame)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_GdcWorkCreate(GDC_HANDLE *pGdcHandle) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_GdcWorkCreate);
    int32_t ret;

    if (!m_req.pack(api)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pGdcHandle)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_GdcWorkAttrSet(GDC_HANDLE nGdcHandle, const AX_IVPS_GDC_ATTR_T *ptGdcAttr) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_GdcWorkAttrSet);
    int32_t ret;

    axcl::pkg::uint8_array user_map;
    if (AX_IVPS_GDC_MAP_USER == ptGdcAttr->eGdcType) {
        user_map.size = ptGdcAttr->tMapUserAttr.nMeshNumV * ptGdcAttr->tMapUserAttr.nMeshNumH * 16;
        user_map.data = reinterpret_cast<AX_U8 *>(ptGdcAttr->tMapUserAttr.pUserMap);
    }

    if (!m_req.pack(api, nGdcHandle, ptGdcAttr, user_map)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_GdcWorkRun(GDC_HANDLE nGdcHandle, const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_GdcWorkRun);
    int32_t ret;

    if (!m_req.pack(api, ptSrc, ptDst /* I|O */)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, ptDst)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_GdcWorkDestroy(GDC_HANDLE nGdcHandle) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_GdcWorkDestroy);
    int32_t ret;

    if (!m_req.pack(api, nGdcHandle)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_FisheyePointQueryDst2Src(AX_IVPS_POINT_NICE_T *ptSrcPoint, const AX_IVPS_POINT_NICE_T *ptDstPoint, AX_U16 nInputW,
                                              AX_U16 nInputH, AX_U8 nRgnIdx, const AX_IVPS_FISHEYE_ATTR_T *ptFisheyeAttr) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_FisheyePointQueryDst2Src);
    int32_t ret;

    if (!m_req.pack(api, ptDstPoint, nInputW, nInputH, nRgnIdx, ptFisheyeAttr)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, ptSrcPoint)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AX_IVPS_FisheyePointQuerySrc2Dst(AX_IVPS_POINT_NICE_T *ptDstPoint, const AX_IVPS_POINT_NICE_T *ptSrcPoint, AX_U16 nInputW,
                                              AX_U16 nInputH, AX_U8 nRgnIdx, const AX_IVPS_FISHEYE_ATTR_T *ptFisheyeAttr) {
    axcl::pkg::native::IVPS_API api(axcl::pkg::native::IVPS_API::AX_IVPS_FisheyePointQuerySrc2Dst);
    int32_t ret;

    if (!m_req.pack(api, ptSrcPoint, nInputW, nInputH, nRgnIdx, ptFisheyeAttr)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, ptDstPoint)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::CmmCopy(axcl::pkg::native::IVPS_API api, AX_U64 nSrcPhyAddr, AX_U64 nDstPhyAddr, AX_U64 nMemSize) {
    int32_t ret;

    if (!m_req.pack(api, nSrcPhyAddr, nDstPhyAddr, nMemSize)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::FlipAndRotation(axcl::pkg::native::IVPS_API api, const AX_VIDEO_FRAME_T *ptSrc, AX_IVPS_CHN_FLIP_MODE_E eFlipMode,
                             AX_IVPS_ROTATION_E eRotation, AX_VIDEO_FRAME_T *ptDst) {
    int32_t ret;

    if (!m_req.pack(api, ptSrc, eFlipMode, eRotation, ptDst /* I|O */)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, ptDst)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::Csc(axcl::pkg::native::IVPS_API api, const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst) {
    int32_t ret;

    if (!m_req.pack(api, ptSrc, ptDst /*I|O*/)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, ptDst)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::CropResize(axcl::pkg::native::IVPS_API api, const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst,
                        const AX_IVPS_ASPECT_RATIO_T *ptAspectRatio) {
    int32_t ret;

    if (!m_req.pack(api, ptSrc, ptDst /*I|O*/, ptAspectRatio)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, ptDst)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::CropResizeV2(axcl::pkg::native::IVPS_API api, const AX_VIDEO_FRAME_T *ptSrc, const AX_IVPS_RECT_T tBox[], AX_U32 nCropNum,
                          AX_VIDEO_FRAME_T *ptDst[], const AX_IVPS_ASPECT_RATIO_T *ptAspectRatio) {
    int32_t ret;

    /* boxes is already continuos memory */
    axcl::pkg::uint8_array boxes;
    boxes.size = sizeof(AX_IVPS_RECT_T) * nCropNum;
    boxes.data = const_cast<AX_U8 *>(reinterpret_cast<const AX_U8 *>(&tBox[0]));

    /* reallocate continuous memory */
    axcl::pkg::uint8_array dsts_input;
    dsts_input.size = sizeof(AX_VIDEO_FRAME_T) * nCropNum;
    /* let smart pointer to manage memory */
    std::unique_ptr<AX_U8[]> sp = std::make_unique<AX_U8[]>(dsts_input.size);
    if (!sp) {
        LOG_MM_E(TAG, "malloc dst image memory fail");
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_NO_MEMORY);
    } else {
        dsts_input.data = sp.get();

        AX_VIDEO_FRAME_T *img = reinterpret_cast<AX_VIDEO_FRAME_T *>(dsts_input.data);
        for (AX_U32 i = 0; i < nCropNum; ++i) {
            *img++ = *ptDst[i];
        }
    }

    if (!m_req.pack(api, ptSrc, boxes, nCropNum, dsts_input, ptAspectRatio)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::CropResizeV3(axcl::pkg::native::IVPS_API api, const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst[], AX_U32 nNum,
                          const AX_IVPS_ASPECT_RATIO_T *ptAspectRatio) {
    int32_t ret;

    /* reallocate continuous memory */
    axcl::pkg::uint8_array dsts_input;
    dsts_input.size = sizeof(AX_VIDEO_FRAME_T) * nNum;
    /* let smart pointer to manage memory */
    std::unique_ptr<AX_U8[]> sp = std::make_unique<AX_U8[]>(dsts_input.size);
    if (!sp) {
        LOG_MM_E(TAG, "malloc dst image memory fail");
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_NO_MEMORY);
    } else {
        dsts_input.data = sp.get();

        AX_VIDEO_FRAME_T *img = reinterpret_cast<AX_VIDEO_FRAME_T *>(dsts_input.data);
        for (AX_U32 i = 0; i < nNum; ++i) {
            *img++ = *ptDst[i];
        }
    }

    if (!m_req.pack(api, ptSrc, dsts_input, nNum, ptAspectRatio)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::CropResizeV4(axcl::pkg::native::IVPS_API api, const AX_VIDEO_FRAME_T *ptSrc, AX_VIDEO_FRAME_T *ptDst,
                          const AX_IVPS_ASPECT_RATIO_T *ptAspectRatio, const AX_IVPS_SCALE_STEP_T *ptScaleStep) {
    int32_t ret;

    if (!m_req.pack(api, ptSrc, ptDst /* I|O */, ptAspectRatio, ptScaleStep)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, ptDst)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::SetScaleCoefLevel(axcl::pkg::native::IVPS_API api, const AX_IVPS_SCALE_RANGE_T *ScaleRange,
                               const AX_IVPS_SCALE_COEF_LEVEL_T *CoefLevel) {
    int32_t ret;

    if (!m_req.pack(api, ScaleRange, CoefLevel)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::GetScaleCoefLevel(axcl::pkg::native::IVPS_API api, const AX_IVPS_SCALE_RANGE_T *ScaleRange,
                               AX_IVPS_SCALE_COEF_LEVEL_T *CoefLevel) {
    int32_t ret;

    if (!m_req.pack(api, ScaleRange)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, CoefLevel)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AlphaBlending(axcl::pkg::native::IVPS_API api, const AX_VIDEO_FRAME_T *ptSrc, const AX_VIDEO_FRAME_T *ptOverlay,
                           const AX_IVPS_POINT_T tOffset, AX_U8 nAlpha, AX_VIDEO_FRAME_T *ptDst) {
    int32_t ret;

    if (!m_req.pack(api, ptSrc, ptOverlay, tOffset, nAlpha, ptDst /*I|O*/)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, ptDst)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AlphaBlendingV2(axcl::pkg::native::IVPS_API api, const AX_VIDEO_FRAME_T *ptSrc, const AX_VIDEO_FRAME_T *ptOverlay,
                             const AX_IVPS_POINT_T tOffset, const AX_IVPS_ALPHA_LUT_T *ptSpAlpha, AX_VIDEO_FRAME_T *ptDst) {
    int32_t ret;

    if (!m_req.pack(api, ptSrc, ptOverlay, tOffset, ptSpAlpha, ptDst /*I|O*/)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, ptDst)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::AlphaBlendingV3(axcl::pkg::native::IVPS_API api, const AX_VIDEO_FRAME_T *ptSrc, const AX_OVERLAY_T *ptOverlay,
                             AX_VIDEO_FRAME_T *ptDst) {
    int32_t ret;

    if (!m_req.pack(api, ptSrc, ptOverlay, ptDst /*I|O*/)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, ptDst)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::DrawOsd(axcl::pkg::native::IVPS_API api, const AX_VIDEO_FRAME_T *ptFrame, const AX_OSD_BMP_ATTR_T arrBmp[], AX_U32 nNum) {
    int32_t ret;

    axcl::pkg::uint8_array bmps;
    bmps.size = sizeof(AX_OSD_BMP_ATTR_T) * nNum;
    bmps.data = const_cast<AX_U8 *>(reinterpret_cast<const AX_U8 *>(&arrBmp[0]));
    if (!m_req.pack(api, ptFrame, bmps, nNum)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ivps::DrawMosaic(axcl::pkg::native::IVPS_API api, const AX_VIDEO_FRAME_T *ptSrc, AX_IVPS_RGN_MOSAIC_T tMosaic[], AX_U32 nNum) {
    int32_t ret;

    axcl::pkg::uint8_array mosaics;
    mosaics.size = sizeof(AX_IVPS_RGN_MOSAIC_T) * nNum;
    mosaics.data = reinterpret_cast<AX_U8 *>(&tMosaic[0]);
    if (!m_req.pack(api, ptSrc, mosaics, nNum)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVPS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}