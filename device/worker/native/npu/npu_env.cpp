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

NATIVE_ENGINE_API_IMPL(AX_ENGINE_GetVersion) {
    LOG_MM_D(TAG, "Start...");

    if (!response->unpack(response->get_api())) {
        LOG_MM_E(TAG, "Failed to unpack request message.");
        return false;
    }
    LOG_MM_D(TAG, "Request message unpacked.");

    const auto* ver_str = reinterpret_cast<const uint8_t*>(::AX_ENGINE_GetVersion());
    const auto ver_str_size = ::strlen(reinterpret_cast<const char*>(ver_str));

    if (const auto ver = pkg::uint8_array{const_cast<uint8_t*>(ver_str), static_cast<uint32_t>(ver_str_size)};
        !request->pack(response->get_api(), ver)) {
        LOG_MM_E(TAG, "Failed to pack response.");
        return false;
    }
    LOG_MM_D(TAG, "Response message send.");

    return true;
}

NATIVE_ENGINE_API_IMPL(AX_ENGINE_NPUReset) {
    LOG_MM_D(TAG, "Start...");

    if (!response->unpack(response->get_api())) {
        LOG_MM_E(TAG, "Failed to unpack request message.");
        return false;
    }
    LOG_MM_D(TAG, "Request message unpacked.");

    ::AX_ENGINE_NPUReset();

    if (!request->pack(response->get_api())) {
        LOG_MM_E(TAG, "Failed to pack response.");
        return false;
    }
    LOG_MM_D(TAG, "Response message send.");

    return true;
}

NATIVE_ENGINE_API_IMPL(AX_ENGINE_Init) {
    LOG_MM_D(TAG, "Start...");

    AX_ENGINE_NPU_ATTR_T attr{};

    if (!response->unpack(response->get_api(), &attr)) {
        LOG_MM_E(TAG, "Failed to unpack request message.");
        return false;
    }
    LOG_MM_D(TAG, "Request message unpacked.");

    const AX_S32 ret = ::AX_ENGINE_Init(&attr);
    if (!request->pack(response->get_api(), ret)) {
        LOG_MM_E(TAG, "Failed to pack response(ret: {}).", ret);
        return false;
    }
    LOG_MM_D(TAG, "Response message send(ret: {}).", ret);

    return true;
}

NATIVE_ENGINE_API_IMPL(AX_ENGINE_GetVNPUAttr) {
    LOG_MM_D(TAG, "Start...");

    if (!response->unpack(response->get_api())) {
        LOG_MM_E(TAG, "Failed to unpack request message.");
        return false;
    }
    LOG_MM_D(TAG, "Request message unpacked.");

    AX_ENGINE_NPU_ATTR_T attr{};
    const AX_S32 ret = ::AX_ENGINE_GetVNPUAttr(&attr);

    if (!request->pack(response->get_api(), ret, &attr)) {
        LOG_MM_E(TAG, "Failed to pack response(ret: {}).", ret);
        return false;
    }
    LOG_MM_D(TAG, "Response message send(ret: {}).", ret);

    return true;
}

NATIVE_ENGINE_API_IMPL(AX_ENGINE_Deinit) {
    LOG_MM_D(TAG, "Start...");

    if (!response->unpack(response->get_api())) {
        LOG_MM_E(TAG, "Failed to unpack request message.");
        return false;
    }
    LOG_MM_D(TAG, "Request message unpacked.");

    const AX_S32 ret = ::AX_ENGINE_Deinit();
    if (!request->pack(response->get_api(), ret)) {
        LOG_MM_E(TAG, "Failed to pack response(ret: {}).", ret);
        return false;
    }
    LOG_MM_D(TAG, "Response message send(ret: {}).", ret);

    return true;
}

}  // namespace axcl::worker
