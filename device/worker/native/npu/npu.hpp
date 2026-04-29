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

#include "package/device/native/module/npu_request.hpp"
#include "package/device/native/module/npu_response.hpp"

#include "base.hpp"
#include "native_api.hpp"

#define NATIVE_ENGINE_API_DECL(_API_) \
            NATIVE_API_DECL(_API_, NATIVE_API_PARAM(npu))

#define NATIVE_ENGINE_API_IMPL(_API_) \
            NATIVE_API_IMPL(npu, _API_, NATIVE_API_PARAM(npu))

#define NATIVE_ENGINE_API_APPEND(_API_) \
            NATIVE_API_APPEND(ENGINE, _API_, NATIVE_API_PARAM(npu))

namespace axcl::worker {

class dispatcher;
class npu final: public base {
public:
    explicit npu(dispatcher *dispatcher);
    ~npu() override = default;

    void run(const void *data, const size_t& size) override;

private:
    using array = pkg::uint8_array;

    NATIVE_ENGINE_API_DECL(AX_ENGINE_GetVersion);
    NATIVE_ENGINE_API_DECL(AX_ENGINE_NPUReset);
    NATIVE_ENGINE_API_DECL(AX_ENGINE_Init);
    NATIVE_ENGINE_API_DECL(AX_ENGINE_GetVNPUAttr);
    NATIVE_ENGINE_API_DECL(AX_ENGINE_Deinit);
    NATIVE_ENGINE_API_DECL(AX_ENGINE_GetModelType);
    NATIVE_ENGINE_API_DECL(AX_ENGINE_CreateHandle);
    NATIVE_ENGINE_API_DECL(AX_ENGINE_CreateHandleV2);
    NATIVE_ENGINE_API_DECL(AX_ENGINE_DestroyHandle);
    NATIVE_ENGINE_API_DECL(AX_ENGINE_GetIOInfo);
    NATIVE_ENGINE_API_DECL(AX_ENGINE_GetGroupIOInfoCount);
    NATIVE_ENGINE_API_DECL(AX_ENGINE_GetGroupIOInfo);
    NATIVE_ENGINE_API_DECL(AX_ENGINE_GetHandleModelType);
    NATIVE_ENGINE_API_DECL(AX_ENGINE_CreateContext);
    NATIVE_ENGINE_API_DECL(AX_ENGINE_CreateContextV2);
    NATIVE_ENGINE_API_DECL(AX_ENGINE_RunSync);
    NATIVE_ENGINE_API_DECL(AX_ENGINE_RunSyncV2);
    NATIVE_ENGINE_API_DECL(AX_ENGINE_RunGroupIOSync);
    NATIVE_ENGINE_API_DECL(AX_ENGINE_SetAffinity);
    NATIVE_ENGINE_API_DECL(AX_ENGINE_GetAffinity);
    NATIVE_ENGINE_API_DECL(AX_ENGINE_GetCMMUsage);
    NATIVE_ENGINE_API_DECL(AX_ENGINE_GetModelToolsVersion);

    native_api<pkg::native::ENGINE_API, pkg::device::npu_response, pkg::device::npu_request> m_api;
};

}  // namespace axcl::worker
