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
#include "npu_io_holder.hpp"

AX_S32 npu::AX_ENGINE_GetModelType(const AX_VOID* pData, const AX_U32 nDataSize, AX_ENGINE_MODEL_TYPE_T* pModelType) {
    constexpr auto api(axcl::pkg::native::ENGINE_API::AX_ENGINE_GetModelType);

    if (!m_req.pack(api, reinterpret_cast<AX_U64>(pData), nDataSize)) {
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
    if (!m_rsp.unpack(api, &ret, pModelType)) {
        LOG_MM_E(TAG, RSP_UNP_FAIL);
        return AXCL_DEF_ENGINE_ERR(AXCL_ERR_DECODE);
    }
    LOG_MM_D(TAG, RSP_UNP_CODE, ret);

    return ret;
}

AX_S32 npu::AX_ENGINE_CreateHandle(AX_ENGINE_HANDLE* pHandle, const AX_VOID* pData, const AX_U32 nDataSize) {
    constexpr auto api(axcl::pkg::native::ENGINE_API::AX_ENGINE_CreateHandle);

    if (!m_req.pack(api, reinterpret_cast<AX_U64>(pData), nDataSize)) {
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
    if (!m_rsp.unpack(api, &ret, pHandle)) {
        LOG_MM_E(TAG, RSP_UNP_FAIL);
        return AXCL_DEF_ENGINE_ERR(AXCL_ERR_DECODE);
    }
    LOG_MM_D(TAG, RSP_UNP_CODE, ret);

    return ret;
}

AX_S32 npu::AX_ENGINE_CreateHandleV2(AX_ENGINE_HANDLE* pHandle, const AX_VOID* pData, const AX_U32 nDataSize, const AX_ENGINE_HANDLE_EXTRA_T* pExtraParam) {
    if (nullptr == pExtraParam) {
        return AX_ENGINE_CreateHandle(pHandle, pData, nDataSize);
    }

    constexpr auto api(axcl::pkg::native::ENGINE_API::AX_ENGINE_CreateHandleV2);

    axcl::pkg::uint8_array name{nullptr, 0};
    std::unique_ptr<AX_U8[]> name_str;

    if (nullptr != pExtraParam->pName) {
        const auto name_str_size = ::strlen(reinterpret_cast<const char*>(pExtraParam->pName));
        name_str = std::make_unique<AX_U8[]>(name_str_size);
        name = {name_str.get(), static_cast<uint32_t>(name_str_size)};
    }

    if (!m_req.pack(api, reinterpret_cast<AX_U64>(pData), nDataSize, pExtraParam->nNpuSet, name)) {
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
    if (!m_rsp.unpack(api, &ret, pHandle)) {
        LOG_MM_E(TAG, RSP_UNP_FAIL);
        return AXCL_DEF_ENGINE_ERR(AXCL_ERR_DECODE);
    }
    LOG_MM_D(TAG, RSP_UNP_CODE, ret);

    return ret;
}

AX_S32 npu::AX_ENGINE_DestroyHandle(AX_ENGINE_HANDLE nHandle) {
    constexpr auto api(axcl::pkg::native::ENGINE_API::AX_ENGINE_DestroyHandle);

    if (!m_req.pack(api, reinterpret_cast<AX_U64>(nHandle))) {
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
    if (!m_rsp.unpack(api, &ret)) {
        LOG_MM_E(TAG, RSP_UNP_FAIL);
        return AXCL_DEF_ENGINE_ERR(AXCL_ERR_DECODE);
    }
    LOG_MM_D(TAG, RSP_UNP_CODE, ret);

    {
        std::lock_guard lock(g_npu_map_mutex);
        if (const auto package = g_npu_map->map.extract(reinterpret_cast<AX_U64>(nHandle)); !package.empty()) {
            for (const auto& [index, info] : package.mapped()) {
                if (nullptr != info) {
                    free_one_info(info);
                }
            }
        }
        LOG_MM_D(TAG, "Handle: {:#x} info was extracted.", reinterpret_cast<AX_U64>(nHandle));
    }

    LOG_MM_D(TAG, "Handle: {:#x} destroyed.", reinterpret_cast<AX_U64>(nHandle));
    return ret;
}
