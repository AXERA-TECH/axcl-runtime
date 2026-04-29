/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "dsp.hpp"
#include <fstream>
#include <memory>
#include "axcl_logger.hpp"
#include "res_guard.hpp"

#define TAG "dsp"

AX_S32 dsp::AX_DSP_PowerOn(const AX_DSP_ID_E enDspId) {
    constexpr auto api(axcl::pkg::native::DSP_API::AX_DSP_PowerOn);
    AX_S32 ret;

    if (!m_req.pack(api, enDspId)) {
        return AXCL_DEF_DSP_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_DSP_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 dsp::AX_DSP_PowerOff(const AX_DSP_ID_E enDspId) {
    constexpr auto api(axcl::pkg::native::DSP_API::AX_DSP_PowerOff);
    AX_S32 ret;

    if (!m_req.pack(api, enDspId)) {
        return AXCL_DEF_DSP_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_DSP_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 dsp::AX_DSP_LoadBin(const AX_DSP_ID_E enDspId, const char *pszBinFileName, const AX_DSP_MEM_TYPE_E enMemType) {
    constexpr auto api(axcl::pkg::native::DSP_API::AX_DSP_PowerOff);

    std::ifstream ifs(pszBinFileName, std::ios::in | std::ios::binary | std::ios::ate);
    if (!ifs.is_open()) {
        LOG_MM_E(TAG, "load {} failed", pszBinFileName);
        return AXCL_DEF_DSP_ERR(AX_DSP_OPEN_FAIL);
    }

    const auto file_size = ifs.tellg();
    if (!file_size) {
        ifs.close();
        LOG_MM_E(TAG, "{} is empty", pszBinFileName);
        return AXCL_DEF_DSP_ERR(AX_DSP_OPEN_FAIL);
    }

    auto bin = std::make_unique<char[]>(file_size);
    if (!bin) {
        ifs.close();
        LOG_MM_E(TAG, "malloc {} failed", static_cast<int32_t>(file_size));
        return AXCL_DEF_DSP_ERR(AXCL_ERR_NO_MEMORY);
    }

    ifs.seekg(0, std::ios::beg);
    ifs.read(bin.get(), file_size);
    ifs.close();

    auto mem_holder = res_guard<void*>(
        [&file_size]() -> void* {
            void *mem = nullptr;
            axclrtMalloc(&mem, file_size, axclrtMemMallocPolicy{});
            return mem;
        },
        [](void*& dev) { if (dev != nullptr) axclrtFree(dev); }
    );

    if (nullptr == mem_holder.get()) {
        LOG_MM_E(TAG, "malloc device memory for dsp bin failed");
        return AXCL_DEF_DSP_ERR(AXCL_ERR_NO_MEMORY);
    }

    AX_S32 ret = axclrtMemcpy(mem_holder.get(), bin.get(), file_size, AXCL_MEMCPY_HOST_TO_DEVICE);
    if (0 != ret) {
        LOG_MM_E(TAG, "memcpy dsp bin from host to device failed");
        return ret;
    }

    if (!m_req.pack(api, enDspId, reinterpret_cast<uint64_t>(mem_holder.get()), file_size, enMemType)) {
        return AXCL_DEF_DSP_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_DSP_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 dsp::AX_DSP_EnableCore(const AX_DSP_ID_E enDspId) {
    constexpr auto api(axcl::pkg::native::DSP_API::AX_DSP_EnableCore);
    AX_S32 ret;

    if (!m_req.pack(api, enDspId)) {
        return AXCL_DEF_DSP_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_DSP_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 dsp::AX_DSP_DisableCore(const AX_DSP_ID_E enDspId) {
    constexpr auto api(axcl::pkg::native::DSP_API::AX_DSP_DisableCore);
    AX_S32 ret;

    if (!m_req.pack(api, enDspId)) {
        return AXCL_DEF_DSP_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_DSP_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 dsp::AX_DSP_PRC(AX_DSP_HANDLE *phHandle, const AX_DSP_MESSAGE_T *pstMsg, const AX_DSP_ID_E enDspId, const AX_DSP_PRI_E enPri) {
    constexpr auto api(axcl::pkg::native::DSP_API::AX_DSP_PRC);
    AX_S32 ret;

    if (!m_req.pack(api, *pstMsg, enDspId, enPri)) {
        return AXCL_DEF_DSP_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, phHandle)) {
        return AXCL_DEF_DSP_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 dsp::AX_DSP_Query(const AX_DSP_ID_E enDspId, const AX_DSP_HANDLE hHandle, AX_DSP_MESSAGE_T *msg, const AX_BOOL bBlock) {
    constexpr auto api(axcl::pkg::native::DSP_API::AX_DSP_Query);
    AX_S32 ret;

    if (!m_req.pack(api, enDspId, hHandle, bBlock)) {
        return AXCL_DEF_DSP_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, msg)) {
        return AXCL_DEF_DSP_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}
