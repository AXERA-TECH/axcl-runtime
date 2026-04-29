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
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <limits>
#include <memory>
#include <string>
#include <vector>
#include "axcl_base.h"
#include "axcl_rt.h"
#include "axcl_rt_internal.h"
#include "axcl_rt_type.h"
#include "axcl_rt_usrwork_type.h"
#include "device/device.hpp"
#include "device/device_manager.hpp"
#include "axcl_logger.hpp"
#ifndef WINDOWS
#include <algorithm>
#include <cctype>
#endif

#define TAG "usrwork"

int32_t axcl::rt::usrwork::transfer_file(const char *src_path, const char *dst_path, axclrtFileTransferPolicy policy) {
    if (!src_path || (!dst_path && policy != axclrtFileTransferPolicy::FILE_TRANSFER_REMOVE_DEVICE_FILE)) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_USRWORK_NULL_POINTER;
    }

    axclrtStream stream;
    axclrtEnvironment env;
    int32_t ret;
    if (ret = get_runtime_environment(stream, env); AXCL_SUCC != ret) {
        return ret;
    }

    m_req.transfer_file(src_path, dst_path, static_cast<uint32_t>(policy));
    if (policy == axclrtFileTransferPolicy::FILE_TRANSFER_FROM_HOST_TO_DEVICE) {
        std::ifstream file(src_path, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            LOG_MM_E(TAG, "open src file {} fail", src_path);
            return AXCL_ERR_USRWORK_OPEN_FILE;
        }
        auto fsize = file.tellg();
        if (0 == fsize) {
            LOG_MM_E(TAG, "src file {} is empty", src_path);
            return AXCL_ERR_USRWORK_EMPTY_FILE;
        }

        uint32_t file_size = static_cast<uint32_t>(fsize);

        file.seekg(0, std::ios::beg);
        std::vector<uint8_t> data(file_size);
        file.read(reinterpret_cast<char *>(data.data()), file_size);
        file.close();

        m_req.set_data(data.data(), file_size);
    } else if (policy == axclrtFileTransferPolicy::FILE_TRANSFER_FROM_DEVICE_TO_DEVICE) {
#if defined(WINDOWS)
        if (0 == _stricmp(src_path, dst_path)) {
#else
        if (0 == strcasecmp(src_path, dst_path)) {
#endif

            LOG_MM_E(TAG, "src path and dst path are the same");
            return AXCL_ERR_USRWORK_INVALID_PATH;
        }
    }

    if (ret = invoke(stream, env, NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (policy == axclrtFileTransferPolicy::FILE_TRANSFER_FROM_DEVICE_TO_HOST) {
        // device to host, recv data from device
        size_t fsize = 0;
        const void *data = m_rsp.get_data(fsize);
        if (!data || 0 == fsize) {
            LOG_MM_E(TAG, "get file data from device fail, fsize: {}", fsize);
            return AXCL_ERR_USRWORK_EXECUTE_FAIL;
        }

        std::string dir = std::string(dst_path).substr(0, std::string(dst_path).find_last_of('/'));
        if (!dir.empty()) {
            std::filesystem::create_directories(dir);
        }

        std::ofstream file(dst_path, std::ios::binary);
        if (!file.is_open()) {
            LOG_MM_E(TAG, "open dst file {} fail", dst_path);
            return AXCL_ERR_USRWORK_OPEN_FILE;
        }

        file.write(reinterpret_cast<const char *>(data), fsize);
        file.close();
    }

    if (policy == axclrtFileTransferPolicy::FILE_TRANSFER_REMOVE_DEVICE_FILE) {
        LOG_MM_I(TAG, "remove {} from device succ", src_path);
    } else {
        LOG_MM_I(TAG, "transfer_file from {} to {} with policy {} succ", src_path, dst_path, static_cast<uint32_t>(policy));
    }

    return AXCL_SUCC;
}

int32_t axcl::rt::usrwork::exec(const char *path, const int32_t *argc, const char *argv[], uint32_t *pid) {
    if (!path) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_USRWORK_NULL_POINTER;
    }

    axclrtStream stream;
    axclrtEnvironment env;
    int32_t ret;
    if (ret = get_runtime_environment(stream, env); AXCL_SUCC != ret) {
        return ret;
    }

    std::vector<std::string> args;
    if (argv && argc) {
        for (int32_t i = 0; i < *argc; ++i) {
            if (argv[i]) {
                args.push_back(argv[i]);
            }
        }
    }

    int32_t device_id;
    if (ret = axclrtGetDevice(&device_id); AXCL_SUCC != ret) {
        LOG_MM_E(TAG, "Failed to get device, error: {}", ret);
        return ret;
    }

    constexpr uint32_t port_num = sizeof(axcl::comm::comm_args_t::pcie.port)/sizeof(axcl::comm::comm_args_t::pcie.port[0]);
    std::vector<uint32_t> ports(port_num);
    if (ret = DEVICE_MANAGER()->request_usr_ports(device_id, port_num, ports.data()); AXCL_SUCC != ret) {
        return ret;
    }

    axcl::comm::comm_args_t pcie_args = {};
    pcie_args.pcie.handshake = false;
    pcie_args.pcie.target = device_id;
    for (size_t i = 0; i < port_num; ++i) {
        pcie_args.pcie.port[i] = ports[i];
    }

    auto channel = std::make_shared<usrworkchannel>();
    if (!channel) {
        LOG_MM_E(TAG, "Failed to create channel");
        return AXCL_ERR_USRWORK_EXECUTE_FAIL;
    }

    uint32_t heartbeat_interval = 500;
    m_req.exec_worker(path, args, heartbeat_interval, port_num, ports);
    ret = invoke(stream, env, NO_TIMEOUT);
    if (ret != AXCL_SUCC) {
        LOG_MM_E(TAG, "exec_worker invoke failed, error: {}", ret);
        channel->close();
        return ret;
    }

    uint32_t actual_pid = m_rsp.get_pid();
    if (actual_pid == 0) {
        LOG_MM_E(TAG, "Invalid pid returned from exec_worker");
        channel->close();
        return AXCL_ERR_USRWORK_EXECUTE_FAIL;
    }

    const int32_t status = m_rsp.get_status();
    if (status != 0) {
        LOG_MM_E(TAG, "exec_worker returned non-zero status: {}", status);
        channel->close();
        return AXCL_ERR_USRWORK_EXECUTE_FAIL;
    }

    channel->set_pid(actual_pid);
    ret = channel->open(pcie_args);
    if (ret != AXCL_SUCC) {
        LOG_MM_E(TAG, "Failed to open channel, error: {}", ret);
        return ret;
    }

    if (ret = add_process(actual_pid, path, channel); AXCL_SUCC != ret) {
        LOG_MM_E(TAG, "Failed to add process, error: {}", ret);
        channel->close();
        return ret;
    }

    if (pid) {
        *pid = actual_pid;
    }

    LOG_MM_D(TAG, "exec {} with pid {} succ", path, actual_pid);
    return AXCL_SUCC;
}

int32_t axcl::rt::usrwork::kill(uint32_t pid) {
    if (pid == 0) {
        LOG_MM_E(TAG, "invalid pid");
        return AXCL_ERR_USRWORK_PROCESS_NOT_FOUND;
    }

    axclrtStream stream;
    axclrtEnvironment env;
    int32_t ret;
    if (ret = get_runtime_environment(stream, env); AXCL_SUCC != ret) {
        return ret;
    }

    m_req.kill_worker(pid);
    if (ret = invoke(stream, env, NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (ret = del_process(pid); AXCL_SUCC != ret) {
        LOG_MM_E(TAG, "Failed to delete process, error: {}", ret);
    }

    const int32_t status = m_rsp.get_status();
    if (status != 0) {
        LOG_MM_E(TAG, "kill_worker returned non-zero status: {}", status);
        return AXCL_ERR_USRWORK_EXECUTE_FAIL;
    }

    LOG_MM_I(TAG, "kill process {} succ", pid);
    return AXCL_SUCC;
}

int32_t axcl::rt::usrwork::send(uint32_t pid, const void *buf, uint32_t size, int32_t timeout) {
    if (!buf || size == 0) {
        LOG_MM_E(TAG, "invalid parameters");
        return AXCL_ERR_USRWORK_NULL_POINTER;
    }

    if (pid == 0) {
        LOG_MM_E(TAG, "invalid pid");
        return AXCL_ERR_USRWORK_PROCESS_NOT_FOUND;
    }

    ProcessInfo info;
    int32_t ret = get_process(pid, info);
    if (ret != AXCL_SUCC) {
        LOG_MM_E(TAG, "Process with pid {} not found", pid);
        return ret;
    }

    if (!info.channel) {
        LOG_MM_E(TAG, "Channel for pid {} is invalid", pid);
        return AXCL_ERR_USRWORK_EXECUTE_FAIL;
    }

    ret = info.channel->send(buf, size, timeout);
    if (ret != AXCL_SUCC) {
        LOG_MM_E(TAG, "Failed to send data, error: {}", ret);
        return ret;
    }

    LOG_MM_I(TAG, "send {} bytes to process {} succ", size, pid);
    return AXCL_SUCC;
}

int32_t axcl::rt::usrwork::recv(uint32_t pid, void *buf, uint32_t bufsize, uint32_t *recvlen, int32_t timeout) {
    if (!buf || bufsize == 0) {
        LOG_MM_E(TAG, "invalid parameters");
        return AXCL_ERR_USRWORK_NULL_POINTER;
    }

    if (pid == 0) {
        LOG_MM_E(TAG, "invalid pid");
        return AXCL_ERR_USRWORK_PROCESS_NOT_FOUND;
    }

    ProcessInfo info;
    int32_t ret = get_process(pid, info);
    if (ret != AXCL_SUCC) {
        LOG_MM_E(TAG, "Process with pid {} not found", pid);
        return ret;
    }

    if (!info.channel) {
        LOG_MM_E(TAG, "Channel for pid {} is invalid", pid);
        return AXCL_ERR_USRWORK_EXECUTE_FAIL;
    }

    ret = info.channel->recv(buf, bufsize, recvlen, timeout);
    if (ret != AXCL_SUCC) {
        LOG_MM_E(TAG, "Failed to recv data, error: {}", ret);
        return ret;
    }

    LOG_MM_I(TAG, "recv {} bytes from process {} succ", *recvlen, pid);
    return AXCL_SUCC;
}

int32_t axcl::rt::usrwork::get_runtime_environment(axclrtStream &stream, axclrtEnvironment &env) {
    int32_t ret;
    axclrtContext context;

    ret = axclrtGetCurrentContext(&context);
    if (ret != AXCL_SUCC) {
        return ret;
    }

    ret = axclrtGetDefaultStream(context, &stream);
    if (ret != AXCL_SUCC) {
        return ret;
    }

    ret = axclrtGetEnviromentByStream(stream, &env);
    if (ret != AXCL_SUCC) {
        return ret;
    }

    return AXCL_SUCC;
}

int32_t axcl::rt::usrwork::invoke(axclrtStream stream, const axclrtEnvironment &env, int32_t timeout) {
    const auto api = m_req.get_api();
    const bool has_response = true;
    const int64_t flag = axcl::comm::PCIE_DMA;

    axcl::pkg::head hd = {};
    hd.seq_num = axclrtGetToken();
    hd.device = env.device;
    hd.context = env.context;
    hd.stream = env.stream;
    hd.command = 0;
    hd.type = CREATE_PACKAGE_HEAD_TYPE(axcl::pkg::PROTO, axcl::pkg::module::RUNTIME, axcl::pkg::runtime::module::USRWORK, api);

    static uint32_t pid = 0;
    if (pid == 0) {
        pid = static_cast<uint32_t>(getpid());
    }

    static thread_local uint32_t tid = 0;
    if (tid == 0) {
        tid = gettid();
    }

    m_req.set_env({pid, tid});

    uint32_t len = 0;
    void *pkg = m_req.encode(hd, len);
    if (!pkg) {
        LOG_MM_E(TAG, "encode api {} fail", static_cast<int32_t>(api));
        return AXCL_ERR_USRWORK_ENCODE;
    }

    int32_t ret;
    axclrtTask task = nullptr;
    ret = axclrtCreateTask(&task, stream, hd.seq_num, pkg, len, timeout, has_response, flag);
    if (ret != AXCL_SUCC) {
        return ret;
    }

    auto deleter = [](axclrtTask task) {
        if (task) {
            axclrtDestoryTask(task);
        }
    };
    std::unique_ptr<void, decltype(deleter)> sp(task, deleter);

    ret = axclrtWaitTaskFinish(task);
    if (ret != AXCL_SUCC) {
        return ret;
    }

    if (has_response) {
        ret = axclrtGetTaskResponse(task, &pkg, &len);
        if (ret != AXCL_SUCC) {
            return ret;
        }

        if (!m_rsp.decode(pkg, len)) {
            LOG_MM_E(TAG, "decode api {} response fail", static_cast<int32_t>(api));
            return AXCL_ERR_USRWORK_DECODE;
        }

        if (m_rsp.get_api() != api) {
            LOG_MM_E(TAG, "response api {} is not equal to request api {}", static_cast<int32_t>(m_rsp.get_api()),
                     static_cast<int32_t>(api));
            return AXCL_ERR_USRWORK_UNEXPECT_RESPONSE;
        }

        const int32_t status = m_rsp.get_status();
        if (status != 0) {
            LOG_MM_E(TAG, "request api {} return fail, status: {}", static_cast<int32_t>(api), status);
            return AXCL_ERR_USRWORK_EXECUTE_FAIL;
        }
    }

    return AXCL_SUCC;
}

axcl::rt::usrwork::~usrwork() {
    for (auto &[pid, info] : m_processes) {
        if (info.channel) {
            info.channel->close();
        }
    }
    m_processes.clear();
}

int32_t axcl::rt::usrwork::add_process(uint32_t pid, const std::string &path, std::shared_ptr<usrworkchannel> channel) {
    if (pid == 0) {
        LOG_MM_E(TAG, "invalid pid");
        return AXCL_ERR_USRWORK_INVALID_PATH;
    }

    std::lock_guard<std::mutex> lock(m_mutex);

    if (m_processes.find(pid) != m_processes.end()) {
        LOG_MM_W(TAG, "Process with pid {} already exists, overwriting", pid);
    }

    m_processes[pid] = ProcessInfo(path, channel);
    LOG_MM_I(TAG, "Added process pid {} with path {}", pid, path);
    return AXCL_SUCC;
}

int32_t axcl::rt::usrwork::get_process(uint32_t pid, ProcessInfo &info) {
    if (pid == 0) {
        LOG_MM_E(TAG, "invalid pid");
        return AXCL_ERR_USRWORK_INVALID_PATH;
    }

    std::lock_guard<std::mutex> lock(m_mutex);

    auto it = m_processes.find(pid);
    if (it == m_processes.end()) {
        LOG_MM_E(TAG, "Process with pid {} not found", pid);
        return AXCL_ERR_USRWORK_PROCESS_NOT_FOUND;
    }

    info = it->second;
    return AXCL_SUCC;
}

int32_t axcl::rt::usrwork::del_process(uint32_t pid) {
    if (pid == 0) {
        LOG_MM_E(TAG, "invalid pid");
        return AXCL_ERR_USRWORK_INVALID_PATH;
    }

    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_processes.find(pid);
    if (it == m_processes.end()) {
        LOG_MM_W(TAG, "Process with pid {} not found for deletion", pid);
        return AXCL_ERR_USRWORK_PROCESS_NOT_FOUND;
    }

    if (it->second.channel) {
        const auto &args = it->second.channel->get_args();
        it->second.channel->close();
        DEVICE_MANAGER()->destroy_usr_ports(args.pcie.target, sizeof(args.pcie.port)/sizeof(args.pcie.port[0]),
                                            (uint32_t *)args.pcie.port);
    }

    m_processes.erase(it);

    LOG_MM_I(TAG, "Deleted process pid {}", pid);
    return AXCL_SUCC;
}

bool axcl::rt::usrwork::has_process(uint32_t pid) {
    if (pid == 0) {
        return false;
    }

    std::lock_guard<std::mutex> lock(m_mutex);
    return m_processes.find(pid) != m_processes.end();
}

size_t axcl::rt::usrwork::get_process_count() {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_processes.size();
}