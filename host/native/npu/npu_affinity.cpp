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
#include "npu_string.hpp"

AX_S32 npu::AX_ENGINE_SetAffinity(AX_ENGINE_HANDLE nHandle, const AX_ENGINE_NPU_SET_T nNpuSet) {
    constexpr auto api(axcl::pkg::native::ENGINE_API::AX_ENGINE_SetAffinity);

    if (!m_req.pack(api, reinterpret_cast<AX_U64>(nHandle), nNpuSet)) {
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

    return ret;
}

AX_S32 npu::AX_ENGINE_GetAffinity(AX_ENGINE_HANDLE nHandle, AX_ENGINE_NPU_SET_T* pNpuSet) {
    constexpr auto api(axcl::pkg::native::ENGINE_API::AX_ENGINE_GetAffinity);

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
    if (!m_rsp.unpack(api, &ret, pNpuSet)) {
        LOG_MM_E(TAG, RSP_UNP_FAIL);
        return AXCL_DEF_ENGINE_ERR(AXCL_ERR_DECODE);
    }
    LOG_MM_D(TAG, RSP_UNP_CODE, ret);

    return ret;
}
