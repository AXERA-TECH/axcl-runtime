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

#include <atomic>
#include <cstdint>

namespace axcl {

class token {
public:
    token(void) = default;
    token(const token &) = delete;
    token &operator=(const token &) = delete;

    uint64_t gen() {
        return m_token++;
    }

private:
    std::atomic<uint64_t> m_token = {1};
};

}  // namespace axcl
