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
#include "res_guard.hpp"

#include <ax_sys_api.h>
#include <ax_engine_api.h>

#define TAG "engine"

namespace axcl::worker {

NATIVE_ENGINE_API_IMPL(AX_ENGINE_GetModelType) {
    LOG_MM_D(TAG, "Start...");

    AX_U64 addr;
    AX_U32 size;

    if (!response->unpack(response->get_api(), &addr, &size)) {
        LOG_MM_E(TAG, "Failed to unpack request message.");
        return false;
    }
    LOG_MM_D(TAG, "Request message unpacked.");

    AX_S32 ret = 0;
    AX_ENGINE_MODEL_TYPE_T model_type{};

    auto map_guard = res_guard<void*>(
        ::AX_SYS_Mmap(addr, size),
        [&size](void* ptr) {
            if (nullptr != ptr) {::AX_SYS_Munmap(ptr, size);}
        }
    );

    if (nullptr != map_guard.get()) {
        LOG_MM_D(TAG, "Mmap phy({:#x}, size: {}) to vir({:#x}).", addr, size, reinterpret_cast<AX_U64>(map_guard.get()));
        ret = ::AX_ENGINE_GetModelType(map_guard.get(), size, &model_type);
    } else {
        LOG_MM_E(TAG, "Failed mmap phy: {:#x}, size: {}.", addr, size);
        ret = AX_ERR_CMM_MMAP_FAIL;
    }

    if (!request->pack(response->get_api(), ret, model_type)) {
        LOG_MM_E(TAG, "Failed to pack response(ret: {}).", ret);
        return false;
    }
    LOG_MM_D(TAG, "Response message send(ret: {}).", ret);

    return true;
}

NATIVE_ENGINE_API_IMPL(AX_ENGINE_GetHandleModelType) {
    LOG_MM_D(TAG, "Start...");

    AX_U64 handle{};

    if (!response->unpack(response->get_api(), &handle)) {
        LOG_MM_E(TAG, "Failed to unpack request message.");
        return false;
    }
    LOG_MM_D(TAG, "Request message unpacked.");

    AX_ENGINE_MODEL_TYPE_T type{};
    const AX_S32 ret = ::AX_ENGINE_GetHandleModelType(reinterpret_cast<void*>(handle), &type);
    if (!request->pack(response->get_api(), ret, type)) {
        LOG_MM_E(TAG, "Failed to pack failed response(ret: {}).", ret);
        return false;
    }
    LOG_MM_D(TAG, "Response message send(ret: {}).", ret);

    return true;
}

NATIVE_ENGINE_API_IMPL(AX_ENGINE_GetCMMUsage) {
    LOG_MM_D(TAG, "Start...");

    AX_U64 handle{};

    if (!response->unpack(response->get_api(), &handle)) {
        LOG_MM_E(TAG, "Failed to unpack request message.");
        return false;
    }
    LOG_MM_D(TAG, "Request message unpacked.");

    AX_ENGINE_CMM_INFO info{};
    const AX_S32 ret = ::AX_ENGINE_GetCMMUsage(reinterpret_cast<void*>(handle), &info);
    if (!request->pack(response->get_api(), ret, &info)) {
        LOG_MM_E(TAG, "Failed to pack response(ret: {}).", ret);
        return false;
    }
    LOG_MM_D(TAG, "Response message send(ret: {}).", ret);
    LOG_MM_D(TAG, "CMM usage: {}.", info.nCMMSize);

    return true;
}

NATIVE_ENGINE_API_IMPL(AX_ENGINE_GetModelToolsVersion) {
    LOG_MM_D(TAG, "Start...");

    AX_U64 handle{};

    if (!response->unpack(response->get_api(), &handle)) {
        LOG_MM_E(TAG, "Failed to unpack request message.");
        return false;
    }
    LOG_MM_D(TAG, "Request message unpacked.");

    const auto* ver_str = reinterpret_cast<const uint8_t*>(::AX_ENGINE_GetModelToolsVersion(reinterpret_cast<void*>(handle)));
    const auto ver_str_size = ::strlen(reinterpret_cast<const char*>(ver_str));

    if (const auto ver = pkg::uint8_array{const_cast<uint8_t*>(ver_str), static_cast<uint32_t>(ver_str_size)};
        !request->pack(response->get_api(), ver)) {
        LOG_MM_E(TAG, "Failed to pack response.");
        return false;
    }
    LOG_MM_D(TAG, "Response message send.");

    return true;
}

}  // namespace axcl::worker
