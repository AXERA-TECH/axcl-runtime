/*
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 * Author: wanglusheng@axera-tech.com
 */

#pragma once

#include <vector>
#include <string>

namespace axcl::pkg::runtime::engine {

    enum class API : uint8_t
    {
        INIT = 0,
        FINAL,
        GET_NPU_TYPE,
        GET_ENGINE_VERSION,
        LOAD,
        GET_VERSION,
        GET_TYPE,
        GET_USAGE,
        UNLOAD,
        CREATE_CONTEXT,
        GET_INFO,
        RUN,
        SET_AFFINITY,
        GET_AFFINITY,
        P2P,
    };

    enum class NPU : uint8_t
    {
        DISABLE = 0,
        SPLIT = 1,
        LITTLE_BIG = 2,
        BIG_LITTLE = 3,
    };

    enum class MODEL : uint8_t
    {
        NPU1 = 0,
        NPU2 = 1,
        NPU3 = 2,
    };

    enum class FORMAT : uint8_t
    {
        NONE = 0,
        INT4 = 1,
        UINT4 = 2,
        INT8 = 3,
        UINT8 = 4,
        INT16 = 5,
        UINT16 = 6,
        INT32 = 7,
        UINT32 = 8,
        INT64 = 9,
        UINT64 = 10,
        FP4 = 11,
        FP8 = 12,
        FP16 = 13,
        BF16 = 14,
        FP32 = 15,
        FP64 = 16,
    };

    enum class LAYOUT : uint8_t
    {
        NONE = 0,
        NHWC = 1,
        NCHW = 2,
    };

    struct io
    {
        uint64_t size;
        std::vector<uint64_t> shape;
        std::vector<uint64_t> stride;
    };

    struct io_meta
    {
        std::string name;
        LAYOUT layout;
        FORMAT format;
        std::vector<io> groups;
    };

    struct meta
    {
        std::vector<io_meta> inputs;
        std::vector<io_meta> outputs;
    };

    struct buffer
    {
        std::string name;
        uint64_t address;
        uint64_t size;
    };

    struct desc
    {
        std::vector<buffer> inputs;
        std::vector<buffer> outputs;
        size_t batch;
    };
}
