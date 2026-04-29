/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include <ctime>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <iomanip>

#include "latency.hpp"
#include "dispatcher.hpp"
#include "axcl_logger.hpp"

#define TAG "latency"

namespace axcl::worker {

latency::latency(dispatcher *dispatcher) : base(dispatcher) {
}

void latency::run(const void *data, const size_t& size) {
    void *msg = nullptr;
    uint32_t msg_len = 0;

    size_t data_size = 0;
    axcl::pkg::device::latency_response response;
    axcl::pkg::device::latency_request request;

    if (response.decode(data, size)) {
        const void* data  = response.get_latency_data(data_size);
        request.set_latency_data(data, data_size);
    } else {
        LOG_MM_E(TAG, "latency::run {} bytes, but decode failed", size);
        void* data = nullptr;
        request.set_latency_data(data, data_size);
    }

    {
        const auto hd = static_cast<const axcl::pkg::head *>(data);
        msg = request.encode(*hd, msg_len);
    }

    if (msg && msg_len) {
        get_dispatcher()->ack(msg, msg_len, -1, -1);
    }
}

}  // namespace axcl::worker
