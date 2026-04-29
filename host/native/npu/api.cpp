/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "axcl_npu.h"

#include "axcl_module_version.h"

#include "npu.hpp"
#include "axcl_logger.hpp"

#define TAG "engine"

AXCL_EXPORT const AX_CHAR* AXCL_ENGINE_GetVersion(AX_VOID) {
    LOG_MM_D(TAG, "{}", axcl_module_version);

    npu api;
    return api.AX_ENGINE_GetVersion();
}

AXCL_EXPORT AX_VOID AXCL_ENGINE_NPUReset(AX_VOID) {
    npu api;
    api.AX_ENGINE_NPUReset();
}

AXCL_EXPORT AX_S32 AXCL_ENGINE_Init(AX_ENGINE_NPU_ATTR_T* pNpuAttr) {
    npu api;
    return api.AX_ENGINE_Init(pNpuAttr);
}

AXCL_EXPORT AX_S32 AXCL_ENGINE_GetVNPUAttr(AX_ENGINE_NPU_ATTR_T* pNpuAttr) {
    npu api;
    return api.AX_ENGINE_GetVNPUAttr(pNpuAttr);
}

AXCL_EXPORT AX_S32 AXCL_ENGINE_Deinit(AX_VOID) {
    npu api;
    return api.AX_ENGINE_Deinit();
}

AXCL_EXPORT AX_S32 AXCL_ENGINE_GetModelType(const AX_VOID* pData, AX_U32 nDataSize, AX_ENGINE_MODEL_TYPE_T* pModelType) {
    npu api;
    return api.AX_ENGINE_GetModelType(pData, nDataSize, pModelType);
}

AXCL_EXPORT AX_S32 AXCL_ENGINE_CreateHandle(AX_ENGINE_HANDLE* pHandle, const AX_VOID* pData, AX_U32 nDataSize) {
    npu api;
    return api.AX_ENGINE_CreateHandle(pHandle, pData, nDataSize);
}

AXCL_EXPORT AX_S32 AXCL_ENGINE_CreateHandleV2(AX_ENGINE_HANDLE* pHandle, const AX_VOID* pData, AX_U32 nDataSize,
                                              AX_ENGINE_HANDLE_EXTRA_T* pExtraParam) {
    npu api;
    return api.AX_ENGINE_CreateHandleV2(pHandle, pData, nDataSize, pExtraParam);
}

AXCL_EXPORT AX_S32 AXCL_ENGINE_DestroyHandle(AX_ENGINE_HANDLE nHandle) {
    npu api;
    return api.AX_ENGINE_DestroyHandle(nHandle);
}

AXCL_EXPORT AX_S32 AXCL_ENGINE_GetIOInfo(AX_ENGINE_HANDLE nHandle, AX_ENGINE_IO_INFO_T** pIO) {
    npu api;
    return api.AX_ENGINE_GetIOInfo(nHandle, pIO);
}

AXCL_EXPORT AX_S32 AXCL_ENGINE_GetGroupIOInfoCount(AX_ENGINE_HANDLE nHandle, AX_U32* pCount) {
    npu api;
    return api.AX_ENGINE_GetGroupIOInfoCount(nHandle, pCount);
}

AXCL_EXPORT AX_S32 AXCL_ENGINE_GetGroupIOInfo(AX_ENGINE_HANDLE nHandle, AX_U32 nIndex, AX_ENGINE_IO_INFO_T** pIO) {
    npu api;
    return api.AX_ENGINE_GetGroupIOInfo(nHandle, nIndex, pIO);
}

AXCL_EXPORT AX_S32 AXCL_ENGINE_GetHandleModelType(AX_ENGINE_HANDLE nHandle, AX_ENGINE_MODEL_TYPE_T* pModelType) {
    npu api;
    return api.AX_ENGINE_GetHandleModelType(nHandle, pModelType);
}

AXCL_EXPORT AX_S32 AXCL_ENGINE_CreateContext(AX_ENGINE_HANDLE handle) {
    npu api;
    return api.AX_ENGINE_CreateContext(handle);
}

AXCL_EXPORT AX_S32 AXCL_ENGINE_CreateContextV2(AX_ENGINE_HANDLE nHandle, AX_ENGINE_CONTEXT_T* pContext) {
    npu api;
    return api.AX_ENGINE_CreateContextV2(nHandle, pContext);
}

AXCL_EXPORT AX_S32 AXCL_ENGINE_RunSync(AX_ENGINE_HANDLE handle, AX_ENGINE_IO_T* pIO) {
    npu api;
    return api.AX_ENGINE_RunSync(handle, pIO);
}

AXCL_EXPORT AX_S32 AXCL_ENGINE_RunSyncV2(AX_ENGINE_HANDLE handle, AX_ENGINE_CONTEXT_T context, AX_ENGINE_IO_T* pIO) {
    npu api;
    return api.AX_ENGINE_RunSyncV2(handle, context, pIO);
}

AXCL_EXPORT AX_S32 AXCL_ENGINE_RunGroupIOSync(AX_ENGINE_HANDLE handle, AX_ENGINE_CONTEXT_T context, AX_U32 nIndex, AX_ENGINE_IO_T* pIO) {
    npu api;
    return api.AX_ENGINE_RunGroupIOSync(handle, context, nIndex, pIO);
}

AXCL_EXPORT AX_S32 AXCL_ENGINE_SetAffinity(AX_ENGINE_HANDLE nHandle, AX_ENGINE_NPU_SET_T nNpuSet) {
    npu api;
    return api.AX_ENGINE_SetAffinity(nHandle, nNpuSet);
}

AXCL_EXPORT AX_S32 AXCL_ENGINE_GetAffinity(AX_ENGINE_HANDLE nHandle, AX_ENGINE_NPU_SET_T* pNpuSet) {
    npu api;
    return api.AX_ENGINE_GetAffinity(nHandle, pNpuSet);
}

AXCL_EXPORT AX_S32 AXCL_ENGINE_GetCMMUsage(AX_ENGINE_HANDLE nHandle, AX_ENGINE_CMM_INFO* pCMMInfo) {
    npu api;
    return api.AX_ENGINE_GetCMMUsage(nHandle, pCMMInfo);
}

AXCL_EXPORT const AX_CHAR* AXCL_ENGINE_GetModelToolsVersion(AX_ENGINE_HANDLE nHandle) {
    npu api;
    return api.AX_ENGINE_GetModelToolsVersion(nHandle);
}
