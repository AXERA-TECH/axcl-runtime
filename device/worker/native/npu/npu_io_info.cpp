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

#include "npu_gen_meta.hpp"
#include "axcl_logger.hpp"

#include <ax_engine_api.h>

#define TAG "engine"

namespace axcl::worker {

NATIVE_ENGINE_API_IMPL(AX_ENGINE_GetIOInfo) {
    LOG_MM_D(TAG, "Start...");

    AX_U64 handle{};
    if (!response->unpack(response->get_api(), &handle)) {
        LOG_MM_E(TAG, "Failed to unpack request message.");
        return false;
    }
    LOG_MM_D(TAG, "Request message unpacked.");

    AX_ENGINE_IO_INFO_T* info{};

    const AX_S32 ret = ::AX_ENGINE_GetIOInfo(reinterpret_cast<void*>(handle), &info);
    if (0 != ret || info == nullptr) {
        if (!request->pack(response->get_api(), ret, array{nullptr, 0}, 0, array{nullptr, 0}, 0,
            info->nMaxBatchSize, info->bDynamicBatchSize)) {
            LOG_MM_E(TAG, "Failed to pack response(ret: {}).", ret);
        }
        LOG_MM_D(TAG, "Response message send(ret: {}).", ret);
        return false;
    }
    LOG_MM_D(TAG, "Get io info success(ret: {}).", ret);

    const auto& [inputs, inputs_buffer] = gen_meta_package(info->pInputs, info->nInputSize);
    const auto& [outputs, outputs_buffer] = gen_meta_package(info->pOutputs, info->nOutputSize);

    if ( !request->pack(response->get_api(), ret, inputs, info->nInputSize, outputs, info->nOutputSize,
        info->nMaxBatchSize, info->bDynamicBatchSize)) {
        LOG_MM_E(TAG, "Failed to pack response(ret: {}).", ret);
        return false;
    }
    LOG_MM_D(TAG, "Response message send(input: {}, output: {}, ret: {}).", info->nInputSize, info->nOutputSize, ret);
    LOG_MM_D(TAG, "Input buffer size: {}, output buffer size: {}.", inputs.size, outputs.size);

    return true;
}

NATIVE_ENGINE_API_IMPL(AX_ENGINE_GetGroupIOInfoCount) {
    LOG_MM_D(TAG, "Start...");

    AX_U64 handle{};

    if (!response->unpack(response->get_api(), &handle)) {
        LOG_MM_E(TAG, "Failed to unpack request message.");
        return false;
    }
    LOG_MM_D(TAG, "Request message unpacked.");

    AX_U32 count{};
    const AX_S32 ret = ::AX_ENGINE_GetGroupIOInfoCount(reinterpret_cast<void*>(handle), &count);
    if (!request->pack(response->get_api(), ret, count)) {
        LOG_MM_E(TAG, "Failed to pack response(ret: {}).", ret);
        return false;
    }
    LOG_MM_D(TAG, "Response message send(ret: {}).", ret);

    return true;
}

NATIVE_ENGINE_API_IMPL(AX_ENGINE_GetGroupIOInfo) {
    LOG_MM_D(TAG, "Start...");

    AX_U64 handle{};
    AX_U32 index{};

    if (!response->unpack(response->get_api(), &handle, &index)) {
        LOG_MM_E(TAG, "Failed to unpack request message.");
        return false;
    }
    LOG_MM_D(TAG, "Request message unpacked.");

    AX_ENGINE_IO_INFO_T* info{};

    const AX_S32 ret = ::AX_ENGINE_GetGroupIOInfo(reinterpret_cast<void*>(handle), index, &info);
    if (0 != ret || info == nullptr) {
        if ( !request->pack(response->get_api(), ret, array{nullptr, 0}, 0, array{nullptr, 0}, 0,
            info->nMaxBatchSize, info->bDynamicBatchSize)) {
            LOG_MM_E(TAG, "Failed to pack failed response(ret: {}).", ret);
            return false;
        }
        LOG_MM_D(TAG, "Failed response message send(ret: {}).", ret);
    }
    LOG_MM_D(TAG, "Get io info success(ret: {}).", ret);

    const auto& [inputs, inputs_buffer] = gen_meta_package(info->pInputs, info->nInputSize);
    const auto& [outputs, outputs_buffer] = gen_meta_package(info->pOutputs, info->nOutputSize);

    if (!request->pack(response->get_api(), ret, inputs, info->nInputSize, outputs, info->nOutputSize,
        info->nMaxBatchSize, info->bDynamicBatchSize)) {
        LOG_MM_E(TAG, "Failed to pack response(ret: {}).", ret);
        return false;
    }
    LOG_MM_D(TAG, "Response message send(ret: {}).", ret);

    return true;
}

}  // namespace axcl::worker
