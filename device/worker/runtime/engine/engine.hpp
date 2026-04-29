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

#include "package/include/runtime/engine/type.hpp"
#include "package/device/runtime/engine/engine_request.hpp"
#include "package/device/runtime/engine/engine_response.hpp"

#include "base.hpp"
#include "runtime_api.hpp"

#include <mutex>
#include <unordered_map>

#define RUNTIME_ENGINE_API_DECL(_API_) \
            RUNTIME_API_DECL(_API_, RUNTIME_API_PARAM(engine))

#define RUNTIME_ENGINE_API_IMPL(_API_) \
            RUNTIME_API_IMPL(axcl::worker::engine, _API_, RUNTIME_API_PARAM(engine))

#define RUNTIME_ENGINE_API_APPEND(_API_) \
            RUNTIME_API_APPEND(engine, _API_, RUNTIME_API_PARAM(engine))

namespace axcl::worker {

class dispatcher;
class engine final: public base {
public:
    explicit engine(dispatcher *dispatcher);
    ~engine() override;

    void run(const void *data, const size_t& size) override;

private:
    RUNTIME_ENGINE_API_DECL(INIT);
    RUNTIME_ENGINE_API_DECL(FINAL);
    RUNTIME_ENGINE_API_DECL(GET_NPU_TYPE);
    RUNTIME_ENGINE_API_DECL(GET_ENGINE_VERSION);
    RUNTIME_ENGINE_API_DECL(LOAD);
    RUNTIME_ENGINE_API_DECL(GET_VERSION);
    RUNTIME_ENGINE_API_DECL(GET_TYPE);
    RUNTIME_ENGINE_API_DECL(GET_USAGE);
    RUNTIME_ENGINE_API_DECL(UNLOAD);
    RUNTIME_ENGINE_API_DECL(CREATE_CONTEXT);
    RUNTIME_ENGINE_API_DECL(GET_INFO);
    RUNTIME_ENGINE_API_DECL(RUN);
    RUNTIME_ENGINE_API_DECL(SET_AFFINITY);
    RUNTIME_ENGINE_API_DECL(GET_AFFINITY);
    RUNTIME_ENGINE_API_DECL(P2P);

    std::mutex m_mutex;
    std::unordered_map<uint64_t, pkg::runtime::engine::meta> m_map;
    runtime_api<pkg::runtime::engine::API, pkg::device::engine_response, pkg::device::engine_request> m_api;
};

}  // namespace axcl::worker
