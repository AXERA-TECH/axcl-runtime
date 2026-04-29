/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#pragma once

#include <ax_engine_api.h>

namespace axcl::worker {

static auto decode_io_package(const pkg::uint8_array& input, const AX_U32 input_size, const pkg::uint8_array& output, const AX_U32 output_size) {
    using shared_buffer = std::shared_ptr<AX_ENGINE_IO_BUFFER_T[]>;

    struct _io
    {
        uint64_t phy = 0;
        uint64_t vir = 0;
        uint64_t size = 0;
        uint64_t stride_size = 0;
        uint64_t stride[8] = {};
    };

    AX_ENGINE_IO_T io{};
    io.nInputSize = input_size;
    io.nOutputSize = output_size;

    std::list<std::shared_ptr<AX_ENGINE_IO_BUFFER_T[]>> io_buffers;
    const shared_buffer input_buffer(new AX_ENGINE_IO_BUFFER_T[input_size], std::default_delete<AX_ENGINE_IO_BUFFER_T[]>());
    const shared_buffer output_buffer(new AX_ENGINE_IO_BUFFER_T[output_size], std::default_delete<AX_ENGINE_IO_BUFFER_T[]>());

    io_buffers.push_back(input_buffer);
    io_buffers.push_back(output_buffer);

    io.pInputs = input_buffer.get();
    io.pOutputs = output_buffer.get();

    const auto input_ptr = reinterpret_cast<_io*>(input.data);
    const auto output_ptr = reinterpret_cast<_io*>(output.data);

    std::list<std::shared_ptr<AX_S32[]>> stride_buffers;

    for (AX_U32 i = 0; i < input_size; i++) {
        io.pInputs[i].phyAddr = input_ptr[i].phy;
        io.pInputs[i].pVirAddr = reinterpret_cast<void*>(input_ptr[i].vir);
        io.pInputs[i].nSize = input_ptr[i].size;
        io.pInputs[i].nStrideSize = input_ptr[i].stride_size;

        if (0 < input_ptr[i].stride_size) {
            auto stride = std::make_shared<AX_S32[]>(input_ptr[i].stride_size);
            stride_buffers.push_back(stride);
            io.pInputs[i].pStride = stride.get();
            for (uint64_t j = 0; j < input_ptr[i].stride_size; j++) {
                io.pInputs[i].pStride[j] = static_cast<AX_S32>(input_ptr[i].stride[j]);
            }
        }
        else {
            io.pInputs[i].pStride = nullptr;
        }
    }

    for (AX_U32 i = 0; i < output_size; i++) {
        io.pOutputs[i].phyAddr = output_ptr[i].phy;
        io.pOutputs[i].pVirAddr = reinterpret_cast<void*>(output_ptr[i].vir);
        io.pOutputs[i].nSize = output_ptr[i].size;
        io.pOutputs[i].nStrideSize = output_ptr[i].stride_size;

        if (0 < output_ptr[i].stride_size) {
            auto stride = std::make_shared<AX_S32[]>(output_ptr[i].stride_size);
            stride_buffers.push_back(stride);
            io.pOutputs[i].pStride = stride.get();
            for (uint64_t j = 0; j < output_ptr[i].stride_size; j++) {
                io.pOutputs[i].pStride[j] = static_cast<AX_S32>(output_ptr[i].stride[j]);
            }
        }
        else {
            io.pOutputs[i].pStride = nullptr;
        }
    }

    return std::make_tuple(io, io_buffers, stride_buffers);
}

}  // namespace axcl::worker
