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

#include "package/device/runtime/system/log/log_request.hpp"
#include "package/device/runtime/system/log/log_response.hpp"
#include "base.hpp"

namespace axcl::worker {

class dispatcher;
class log : public base {
public:
    log(dispatcher *dispatcher);
    ~log() = default;

    virtual void run(const void *data, const size_t& size) override;

private:
    void DUMP(const void *data, const size_t& size);
};

}  // namespace axcl::worker
