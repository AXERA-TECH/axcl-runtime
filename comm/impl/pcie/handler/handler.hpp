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

#include <cstddef>
#include <cstdint>
#include <memory>
#include "axcl_package.hpp"

class handler {
public:
    virtual ~handler() = default;

    virtual size_t send(const void *buf, size_t size, int32_t timeout) = 0;
    virtual size_t recv(void **buf, int32_t timeout) = 0;
};
