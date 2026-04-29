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

// TODO: need to be optimized
static auto gen_io_package(const AX_ENGINE_IO_BUFFER_T* io, const AX_U32& size) {
    using array = axcl::pkg::uint8_array;

    struct _io
    {
        uint64_t phy = 0;
        uint64_t vir = 0;
        uint64_t size = 0;
        uint64_t stride_size = 0;
        uint64_t stride[8] = {};
    };

    const auto buffer_size = size * sizeof(_io);
    const std::shared_ptr<uint8_t[]> buffer(new uint8_t[buffer_size]());

    const array package = {buffer.get(), static_cast<uint32_t>(buffer_size)};
    const auto io_fill_ptr = reinterpret_cast<_io *>(package.data);

    for (AX_U32 i = 0; i < size; i++) {
        io_fill_ptr[i].phy = io[i].phyAddr;
        io_fill_ptr[i].vir = reinterpret_cast<uint64_t>(io[i].pVirAddr);
        io_fill_ptr[i].size = io[i].nSize;
        io_fill_ptr[i].stride_size = io[i].nStrideSize;
        for (AX_U8 j = 0; j < io[i].nStrideSize; j++) {
            io_fill_ptr[i].stride[j] = io[i].pStride[j];
        }
    }

    return std::make_tuple(package, buffer);
}

AX_S32 npu::AX_ENGINE_RunSync(AX_ENGINE_HANDLE handle, const AX_ENGINE_IO_T* pIO) {
    dump_io(pIO);

    constexpr auto api(axcl::pkg::native::ENGINE_API::AX_ENGINE_RunSync);

    const auto input_pack = gen_io_package(pIO->pInputs, pIO->nInputSize);
    const auto output_pack = gen_io_package(pIO->pOutputs, pIO->nOutputSize);

    if (!m_req.pack(api, reinterpret_cast<AX_U64>(handle),
        std::get<0>(input_pack), pIO->nInputSize, std::get<0>(output_pack), pIO->nOutputSize,
        pIO->nBatchSize, pIO->nParallelRun)) {
        return AXCL_DEF_ENGINE_ERR(AXCL_ERR_ENCODE);
    }

    if (const int32_t ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    int32_t ret;
    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_ENGINE_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 npu::AX_ENGINE_RunSyncV2(AX_ENGINE_HANDLE handle, AX_ENGINE_CONTEXT_T context, const AX_ENGINE_IO_T* pIO) {
    dump_io(pIO);

    constexpr auto api(axcl::pkg::native::ENGINE_API::AX_ENGINE_RunSyncV2);

    const auto input_pack = gen_io_package(pIO->pInputs, pIO->nInputSize);
    const auto output_pack = gen_io_package(pIO->pOutputs, pIO->nOutputSize);

    if (!m_req.pack(api, reinterpret_cast<AX_U64>(handle), reinterpret_cast<AX_U64>(context),
        std::get<0>(input_pack), pIO->nInputSize, std::get<0>(output_pack), pIO->nOutputSize,
        pIO->nBatchSize, pIO->nParallelRun)) {
        return AXCL_DEF_ENGINE_ERR(AXCL_ERR_ENCODE);
    }

    if (const int32_t ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    int32_t ret;
    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_ENGINE_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 npu::AX_ENGINE_RunGroupIOSync(AX_ENGINE_HANDLE handle, AX_ENGINE_CONTEXT_T context, const AX_U32 nIndex, const AX_ENGINE_IO_T* pIO) {
    dump_io(pIO);

    constexpr auto api(axcl::pkg::native::ENGINE_API::AX_ENGINE_RunGroupIOSync);

    const auto input_pack = gen_io_package(pIO->pInputs, pIO->nInputSize);
    const auto output_pack = gen_io_package(pIO->pOutputs, pIO->nOutputSize);

    if (!m_req.pack(api, reinterpret_cast<AX_U64>(handle), reinterpret_cast<AX_U64>(context), nIndex,
        std::get<0>(input_pack), pIO->nInputSize, std::get<0>(output_pack), pIO->nOutputSize,
        pIO->nBatchSize, pIO->nParallelRun)) {
        return AXCL_DEF_ENGINE_ERR(AXCL_ERR_ENCODE);
    }

    if (const int32_t ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    int32_t ret;
    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_ENGINE_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}
