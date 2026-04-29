/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "log.hpp"
#include <string.h>
#include <filesystem>
#include <fstream>
#include "axcl_rt.h"
#include "axcl_rt_internal.h"
#include "device/device.hpp"
#include "device/device_manager.hpp"
#include "axcl_logger.hpp"
#include "os.hpp"

#define TAG "log"

axclError axcl::rt::system::log::dump(axclrtLogKind kind, axclrtFlagKind flag, const char *output_path) {
    axclrtStream stream;
    axclrtEnvironment env;
    axclError ret;

    if (!std::filesystem::exists(output_path)) {
        try {
            std::filesystem::create_directories(output_path);
        }

        catch (const std::filesystem::filesystem_error &e) {
            LOG_MM_E(TAG, "create {} failed, {}", output_path, e.what());
            return AXCL_ERR_LOG_DUMP;
        }
    }

    if (ret = get_runtime_environment(stream, env); AXCL_SUCC != ret) {
        return ret;
    }

    m_req.set_type(kind);
    m_req.set_flag(flag);

    if (ret = invoke(stream, env, NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    std::string path = "./";

    if (output_path) {
        path = output_path;

        if (path.back() != '/') {
            path += "/";
        }
    }

    // post handler
    {
        std::string log_name = m_rsp.get_name();
        std::string log_content = m_rsp.get_content();

        path += "dev" + std::to_string(env.device) + "_";
        path += log_name;

        std::ofstream file(path, std::ios::binary);
        if (file.is_open()) {
            file.write(log_content.data(), log_content.size());
            file.close();
            LOG_MM_C(TAG, "log dump finished: {}", path.c_str());
        } else {
            LOG_MM_E(TAG, "log dump failed: {}, type: {}, flag: {}", path.c_str(), static_cast<int32_t>(kind), static_cast<int32_t>(flag));
        }
    }

    return AXCL_SUCC;
}

axclError axcl::rt::system::log::get_runtime_environment(axclrtStream &stream, axclrtEnvironment &env) {
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

axclError axcl::rt::system::log::invoke(axclrtStream stream, const axclrtEnvironment &env, int32_t timeout) {
    pkg::head hd{};
    hd.seq_num = axclrtGetToken();
    hd.device = env.device;
    hd.context = env.context;
    hd.stream = env.stream;
    hd.command = 0;

    m_req.set_env({static_cast<uint32_t>(getpid()), gettid()});

    auto module = m_req.get_module();

    hd.type = CREATE_PACKAGE_HEAD_TYPE(axcl::pkg::PROTO, axcl::pkg::module::RUNTIME, axcl::pkg::runtime::module::SYSTEM, module);

    uint32_t len;
    void *pkg = this->m_req.encode(hd, len);
    if (!pkg) {
        LOG_MM_E(TAG, "encode log fail");
        return AXCL_ERR_LOG_DUMP;
    }

    axclError ret;
    axclrtTask task;
    if (ret = axclrtCreateTask(&task, stream, hd.seq_num, pkg, len, timeout, true, comm::PCIE_MSG); AXCL_SUCC != ret) {
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

    if (!this->m_rsp.decode(pkg, len)) {
        LOG_MM_E(TAG, "decode log fail");
        return AXCL_ERR_LOG_DUMP;
    }

    return AXCL_SUCC;
}
