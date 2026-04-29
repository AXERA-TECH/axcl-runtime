/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "control.hpp"
#include <string.h>
#include "axcl_rt.h"
#include "axcl_rt_internal.h"
#include "context/context.hpp"
#include "stream/stream.hpp"
#include "stream/stream_manager.hpp"
#include "device/device.hpp"
#include "device/device_manager.hpp"
#include "axcl_logger.hpp"
#include "os.hpp"

#define TAG "control"

axclError axcl::rt::system::control::create_context(int32_t context_id) {
    axclrtStream stream;
    axclrtEnvironment env;
    axclError ret;

    if (ret = get_runtime_environment(stream, env); AXCL_SUCC != ret) {
        return ret;
    }

    if (env.context != context_id) {
        LOG_MM_E(TAG, "current runtime enviroment context id {} is not equal to {}", env.context, context_id);
        return AXCL_ERR_SYSCTRL_CHECK_CONTEXT_ID;
    }

    auto pid = getpid();
    auto tid = gettid();
    auto stream_id = static_cast<axcl::rt::stream*>(stream)->get_stream_id();

    m_req.set_env({static_cast<uint32_t>(pid), tid});
    m_req.create_context(pid, tid, context_id, stream_id);

    if (ret = invoke(stream, env, static_cast<uint32_t>(axcl::pkg::runtime::system::command::CREATE_CONTEXT), NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    auto slave_pid = m_rsp.get_pid();
    auto slave_tid = m_rsp.get_tid();

    LOG_MM_I(TAG, "context({}) created: host pid({}), tid({})<=====>slave pid({}), tid({})", context_id, pid, tid, slave_pid, slave_tid);

    return AXCL_SUCC;
}

axclError axcl::rt::system::control::destroy_context(int32_t context_id) {
    axclrtStream stream;
    axclrtEnvironment env;
    axclError ret;

    if (ret = get_runtime_environment(stream, env); AXCL_SUCC != ret) {
        return ret;
    }

    if (env.context != context_id) {
        LOG_MM_E(TAG, "current runtime enviroment context id {} is not equal to {}", env.context, context_id);
        return AXCL_ERR_SYSCTRL_CHECK_CONTEXT_ID;
    }

    auto pid = getpid();
    auto tid = gettid();
    auto stream_id = static_cast<axcl::rt::stream*>(stream)->get_stream_id();

    m_req.set_env({static_cast<uint32_t>(pid), tid});
    m_req.destroy_context(pid, tid, context_id, stream_id);

    if (ret = invoke(stream, env, static_cast<uint32_t>(axcl::pkg::runtime::system::command::DESTROY_CONTEXT), NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    auto slave_pid = m_rsp.get_pid();
    auto slave_tid = m_rsp.get_tid();

    LOG_MM_I(TAG, "context({}) destroied: host pid({}), tid({})<=====>slave pid({}), tid({})", context_id, pid, tid, slave_pid, slave_tid);

    return AXCL_SUCC;
}

axclError axcl::rt::system::control::create_stream(int32_t context_id, axclrtStream stream) {
    axclError ret;

    axclrtEnvironment env;
    if (ret = axclrtGetEnviromentByStream(stream, &env); AXCL_SUCC != ret) {
        return ret;
    }

    if (env.context != context_id) {
        LOG_MM_E(TAG, "current runtime enviroment context id {} is not equal to {}", env.context, context_id);
        return AXCL_ERR_SYSCTRL_CHECK_CONTEXT_ID;
    }

    auto pid = getpid();
    auto tid = gettid();
    auto stream_id = static_cast<axcl::rt::stream*>(stream)->get_stream_id();

    m_req.set_env({static_cast<uint32_t>(pid), tid});
    m_req.create_stream(context_id, stream_id);

    if (ret = invoke(stream, env, static_cast<uint32_t>(axcl::pkg::runtime::system::command::CREATE_STREAM), NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    return AXCL_SUCC;
}

axclError axcl::rt::system::control::destroy_stream(int32_t context_id, axclrtStream stream) {

    axclError ret;

    axclrtEnvironment env;
    if (ret = axclrtGetEnviromentByStream(stream, &env); AXCL_SUCC != ret) {
        return ret;
    }

    if (env.context != context_id) {
        LOG_MM_E(TAG, "current runtime enviroment context id {} is not equal to {}", env.context, context_id);
        return AXCL_ERR_SYSCTRL_CHECK_CONTEXT_ID;
    }

    auto pid = getpid();
    auto tid = gettid();
    auto stream_id = static_cast<axcl::rt::stream*>(stream)->get_stream_id();

    m_req.set_env({static_cast<uint32_t>(pid), tid});
    m_req.destroy_stream(context_id, stream_id);

    if (ret = invoke(stream, env, static_cast<uint32_t>(axcl::pkg::runtime::system::command::DESTROY_STREAM), NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    return AXCL_SUCC;
}

axclError axcl::rt::system::control::sync_stream(int32_t context_id, axclrtStream stream, int32_t timeout_ms) {

    axclError ret;

    axclrtEnvironment env;
    if (ret = axclrtGetEnviromentByStream(stream, &env); AXCL_SUCC != ret) {
        return ret;
    }

    if (env.context != context_id) {
        LOG_MM_E(TAG, "current runtime enviroment context id {} is not equal to {}", env.context, context_id);
        return AXCL_ERR_SYSCTRL_CHECK_CONTEXT_ID;
    }

    auto pid = getpid();
    auto tid = gettid();
    auto stream_id = static_cast<axcl::rt::stream*>(stream)->get_stream_id();

    m_req.set_env({static_cast<uint32_t>(pid), tid});
    m_req.sync_stream(context_id, stream_id, timeout_ms);

    ret = invoke(stream, env, static_cast<uint32_t>(axcl::pkg::runtime::system::command::SYNC_STREAM), timeout_ms);
    if (AXCL_SUCC != ret) {
        LOG_MM_E(TAG, "invoke failed, context={} stream={} ret={:#x}", context_id, stream_id, static_cast<uint32_t>(ret));
        return ret;
    }

    ret = m_rsp.get_status();
    return ret;
}

axclError axcl::rt::system::control::create_event(int32_t event_id) {

    axclError ret;

    axclrtContext context;
    axclrtStream stream;
    axclrtEnvironment env;

    if (ret = axclrtGetCurrentContext(&context); AXCL_SUCC != ret) {
        return ret;
    }

    if (ret = axclrtGetDefaultStream(context, &stream); AXCL_SUCC != ret) {
        return ret;
    }

    if (ret = axclrtGetEnviromentByStream(stream, &env); AXCL_SUCC != ret) {
        return ret;
    }

    auto pid = getpid();
    auto tid = gettid();
    auto stream_id = static_cast<axcl::rt::stream*>(stream)->get_stream_id();
    auto context_id = static_cast<axcl::rt::context*>(context)->get_context_id();

    m_req.set_env({static_cast<uint32_t>(pid), tid});
    m_req.create_event(context_id, stream_id, event_id);

    if (ret = invoke(stream, env, static_cast<uint32_t>(axcl::pkg::runtime::system::command::CREATE_EVENT), NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    return AXCL_SUCC;
}

axclError axcl::rt::system::control::destroy_event(int32_t event_id) {
    axclError ret;

    axclrtContext context;
    axclrtStream stream;
    axclrtEnvironment env;

    if (ret = axclrtGetCurrentContext(&context); AXCL_SUCC != ret) {
        return ret;
    }

    if (ret = axclrtGetDefaultStream(context, &stream); AXCL_SUCC != ret) {
        return ret;
    }

    if (ret = axclrtGetEnviromentByStream(stream, &env); AXCL_SUCC != ret) {
        return ret;
    }

    auto pid = getpid();
    auto tid = gettid();
    auto stream_id = static_cast<axcl::rt::stream*>(stream)->get_stream_id();
    auto context_id = static_cast<axcl::rt::context*>(context)->get_context_id();

    m_req.set_env({static_cast<uint32_t>(pid), tid});
    m_req.destroy_event(context_id, stream_id, event_id);

    if (ret = invoke(stream, env, static_cast<uint32_t>(axcl::pkg::runtime::system::command::DESTROY_EVENT), NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    return AXCL_SUCC;
}

axclError axcl::rt::system::control::record_event(axclrtStream stream, int32_t event_id) {
    axclError ret;

    axclrtEnvironment env;
    if (ret = axclrtGetEnviromentByStream(stream, &env); AXCL_SUCC != ret) {
        return ret;
    }

    auto pid = getpid();
    auto tid = gettid();
    auto stream_id = static_cast<axcl::rt::stream*>(stream)->get_stream_id();
    auto context_id = static_cast<axcl::rt::stream*>(stream)->get_stream_manager()->get_context()->get_context_id();

    m_req.set_env({static_cast<uint32_t>(pid), tid});
    m_req.record_event(context_id, stream_id, event_id);

    if (ret = invoke(stream, env, static_cast<uint32_t>(axcl::pkg::runtime::system::command::RECORD_EVENT), NO_TIMEOUT, false); AXCL_SUCC != ret) {
        return ret;
    }

    return AXCL_SUCC;
}

axclError axcl::rt::system::control::stream_wait_event(axclrtStream stream, int32_t event_id, int32_t timeout_ms) {
    axclError ret;

    axclrtEnvironment env;
    if (ret = axclrtGetEnviromentByStream(stream, &env); AXCL_SUCC != ret) {
        return ret;
    }

    auto pid = getpid();
    auto tid = gettid();
    auto stream_id = static_cast<axcl::rt::stream*>(stream)->get_stream_id();
    auto context_id = static_cast<axcl::rt::stream*>(stream)->get_stream_manager()->get_context()->get_context_id();

    m_req.set_env({static_cast<uint32_t>(pid), tid});
    m_req.wait_event(context_id, stream_id, event_id, timeout_ms);

    if (ret = invoke(stream, env, static_cast<uint32_t>(axcl::pkg::runtime::system::command::WAIT_EVENT), NO_TIMEOUT, false); AXCL_SUCC != ret) {
        return ret;
    }

    return AXCL_SUCC;
}

axclError axcl::rt::system::control::execute_shell_cmd(const char *cmd, const char *const args[], size_t argc, const char **output, int32_t timeout) {
    if (!cmd) {
        LOG_MM_E(TAG, "nil cmd");
        return AXCL_ERR_SYSCTRL_NULL_POINTER;
    }

    axclrtStream stream;
    axclrtEnvironment env;
    axclError ret;

    if (ret = get_runtime_environment(stream, env); AXCL_SUCC != ret) {
        return ret;
    }

    m_req.set_shell_cmd(cmd, args, argc);

    if (ret = invoke(stream, env, static_cast<uint32_t>(axcl::pkg::runtime::system::command::SHELL), timeout, output ? true : false); AXCL_SUCC != ret) {
        return ret;
    }

    if (output) {
        if (int32_t status = m_rsp.get_shell_cmd_status(); 0 != status) {
            LOG_MM_E(TAG, "execute shell cmd {}  fail, status = {}", cmd, status);
            return AXCL_ERR_SYSCTRL_EXECUTE_FAIL;
        }

        const std::string &s = m_rsp.get_shell_cmd_output();
        if (!s.empty() && output) {
            LOG_MM_I(TAG, "{}", s);

            thread_local std::string v;
            v = s;
            *output = v.data();
        }
    }

    return AXCL_SUCC;
}

axclError axcl::rt::system::control::change_pcie_sub_id(uint32_t sub_vendor_id, uint32_t sub_device_id) {
    axclrtStream stream;
    axclrtEnvironment env;
    axclError ret;

    if (ret = get_runtime_environment(stream, env); AXCL_SUCC != ret) {
        return ret;
    }

    m_req.set_pcie_sub_id(sub_vendor_id, sub_device_id);

    constexpr int32_t timeout = 5000;
    if (ret = invoke(stream, env, static_cast<uint32_t>(axcl::pkg::runtime::system::command::SETUP_PCIE_SUB_ID), timeout); AXCL_SUCC != ret) {
        return ret;
    }

    if (int32_t status = m_rsp.get_pcie_sub_id_setup_status(); 0 != status) {
        LOG_MM_E(TAG, "change pcie EP sub id fail, status = {}", status);
        return AXCL_ERR_SYSCTRL_EXECUTE_FAIL;
    }

    LOG_MM_I(TAG, "change EP sub vendor id = {}, sub device id = {} success", sub_vendor_id, sub_device_id);
    return AXCL_SUCC;
}

axclError axcl::rt::system::control::get_runtime_environment(axclrtStream &stream, axclrtEnvironment &env) {
    axclError ret;
    axclrtContext context;
    if (ret = axclrtGetCurrentContext(&context); AXCL_SUCC != ret) {
        return ret;
    }

    if (ret = axclrtGetDefaultStream(context, &stream); AXCL_SUCC != ret) {
        return ret;
    }

    if (ret = axclrtGetEnviromentByStream(stream, &env); AXCL_SUCC != ret) {
        return ret;
    }

    return AXCL_SUCC;
}

axclError axcl::rt::system::control::invoke(axclrtStream stream, const axclrtEnvironment &env, uint32_t control_cmd, int32_t timeout, bool has_resp) {
    pkg::head hd{};
    hd.seq_num = axclrtGetToken();
    hd.device = env.device;
    hd.context = env.context;
    hd.stream = env.stream;
    hd.command = control_cmd;

    auto module = m_req.get_module();
    auto cmd = m_req.get_cmd();

    if (0 == m_req.get_env().pid || 0 == m_req.get_env().tid) {
        static uint32_t pid = 0;
        if (0 == pid) {
            pid = static_cast<uint32_t>(getpid());
        }

        static thread_local uint32_t tid = 0;
        if (0 == tid) {
            tid = gettid();
        }

        m_req.set_env({pid, tid});
    }

    hd.type = CREATE_PACKAGE_HEAD_TYPE(axcl::pkg::PROTO, axcl::pkg::module::RUNTIME, axcl::pkg::runtime::module::SYSTEM, module);

    uint32_t len;
    void *pkg = this->m_req.encode(hd, len);
    if (!pkg) {
        LOG_MM_E(TAG, "encode cmd {} fail", static_cast<int32_t>(cmd));
        return AXCL_ERR_SYSCTRL_ENCODE;
    }

    axclError ret;
    axclrtTask task;
    if (ret = axclrtCreateTask(&task, stream, hd.seq_num, pkg, len, timeout, has_resp, comm::PCIE_MSG); AXCL_SUCC != ret) {
        return ret;
    }

    auto deleter = [](axclrtTask task) { axclrtDestoryTask(task); };
    std::unique_ptr<void, decltype(deleter)> sp(task, deleter);

    if (ret = axclrtWaitTaskFinish(task); AXCL_SUCC != ret) {
        return ret;
    }

    if (has_resp) {
        if (ret = axclrtGetTaskResponse(task, &pkg, &len); AXCL_SUCC != ret) {
            return ret;
        }

        if (!this->m_rsp.decode(pkg, len)) {
            LOG_MM_E(TAG, "decode cmd {} fail", static_cast<int32_t>(cmd));
            return AXCL_ERR_SYSCTRL_DECODE;
        }

        if (m_rsp.get_cmd() != cmd) {
            LOG_MM_E(TAG, "response cmd {} is not equal to request cmd {}", static_cast<int32_t>(m_rsp.get_cmd()), static_cast<int32_t>(cmd));
            return AXCL_ERR_SYSCTRL_UNEXPECT_RESPONSE;
        }
    }

    return AXCL_SUCC;
}
