/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "axcl_base.h"

#include "axcl_module_version.h"

#include "axcl_dsp.h"
#include "axcl_logger.hpp"
#include "dsp.hpp"

#define TAG "dsp"
#define CHECK_NULL_POINTER(p)                           \
    if (!(p)) {                                         \
        LOG_MM_E(TAG, "nil pointer");                   \
        return AXCL_DEF_DSP_ERR(AXCL_ERR_NULL_POINTER); \
    }

AXCL_EXPORT AX_S32 AXCL_DSP_PowerOn(const AX_DSP_ID_E enDspId) {
    dsp api;
    return api.AX_DSP_PowerOn(enDspId);
}

AXCL_EXPORT AX_S32 AXCL_DSP_PowerOff(const AX_DSP_ID_E enDspId) {
    dsp api;
    return api.AX_DSP_PowerOff(enDspId);
}

AXCL_EXPORT AX_S32 AXCL_DSP_LoadBin(const AX_DSP_ID_E enDspId, const char *pszBinFileName, const AX_DSP_MEM_TYPE_E enMemType) {
    CHECK_NULL_POINTER(pszBinFileName)

    dsp api;
    return api.AX_DSP_LoadBin(enDspId, pszBinFileName, enMemType);
}

AXCL_EXPORT AX_S32 AXCL_DSP_EnableCore(const AX_DSP_ID_E enDspId) {
    dsp api;
    return api.AX_DSP_EnableCore(enDspId);
}

AXCL_EXPORT AX_S32 AXCL_DSP_DisableCore(const AX_DSP_ID_E enDspId) {
    dsp api;
    return api.AX_DSP_DisableCore(enDspId);
}

AXCL_EXPORT AX_S32 AXCL_DSP_PRC(AX_DSP_HANDLE *phHandle, const AX_DSP_MESSAGE_T *pstMsg, const AX_DSP_ID_E enDspId, const AX_DSP_PRI_E enPri) {
    CHECK_NULL_POINTER(pstMsg)

    dsp api;
    return api.AX_DSP_PRC(phHandle, pstMsg, enDspId, enPri);
}

AXCL_EXPORT AX_S32 AXCL_DSP_Query(const AX_DSP_ID_E enDspId, const AX_DSP_HANDLE hHandle, AX_DSP_MESSAGE_T *msg, const AX_BOOL bBlock) {
    CHECK_NULL_POINTER(msg)

    dsp api;
    return api.AX_DSP_Query(enDspId, hHandle, msg, bBlock);
}
