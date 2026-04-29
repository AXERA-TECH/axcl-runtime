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

namespace axcl::worker {

class idispatcher {
public:
    virtual void ack(const void* data, size_t size, int64_t flag, int32_t timeout) = 0;
    virtual ~idispatcher() = default;
};

}  // namespace axcl::worker
