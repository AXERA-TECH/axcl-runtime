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
#include "system.hpp"
#include "memory.hpp"
#include "engine.hpp"
#include "p2p.hpp"
#include "usrwork.hpp"

namespace axcl::worker {

class dispatcher;
class runtime final : public base {
public:
    explicit runtime(dispatcher *dispatcher);
    ~runtime() override = default;

    void run(const void *data, const size_t& size) override;

private:
    std::unique_ptr<system> m_system;
    std::unique_ptr<memory> m_memory;
    std::unique_ptr<engine> m_engine;
    std::unique_ptr<p2p> m_p2p;
    std::unique_ptr<usrwork> m_usrwork;
};

}  // namespace axcl::worker
