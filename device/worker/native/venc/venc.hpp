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

#include "package/device/native/module/venc_request.hpp"
#include "package/device/native/module/venc_response.hpp"
#include "ax_venc_api.h"
#include "base.hpp"
#include "native_api.hpp"

#define NATIVE_VENC_API_DECL(_API_) \
            NATIVE_API_DECL(_API_, NATIVE_API_PARAM(venc))

#define NATIVE_VENC_API_IMPL(_API_) \
            NATIVE_API_IMPL(venc, _API_, NATIVE_API_PARAM(venc))

#define NATIVE_VENC_API_APPEND(_API_) \
            NATIVE_API_APPEND(VENC, _API_, NATIVE_API_PARAM(venc))

namespace axcl::worker {

class dispatcher;
class venc : public base {
public:
    venc(dispatcher *dispatcher);
    ~venc() = default;

    virtual void run(const void *data, const size_t& size) override;

private:
    NATIVE_VENC_API_DECL(AX_VENC_Init);
    NATIVE_VENC_API_DECL(AX_VENC_Deinit);
    NATIVE_VENC_API_DECL(AX_VENC_CreateChn);
    NATIVE_VENC_API_DECL(AX_VENC_CreateChnEx);
    NATIVE_VENC_API_DECL(AX_VENC_DestroyChn);
    NATIVE_VENC_API_DECL(AX_VENC_SendFrame);
    NATIVE_VENC_API_DECL(AX_VENC_SendFrameEx);
    NATIVE_VENC_API_DECL(AX_VENC_SelectGrp);
    NATIVE_VENC_API_DECL(AX_VENC_SelectClearGrp);
    NATIVE_VENC_API_DECL(AX_VENC_SelectGrpAddChn);
    NATIVE_VENC_API_DECL(AX_VENC_SelectGrpDeleteChn);
    NATIVE_VENC_API_DECL(AX_VENC_SelectGrpQuery);
    NATIVE_VENC_API_DECL(AX_VENC_GetFd);
    NATIVE_VENC_API_DECL(AX_VENC_GetStream);
    NATIVE_VENC_API_DECL(AX_VENC_ReleaseStream);
    NATIVE_VENC_API_DECL(AX_VENC_GetStreamBufInfo);
    NATIVE_VENC_API_DECL(AX_VENC_StartRecvFrame);
    NATIVE_VENC_API_DECL(AX_VENC_StopRecvFrame);
    NATIVE_VENC_API_DECL(AX_VENC_ResetChn);
    NATIVE_VENC_API_DECL(AX_VENC_SetRoiAttr);
    NATIVE_VENC_API_DECL(AX_VENC_GetRoiAttr);
    NATIVE_VENC_API_DECL(AX_VENC_SetRcParam);
    NATIVE_VENC_API_DECL(AX_VENC_GetRcParam);
    NATIVE_VENC_API_DECL(AX_VENC_SetModParam);
    NATIVE_VENC_API_DECL(AX_VENC_GetModParam);
    NATIVE_VENC_API_DECL(AX_VENC_SetVuiParam);
    NATIVE_VENC_API_DECL(AX_VENC_GetVuiParam);
    NATIVE_VENC_API_DECL(AX_VENC_SetChnAttr);
    NATIVE_VENC_API_DECL(AX_VENC_GetChnAttr);
    NATIVE_VENC_API_DECL(AX_VENC_SetRateJamStrategy);
    NATIVE_VENC_API_DECL(AX_VENC_GetRateJamStrategy);
    NATIVE_VENC_API_DECL(AX_VENC_SetSuperFrameStrategy);
    NATIVE_VENC_API_DECL(AX_VENC_GetSuperFrameStrategy);
    NATIVE_VENC_API_DECL(AX_VENC_SetIntraRefresh);
    NATIVE_VENC_API_DECL(AX_VENC_GetIntraRefresh);
    NATIVE_VENC_API_DECL(AX_VENC_SetUsrData);
    NATIVE_VENC_API_DECL(AX_VENC_GetUsrData);
    NATIVE_VENC_API_DECL(AX_VENC_SetSliceSplit);
    NATIVE_VENC_API_DECL(AX_VENC_GetSliceSplit);
    NATIVE_VENC_API_DECL(AX_VENC_RequestIDR);
    NATIVE_VENC_API_DECL(AX_VENC_QueryStatus);
    NATIVE_VENC_API_DECL(AX_VENC_SetJpegParam);
    NATIVE_VENC_API_DECL(AX_VENC_GetJpegParam);
    NATIVE_VENC_API_DECL(AX_VENC_JpegEncodeOneFrame);
    NATIVE_VENC_API_DECL(AX_VENC_JpegGetThumbnail);

private:
    native_api<axcl::pkg::native::VENC_API, axcl::pkg::device::venc_response, axcl::pkg::device::venc_request> m_api;
};

}  // namespace axcl::worker
