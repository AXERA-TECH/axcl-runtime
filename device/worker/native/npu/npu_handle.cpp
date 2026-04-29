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
#define LEADING_NAME "pid"

namespace axcl::worker {

NATIVE_ENGINE_API_IMPL(AX_ENGINE_CreateHandle) {
    LOG_MM_D(TAG, "Start...");

    AX_U64 addr;
    AX_U32 size;

    if (!response->unpack(response->get_api(), &addr, &size)) {
        LOG_MM_E(TAG, "Failed to unpack request message.");
        return false;
    }
    LOG_MM_D(TAG, "Request message unpacked.");

    AX_S32 ret = 0;
    AX_ENGINE_HANDLE handle{};

    auto map_guard = res_guard<void*>(
        ::AX_SYS_Mmap(addr, size),
        [&size](void* ptr) {
            if (nullptr != ptr) {::AX_SYS_Munmap(ptr, size);}
        }
    );

    if (nullptr != map_guard.get()) {
        LOG_MM_D(TAG, "Mmap phy({:#x}) to vir({:#x}).", addr, reinterpret_cast<AX_U64>(map_guard.get()));

        const auto pid = response->get_env().pid;
        const auto model_name = LEADING_NAME + std::to_string(pid);

        AX_ENGINE_HANDLE_EXTRA_T extra{};
        extra.pName = (AX_S8*)(model_name.c_str());
        ret = ::AX_ENGINE_CreateHandleV2(&handle, map_guard.get(), size, &extra);
    } else {
        LOG_MM_E(TAG, "Failed mmap phy({:#x}).", addr);
        ret = AX_ERR_CMM_MMAP_FAIL;
    }

    if (!request->pack(response->get_api(), ret, reinterpret_cast<AX_U64>(handle))) {
        LOG_MM_E(TAG, "Failed to pack response(ret: {}).", ret);
        return false;
    }
    LOG_MM_D(TAG, "Response message send(ret: {}).", ret);

    return true;
}

NATIVE_ENGINE_API_IMPL(AX_ENGINE_CreateHandleV2) {
    LOG_MM_D(TAG, "Start...");

    AX_U64 addr;
    AX_U32 size;

    // 1. unpack request message, get phy address & size & extra
    AX_ENGINE_HANDLE_EXTRA_T extra{};
    pkg::uint8_array name{};
    if (!response->unpack(response->get_api(), &addr, &size, &extra.nNpuSet, &name)) {
        LOG_MM_E(TAG, "Failed to unpack request message.");
        return false;
    }
    LOG_MM_D(TAG, "Request message unpacked.");

    // 2. copy name to extra, empty name is under consideration
    AX_S8 name_str[name.size + 1] = {};
    ::memcpy(name_str, name.data, name.size);
    const auto pid = response->get_env().pid;
    const auto model_name = LEADING_NAME + std::to_string(pid) + "_" + std::string(reinterpret_cast<char *>(name_str));

    extra.pName = (AX_S8*)model_name.c_str();

    // 3. mmap phy address to vir address
    AX_S32 ret = 0;

    auto map_guard = res_guard<void*>(
        ::AX_SYS_Mmap(addr, size),
        [&size](void* ptr) {
            if (nullptr != ptr) {::AX_SYS_Munmap(ptr, size);}
        }
    );

    // 4. create handle, if mmap success
    AX_ENGINE_HANDLE handle{};
    if (nullptr != map_guard.get()) {
        LOG_MM_D(TAG, "Mmap phy({:#x}) to vir({:#x}).", addr, reinterpret_cast<AX_U64>(map_guard.get()));
        ret = ::AX_ENGINE_CreateHandleV2(&handle, map_guard.get(), size, &extra);
    } else {
        LOG_MM_E(TAG, "Failed mmap phy({:#x}).", addr);
        ret = AX_ERR_CMM_MMAP_FAIL;
    }

    // 5. pack response message
    if (!request->pack(response->get_api(), ret, reinterpret_cast<AX_U64>(handle))) {
        LOG_MM_E(TAG, "Failed to pack response(ret: {}).", ret);
        return false;
    }
    LOG_MM_D(TAG, "Response message send(ret: {}).", ret);

    return true;
}

NATIVE_ENGINE_API_IMPL(AX_ENGINE_DestroyHandle) {
    LOG_MM_D(TAG, "Start...");

    AX_U64 handle{};

    if (!response->unpack(response->get_api(), &handle)) {
        LOG_MM_E(TAG, "Failed to unpack request message.");
        return false;
    }
    LOG_MM_D(TAG, "Request message unpacked.");

    const AX_S32 ret = ::AX_ENGINE_DestroyHandle(reinterpret_cast<void*>(handle));
    if (!request->pack(response->get_api(), ret)) {
        LOG_MM_E(TAG, "Failed to pack response(ret: {}).", ret);
        return false;
    }
    LOG_MM_D(TAG, "Response message send(ret: {}).", ret);

    return true;
}

}  // namespace axcl::worker
