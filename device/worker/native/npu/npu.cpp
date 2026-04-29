/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "npu.hpp"
#include "dispatcher.hpp"

#define TAG "engine"

namespace axcl::worker {

npu::npu(dispatcher *dispatcher) : base(dispatcher), m_api(TAG) {
    LOG_MM_D(TAG, "axcl npu constructed...");
    m_api.append({
        NATIVE_ENGINE_API_APPEND(AX_ENGINE_GetVersion),
        NATIVE_ENGINE_API_APPEND(AX_ENGINE_NPUReset),
        NATIVE_ENGINE_API_APPEND(AX_ENGINE_Init),
        NATIVE_ENGINE_API_APPEND(AX_ENGINE_GetVNPUAttr),
        NATIVE_ENGINE_API_APPEND(AX_ENGINE_Deinit),
        NATIVE_ENGINE_API_APPEND(AX_ENGINE_GetModelType),
        NATIVE_ENGINE_API_APPEND(AX_ENGINE_CreateHandle),
        NATIVE_ENGINE_API_APPEND(AX_ENGINE_CreateHandleV2),
        NATIVE_ENGINE_API_APPEND(AX_ENGINE_DestroyHandle),
        NATIVE_ENGINE_API_APPEND(AX_ENGINE_GetIOInfo),
        NATIVE_ENGINE_API_APPEND(AX_ENGINE_GetGroupIOInfoCount),
        NATIVE_ENGINE_API_APPEND(AX_ENGINE_GetGroupIOInfo),
        NATIVE_ENGINE_API_APPEND(AX_ENGINE_GetHandleModelType),
        NATIVE_ENGINE_API_APPEND(AX_ENGINE_CreateContext),
        NATIVE_ENGINE_API_APPEND(AX_ENGINE_CreateContextV2),
        NATIVE_ENGINE_API_APPEND(AX_ENGINE_RunSync),
        NATIVE_ENGINE_API_APPEND(AX_ENGINE_RunSyncV2),
        NATIVE_ENGINE_API_APPEND(AX_ENGINE_RunGroupIOSync),
        NATIVE_ENGINE_API_APPEND(AX_ENGINE_SetAffinity),
        NATIVE_ENGINE_API_APPEND(AX_ENGINE_GetAffinity),
        NATIVE_ENGINE_API_APPEND(AX_ENGINE_GetCMMUsage),
        NATIVE_ENGINE_API_APPEND(AX_ENGINE_GetModelToolsVersion),
    });
}

void npu::run(const void *data, const size_t& size) {
    LOG_MM_D(TAG, "axcl npu run...");
    m_api.run(get_dispatcher(), data, size);
}

}  // namespace axcl::worker
