/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "port.hpp"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <unistd.h>
#include "axcl_logger.hpp"
#include "os.hpp"

#define TAG "port"

namespace axcl::daemon {

#define SLAVE_WORKER_PROCESS_PATH "/opt/bin/axclSlave/slave_worker"
#define SLAVE_WORKER_PROCESS_NAME "slave_worker"

#define PORTS_TOKEN(ports) \
    std::make_pair(std::make_pair(ports[0], ports[1]), std::make_pair(ports[2], ports[3]))

bool port::open() {
    return true;
}

bool port::start() {
    /* start alive monitor thread to monitor process alive */
    m_alive_thread.start("alive", &port::alive, this);

    return true;
}

void port::close() {
    m_alive_thread.stop();
    m_alive_task.set();
    m_alive_thread.join();
}

void port::on_port_allocate(const device_info &device_info) {
    if (AXCL_PORT_CREATE == device_info.cmd) {
        std::lock_guard<std::mutex> lck(m_mtx_maps);

        if (4 != device_info.port_num) {
            LOG_MM_E(TAG, "invalid port_num {}, ports {} {} {} {}", device_info.port_num, device_info.ports[0], device_info.ports[1], device_info.ports[2], device_info.ports[3]);
            return;
        }

        port_token_t token = PORTS_TOKEN(device_info.ports);
        auto iter = m_maps.find(token);

        if (iter != m_maps.end()) {
            LOG_MM_E(TAG, "port {} {} {} {} already launched", device_info.ports[0], device_info.ports[1], device_info.ports[2], device_info.ports[3]);
            kill_process(device_info.ports);
        }

        launch_process(device_info, device_info.dma_buf_size, device_info.log_level);
    } else if (AXCL_PORT_DESTROY == device_info.cmd) {
        std::lock_guard<std::mutex> lck(m_mtx_maps);

        if (4 != device_info.port_num) {
            LOG_MM_E(TAG, "invalid port_num {}, ports {} {} {} {}", device_info.port_num, device_info.ports[0], device_info.ports[1], device_info.ports[2], device_info.ports[3]);
            return;
        }

        port_token_t token = PORTS_TOKEN(device_info.ports);
        auto iter = m_maps.find(token);

        if (iter == m_maps.end()) {
            LOG_MM_E(TAG, "port {} {} {} {} not launched", device_info.ports[0], device_info.ports[1], device_info.ports[2], device_info.ports[3]);
            return;
        }

        kill_process(device_info.ports);
    }
}

void port::launch_process(const device_info &device_info, uint32_t dma_buf_size, uint32_t log_level) {
    pid_t pid;

    auto &ports = device_info.ports;

    pid = vfork();
    if (pid < 0) {
        LOG_MM_E(TAG, "vfork failed, errno: {} {}", errno, ::strerror(errno));
        // fixme.
        _exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // child process
        std::string arg0;
        std::string arg1;
        std::string arg2;
        std::string arg3;
        std::string arg4;
        std::string arg5;

        arg0 = std::to_string(ports[0]);
        arg1 = std::to_string(ports[1]);
        arg2 = std::to_string(ports[2]);
        arg3 = std::to_string(ports[3]);
        arg4 = std::to_string(dma_buf_size);
        arg5 = std::to_string(log_level);

        LOG_MM_I(TAG, "launching cmd: {} {} {} {} {} {} {}", SLAVE_WORKER_PROCESS_PATH, arg0.c_str(), arg1.c_str(), arg2.c_str(), arg3.c_str(), arg4.c_str(), arg5.c_str());

        execl(SLAVE_WORKER_PROCESS_PATH, SLAVE_WORKER_PROCESS_NAME, arg0.c_str(), arg1.c_str(), arg2.c_str(), arg3.c_str() , arg4.c_str() , arg5.c_str(), NULL);

        LOG_MM_E(TAG, "launch failed, errno: {} {}", errno, ::strerror(errno));

        _exit(EXIT_FAILURE);
    } else {
        // parent process
        port_token_t token = PORTS_TOKEN(ports);

        LOG_MM_I(TAG, "host pid({})<=====>slave pid({}), ports {} {} {} {} launched...", device_info.pid, pid, ports[0], ports[1], ports[2], ports[3]);

        m_maps[token][0] = device_info.pid;
        m_maps[token][1] = pid;
    }
}

void port::kill_process(const uint32_t ports[4]) {
    port_token_t token = PORTS_TOKEN(ports);

    auto pids = m_maps[token];
    std::string cmd = "kill -15 " + std::to_string(pids[1]);

    LOG_MM_I(TAG, "kill process cmd: {}, port {} {} {} {}", cmd.c_str(), ports[0], ports[1], ports[2], ports[3]);

    std::system(cmd.c_str());

    m_maps.erase(token);

    // wait pid killed
    int status;
    waitpid(pids[1], &status, 0);

    LOG_MM_I(TAG, "host pid({})<=====>slave pid({}), ports {} {} {} {} killed...", pids[0], pids[1], ports[0], ports[1], ports[2], ports[3]);
}

void port::alive() {
    LOG_MM_I(TAG, "+++");

    while (m_alive_thread.running()) {
        // check keep alive
        {
            std::lock_guard<std::mutex> lck(m_mtx_maps);
            for (auto iter = m_maps.begin(); iter != m_maps.end();) {
                if (!is_alive(iter->second[1])) {
                    m_maps.erase(iter++);
                    continue;
                }

                ++iter;
            }
        }

        m_alive_task.wait(PORT_ALIVE_INTERVAL);
    }

    LOG_MM_I(TAG, "---");
}

bool port::is_alive(const pid_t &pid) {
    struct stat buffer;
    std::string path = "/proc/" + std::to_string(pid);

    return (bool)(stat(path.c_str(), &buffer) == 0);
}

}  // namespace axcl::daemon
