/**************************************************************************************************
 *
 * Copyright (c) 2019-2025 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "latency.hpp"
#include "axcl_rt.h"
#include "axcl_rt_internal.h"
#include "device/device.hpp"
#include "axcl_logger.hpp"
#include "os.hpp"
#include "axcl_comm.hpp"

#define TAG "latency"

axclError axcl::rt::system::latency::loopback(const void* data, uint32_t size, bool check, int comm_mode) {
    axclError ret;
    m_req.set_latency_data(data, size);

    if (ret = invoke(NO_TIMEOUT, comm_mode); AXCL_SUCC != ret) {
        return ret;
    }

    size_t back_size;
    const void* back_data = m_rsp.get_latency_data(back_size);
    if (back_size != size) {
        LOG_MM_E(TAG, "sent {} bytes, but loopback recv {} bytes", size, back_size);
        return AXCL_ERR_LATENCY_VERIFY_FAIL;
    }

    if (check) {
        if (0 != memcmp(back_data, data, size)) {
            LOG_MM_E(TAG, "loopback check data fail");
            return AXCL_ERR_LATENCY_VERIFY_FAIL;
        }
    }

    return ret;
}

axclError axcl::rt::system::latency::invoke(int32_t timeout, int comm_mode) {

    if (timeout >= 0) {
        timeout += 3000;
    }

    axclError ret;
    axclrtContext context;
    if (ret = axclrtGetCurrentContext(&context); AXCL_SUCC != ret) {
        return ret;
    }

    axclrtStream stream;
    if (ret = axclrtGetDefaultStream(context, &stream); AXCL_SUCC != ret) {
        return ret;
    }

    axclrtEnvironment env;
    if (ret = axclrtGetEnviromentByStream(stream, &env); AXCL_SUCC != ret) {
        return ret;
    }

    pkg::head hd;
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
    hd.type = CREATE_PACKAGE_HEAD_TYPE(axcl::pkg::PROTO, axcl::pkg::module::RUNTIME, axcl::pkg::runtime::module::SYSTEM, 1);

    uint32_t len;
    void* pkg = this->m_req.encode(hd, len);
    if (!pkg) {
        LOG_MM_E(TAG, "encode latency package fail");
        return AXCL_ERR_LATENCY_ENCODE;
    }

    axclrtTask task;
    if (ret = axclrtCreateTask(&task, stream, hd.seq_num, pkg, len, timeout, true, comm_mode); AXCL_SUCC != ret) {
        return ret;
    }

    auto deleter = [](axclrtTask task) { axclrtDestoryTask(task); };
    std::unique_ptr<void, decltype(deleter)> sp(task, deleter);

    if (ret = axclrtWaitTaskFinish(task); AXCL_SUCC != ret) {
        return ret;
    }

    if (ret = axclrtGetTaskResponse(task, &pkg, &len); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.decode(pkg, len)) {
        LOG_MM_E(TAG, "decode latency package fail");
        return AXCL_ERR_LATENCY_DECODE;
    }

    return AXCL_SUCC;
}
