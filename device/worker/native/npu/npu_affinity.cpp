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
#include "axcl_logger.hpp"

#include <ax_engine_api.h>

#define TAG "engine"

namespace axcl::worker {

NATIVE_ENGINE_API_IMPL(AX_ENGINE_SetAffinity) {
    LOG_MM_D(TAG, "Start...");

    AX_U64 handle{};
    AX_ENGINE_NPU_SET_T set{};

    if (!response->unpack(response->get_api(), &handle, &set)) {
        LOG_MM_E(TAG, "Failed to unpack request message.");
        return false;
    }
    LOG_MM_D(TAG, "Request message unpacked.");

    const AX_S32 ret = ::AX_ENGINE_SetAffinity(reinterpret_cast<void*>(handle), set);
    if (!request->pack(response->get_api(), ret)) {
        LOG_MM_E(TAG, "Failed to pack response(ret: {}).", ret);
        return false;
    }
    LOG_MM_D(TAG, "Response message send(ret: {}).", ret);

    return true;
}

NATIVE_ENGINE_API_IMPL(AX_ENGINE_GetAffinity) {
    LOG_MM_D(TAG, "Start...");

    AX_U64 handle{};

    if (!response->unpack(response->get_api(), &handle)) {
        LOG_MM_E(TAG, "Failed to unpack request message.");
        return false;
    }
    LOG_MM_D(TAG, "Request message unpacked.");

    AX_ENGINE_NPU_SET_T set{};
    const AX_S32 ret = ::AX_ENGINE_GetAffinity(reinterpret_cast<void*>(handle), &set);

    if (!request->pack(response->get_api(), ret, set)) {
        LOG_MM_E(TAG, "Failed to pack response(ret: {}).", ret);
        return false;
    }
    LOG_MM_D(TAG, "Response message send(ret: {}).", ret);

    return true;
}

}  // namespace axcl::worker
