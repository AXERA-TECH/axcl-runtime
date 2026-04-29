/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "runtime.hpp"
#include "dispatcher.hpp"
#include <chrono>
#include <string>
#include "axcl_logger.hpp"
#include "protocol.hpp"

#define TAG "runtime"

namespace axcl::worker {

runtime::runtime(dispatcher *dispatcher) : base(dispatcher) {
    m_system = std::make_unique<system>(dispatcher);
    m_memory = std::make_unique<memory>(dispatcher);
    m_engine = std::make_unique<engine>(dispatcher);
    m_p2p = std::make_unique<p2p>(dispatcher);
    m_usrwork = std::make_unique<usrwork>(dispatcher);
}

void runtime::run(const void *data, const size_t& size) {
    switch (const auto hd = static_cast<const pkg::head *>(data); static_cast<axcl::pkg::runtime::module>(GET_PACKAGE_SUB_MODULE(hd->type))) {
    case axcl::pkg::runtime::module::SYSTEM:
        m_system->run(data, size);
        break;
    case axcl::pkg::runtime::module::MEMORY:
        m_memory->run(data, size);
        break;
    case axcl::pkg::runtime::module::ENGINE:
        m_engine->run(data, size);
        break;
    case axcl::pkg::runtime::module::P2P:
        m_p2p->run(data, size);
        break;
    case axcl::pkg::runtime::module::USRWORK:
        m_usrwork->run(data, size);
        break;
    default:
        break;
    }
}

}  // namespace axcl::worker
