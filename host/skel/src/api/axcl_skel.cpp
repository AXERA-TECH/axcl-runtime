/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "axcl_skel.h"
#include "axcl_module_version.h"
#include "err.hpp"
#include "log.hpp"
#include "skelMgr.hpp"

#define SKEL_API extern "C" __attribute__((visibility("default")))

///
SKEL_API AX_S32 AXCL_SKEL_Init(const AXCL_SKEL_INIT_PARAM_T *pstParam) {
#if defined(SKEL_BUILD_VERSION)
    ALOGE("AXCL SKEL SDK {} BUILD: {} {}", SKEL_BUILD_VERSION, __DATE__, __TIME__);
#endif

    return CSKELMgr::GetInstance()->Init(pstParam);
}

SKEL_API AX_S32 AXCL_SKEL_DeInit(AX_VOID) {
    return CSKELMgr::GetInstance()->DeInit();
}

SKEL_API AX_S32 AXCL_SKEL_Create(const AXCL_SKEL_HANDLE_PARAM_T *pstParam, AXCL_SKEL_HANDLE *pHandle) {
    return CSKELMgr::GetInstance()->Create(pstParam, pHandle);
}

SKEL_API AX_S32 AXCL_SKEL_Destroy(AXCL_SKEL_HANDLE handle) {
    return CSKELMgr::GetInstance()->Destroy(handle);
}

SKEL_API AX_S32 AXCL_SKEL_RegisterResultCallback(AXCL_SKEL_HANDLE handle, AXCL_SKEL_RESULT_CALLBACK_FUNC callback, AX_VOID *pUserData) {
    return CSKELMgr::GetInstance()->RegisterResultCallback(handle, callback, pUserData);
}

SKEL_API AX_S32 AXCL_SKEL_SendFrame(AXCL_SKEL_HANDLE handle, const AXCL_SKEL_FRAME_T *pstFrame, AX_S32 nTimeout) {
    return CSKELMgr::GetInstance()->SendFrame(handle, pstFrame, nTimeout);
}

SKEL_API AX_S32 AXCL_SKEL_GetResult(AXCL_SKEL_HANDLE handle, AXCL_SKEL_RESULT_T **ppstResult, AX_S32 nTimeout) {
    return CSKELMgr::GetInstance()->GetResult(handle, ppstResult, nTimeout);
}

SKEL_API AX_S32 AXCL_SKEL_Release(AX_VOID *p) {
    return CSKELMgr::GetInstance()->Release(p);
}

SKEL_API AX_S32 AXCL_SKEL_GetCapability(const AXCL_SKEL_CAPABILITY_T **ppstCapability) {
    return CSKELMgr::GetInstance()->GetCapability(ppstCapability);
}

SKEL_API AX_S32 AXCL_SKEL_GetVersion(const AXCL_SKEL_VERSION_INFO_T **ppstVersion) {
    return CSKELMgr::GetInstance()->GetVersion(ppstVersion);
}

SKEL_API AX_S32 AXCL_SKEL_GetConfig(AXCL_SKEL_HANDLE handle, const AXCL_SKEL_CONFIG_T **ppstConfig) {
    return CSKELMgr::GetInstance()->GetConfig(handle, ppstConfig);
}

SKEL_API AX_S32 AXCL_SKEL_SetConfig(AXCL_SKEL_HANDLE handle, const AXCL_SKEL_CONFIG_T *pstConfig) {
    return CSKELMgr::GetInstance()->SetConfig(handle, pstConfig);
}
