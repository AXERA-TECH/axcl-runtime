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

#include "package/device/native/module/vdec_request.hpp"
#include "package/device/native/module/vdec_response.hpp"
#include "ax_vdec_api.h"
#include "base.hpp"
#include "native_api.hpp"

#define NATIVE_VDEC_API_DECL(_API_) \
            NATIVE_API_DECL(_API_, NATIVE_API_PARAM(vdec))

#define NATIVE_VDEC_API_IMPL(_API_) \
            NATIVE_API_IMPL(vdec, _API_, NATIVE_API_PARAM(vdec))

#define NATIVE_VDEC_API_APPEND(_API_) \
            NATIVE_API_APPEND(VDEC, _API_, NATIVE_API_PARAM(vdec))

namespace axcl::worker {

class dispatcher;
class vdec : public base {
public:
    vdec(dispatcher *dispatcher);
    ~vdec() = default;

    virtual void run(const void *data, const size_t& size) override;

private:
    NATIVE_VDEC_API_DECL(AX_VDEC_Init);
    NATIVE_VDEC_API_DECL(AX_VDEC_Deinit);
    NATIVE_VDEC_API_DECL(AX_VDEC_ExtractStreamHeaderInfo);
    NATIVE_VDEC_API_DECL(AX_VDEC_CreateGrp);
    NATIVE_VDEC_API_DECL(AX_VDEC_CreateGrpEx);
    NATIVE_VDEC_API_DECL(AX_VDEC_DestroyGrp);
    NATIVE_VDEC_API_DECL(AX_VDEC_GetGrpAttr);
    NATIVE_VDEC_API_DECL(AX_VDEC_SetGrpAttr);
    NATIVE_VDEC_API_DECL(AX_VDEC_StartRecvStream);
    NATIVE_VDEC_API_DECL(AX_VDEC_StopRecvStream);
    NATIVE_VDEC_API_DECL(AX_VDEC_QueryStatus);
    NATIVE_VDEC_API_DECL(AX_VDEC_ResetGrp);
    NATIVE_VDEC_API_DECL(AX_VDEC_SetGrpParam);
    NATIVE_VDEC_API_DECL(AX_VDEC_GetGrpParam);
    NATIVE_VDEC_API_DECL(AX_VDEC_SelectGrp);
    NATIVE_VDEC_API_DECL(AX_VDEC_SendStream);
    NATIVE_VDEC_API_DECL(AX_VDEC_GetChnFrame);
    NATIVE_VDEC_API_DECL(AX_VDEC_ReleaseChnFrame);
    NATIVE_VDEC_API_DECL(AX_VDEC_GetUserData);
    NATIVE_VDEC_API_DECL(AX_VDEC_ReleaseUserData);
    NATIVE_VDEC_API_DECL(AX_VDEC_SetUserPic);
    NATIVE_VDEC_API_DECL(AX_VDEC_EnableUserPic);
    NATIVE_VDEC_API_DECL(AX_VDEC_DisableUserPic);
    NATIVE_VDEC_API_DECL(AX_VDEC_SetDisplayMode);
    NATIVE_VDEC_API_DECL(AX_VDEC_GetDisplayMode);
    NATIVE_VDEC_API_DECL(AX_VDEC_AttachPool);
    NATIVE_VDEC_API_DECL(AX_VDEC_DetachPool);
    NATIVE_VDEC_API_DECL(AX_VDEC_EnableChn);
    NATIVE_VDEC_API_DECL(AX_VDEC_DisableChn);
    NATIVE_VDEC_API_DECL(AX_VDEC_SetChnAttr);
    NATIVE_VDEC_API_DECL(AX_VDEC_GetChnAttr);
    NATIVE_VDEC_API_DECL(AX_VDEC_JpegDecodeOneFrame);
    NATIVE_VDEC_API_DECL(AX_VDEC_GetStreamBufInfo);
    NATIVE_VDEC_API_DECL(AX_VDEC_GetVuiParam);

private:
    native_api<axcl::pkg::native::VDEC_API, axcl::pkg::device::vdec_response, axcl::pkg::device::vdec_request> m_api;
};

}  // namespace axcl::worker
