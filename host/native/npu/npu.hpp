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

#include "axcl_npu_type.h"
#include "native.hpp"

#include "package/host/native/module/npu_request.hpp"
#include "package/host/native/module/npu_response.hpp"

class npu final: public native<axcl::pkg::host::npu_request, axcl::pkg::host::npu_response> {
public:
    npu() : native(axcl::pkg::native::module::ENGINE) {}
    ~npu() = default;

    const AX_CHAR* AX_ENGINE_GetVersion();

    AX_VOID AX_ENGINE_NPUReset();

    AX_S32 AX_ENGINE_Init(AX_ENGINE_NPU_ATTR_T* pNpuAttr);
    AX_S32 AX_ENGINE_GetVNPUAttr(AX_ENGINE_NPU_ATTR_T* pNpuAttr);
    AX_S32 AX_ENGINE_Deinit();

    AX_S32 AX_ENGINE_GetModelType(const AX_VOID* pData, AX_U32 nDataSize, AX_ENGINE_MODEL_TYPE_T* pModelType);

    AX_S32 AX_ENGINE_CreateHandle(AX_ENGINE_HANDLE* pHandle, const AX_VOID* pData, AX_U32 nDataSize);
    AX_S32 AX_ENGINE_CreateHandleV2(AX_ENGINE_HANDLE* pHandle, const AX_VOID* pData, AX_U32 nDataSize, const AX_ENGINE_HANDLE_EXTRA_T* pExtraParam);
    AX_S32 AX_ENGINE_DestroyHandle(AX_ENGINE_HANDLE nHandle);

    AX_S32 AX_ENGINE_GetIOInfo(AX_ENGINE_HANDLE nHandle, AX_ENGINE_IO_INFO_T** pIO);
    AX_S32 AX_ENGINE_GetGroupIOInfoCount(AX_ENGINE_HANDLE nHandle, AX_U32* pCount);
    AX_S32 AX_ENGINE_GetGroupIOInfo(AX_ENGINE_HANDLE nHandle, AX_U32 nIndex, AX_ENGINE_IO_INFO_T** pIO);

    AX_S32 AX_ENGINE_GetHandleModelType(AX_ENGINE_HANDLE nHandle, AX_ENGINE_MODEL_TYPE_T* pModelType);

    AX_S32 AX_ENGINE_CreateContext(AX_ENGINE_HANDLE handle);
    AX_S32 AX_ENGINE_CreateContextV2(AX_ENGINE_HANDLE nHandle, AX_ENGINE_CONTEXT_T* pContext);

    AX_S32 AX_ENGINE_RunSync(AX_ENGINE_HANDLE handle, const AX_ENGINE_IO_T* pIO);
    AX_S32 AX_ENGINE_RunSyncV2(AX_ENGINE_HANDLE handle, AX_ENGINE_CONTEXT_T context, const AX_ENGINE_IO_T* pIO);
    AX_S32 AX_ENGINE_RunGroupIOSync(AX_ENGINE_HANDLE handle, AX_ENGINE_CONTEXT_T context, AX_U32 nIndex, const AX_ENGINE_IO_T* pIO);

    AX_S32 AX_ENGINE_SetAffinity(AX_ENGINE_HANDLE nHandle, AX_ENGINE_NPU_SET_T nNpuSet);
    AX_S32 AX_ENGINE_GetAffinity(AX_ENGINE_HANDLE nHandle, AX_ENGINE_NPU_SET_T* pNpuSet);

    AX_S32 AX_ENGINE_GetCMMUsage(AX_ENGINE_HANDLE nHandle, AX_ENGINE_CMM_INFO* pCMMInfo);

    const AX_CHAR* AX_ENGINE_GetModelToolsVersion(AX_ENGINE_HANDLE nHandle);

private:
    using array = axcl::pkg::uint8_array;

    static void dump_io_info(const AX_ENGINE_IO_INFO_T* info);
    static void dump_io(const AX_ENGINE_IO_T* pIO);

    static void decode_meta(AX_ENGINE_IOMETA_T* meta_array, const array& meta_package, const AX_U32& package_count);
    static uint8_t* alloc_info_buffer(const array& input_package, const AX_U32& input_size, const array& output_package, const AX_U32& output_size);
};
