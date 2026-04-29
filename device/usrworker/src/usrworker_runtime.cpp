/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "axcl_worker_runtime.h"
#include "usrworker.h"
#include "usrworker_def.h"
#include "axcl_logger.hpp"
#include <cstdint>
#include <memory>
#include <atomic>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <algorithm>
#include <functional>

#define USRWORKER_API extern "C" __attribute__((visibility("default")))

namespace axcl::usrworker {

static std::unique_ptr<::usrworker> g_usrworker = nullptr;
static std::atomic<bool> g_initialized{false};
static axclrtWorkerStatusCallback g_status_cb = nullptr;

__attribute__((destructor))
void cleanup() {
    if (g_initialized.load()) {
        axclrtWorkerDeInit();
    }
}

USRWORKER_API int32_t axclrtWorkerInit() {
    if (g_initialized.load()) {
        return 0;
    }

    try {
        g_usrworker = std::make_unique<::usrworker>();
        if (!g_usrworker) {
            return -1;
        }

        if (g_usrworker->init() != 0) {
            g_usrworker.reset();
            return -1;
        }

        g_initialized.store(true);
        return 0;

    } catch (const std::exception& e) {
        g_usrworker.reset();
        return -1;
    }
}

USRWORKER_API int32_t axclrtWorkerDeInit() {
    if (!g_initialized.load()) {
        return 0;
    }

    try {
        if (g_usrworker) {
            g_usrworker->deinit();
            g_usrworker.reset();
        }

        g_initialized.store(false);
        return 0;
    } catch (const std::exception& e) {
        return -1;
    }
}

USRWORKER_API int32_t axclrtWorkerSend(const void *buf, uint32_t size, int32_t timeout) {
    if (!g_initialized.load()) {
        return -1;
    }

    if (!buf || size == 0) {
        return -1;
    }

    if (timeout < 0) {
        return -1;
    }

    if (!g_usrworker) {
        return -1;
    }

    return g_usrworker->send_data(buf, size, timeout);
}

USRWORKER_API int32_t axclrtWorkerRecv(void *buf, uint32_t buf_size, uint32_t* recvlen, int32_t timeout) {
    if (!g_initialized.load()) {
        return -1;
    }

    if (!buf || buf_size == 0) {
        return -1;
    }

    if (timeout < 0) {
        return -1;
    }

    if (!g_usrworker) {
        return -1;
    }

    return g_usrworker->recv_data(buf, buf_size, recvlen, timeout);
}

USRWORKER_API void axclrtWorkerSetStatusCallback(axclrtWorkerStatusCallback cb) {
    g_status_cb = cb;
    if (g_usrworker) {
        g_usrworker->set_status_callback([](AXCL_WORKER_STATUS_E status) {
            if (g_status_cb) g_status_cb(status);
        });
    }
}

} // namespace axcl::usrworker