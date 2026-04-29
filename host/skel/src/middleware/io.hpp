/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * License); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * AS IS BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#pragma once

#include <cstdio>
#include <cstring>
#include <vector>
#include <array>
#include <string>

#include "def.hpp"
#include "axcl_sys.h"
#include "ax_engine_type.h"
#include "axcl_rt_device.h"
#include "axcl_rt_memory.h"
#include "dma_buffer.hpp"

namespace axcl::skel {
typedef enum {
    SKEL_IO_BUFFER_STRATEGY_DEFAULT,
    SKEL_IO_BUFFER_STRATEGY_CACHED
} SKEL_IO_BUFFER_STRATEGY_T;

#define SKEL_IO_CMM_ALIGN_SIZE 128

typedef struct {
    dma_buffer* allocator;
    AX_U64 phyAddr;
    AX_VOID *pVirAddr;
} AXCL_SKEL_IO_BUFFER_T;

static inline AX_S32 query_model_input_size(const AX_ENGINE_IO_INFO_T* io_info, std::array<int, 2> &input_size,
                                       AX_IMG_FORMAT_E &eDtype) {
    int height = 0;
    int width = 0;
    int size = 0;
    int channel = 0;
    int data_type_size = 0;
    auto& input = io_info->pInputs[0];

    switch (input.eLayout) {
        case AX_ENGINE_TENSOR_LAYOUT_NHWC:
            height = input.pShape[1];
            width = input.pShape[2];
            channel = input.pShape[3];
            size = input.nSize;
            break;
        case AX_ENGINE_TENSOR_LAYOUT_NCHW:
            channel = input.pShape[1];
            height = input.pShape[2];
            width = input.pShape[3];
            size = input.nSize;
            break;
        default:
            height = input.pShape[1];
            width = input.pShape[2];
            channel = input.pShape[3];
            size = input.nSize;
            break;
    }

    switch (input.eDataType) {
        case AX_ENGINE_DT_UINT8:
        case AX_ENGINE_DT_SINT8:
            data_type_size = 1;
            break;
        case AX_ENGINE_DT_UINT16:
        case AX_ENGINE_DT_SINT16:
            data_type_size = 2;
            break;
        case AX_ENGINE_DT_FLOAT32:
            data_type_size = 4;
            break;
        case AX_ENGINE_DT_SINT32:
        case AX_ENGINE_DT_UINT32:
            data_type_size = 4;
            break;
        case AX_ENGINE_DT_FLOAT64:
            data_type_size = 8;
            break;
        case AX_ENGINE_DT_UINT10_PACKED:
        case AX_ENGINE_DT_UINT12_PACKED:
        case AX_ENGINE_DT_UINT14_PACKED:
        case AX_ENGINE_DT_UINT16_PACKED:
        default:
            return -1;
    }

    ALOGI("eLayout:{}, eDataType:{}, channel:{}, height:{}, width:{}, size:{}, data_type_size:{}",
            input.eLayout, channel, height, width, size, data_type_size, input.eDataType);

    if (channel == 0 || height == 0 || width == 0 || size == 0 || data_type_size == 0) {
        return -1;
    }

// FIXME. only for YUV420
#if 1
    input_size[0] = (size * 2)/ (width * data_type_size * 3);
    input_size[1] = width;
    eDtype = AX_FORMAT_YUV420_SEMIPLANAR;
#else
    printf("eColorSpace:%d\n",
            input.pExtraMeta->eColorSpace);

    switch (input.pExtraMeta->eColorSpace) {
        case AX_ENGINE_CS_BGR:
            input_size[0] = size / (width * data_type_size * 3);
            input_size[1] = width;
            eDtype = AX_FORMAT_BGR888;
            break;
        case AX_ENGINE_CS_RGB:
            input_size[0] = size / (width * data_type_size * 3);
            input_size[1] = width;
            eDtype = AX_FORMAT_RGB888;
            break;
        case AX_ENGINE_CS_NV12:
            input_size[0] = (size * 2)/ (width * data_type_size * 3);
            input_size[1] = width;
            eDtype = AX_FORMAT_YUV420_SEMIPLANAR;
            break;
        case AX_ENGINE_CS_RGBA:
        case AX_ENGINE_CS_GRAY:
        case AX_ENGINE_CS_NV21:
        case AX_ENGINE_CS_YUV444:
        case AX_ENGINE_CS_RAW8:
        case AX_ENGINE_CS_RAW10:
        case AX_ENGINE_CS_RAW12:
        case AX_ENGINE_CS_RAW14:
        case AX_ENGINE_CS_RAW16:
        case AX_ENGINE_CS_FEATUREMAP:
        default:
            return -1;
    }
#endif

    return 0;
}

static inline void brief_io_info(std::string strModel, const AX_ENGINE_IO_INFO_T* io_info) {
    auto describe_shape_type = [](AX_ENGINE_TENSOR_LAYOUT_T type) -> const char* {
        switch (type) {
            case AX_ENGINE_TENSOR_LAYOUT_NHWC:
                return "NHWC";
            case AX_ENGINE_TENSOR_LAYOUT_NCHW:
                return "NCHW";
            default:
                return "unknown";
        }
    };
    auto describe_data_type = [](AX_ENGINE_DATA_TYPE_T type) -> const char* {
        switch (type) {
            case AX_ENGINE_DT_UINT8:
                return "uint8";
            case AX_ENGINE_DT_UINT16:
                return "uint16";
            case AX_ENGINE_DT_FLOAT32:
                return "float32";
            case AX_ENGINE_DT_SINT16:
                return "sint16";
            case AX_ENGINE_DT_SINT8:
                return "sint8";
            case AX_ENGINE_DT_SINT32:
                return "sint32";
            case AX_ENGINE_DT_UINT32:
                return "uint32";
            case AX_ENGINE_DT_FLOAT64:
                return "float64";
            case AX_ENGINE_DT_UINT10_PACKED:
                return "uint10_packed";
            case AX_ENGINE_DT_UINT12_PACKED:
                return "uint12_packed";
            case AX_ENGINE_DT_UINT14_PACKED:
                return "uint14_packed";
            case AX_ENGINE_DT_UINT16_PACKED:
                return "uint16_packed";
            default:
                return "unknown";
        }
    };
    auto describe_memory_type = [](AX_ENGINE_MEMORY_TYPE_T type) -> const char* {
        switch (type) {
            case AX_ENGINE_MT_PHYSICAL:
                return "Physical";
            case AX_ENGINE_MT_VIRTUAL:
                return "Virtual";
            default:
                return "unknown";
        }
    };
    auto describe_color_space = [](AX_ENGINE_COLOR_SPACE_T cs) -> const char* {
        switch (cs) {
            case AX_ENGINE_CS_FEATUREMAP:
                return "FeatureMap";
            case AX_ENGINE_CS_BGR:
                return "BGR";
            case AX_ENGINE_CS_RGB:
                return "RGB";
            case AX_ENGINE_CS_RGBA:
                return "RGBA";
            case AX_ENGINE_CS_GRAY:
                return "GRAY";
            case AX_ENGINE_CS_NV12:
                return "NV12";
            case AX_ENGINE_CS_NV21:
                return "NV21";
            case AX_ENGINE_CS_YUV444:
                return "YUV444";
            case AX_ENGINE_CS_RAW8:
                return "RAW8";
            case AX_ENGINE_CS_RAW10:
                return "RAW10";
            case AX_ENGINE_CS_RAW12:
                return "RAW12";
            case AX_ENGINE_CS_RAW14:
                return "RAW14";
            case AX_ENGINE_CS_RAW16:
                return "RAW16";
            default:
                return "unknown";
        }
    };
    printf("Model Name: %s\n", strModel.c_str());
    printf("Max Batch Size %d\n", io_info->nMaxBatchSize);
    printf("Support Dynamic Batch? %s\n", io_info->bDynamicBatchSize == AX_TRUE ? "Yes" : "No");

    for (uint32_t i = 0; i < io_info->nInputSize; ++i) {
        auto& input = io_info->pInputs[i];
        printf("Input[%d]: %s\n", i, input.pName);
        printf("    Shape [");
        for (uint32_t j = 0; j < input.nShapeSize; ++j) {
            printf("%d", (int)input.pShape[j]);
            if (j + 1 < input.nShapeSize) printf(", ");
        }
        printf("] %s %s %s %s\n", describe_shape_type(input.eLayout), describe_data_type(input.eDataType),
               input.pExtraMeta ? describe_color_space(input.pExtraMeta->eColorSpace) : "",
               input.nQuantizationValue > 0 ? ("Q=" + std::to_string(input.nQuantizationValue)).c_str() : "");
        printf("    Memory %s\n", describe_memory_type(input.eMemoryType));
        printf("    Size %u\n", input.nSize);
    }
    for (uint32_t i = 0; i < io_info->nOutputSize; ++i) {
        auto& output = io_info->pOutputs[i];
        printf("Output[%d]: %s\n", i, output.pName);
        printf("    Shape [");
        for (uint32_t j = 0; j < output.nShapeSize; ++j) {
            printf("%d", (int)output.pShape[j]);
            if (j + 1 < output.nShapeSize) printf(", ");
        }
        printf("] %s %s %s\n", describe_shape_type(output.eLayout), describe_data_type(output.eDataType),
               output.nQuantizationValue > 0 ? ("Q=" + std::to_string(output.nQuantizationValue)).c_str() : "");
        printf("    Memory %s\n", describe_memory_type(output.eMemoryType));
        printf("    Size %u\n", output.nSize);
    }
}

static inline AX_S32 alloc_engine_buffer(const std::string& token, size_t index, const AX_ENGINE_IOMETA_T* pMeta, AX_ENGINE_IO_BUFFER_T* pBuf, SKEL_IO_BUFFER_STRATEGY_T eStrategy = SKEL_IO_BUFFER_STRATEGY_DEFAULT) {
    AX_S32 ret = -1;
    if (eStrategy != SKEL_IO_BUFFER_STRATEGY_DEFAULT && eStrategy != SKEL_IO_BUFFER_STRATEGY_CACHED) {
        fprintf(stderr, "strategy %d not supported\n", (int)eStrategy);
        return -1;
    }
    memset(pBuf, 0, sizeof(AX_ENGINE_IO_BUFFER_T));
    pBuf->nSize = pMeta->nSize;

    const std::string token_name = "skel_" + token + "_out_" + std::to_string(index);

    if (eStrategy == SKEL_IO_BUFFER_STRATEGY_CACHED) {
        ret = AXCL_SYS_MemAllocCached((AX_U64*)&pBuf->phyAddr, &pBuf->pVirAddr, pBuf->nSize, SKEL_IO_CMM_ALIGN_SIZE, (const AX_S8*)token_name.c_str());
    }
    else {
        ret = AXCL_SYS_MemAlloc((AX_U64*)&pBuf->phyAddr, &pBuf->pVirAddr, pBuf->nSize, SKEL_IO_CMM_ALIGN_SIZE, (const AX_S8*)token_name.c_str());
    }

    return ret;
}

static inline AX_S32 free_engine_buffer(AX_ENGINE_IO_BUFFER_T* pBuf) {
    if (pBuf->phyAddr == 0) {
        delete[] reinterpret_cast<uint8_t*>(pBuf->pVirAddr);
    }
    else {
        AXCL_SYS_MemFree(pBuf->phyAddr, pBuf->pVirAddr);
    }
    pBuf->phyAddr = 0;
    pBuf->pVirAddr = nullptr;

    return 0;
}

static inline void free_io_index(AX_ENGINE_IO_BUFFER_T* io_buf, size_t index) {
    AX_ENGINE_IO_BUFFER_T* pBuf = io_buf + index;
    free_engine_buffer(pBuf);
}

static inline void free_io(AX_ENGINE_IO_T &io) {
    if (io.pInputs) {
        delete[] io.pInputs;
        io.pInputs = nullptr;
    }

    if (io.pOutputs) {
        for (size_t j = 0; j < io.nOutputSize; ++j) {
            free_io_index(io.pOutputs, j);
        }
        delete[] io.pOutputs;
        io.pOutputs = nullptr;
    }
}

static inline void free_io(AX_ENGINE_IO_T &io,
                                std::vector<std::vector<AX_ENGINE_IO_BUFFER_T>> &vecOutputBuffer,
                                std::vector<std::vector<AXCL_SKEL_IO_BUFFER_T>> &vecSkelOutput) {
    if (io.pInputs) {
        delete[] io.pInputs;
        io.pInputs = nullptr;
    }

    if (io.pOutputs) {
        for (size_t index = 0; index < vecOutputBuffer.size(); ++index) {
            AX_ENGINE_IO_BUFFER_T *pOutputs = &vecOutputBuffer[index][0];
            for (size_t j = 0; j < io.nOutputSize; ++j) {
                free_io_index(pOutputs, j);
            }
        }

        for (size_t index = 0; index < vecSkelOutput.size(); ++index) {
            for (size_t j = 0; j < io.nOutputSize; ++j) {
                AXCL_SKEL_IO_BUFFER_T *pOutputs = &vecSkelOutput[index][j];
                if (pOutputs && pOutputs->allocator) {
                    pOutputs->allocator->free();

                    delete pOutputs->allocator;
                    pOutputs->allocator = nullptr;
                }
            }
        }

        delete[] io.pOutputs;
        io.pOutputs = nullptr;
    }
}

static inline int prepare_io(const std::string& token, const AX_ENGINE_IO_INFO_T* info, AX_ENGINE_IO_T &io, SKEL_IO_BUFFER_STRATEGY_T strategy) {
    auto ret = 0;

    memset(&io, 0, sizeof(io));

    if (1 != info->nInputSize) {
        fprintf(stderr, "[ERR]: Only single input was accepted(got %u).\n", info->nInputSize);
        return -1;
    }

    io.pInputs = new AX_ENGINE_IO_BUFFER_T[info->nInputSize];

    if (!io.pInputs) {
        goto EXIT;
    }

    memset(io.pInputs, 0x00, sizeof(AX_ENGINE_IO_BUFFER_T) * info->nInputSize);
    io.nInputSize = info->nInputSize;

    io.pOutputs = new AX_ENGINE_IO_BUFFER_T[info->nOutputSize];

    if (!io.pOutputs) {
        goto EXIT;
    }

    memset(io.pOutputs, 0x00, sizeof(AX_ENGINE_IO_BUFFER_T) * info->nOutputSize);
    io.nOutputSize = info->nOutputSize;

    for (size_t i = 0; i < info->nOutputSize; ++i) {
        auto meta = info->pOutputs[i];
        auto buffer = &io.pOutputs[i];
        alloc_engine_buffer(token, i, &meta, buffer, strategy);

        if (ret != 0) {
            goto EXIT;
        }
    }

EXIT:
    if (ret != 0) {
        free_io(io);
        return -1;
    }

    return 0;
}

static inline int prepare_io(const std::string& token,
                                const AX_ENGINE_IO_INFO_T* info, AX_ENGINE_IO_T &io,
                                const AX_U32 &nOutputBufferDepth,
                                std::vector<std::vector<AX_ENGINE_IO_BUFFER_T>> &vecOutputBuffer,
                                std::vector<std::vector<AXCL_SKEL_IO_BUFFER_T>> &vecSkelOutput,
                                const SKEL_IO_BUFFER_STRATEGY_T &strategy) {
    AX_S32 ret = 0;
    memset(&io, 0, sizeof(io));

    if (1 != info->nInputSize) {
        fprintf(stderr, "[ERR]: Only single input was accepted(got %u).\n", info->nInputSize);
        return -1;
    }

    io.pInputs = new AX_ENGINE_IO_BUFFER_T[info->nInputSize];

    if (!io.pInputs) {
        goto EXIT;
    }

    memset(io.pInputs, 0x00, sizeof(AX_ENGINE_IO_BUFFER_T) * info->nInputSize);
    io.nInputSize = info->nInputSize;

    io.pOutputs = new AX_ENGINE_IO_BUFFER_T[info->nOutputSize];

    if (!io.pOutputs) {
        goto EXIT;
    }

    for (size_t index = 0; index < nOutputBufferDepth; ++index) {
        std::vector<AX_ENGINE_IO_BUFFER_T> outputBuffer;
        std::vector<AXCL_SKEL_IO_BUFFER_T> outputSkel;
        for (size_t i = 0; i < info->nOutputSize; ++i) {
            auto meta = info->pOutputs[i];

            // engine io
            {
                auto buffer = io.pOutputs[i];
                ret = alloc_engine_buffer(token, i, &meta, &buffer, strategy);

                if (ret != 0) {
                    goto EXIT;
                }

                outputBuffer.push_back(buffer);
            }

            // skel io
            {
                AXCL_SKEL_IO_BUFFER_T io_skel;
                memset(&io_skel, 0x00, sizeof(io_skel));

                int32_t device_id;
                ret = axclrtGetDevice(&device_id);
                if (ret!= 0) {
                    goto EXIT;
                }

                io_skel.allocator = new dma_buffer(device_id);

                if (!io_skel.allocator
                    || !io_skel.allocator->alloc(meta.nSize)) {
                    goto EXIT;
                }
                const struct dma_mem& mem = io_skel.allocator->get();

                io_skel.phyAddr = mem.blks[0].phy;
                io_skel.pVirAddr = mem.blks[0].vir;

                outputSkel.push_back(io_skel);
            }
        }

        vecOutputBuffer.push_back(outputBuffer);
        vecSkelOutput.push_back(outputSkel);
    }

    memset(io.pOutputs, 0x00, sizeof(AX_ENGINE_IO_BUFFER_T) * info->nOutputSize);
    io.nOutputSize = info->nOutputSize;

    for (size_t i = 0; i < info->nOutputSize; ++i) {
        auto buffer = &io.pOutputs[i];
        *buffer = vecOutputBuffer[0][i];
    }

EXIT:
    if (ret != 0) {
        free_io(io);
        return -1;
    }

    return 0;
}

static inline AX_S32 push_io_output(const AX_ENGINE_IO_INFO_T* info,
                                            AX_ENGINE_IO_T& io,
                                            std::vector<AX_ENGINE_IO_BUFFER_T> &outputBuffer) {
    for (size_t i = 0; i < info->nOutputSize; ++i) {
        auto buffer = &io.pOutputs[i];
        *buffer = outputBuffer[i];
    }

    return 0;
}

static inline AX_S32 push_io_input(const AX_VIDEO_FRAME_T* pImage, AX_ENGINE_IO_T& io) {
    AX_ENGINE_IO_BUFFER_T* pBuf = &io.pInputs[0];

    pBuf->phyAddr = (AX_ADDR)pImage->u64PhyAddr[0];
    pBuf->pVirAddr = (AX_VOID *)pImage->u64VirAddr[0];
    pBuf->nSize = (AX_U32)pImage->u32FrameSize;

    return 0;
}

static inline AX_S32 cache_io_flush(const AX_ENGINE_IO_BUFFER_T *io_buf, AXCL_SKEL_IO_BUFFER_T *io_skel = nullptr) {
    if (io_buf->phyAddr != 0) {
        AXCL_SYS_MflushCache(io_buf->phyAddr, io_buf->pVirAddr, io_buf->nSize);
    }

    if (io_skel && io_skel->phyAddr) {
        AX_VOID *pAddr = reinterpret_cast<AX_VOID *>(io_buf->phyAddr);
        axclrtMemcpy(reinterpret_cast<void*>(io_skel->phyAddr), pAddr, io_buf->nSize, AXCL_MEMCPY_DEVICE_TO_HOST_PHY);
    }

    return 0;
}

static inline AX_S32 inc_io_ref_cnt(const AX_VIDEO_FRAME_T &stFrame) {
    if (stFrame.u32BlkId[0] > 0) {
        AXCL_POOL_IncreaseRefCnt(stFrame.u32BlkId[0]);
    }
    if (stFrame.u32BlkId[1] > 0) {
        AXCL_POOL_IncreaseRefCnt(stFrame.u32BlkId[1]);
    }
    if (stFrame.u32BlkId[2] > 0) {
        AXCL_POOL_IncreaseRefCnt(stFrame.u32BlkId[2]);
    }

    return 0;
}

static inline AX_S32 dec_io_ref_cnt(const AX_VIDEO_FRAME_T &stFrame) {
    if (stFrame.u32BlkId[0] > 0) {
        AXCL_POOL_DecreaseRefCnt(stFrame.u32BlkId[0]);
    }
    if (stFrame.u32BlkId[1] > 0) {
        AXCL_POOL_DecreaseRefCnt(stFrame.u32BlkId[1]);
    }
    if (stFrame.u32BlkId[2] > 0) {
        AXCL_POOL_DecreaseRefCnt(stFrame.u32BlkId[2]);
    }

    return 0;
}
}  // namespace axcl::skel

using namespace axcl::skel;
