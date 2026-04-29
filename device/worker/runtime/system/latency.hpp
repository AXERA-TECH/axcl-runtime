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

#include "package/device/runtime/system/latency/latency_request.hpp"
#include "package/device/runtime/system/latency/latency_response.hpp"
#include "base.hpp"

namespace axcl::worker {

class dispatcher;
class latency : public base {
public:
    latency(dispatcher *dispatcher);
    ~latency() = default;

    virtual void run(const void *data, const size_t& size) override;
};

}  // namespace axcl::worker
