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

namespace axcl::pkg::runtime {

enum class module : uint8_t {
    SYSTEM = 0,
    MEMORY = 1,
    ENGINE = 2,
    P2P = 3,
    USRWORK = 4,
    UNKNOWN = 127,
};

}  // namespace axcl::pkg::runtime