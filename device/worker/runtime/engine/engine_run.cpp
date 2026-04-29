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
#include "axcl_logger.hpp"

#include <ax_engine_api.h>

#define TAG "engine"

static void fill_engine_buffer(const std::vector<axcl::pkg::runtime::engine::buffer>& src, AX_ENGINE_IO_BUFFER_T* dst) {
    for (size_t i = 0; i < src.size(); ++i) {
        dst[i].nSize = src[i].size;
        dst[i].phyAddr = src[i].address;

        LOG_MM_D(TAG, "IO buffer[{}]: address {:#x},  size {}.", i, src[i].address, src[i].size);
    }
}

RUNTIME_ENGINE_API_IMPL(RUN) {
    LOG_MM_D(TAG, "axclrt engine run...");

    std::lock_guard lock(this->m_mutex);
    const auto handle = response->get_param_handle();
    if (0 == this->m_map.count(handle)) {
        request->set_status(-1);
        return -1;
    }

    const auto context = response->get_param_context();
    const auto group = response->get_param_group();
    const auto &[inputs, outputs, batch] = response->get_param_io();
    const auto noresp = response->get_noresp_flag();

    LOG_MM_D(TAG, "Get handle: {:#x}, context: {:#x}, group: {}.", handle, context, group);
    LOG_MM_D(TAG, "Get io: {{input {}, output {}, batch {}}}.", inputs.size(), outputs.size(), batch);

    AX_ENGINE_IO_T io = {};
    io.nBatchSize = batch;

    AX_ENGINE_IO_BUFFER_T input_buffer[inputs.size()]{};
    AX_ENGINE_IO_BUFFER_T output_buffer[outputs.size()]{};
    io.pInputs = input_buffer;
    io.pOutputs = output_buffer;
    io.nInputSize = inputs.size();
    io.nOutputSize = outputs.size();

    fill_engine_buffer(inputs, input_buffer);
    fill_engine_buffer(outputs, output_buffer);

    auto ret = 0;
    if (1 < this->m_map[handle].inputs[0].groups.size()) {
        ret = ::AX_ENGINE_RunGroupIOSync(reinterpret_cast<void *>(handle), reinterpret_cast<void *>(context), group, &io);
    } else {
        ret = ::AX_ENGINE_RunSyncV2(reinterpret_cast<void *>(handle), reinterpret_cast<void *>(context), &io);
    }

    if (0 == noresp) {
        request->set_status(ret);
    }

    return ret;
}
