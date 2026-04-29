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

#include "base.hpp"
#include "channel.hpp"
#include "native.hpp"
#include "runtime.hpp"

namespace axcl::worker {
class dispatcher;

class impl   : public base {
public:
    impl(dispatcher *dispatcher);
    virtual ~impl() = default;

    virtual void run(const void *data, const size_t& size) override;

private:
    std::unique_ptr<native> m_native;
    std::unique_ptr<runtime> m_runtime;
};

}  // namespace axcl::worker
