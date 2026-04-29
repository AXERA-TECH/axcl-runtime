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
#include "log.hpp"
#include "control.hpp"
#include "latency.hpp"

namespace axcl::worker {

class dispatcher;
class system final : public base {
public:
    explicit system(dispatcher *dispatcher);
    ~system() override = default;

    void run(const void *data, const size_t& size) override;

private:
    std::unique_ptr<log> m_log;
    std::unique_ptr<control> m_control;
    std::unique_ptr<latency> m_latency;
};

}  // namespace axcl::worker
