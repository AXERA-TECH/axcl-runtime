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

#include "axcl_dsp.h"
#include "native.hpp"
#include "package/host/native/module/dsp_request.hpp"
#include "package/host/native/module/dsp_response.hpp"

#define AXCL_DEF_DSP_ERR(e) AXCL_DEF_NATIVE_ERR(static_cast<uint8_t>(axcl::pkg::native::module::DSP), (e))

class dsp final : public native<axcl::pkg::host::dsp_request, axcl::pkg::host::dsp_response> {
public:
    dsp() : native(axcl::pkg::native::module::DSP) {}
    ~dsp() = default;

    AX_S32 AX_DSP_PowerOn(AX_DSP_ID_E enDspId);
    AX_S32 AX_DSP_PowerOff(AX_DSP_ID_E enDspId);
    AX_S32 AX_DSP_LoadBin(AX_DSP_ID_E enDspId, const char *pszBinFileName, AX_DSP_MEM_TYPE_E enMemType);
    AX_S32 AX_DSP_EnableCore(AX_DSP_ID_E enDspId);
    AX_S32 AX_DSP_DisableCore(AX_DSP_ID_E enDspId);
    AX_S32 AX_DSP_PRC(AX_DSP_HANDLE *phHandle, const AX_DSP_MESSAGE_T *pstMsg, AX_DSP_ID_E enDspId, AX_DSP_PRI_E enPri);
    AX_S32 AX_DSP_Query(AX_DSP_ID_E enDspId, AX_DSP_HANDLE hHandle, AX_DSP_MESSAGE_T *msg, AX_BOOL bBlock);
};
