/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "engine.hpp"

#include "dispatcher.hpp"
#include "axcl_logger.hpp"

#include <ax_engine_api.h>

#define TAG "engine"

template<>
bool axcl::worker::runtime_api<axcl::pkg::runtime::engine::API,
                axcl::pkg::device::engine_response,
                axcl::pkg::device::engine_request>::handle_ack_err(
    const axcl::pkg::device::engine_response& decoder, int32_t ret) {

    auto noresp_flag = decoder.get_noresp_flag();
    if (noresp_flag == 0) {
        return false;
    }

    auto api = decoder.get_api();
    if (ret != 0) {
        throw RuntimeApiException(ret, m_module, static_cast<int32_t>(api));
    }

    return true;
}

axcl::worker::engine::engine(dispatcher *dispatcher) : base(dispatcher), m_api(TAG) {
    LOG_MM_D(TAG, "axclrt engine constructed...");
    m_api.append({
        RUNTIME_ENGINE_API_APPEND(INIT),
        RUNTIME_ENGINE_API_APPEND(FINAL),
        RUNTIME_ENGINE_API_APPEND(GET_NPU_TYPE),
        RUNTIME_ENGINE_API_APPEND(GET_ENGINE_VERSION),
        RUNTIME_ENGINE_API_APPEND(LOAD),
        RUNTIME_ENGINE_API_APPEND(GET_VERSION),
        RUNTIME_ENGINE_API_APPEND(GET_TYPE),
        RUNTIME_ENGINE_API_APPEND(GET_USAGE),
        RUNTIME_ENGINE_API_APPEND(UNLOAD),
        RUNTIME_ENGINE_API_APPEND(CREATE_CONTEXT),
        RUNTIME_ENGINE_API_APPEND(GET_INFO),
        RUNTIME_ENGINE_API_APPEND(RUN),
        RUNTIME_ENGINE_API_APPEND(SET_AFFINITY),
        RUNTIME_ENGINE_API_APPEND(GET_AFFINITY),
        RUNTIME_ENGINE_API_APPEND(P2P),
    });
}

axcl::worker::engine::~engine() {
    LOG_MM_D(TAG, "axclrt engine destroyed...");

    for (auto& [handle, meta] : m_map) {
        AX_ENGINE_DestroyHandle(reinterpret_cast<void *>(handle));
    }
}

void axcl::worker::engine::run(const void *data, const size_t& size) {
    LOG_MM_D(TAG, "axclrt engine workload dispatching...");

    m_api.run(get_dispatcher(), data, size);
}
