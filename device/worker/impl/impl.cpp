/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "impl.hpp"
#include "dispatcher.hpp"
#include <chrono>
#include <string>
#include "axcl_logger.hpp"
#include "protocol.hpp"

#define TAG "impl"

namespace axcl::worker {

impl::impl(dispatcher *dispatcher) : base(dispatcher) {
    m_native = std::make_unique<native>(dispatcher);
    m_runtime = std::make_unique<runtime>(dispatcher);
}

void impl::run(const void *data, const size_t& size) {
    const auto hd = static_cast<const axcl::pkg::head *>(data);

    switch (static_cast<axcl::pkg::module>(GET_PACKAGE_MODULE(hd->type))) {
    case axcl::pkg::module::RUNTIME:
        m_runtime->run(data, size);
        break;

    case axcl::pkg::module::NATIVE:
        m_native->run(data, size);
        break;
    default:
        break;
    }
}

}  // namespace axcl::worker
