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

#include <mutex>
#include <unordered_map>

#include "package/device/runtime/p2p/p2p_request.hpp"
#include "package/device/runtime/p2p/p2p_response.hpp"
#include "ax_sys_api.h"
#include "base.hpp"
#include "runtime_api.hpp"

#define RUNTIME_P2P_API_DECL(_API_) \
            RUNTIME_API_DECL(_API_, RUNTIME_API_PARAM(p2p))

#define RUNTIME_P2P_API_IMPL(_API_) \
            RUNTIME_API_IMPL(p2p, _API_, RUNTIME_API_PARAM(p2p))

#define RUNTIME_P2P_API_APPEND(_API_) \
            RUNTIME_API_APPEND(p2p, _API_, RUNTIME_API_PARAM(p2p))

namespace axcl::worker {

class dispatcher;
class p2p : public base {
public:
    p2p(dispatcher *dispatcher);
    ~p2p() = default;

    virtual void run(const void *data, const size_t& size) override;

private:
    RUNTIME_P2P_API_DECL(P2P_SETUP_CONFIG);
    RUNTIME_P2P_API_DECL(P2P_START_BANDWIDTH_TEST);
    RUNTIME_P2P_API_DECL(P2P_REMOVE_CONFIG);

private:
    std::mutex m_mutex;
    std::unordered_map<uint64_t, void *> m_map;
    runtime_api<axcl::pkg::runtime::p2p::API, axcl::pkg::device::p2p_response, axcl::pkg::device::p2p_request> m_api;
};

}  // namespace axcl::worker
