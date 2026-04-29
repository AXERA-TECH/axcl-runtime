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

#include "npu_local.hpp"
#include "npu_string.hpp"

#include <cstring>
#include <iomanip>
#include <list>
#include <map>

#define TAG "engine"

AX_S32 npu::AX_ENGINE_GetIOInfo(AX_ENGINE_HANDLE nHandle, AX_ENGINE_IO_INFO_T** pIO) {
    constexpr auto api(axcl::pkg::native::ENGINE_API::AX_ENGINE_GetIOInfo);

    const auto handle = reinterpret_cast<AX_U64>(nHandle);
    if (!m_req.pack(api, handle)) {
        LOG_MM_E(TAG, REQ_PAK_FAIL);
        return AXCL_DEF_ENGINE_ERR(AXCL_ERR_ENCODE);
    }
    LOG_MM_D(TAG, REQ_PAK_GOOD);

    if (const int32_t ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        LOG_MM_E(TAG, REQ_IVK_FAIL);
        return ret;
    }
    LOG_MM_D(TAG, REQ_IVK_GOOD);

    int32_t ret;
    array inputs{}, outputs{};
    AX_U32 input_size, output_size, max_batch, dynamic_batch;

    if (!m_rsp.unpack(api, &ret, &inputs, &input_size, &outputs, &output_size, &max_batch, &dynamic_batch)) {
        LOG_MM_E(TAG, RSP_UNP_FAIL);
        return AXCL_DEF_ENGINE_ERR(AXCL_ERR_DECODE);
    }
    LOG_MM_D(TAG, "Response message unpacked(input {}, output {}, ret: {}).", input_size, output_size, ret);

    if (AXCL_SUCC == ret) {
        std::lock_guard lock(g_npu_map_mutex);

        // check if handle exists
        if (0 == g_npu_map->map.count(handle)) {
            LOG_MM_D(TAG, "No handle: {:#x} in map.", handle);
            g_npu_map->map.emplace(handle, std::map<AX_U32, AX_ENGINE_IO_INFO_T*>());
            LOG_MM_D(TAG, "Handle: {:#x} info was inserted.", handle);
        }

        auto& pair = g_npu_map->map.at(handle);
        LOG_MM_D(TAG, "Get handle: {:#x} sub map, count: {}.", handle, pair.size());

        if (0 == pair.count(0)) {
            LOG_MM_D(TAG, "Handle: {:#x} does not have a sub map with index: {}.", handle, 0);
            const auto info = alloc_info_buffer(inputs, input_size, outputs, output_size);
            *pIO = reinterpret_cast<AX_ENGINE_IO_INFO_T*>(info);
            pair.emplace(0, *pIO);
        }
        else {
            LOG_MM_D(TAG, "Handle: {:#x} have a sub map with index: {}.", handle, 0);
            *pIO = pair.at(0);
        }
    }

    dump_io_info(*pIO);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    return ret;
}

AX_S32 npu::AX_ENGINE_GetGroupIOInfoCount(AX_ENGINE_HANDLE nHandle, AX_U32* pCount) {
    constexpr auto api(axcl::pkg::native::ENGINE_API::AX_ENGINE_GetGroupIOInfoCount);

    if (!m_req.pack(api, reinterpret_cast<AX_U64>(nHandle))) {
        return AXCL_DEF_ENGINE_ERR(AXCL_ERR_ENCODE);
    }

    if (const int32_t ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    int32_t ret;
    if (!m_rsp.unpack(api, &ret, pCount)) {
        return AXCL_DEF_ENGINE_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 npu::AX_ENGINE_GetGroupIOInfo(AX_ENGINE_HANDLE nHandle, AX_U32 nIndex, AX_ENGINE_IO_INFO_T** pIO) {
    constexpr auto api(axcl::pkg::native::ENGINE_API::AX_ENGINE_GetGroupIOInfo);

    const auto handle = reinterpret_cast<AX_U64>(nHandle);
    if (!m_req.pack(api, handle, nIndex)) {
        return AXCL_DEF_ENGINE_ERR(AXCL_ERR_ENCODE);
    }

    if (const int32_t ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        LOG_MM_E(TAG, REQ_IVK_FAIL);
        return ret;
    }
    LOG_MM_D(TAG, REQ_IVK_GOOD);

    int32_t ret;
    array inputs{}, outputs{};
    AX_U32 input_size, output_size, max_batch, dynamic_batch;

    if (!m_rsp.unpack(api, &ret, &inputs, &input_size, &outputs, &output_size,
        &max_batch, &dynamic_batch)) {
        return AXCL_DEF_ENGINE_ERR(AXCL_ERR_DECODE);
        }

    if (AXCL_SUCC == ret) {
        std::lock_guard lock(g_npu_map_mutex);

        // check if handle exists
        if (0 == g_npu_map->map.count(handle)) {
            LOG_MM_D(TAG, "No handle: {:#x} in map.", handle);
            g_npu_map->map.emplace(handle, std::map<AX_U32, AX_ENGINE_IO_INFO_T*>());
        }

        auto& pair = g_npu_map->map.at(handle);
        LOG_MM_D(TAG, "Get handle: {:#x} sub map, count: {}.", handle, pair.size());

        if (0 == pair.count(nIndex)) {
            LOG_MM_D(TAG, "Handle: {:#x} does not have a sub map with index: {}.", handle, nIndex);
            const auto info = alloc_info_buffer(inputs, input_size, outputs, output_size);
            *pIO = reinterpret_cast<AX_ENGINE_IO_INFO_T*>(info);
            pair.emplace(nIndex, *pIO);
            LOG_MM_D(TAG, "Handle: {:#x} info was inserted.", handle);
        }
        else {
            LOG_MM_D(TAG, "Handle: {:#x} have a sub map with index: {}.", handle, nIndex);
            *pIO = pair.at(nIndex);
        }
    }

    dump_io_info(*pIO);
    return ret;
}
