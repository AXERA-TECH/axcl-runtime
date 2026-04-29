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

#define MAX_VER_LENGTH 256

const AX_CHAR* npu::AX_ENGINE_GetVersion() {
    constexpr auto api(axcl::pkg::native::ENGINE_API::AX_ENGINE_GetVersion);

    // FIXME: multi-thread behave
    thread_local AX_CHAR ver_str[MAX_VER_LENGTH] = {};

    if (0 == ver_str[0]) {
        if (!m_req.pack(api)) {
            LOG_MM_E(TAG, REQ_PAK_FAIL);
            return nullptr;
        }
        LOG_MM_D(TAG, REQ_PAK_GOOD);

        if (AXCL_SUCC != invoke(NO_TIMEOUT)) {
            LOG_MM_E(TAG, REQ_IVK_FAIL);
            return nullptr;
        }
        LOG_MM_D(TAG, REQ_IVK_GOOD);

        axcl::pkg::uint8_array version{nullptr, 0};
        if (!m_rsp.unpack(api, &version)) {
            LOG_MM_E(TAG, RSP_UNP_FAIL);
            return nullptr;
        }
        LOG_MM_D(TAG, RSP_UNP_GOOD);


        const auto ver_size = std::min(version.size, static_cast<uint32_t>((MAX_VER_LENGTH - 1)));
        memcpy(ver_str, version.data, ver_size);
        ver_str[ver_size] = 0;
    }

    return ver_str;
}

AX_VOID npu::AX_ENGINE_NPUReset() {
    constexpr auto api(axcl::pkg::native::ENGINE_API::AX_ENGINE_NPUReset);

    if (!m_req.pack(api)) {
        LOG_MM_E(TAG, REQ_PAK_FAIL);
        return;
    }
    LOG_MM_D(TAG, REQ_PAK_GOOD);

    if (AXCL_SUCC != invoke(NO_TIMEOUT)) {
        LOG_MM_E(TAG, REQ_IVK_FAIL);
        return;
    }
    LOG_MM_D(TAG, REQ_IVK_GOOD);

    if (!m_rsp.unpack(api)) {
        LOG_MM_E(TAG, RSP_UNP_FAIL);
        return;
    }
    LOG_MM_D(TAG, RSP_UNP_GOOD);
}

AX_S32 npu::AX_ENGINE_Init(AX_ENGINE_NPU_ATTR_T* pNpuAttr) {
    constexpr auto api(axcl::pkg::native::ENGINE_API::AX_ENGINE_Init);

    if (!m_req.pack(api, pNpuAttr)) {
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

AX_S32 npu::AX_ENGINE_GetVNPUAttr(AX_ENGINE_NPU_ATTR_T* pNpuAttr) {
    constexpr auto api(axcl::pkg::native::ENGINE_API::AX_ENGINE_GetVNPUAttr);

    if (!m_req.pack(api)) {
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
    if (!m_rsp.unpack(api, &ret, pNpuAttr)) {
        LOG_MM_E(TAG, RSP_UNP_FAIL);
        return AXCL_DEF_ENGINE_ERR(AXCL_ERR_DECODE);
    }
    LOG_MM_D(TAG, RSP_UNP_CODE, ret);

    return ret;
}

AX_S32 npu::AX_ENGINE_Deinit() {
    constexpr auto api(axcl::pkg::native::ENGINE_API::AX_ENGINE_Deinit);

    if (!m_req.pack(api)) {
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
