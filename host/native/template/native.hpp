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

#include <cstdint>
#include <memory>
#include "ax_base_type.h"
#include "axcl_package.hpp"
#include "axcl_rt.h"
#include "axcl_rt_internal.h"
#include "os.hpp"
#include "package/include/native/type.hpp"
#include "package/include/type.hpp"

template <typename REQ, typename RSP>
class native {
public:
    native(axcl::pkg::native::module mod) : m_module(mod) {
    }

protected:
    virtual AX_S32 invoke(int32_t timeout, axclrtEnvironment* rt_env = nullptr) {
        /**
         * fixme
         * The HOST timeout includes the command request (1) + NATIVE API execution (2) + receiving the reply (3).
         * Therefore, the HOST will timeout and return before the NATIVE API timeout, that is, during (1) + (2). At this point, the reply packet from the sub-card on the PCIe link becomes an unclaimed "dirty" packet.
         * The dirty packet data will be released directly during Channel Dispatch without being claimed by anyone, and will not cause dirty memory.
         * WR:
         * The HOST timeout is increased by 3 seconds compared to the original user-configured timeout.
         * Side effects:
         *  In the case of a normal NATIVE SDK timeout: the user-configured timeout does not match the expected timeout, differing by the time taken for (1) + (3).
         *  In the case of NATIVE SDK blocking or abnormal timeout handling, the user-configured timeout and expected timeout differ by 3 seconds.
         *  After the NATIVE SDK timeout, there may be dirty data from receiving the reply (3) still present in the link.
        */
        if (timeout >= 0) {
            timeout += 3000;
        }

        AX_S32 ret;
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

        if (rt_env) {
            *rt_env = env;
        }

        const auto api = m_req.get_api();
        const auto token = axclrtGetToken();
        axcl::pkg::head hd;
        hd.seq_num = token;
        hd.device = env.device;
        hd.context = env.context;
        hd.stream = env.stream;
        hd.type = CREATE_PACKAGE_HEAD_TYPE(axcl::pkg::PROTO, axcl::pkg::module::NATIVE, m_module, api);
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

        uint32_t len;
        void *pkg = m_req.encode(hd, len);
        if (!pkg) {
            return AXCL_DEF_NATIVE_ERR(static_cast<uint8_t>(m_module), AXCL_ERR_ENCODE);
        }

        axclrtTask task;
        if (ret = axclrtCreateTask(&task, stream, token, pkg, len, timeout, true, -1); AXCL_SUCC != ret) {
            return ret;
        }

        /* make a smart pointer to manage the task life cycle */
        auto deleter = [](axclrtTask task) { axclrtDestoryTask(task); };
        std::unique_ptr<void, decltype(deleter)> sp(task, deleter);

        if (ret = axclrtWaitTaskFinish(task); AXCL_SUCC != ret) {
            return ret;
        }

        if (ret = axclrtGetTaskResponse(task, &pkg, &len); AXCL_SUCC != ret) {
            return ret;
        }

        if (!m_rsp.decode(pkg, len)) {
            return AXCL_DEF_NATIVE_ERR(static_cast<uint8_t>(m_module), AXCL_ERR_DECODE);
        }

        return AXCL_SUCC;
    }

protected:
    REQ m_req;
    RSP m_rsp;
    axcl::pkg::native::module m_module;
};
