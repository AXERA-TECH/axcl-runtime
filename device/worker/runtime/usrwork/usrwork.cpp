/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "usrwork.hpp"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <algorithm>
#include <chrono>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include "dispatcher.hpp"
#include "axcl_logger.hpp"

#define TAG "usrwork"

namespace axcl::worker {

usrwork::usrwork(dispatcher* dispatcher) : base(dispatcher), m_api(TAG) {
    m_api.append({
        RUNTIME_USRWORK_API_APPEND(TRANSFER_FILE),
        RUNTIME_USRWORK_API_APPEND(EXEC_WORKER),
        RUNTIME_USRWORK_API_APPEND(KILL_WORKER),
    });

    m_worker_manager = std::make_unique<usrworker_manager>();
    if (0 != m_worker_manager->init()) {
        LOG_MM_E(TAG, "Failed to initialize worker manager");
    }
}

usrwork::~usrwork() {
    m_worker_manager->deinit();
}

void usrwork::run(const void* data, const size_t& size) {
    m_api.run(get_dispatcher(), data, size);
}

RUNTIME_USRWORK_API_IMPL(TRANSFER_FILE) {
    AX_S32 ret = AXCL_SUCC;

    const std::string& src_path = response->get_src_path();
    const std::string& dst_path = response->get_dst_path();
    auto policy = response->get_policy();

    LOG_MM_D(TAG, "Transfer file from {} to {} with policy {}", src_path, dst_path, static_cast<int>(policy));

    if (policy == axcl::pkg::runtime::usrwork::FILE_TRANSFER_POLICY::FILE_TRANSFER_FROM_HOST_TO_DEVICE) {
        std::string dir = dst_path.substr(0, dst_path.find_last_of('/'));
        if (!dir.empty()) {
            mkdir(dir.c_str(), 0755);
        }

        std::ofstream file(dst_path, std::ios::binary);
        if (!file.is_open()) {
            LOG_MM_E(TAG, "open dst file {} fail", dst_path);
            ret = -1;
        } else {
            size_t fsize = 0;
            const void* data = response->get_data(fsize);
            if (fsize > 0) {
                file.write(reinterpret_cast<const char*>(data), fsize);
            }
            file.close();
            if (chmod(dst_path.c_str(), 0755) != 0) {
                LOG_MM_E(TAG, "chmod {} to 0755 fail", dst_path);
                ret = -2;
            }
        }
    } else if (policy == axcl::pkg::runtime::usrwork::FILE_TRANSFER_POLICY::FILE_TRANSFER_FROM_DEVICE_TO_HOST) {
        std::ifstream file(src_path, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            LOG_MM_E(TAG, "open src file {} fail", src_path);
            ret = -1;
        } else {
            size_t fsize = file.tellg();
            file.seekg(0, std::ios::beg);
            std::vector<uint8_t> data(fsize);
            file.read(reinterpret_cast<char*>(data.data()), fsize);
            file.close();
            if (fsize > 0) {
                request->set_data(data.data(), fsize);
            } else {
                LOG_MM_E(TAG, "src file {} is empty", src_path);
                ret = -2;
            }
        }
    } else if (policy == axcl::pkg::runtime::usrwork::FILE_TRANSFER_POLICY::FILE_TRANSFER_FROM_DEVICE_TO_DEVICE) {
        if (std::filesystem::exists(src_path) && src_path != dst_path) {
            std::filesystem::copy(src_path, dst_path, std::filesystem::copy_options::overwrite_existing);
        } else {
            LOG_MM_E(TAG, "src file {} is not exist or dst file {} is same as src file", src_path, dst_path);
            ret = -1;
        }
    } else if (policy == axcl::pkg::runtime::usrwork::FILE_TRANSFER_POLICY::FILE_TRANSFER_REMOVE_DEVICE_FILE) {
        if (std::filesystem::exists(src_path)) {
            std::filesystem::remove(src_path);
            LOG_MM_D(TAG, "remove {} from device succ", src_path);
        } else {
            LOG_MM_E(TAG, "src file {} is not exist", src_path);
            ret = -1;
        }
    } else {
        LOG_MM_E(TAG, "unsupported policy: {}", static_cast<int>(policy));
        ret = -1;
    }

    request->set_status(ret);
    return ret;
}

RUNTIME_USRWORK_API_IMPL(EXEC_WORKER) {
    AX_S32 ret = AXCL_SUCC;

    const std::string& path = response->get_path();
    std::vector<std::string> argv = response->get_argv();
    int32_t timeout = response->get_timeout();

    LOG_MM_D(TAG, "Execute worker: {}, timeout: {} ms, args count: {}", path, timeout, argv.size());
    for (size_t i = 0; i < argv.size(); ++i) {
        LOG_MM_D(TAG, "Arg[{}]: {}", i, argv[i]);
    }

    uint32_t pid = 0;

    uint32_t heartbeat_interval = response->get_heartbeat_interval();
    uint32_t port_num = response->get_port_num();
    std::vector<uint32_t> ports_vec = response->get_ports();

    const IPC_MSG_INIT_T init = {.heartbeat_interval = heartbeat_interval, .port_num = port_num, .ports = {}};

    for (size_t i = 0; i < std::min(static_cast<size_t>(port_num), ports_vec.size()) && i < MAX_PORT_NUM; ++i) {
        const_cast<IPC_MSG_INIT_T&>(init).ports[i] = ports_vec[i];
    }

    LOG_MM_D(TAG, "IPC_MSG_INIT_T: heartbeat_interval={}, port_num={}", heartbeat_interval, port_num);
    for (size_t i = 0; i < std::min(static_cast<size_t>(port_num), static_cast<size_t>(MAX_PORT_NUM)); ++i) {
        LOG_MM_D(TAG, "Port[{}]: {}", i, init.ports[i]);
    }

    timeout = 0;
    if (0 != m_worker_manager->exec_worker(path, argv, init, pid, timeout)) {
        LOG_MM_E(TAG, "Failed to execute worker");
        ret = -1;
    }

    request->set_pid(pid);
    request->set_status(ret);
    return ret;
}

RUNTIME_USRWORK_API_IMPL(KILL_WORKER) {
    AX_S32 ret = AXCL_SUCC;

    uint32_t pid = response->get_pid();
    LOG_MM_D(TAG, "Kill worker with pid: {}", pid);
    if (0 != m_worker_manager->stop_worker(pid)) {
        LOG_MM_D(TAG, "Failed to stop worker");
        ret = -1;
    }
    request->set_status(ret);
    return ret;
}

}  // namespace axcl::worker