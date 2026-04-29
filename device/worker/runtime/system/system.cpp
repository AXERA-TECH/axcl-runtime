/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "system.hpp"
#include "dispatcher.hpp"
#include <chrono>
#include <string>
#include "axcl_logger.hpp"
#include "protocol.hpp"

#define TAG "system"

namespace axcl::worker {

system::system(dispatcher *dispatcher) : base(dispatcher) {
    m_log = std::make_unique<log>(dispatcher);
    m_control = std::make_unique<control>(dispatcher);
    m_latency = std::make_unique<latency>(dispatcher);
}

void system::run(const void *data, const size_t& size) {
    const auto hd = static_cast<const pkg::head *>(data);
    switch (static_cast<axcl::pkg::runtime::system::module>(GET_PACKAGE_SUB_MODULE_API(hd->type))) {
    case axcl::pkg::runtime::system::module::ALIVE:
        LOG_MM_E(TAG, "axcl::pkg::runtime::system::module::ALIVE unreachable here.");
        break;
    case axcl::pkg::runtime::system::module::LATENCY:
        m_latency->run(data, size);
        break;
    case axcl::pkg::runtime::system::module::LOG:
        m_log->run(data, size);
        break;
    case axcl::pkg::runtime::system::module::CONTROL:
        m_control->run(data, size);
        break;
    default:
        break;
    }
}

}  // namespace axcl::worker
