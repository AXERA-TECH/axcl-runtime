/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 * Author: wanglusheng@axera-tech.com
 *
 **************************************************************************************************/

#pragma once

#include <cstdint>

namespace axcl::pkg {

enum class module : uint8_t {
    RUNTIME = 0,
    NATIVE = 1,
    UNKNOWN = 127,
};

struct version {
    uint32_t major_v;
    uint32_t minor_v;
    uint32_t patch_v;
};

struct environment {
    uint32_t pid;
    uint32_t tid;
};

struct uint8_array {
    uint8_t *data = nullptr;
    uint32_t size = 0;
};

}  // namespace axcl::pkg