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

#include <cstdint>

namespace axcl::pkg::runtime::memory {

enum class CHACHE_TYPE : int {
    NON_CACHED = 0,
    CACHED = 1,
};

enum class API : int {
    ALLOC = 0,
    FREE = 1,
    FLUSH = 2,
    INVALIDATE = 3,
    SET = 4,    //!< means memset
    WRITE = 5,  //!< means write to device from host
    READ = 6,   //!< means read to host from device
    SYNC = 7,   //!< means copy from device memory A to device memory B
    CMP = 8,    //!< means memcmp device memory A with device memory B
    UNKNOWN = 127
};

}  // namespace axcl::pkg::runtime::memory