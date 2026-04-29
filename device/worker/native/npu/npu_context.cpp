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

NATIVE_ENGINE_API_IMPL(AX_ENGINE_CreateContext) {
    LOG_MM_D(TAG, "Start...");

    AX_U64 handle{};

    if (!response->unpack(response->get_api(), &handle)) {
        LOG_MM_E(TAG, "Failed to unpack request message.");
        return false;
    }
    LOG_MM_D(TAG, "Request message unpacked.");

    const AX_S32 ret = ::AX_ENGINE_CreateContext(reinterpret_cast<void*>(handle));
    if (!request->pack(response->get_api(), ret)) {
        LOG_MM_E(TAG, "Failed to pack failed response(ret: {}).", ret);
        return false;
    }
    LOG_MM_D(TAG, "Response message send(ret: {}).", ret);

    return true;
}

NATIVE_ENGINE_API_IMPL(AX_ENGINE_CreateContextV2) {
    LOG_MM_D(TAG, "Start...");

    AX_U64 handle{};

    if (!response->unpack(response->get_api(), &handle)) {
        LOG_MM_E(TAG, "Failed to unpack request message.");
        return false;
    }
    LOG_MM_D(TAG, "Request message unpacked.");

    AX_ENGINE_CONTEXT_T context{};
    const AX_S32 ret = ::AX_ENGINE_CreateContextV2(reinterpret_cast<void*>(handle), &context);
    if (!request->pack(response->get_api(), ret, reinterpret_cast<AX_U64>(context))) {
        return false;
    }
    LOG_MM_D(TAG, "Response message send(ret: {}).", ret);

    return true;
}

}  // namespace axcl::worker
