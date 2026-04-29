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

#define AXCL_ENGINE_HOST_DEBUG

#include <ax_engine_api.h>

#define TAG "engine"

namespace axcl::worker {

static void io_info_dump(const AX_ENGINE_IO_T *pIO) {
#ifdef AXCL_ENGINE_HOST_DEBUG
    LOG_MM_D(TAG, "Total input size: {}.", pIO->nInputSize);
    for (AX_U32 i = 0; i < pIO->nInputSize; i++) {
        LOG_MM_D(TAG, "pInputs[{}]: phy {:#x}, vir: {:#x}, size: {}.",
                i, pIO->pInputs[i].phyAddr, reinterpret_cast<AX_U64>(pIO->pInputs[i].pVirAddr), pIO->pInputs[i].nSize);
        if (nullptr != pIO->pInputs[i].pStride) {
            char stride_str[256] = {0};
            sprintf(stride_str, "pInputs[%d]: stride[%d]: {", i, pIO->pInputs[i].nStrideSize);
            for (int j = 0; j < static_cast<int>(pIO->pInputs[i].nStrideSize) - 1; j++) {
                sprintf(stride_str, "%d, ", pIO->pInputs[i].pStride[j]);
            }
            sprintf(stride_str, "%d}.", pIO->pInputs[i].pStride[pIO->pInputs[i].nStrideSize - 1]);
            LOG_MM_D(TAG, "{}", std::string(stride_str));
        }
    }
    LOG_MM_D(TAG, "Total output size: {}.", pIO->nOutputSize);
    for (AX_U32 i = 0; i < pIO->nOutputSize; i++) {
        LOG_MM_D(TAG, "pOutputs[{}]: phy {:#x}, vir: {:#x}, size: {}.",
                i, pIO->pOutputs[i].phyAddr, reinterpret_cast<AX_U64>(pIO->pOutputs[i].pVirAddr), pIO->pOutputs[i].nSize);
        if (nullptr != pIO->pOutputs[i].pStride) {
            char stride_str[256] = {0};
            sprintf(stride_str, "pOutputs[%d]: stride[%d]: {", i, pIO->pOutputs[i].nStrideSize);
            for (int j = 0; j < static_cast<int>(pIO->pOutputs[i].nStrideSize) - 1; j++) {
                sprintf(stride_str, "%d, ", pIO->pOutputs[i].pStride[j]);
            }
            sprintf(stride_str, "%d}.", pIO->pOutputs[i].pStride[pIO->pOutputs[i].nStrideSize - 1]);
            LOG_MM_D(TAG, "{}", std::string(stride_str));
        }
    }
#endif
}

} // namespace axcl::worker
