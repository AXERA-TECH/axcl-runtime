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

#include "engine_p2p_task.hpp"
#include "axcl_logger.hpp"
#include "timer.hpp"

#define TAG "engine"

constexpr uint32_t max_test_count = 10;

static std::string get_elapsed_string(const std::vector<float>& elapsed) {
    std::string str;
    for (size_t i = 0; i < elapsed.size() - 1; i++) {
        str += std::to_string(elapsed[i]) + ",";
    }
    str += std::to_string(elapsed[elapsed.size() - 1 - 1]);
    return str;
}

RUNTIME_ENGINE_API_IMPL(P2P) {
    LOG_MM_D(TAG, "axclrt engine p2p...");

    std::lock_guard lock(this->m_mutex);

    task_router router;
    auto status = router.init();
    if (!status) {
        LOG_MM_E(TAG, "task router init failed.");
        request->set_status(-1);
        return -1;
    }

    const auto group = response->get_param_group();
    const auto size = response->get_param_size();

    std::vector elapsed(max_test_count, 0.f);

    timer timer;
    if (0 != (group & 1)) {
        status = router.set_payload(size, false) && router.launch_parallel_task(elapsed);
        const std::string elapsed_string = get_elapsed_string(elapsed);
        LOG_MM_D(TAG, "p2p parallel task(block: {}) elapsed: ({}) ms.", false, elapsed_string);
    }
    if (0 != (group & 2)) {
        status = router.set_payload(size, true) && router.launch_parallel_task(elapsed);
        const std::string elapsed_string = get_elapsed_string(elapsed);
        LOG_MM_D(TAG, "p2p parallel task(block: {}) elapsed: ({}) ms.", true, elapsed_string);
    }

    if (0 != (group & 4)) {
        status = router.set_payload(size, false) && router.launch_sequential_task(elapsed);
        const std::string elapsed_string = get_elapsed_string(elapsed);
        LOG_MM_D(TAG, "p2p sequential task(block: {}) elapsed: ({}) ms.", false, elapsed_string);
    }
    if (0 != (group & 8)) {
        status = router.set_payload(size, true) && router.launch_sequential_task(elapsed);
        const std::string elapsed_string = get_elapsed_string(elapsed);
        LOG_MM_D(TAG, "p2p sequential task(block: {}) elapsed: ({}) ms.", true, elapsed_string);
    }

    if (0 != (group & 16)) {
        status = router.set_payload(size, false) && router.launch_interlace_task(elapsed);
        const std::string elapsed_string = get_elapsed_string(elapsed);
        LOG_MM_D(TAG, "p2p interlace task(block: {}) elapsed: ({}) ms.", false, elapsed_string);
    }
    if (0 != (group & 32)) {
        status = router.set_payload(size, true) && router.launch_interlace_task(elapsed);
        const std::string elapsed_string = get_elapsed_string(elapsed);
        LOG_MM_D(TAG, "p2p interlace task(block: {}) elapsed: ({}) ms.", true, elapsed_string);
    }

    request->set_param_elapsed(elapsed);
    LOG_MM_D(TAG, "p2p send & recv done, total elapsed: {} ms.", timer.elapsed());

    if (status) {
        status &= router.verify_buffer();
    }

    request->set_status(status ? 0 : -1);
    LOG_MM_D(TAG, "main thread exit: {}.", status);
    return status ? 0 : -1;
}
