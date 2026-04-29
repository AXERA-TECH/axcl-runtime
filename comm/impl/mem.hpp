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

#include <stdlib.h>
#include <cstddef>

class mem {
public:
    mem();
    ~mem();

    void *malloc(size_t size) {
        return ::malloc(size);
    }

    void free(void *buf) {
        return ::free(buf);
    }
};
