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

#include "package/device/native/module/ivps_request.hpp"
#include "package/device/native/module/ivps_response.hpp"
#include "ax_ivps_api.h"
#include "base.hpp"
#include "native_api.hpp"

#define NATIVE_IVPS_API_DECL(_API_) \
            NATIVE_API_DECL(_API_, NATIVE_API_PARAM(ivps))

#define NATIVE_IVPS_API_IMPL(_API_) \
            NATIVE_API_IMPL(ivps, _API_, NATIVE_API_PARAM(ivps))

#define NATIVE_IVPS_API_APPEND(_API_) \
            NATIVE_API_APPEND(IVPS, _API_, NATIVE_API_PARAM(ivps))

namespace axcl::worker {

class dispatcher;
class ivps : public base {
public:
    ivps(dispatcher *dispatcher);
    ~ivps() = default;

    virtual void run(const void *data, const size_t& size) override;

private:
    NATIVE_IVPS_API_DECL(AX_IVPS_Init);
    NATIVE_IVPS_API_DECL(AX_IVPS_Deinit);
    NATIVE_IVPS_API_DECL(AX_IVPS_CreateGrp);
    NATIVE_IVPS_API_DECL(AX_IVPS_CreateGrpEx);
    NATIVE_IVPS_API_DECL(AX_IVPS_DestoryGrp);
    NATIVE_IVPS_API_DECL(AX_IVPS_SetPipelineAttr);
    NATIVE_IVPS_API_DECL(AX_IVPS_GetPipelineAttr);
    NATIVE_IVPS_API_DECL(AX_IVPS_StartGrp);
    NATIVE_IVPS_API_DECL(AX_IVPS_StopGrp);
    NATIVE_IVPS_API_DECL(AX_IVPS_EnableChn);
    NATIVE_IVPS_API_DECL(AX_IVPS_DisableChn);
    NATIVE_IVPS_API_DECL(AX_IVPS_SendFrame);
    NATIVE_IVPS_API_DECL(AX_IVPS_GetChnFrame);
    NATIVE_IVPS_API_DECL(AX_IVPS_ReleaseChnFrame);
    NATIVE_IVPS_API_DECL(AX_IVPS_GetGrpFrame);
    NATIVE_IVPS_API_DECL(AX_IVPS_ReleaseGrpFrame);
    NATIVE_IVPS_API_DECL(AX_IVPS_GetChnFd);
    NATIVE_IVPS_API_DECL(AX_IVPS_GetDebugFifoFrame);
    NATIVE_IVPS_API_DECL(AX_IVPS_ReleaseDebugFifoFrame);
    NATIVE_IVPS_API_DECL(AX_IVPS_CloseAllFd);
    NATIVE_IVPS_API_DECL(AX_IVPS_SetGrpLDCAttr);
    NATIVE_IVPS_API_DECL(AX_IVPS_GetGrpLDCAttr);
    NATIVE_IVPS_API_DECL(AX_IVPS_SetChnLDCAttr);
    NATIVE_IVPS_API_DECL(AX_IVPS_GetChnLDCAttr);
    NATIVE_IVPS_API_DECL(AX_IVPS_SetGrpPoolAttr);
    NATIVE_IVPS_API_DECL(AX_IVPS_SetChnPoolAttr);
    NATIVE_IVPS_API_DECL(AX_IVPS_SetGrpUserFRC);
    NATIVE_IVPS_API_DECL(AX_IVPS_SetChnUserFRC);
    NATIVE_IVPS_API_DECL(AX_IVPS_SetGrpCrop);
    NATIVE_IVPS_API_DECL(AX_IVPS_GetGrpCrop);
    NATIVE_IVPS_API_DECL(AX_IVPS_SetChnAttr);
    NATIVE_IVPS_API_DECL(AX_IVPS_GetChnAttr);
    NATIVE_IVPS_API_DECL(AX_IVPS_EnableBackupFrame);
    NATIVE_IVPS_API_DECL(AX_IVPS_DisableBackupFrame);
    NATIVE_IVPS_API_DECL(AX_IVPS_ResetGrp);
    NATIVE_IVPS_API_DECL(AX_IVPS_GetEngineDutyCycle);
    NATIVE_IVPS_API_DECL(AX_IVPS_RGN_Create);
    NATIVE_IVPS_API_DECL(AX_IVPS_RGN_Destroy);
    NATIVE_IVPS_API_DECL(AX_IVPS_RGN_AttachToFilter);
    NATIVE_IVPS_API_DECL(AX_IVPS_RGN_DetachFromFilter);
    NATIVE_IVPS_API_DECL(AX_IVPS_RGN_Update);
    NATIVE_IVPS_API_DECL(AX_IVPS_CmmCopyTdp);
    NATIVE_IVPS_API_DECL(AX_IVPS_FlipAndRotationTdp);
    NATIVE_IVPS_API_DECL(AX_IVPS_CscTdp);
    NATIVE_IVPS_API_DECL(AX_IVPS_CropResizeTdp);
    NATIVE_IVPS_API_DECL(AX_IVPS_CropResizeV2Tdp);
    NATIVE_IVPS_API_DECL(AX_IVPS_AlphaBlendingTdp);
    NATIVE_IVPS_API_DECL(AX_IVPS_AlphaBlendingV3Tdp);
    NATIVE_IVPS_API_DECL(AX_IVPS_DrawOsdTdp);
    NATIVE_IVPS_API_DECL(AX_IVPS_DrawMosaicTdp);
    NATIVE_IVPS_API_DECL(AX_IVPS_CmmCopyVpp);
    NATIVE_IVPS_API_DECL(AX_IVPS_CropResizeVpp);
    NATIVE_IVPS_API_DECL(AX_IVPS_CropResizeV2Vpp);
    NATIVE_IVPS_API_DECL(AX_IVPS_CropResizeV3Vpp);
    NATIVE_IVPS_API_DECL(AX_IVPS_CscVpp);
    NATIVE_IVPS_API_DECL(AX_IVPS_DrawMosaicVpp);
    NATIVE_IVPS_API_DECL(AX_IVPS_SetScaleCoefLevelVpp);
    NATIVE_IVPS_API_DECL(AX_IVPS_GetScaleCoefLevelVpp);
    NATIVE_IVPS_API_DECL(AX_IVPS_CmmCopyVgp);
    NATIVE_IVPS_API_DECL(AX_IVPS_CscVgp);
    NATIVE_IVPS_API_DECL(AX_IVPS_CropResizeVgp);
    NATIVE_IVPS_API_DECL(AX_IVPS_CropResizeV2Vgp);
    NATIVE_IVPS_API_DECL(AX_IVPS_CropResizeV4Vgp);
    NATIVE_IVPS_API_DECL(AX_IVPS_AlphaBlendingVgp);
    NATIVE_IVPS_API_DECL(AX_IVPS_AlphaBlendingV2Vgp);
    NATIVE_IVPS_API_DECL(AX_IVPS_AlphaBlendingV3Vgp);
    NATIVE_IVPS_API_DECL(AX_IVPS_DrawOsdVgp);
    NATIVE_IVPS_API_DECL(AX_IVPS_DrawMosaicVgp);
    NATIVE_IVPS_API_DECL(AX_IVPS_SetScaleCoefLevelVgp);
    NATIVE_IVPS_API_DECL(AX_IVPS_GetScaleCoefLevelVgp);
    NATIVE_IVPS_API_DECL(AX_IVPS_DrawLine);
    NATIVE_IVPS_API_DECL(AX_IVPS_DrawPolygon);
    NATIVE_IVPS_API_DECL(AX_IVPS_DrawRect);
    NATIVE_IVPS_API_DECL(AX_IVPS_Dewarp);
    NATIVE_IVPS_API_DECL(AX_PyraLite_Gen);
    NATIVE_IVPS_API_DECL(AX_PyraLite_Rcn);
    NATIVE_IVPS_API_DECL(AX_IVPS_GdcWorkCreate);
    NATIVE_IVPS_API_DECL(AX_IVPS_GdcWorkAttrSet);
    NATIVE_IVPS_API_DECL(AX_IVPS_GdcWorkRun);
    NATIVE_IVPS_API_DECL(AX_IVPS_GdcWorkDestroy);
    NATIVE_IVPS_API_DECL(AX_IVPS_FisheyePointQueryDst2Src);
    NATIVE_IVPS_API_DECL(AX_IVPS_FisheyePointQuerySrc2Dst);

private:
    native_api<axcl::pkg::native::IVPS_API, axcl::pkg::device::ivps_response, axcl::pkg::device::ivps_request> m_api;
};

}  // namespace axcl::worker
