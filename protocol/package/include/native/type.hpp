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

namespace axcl::pkg::native {

enum class module : uint8_t {
    SYS = 0,
    VDEC = 1,
    VENC = 2,
    IVPS = 3,
    IVE = 4,
    ENGINE = 5,
    DMADIM = 6,
    DSP = 7,
    UNKNOWN = 127,
};

}  // namespace axcl::pkg::native