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

#include "npu_decode_io.hpp"
#include "npu_dump_io.hpp"

#include "axcl_logger.hpp"
#include "res_guard.hpp"

#include <ax_sys_api.h>
#include <ax_engine_api.h>

#define TAG "engine"

namespace axcl::worker {

NATIVE_ENGINE_API_IMPL(AX_ENGINE_RunSync) {
    LOG_MM_D(TAG, "Start...");

    AX_U64 handle{};
    array input{}, output{};
    AX_U32 input_size, output_size, batch, parallel;

    if (!response->unpack(response->get_api(), &handle, &input, &input_size, &output, &output_size, &batch, &parallel)) {
        LOG_MM_E(TAG, "Failed to unpack request message.");
        return false;
    }
    LOG_MM_D(TAG, "Request message unpacked.");

    auto [io, _1, _2] = decode_io_package(input, input_size, output, output_size);
    io.nBatchSize = batch;
    io.nParallelRun = parallel;

    LOG_MM_D(TAG, "Handle: {:#x}, input size: {}, output size: {}, batch: {}, parallel: {}.",
        handle, input_size, output_size, batch, parallel);

    io_info_dump(&io);

    const AX_S32 ret = ::AX_ENGINE_RunSync(reinterpret_cast<void*>(handle), &io);
    if (!request->pack(response->get_api(), ret)) {
        LOG_MM_E(TAG, "Failed to pack failed response(ret: {}).", ret);
        return false;
    }
    LOG_MM_D(TAG, "Response message send(ret: {}).", ret);

    return true;
}

NATIVE_ENGINE_API_IMPL(AX_ENGINE_RunSyncV2) {
    LOG_MM_D(TAG, "Start...");

    AX_U64 handle, context;
    array input, output;
    AX_U32 input_size, output_size, batch, parallel;

    if (!response->unpack(response->get_api(), &handle, &context, &input, &input_size, &output, &output_size, &batch, &parallel)) {
        LOG_MM_E(TAG, "Failed to unpack request message.");
        return false;
    }
    LOG_MM_D(TAG, "Request message unpacked.");

    auto [io, _1, _2] = decode_io_package(input, input_size, output, output_size);
    io.nBatchSize = batch;
    io.nParallelRun = parallel;

    LOG_MM_D(TAG, "Handle: {:#x}, context: {:#x}, input size: {}, output size: {}, batch: {}, parallel: {}.",
        handle, context, input_size, output_size, batch, parallel);

    io_info_dump(&io);

    const AX_S32 ret = ::AX_ENGINE_RunSyncV2(reinterpret_cast<void*>(handle), reinterpret_cast<void*>(context), &io);
    if (!request->pack(response->get_api(), ret)) {
        LOG_MM_E(TAG, "Failed to pack failed response(ret: {}).", ret);
        return false;
    }
    LOG_MM_D(TAG, "Response message send(ret: {}).", ret);

    return true;
}

NATIVE_ENGINE_API_IMPL(AX_ENGINE_RunGroupIOSync) {
    LOG_MM_D(TAG, "Start...");

    AX_U64 handle, context;
    array input, output;
    AX_U32 index, input_size, output_size, batch, parallel;

    if (!response->unpack(response->get_api(), &handle, &context, &index, &input, &input_size, &output, &output_size, &batch, &parallel)) {
        LOG_MM_E(TAG, "Failed to unpack request message.");
        return false;
    }
    LOG_MM_D(TAG, "Request message unpacked.");

    auto [io, _1, _2] = decode_io_package(input, input_size, output, output_size);
    io.nBatchSize = batch;
    io.nParallelRun = parallel;

    LOG_MM_D(TAG, "Handle: {:#x}, context: {:#x}, index: {}, input size: {}, output size: {}, batch: {}, parallel: {}.",
        handle, context, index, input_size, output_size, batch, parallel);

    io_info_dump(&io);

    LOG_MM_D(TAG, "Run group io starting...");
    const AX_S32 ret = ::AX_ENGINE_RunGroupIOSync(reinterpret_cast<void*>(handle), reinterpret_cast<void*>(context), index, &io);
    if (0 == ret) {
        LOG_MM_D(TAG, "Run group io success.");
    } else {
        LOG_MM_E(TAG, "Run group io failed(ret: {}).", ret);
    }

    if (!request->pack(response->get_api(), ret)) {
        LOG_MM_E(TAG, "Failed to pack failed response(ret: {}).", ret);
        return false;
    }
    LOG_MM_D(TAG, "Response message send(ret: {}).", ret);

    return true;
}
}  // namespace axcl::worker
