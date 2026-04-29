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

#include "axcl_rt_internal.h"
#include "device/device.hpp"
#include "engine_strings.hpp"
#include "axcl_logger.hpp"
#include "os.hpp"

axclError axcl::rt::engine::get_runtime_environment(axclrtStream *stream, axclrtEnvironment &env) {
    axclError ret;

    if (*stream == nullptr) {
        axclrtContext context;
        ret = axclrtGetCurrentContext(&context);
        if (AXCL_SUCC != ret) {
            return ret;
        }

        axclrtStream defaultStream;
        ret = axclrtGetDefaultStream(context, &defaultStream);
        if (AXCL_SUCC != ret) {
            return ret;
        }

        *stream = defaultStream;
    }

    ret = axclrtGetEnviromentByStream(*stream, &env);
    if (AXCL_SUCC != ret) {
        return ret;
    }

    return AXCL_SUCC;
}

axclError axcl::rt::engine::invoke(axclrtStream stream, const axclrtEnvironment &env, const int32_t timeout, bool has_resp) const {
    pkg::head hd{};
    hd.seq_num = axclrtGetToken();
    hd.device = env.device;
    hd.context = env.context;
    hd.stream = env.stream;
    hd.command = 0;

    static uint32_t pid = 0;
    if (0 == pid) {
        pid = static_cast<uint32_t>(getpid());
    }
    static thread_local uint32_t tid = 0;
    if (0 == tid) {
        tid = gettid();
    }
    m_req.set_env({pid, tid});

    const auto api = this->m_req.get_api();
    hd.type = CREATE_PACKAGE_HEAD_TYPE(axcl::pkg::PROTO, axcl::pkg::module::RUNTIME, axcl::pkg::runtime::module::ENGINE, api);

    uint32_t len;
    void *pkg = this->m_req.encode(hd, len);
    if (!pkg) {
        LOG_MM_E(TAG, ENGINE_ERR_ENCODE_FAIL, static_cast<int32_t>(api));
        return AXCL_ERR_ENGINE_ENCODE;
    }

    axclError ret;
    axclrtTask task;
    ret = axclrtCreateTask(&task, stream, hd.seq_num, pkg, len, timeout, has_resp, comm::PCIE_MSG);
    if (AXCL_SUCC != ret) {
        return ret;
    }

    auto deleter = [](axclrtTask task) { axclrtDestoryTask(task); };
    std::unique_ptr<void, decltype(deleter)> sp(task, deleter);

    ret = axclrtWaitTaskFinish(task);
    if (AXCL_SUCC != ret) {
        return ret;
    }

    if (has_resp) {

        ret = axclrtGetTaskResponse(task, &pkg, &len);
        if (AXCL_SUCC != ret) {
            return ret;
        }

        if (!this->m_rsp.decode(pkg, len)) {
            LOG_MM_E(TAG, ENGINE_ERR_DECODE_FAIL, static_cast<int32_t>(api));
            return AXCL_ERR_ENGINE_DECODE;
        }

        int32_t status = this->m_rsp.get_status();
        if (0 != status) {
            LOG_MM_E(TAG, ENGINE_ERR_REQUEST_FAIL, static_cast<int32_t>(api), status);
            return AXCL_ERR_ENGINE_EXECUTE_FAIL;
        }
    }

    return AXCL_SUCC;
}
