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
#include "channel.hpp"
#include "ax_global_type.h"

namespace axcl::worker {

class dispatcher;
class base {
public:
    base(dispatcher *dispatcher) : m_dispatcher(dispatcher) {};
    virtual ~base() = default;

    virtual void run(std::shared_ptr<channel_data> data) {};
    virtual void run(const void *data, const size_t& size) {};

    dispatcher *get_dispatcher() { return m_dispatcher; };

protected:
    dispatcher *m_dispatcher{nullptr};
};

}  // namespace axcl::worker
