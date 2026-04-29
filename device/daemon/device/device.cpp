/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "device.hpp"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "axcl_logger.hpp"
#include "os.hpp"
#include "port.hpp"

#define TAG "device"

namespace axcl::daemon {

device::device(axcl::comm::COMM_TYPE type) : m_comm_type(type), m_fd(-1) {
}

bool device::open() {
    if (m_fd < 0) {
        switch (m_comm_type) {
            case axcl::comm::COMM_TYPE::COMM_TYPE_PCIE:
                m_fd = ::open(DEV_PATH, O_RDWR);
                break;
            default:
                break;
        }
    }

    if (m_fd < 0) {
        LOG_MM_E(TAG, "open {} fail, errno: {} {}", DEV_PATH, errno, ::strerror(errno));
        return false;
    }

    return true;
}

bool device::start() {
    if (m_fd < 0) {
        return false;
    }

    /* start listen thread to monitor device connection */
    m_listen_thread.start("listen", &device::listen, this);

    /* start heart beats thread */
    m_heartbeats_thread.start("heartbeats", &device::heartbeats, this);

    return true;
}

void device::close() {
    LOG_MM_I(TAG, "+++");

    m_listen_thread.stop();
    m_listen_thread.join();

    m_heartbeats_thread.stop();
    m_heartbeats_task.set();
    m_heartbeats_thread.join();

    ::close(m_fd);
    m_fd = -1;

    LOG_MM_I(TAG, "---");
}

void device::dispatch(const device_info &device_info) {
    std::lock_guard<std::mutex> lck(m_mtx_sinks);
    for (auto &&sink : m_sinks) {
        sink->on_port_allocate(device_info);
    }
}

bool device::register_sink(sinker *sink) {
    std::lock_guard<std::mutex> lck(m_mtx_sinks);

    if (std::find(m_sinks.begin(), m_sinks.end(), sink) != m_sinks.end()) {
        LOG_MM_W(TAG, "sink {:#x} is already registered", reinterpret_cast<uintptr_t>(sink));
        return true;
    }

    m_sinks.push_back(sink);

    // LOG_MM_D(TAG, "sink {:#x} is registed", reinterpret_cast<uintptr_t>(sink));
    return true;
}

bool device::unregister_sink(sinker *sink) {
    std::lock_guard<std::mutex> lck(m_mtx_sinks);

    auto it = std::find(m_sinks.begin(), m_sinks.end(), sink);
    if (it != m_sinks.end()) {
        m_sinks.erase(it);

        // LOG_MM_D(TAG, "sink {:#x} is unregisted", reinterpret_cast<uintptr_t>(sink));
        return true;
    }

    LOG_MM_E(TAG, "sink {:#x} is not registed", reinterpret_cast<uintptr_t>(sink));
    return false;
}

void device::listen() {
    LOG_MM_I(TAG, "+++");

   // struct timeval timeout = {.tv_sec = 1, .tv_usec = 0};
    const int fd_max = m_fd + 1;
    fd_set set;
    int ret;

    while (m_listen_thread.running()) {
        /* should set by every time */
        FD_ZERO(&set);
        FD_SET(m_fd, &set);

        ret = ::select(fd_max, &set, NULL, NULL, NULL);

        if (ret < 0) {
            LOG_MM_E(TAG, "select fail, errno: {} {}", errno, ::strerror(errno));
        } else if (0 == ret) {
            /* timeout */
            continue;
        } else {
            if (FD_ISSET(m_fd, &set)) {
                struct device_info dev_info = {0};
                ret = ioctl(m_fd, IOC_AXCL_PORT_MANAGE, &dev_info);

                if (ret < 0) {
                    LOG_MM_E(TAG, "ioctl(IOC_AXCL_PORT_MANAGE) fail, ret: {} {}", ret, ::strerror(errno));
                    continue;
                }

                LOG_MM_I(TAG, "device info: cmd: {}, device: {}, host pid: {}, ports num: {}", dev_info.cmd, dev_info.device, dev_info.pid,
                         dev_info.port_num);
                for (uint32_t i = 0; i < dev_info.port_num; ++i) {
                    LOG_MM_I(TAG, "port[{}]: {}", i, dev_info.ports[i]);
                }
                LOG_MM_I(TAG, "dma buf size: {}, log level: {}", dev_info.dma_buf_size, dev_info.log_level);

                if (AXCL_PORT_CREATE == dev_info.cmd) {
                    struct device_info dev_info_rsp = {0};
                    memset(&dev_info_rsp, 0, sizeof(struct device_info));
                    dev_info_rsp.cmd = AXCL_PORT_CREATE_COMPLETION;
                    dev_info_rsp.device = 0;
                    ret = ioctl(m_fd, IOC_AXCL_PORT_MANAGE, &dev_info_rsp);
                    if (ret < 0) {
                        LOG_MM_E(TAG, "ioctl(IOC_AXCL_PORT_MANAGE) fail, ret: {} {}", ret, ::strerror(errno));
                        continue;
                    }

                    dispatch(dev_info);
                } else if (AXCL_PORT_DESTROY == dev_info.cmd) {
                    LOG_MM_I(TAG, "cmd(AXCL_PORT_DESTROY): host destroy");

                    dispatch(dev_info);
                }
            }
        }
    }

    LOG_MM_I(TAG, "---");
}

void device::heartbeats() {
    LOG_MM_I(TAG, "+++");

    struct device_heart_beat heartbeats = {0};
    heartbeats.device = 0;
    heartbeats.interval = DEV_HEARTBEATS_INTERVAL;

    while (m_heartbeats_thread.running()) {
        // heartbeats.timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        ++heartbeats.timestamp;
        int ret = ioctl(m_fd, IOC_AXCL_HEART_BEATS, &heartbeats);
        if (ret < 0) {
            LOG_MM_E(TAG, "ioctl(IOC_AXCL_HEART_BEATS) fail, ret: {}", ret);
        }

        // m_heartbeats_task.wait(DEV_HEARTBEATS_SLEEP);
        std::this_thread::sleep_for(std::chrono::milliseconds(DEV_HEARTBEATS_SLEEP));
    }

    LOG_MM_I(TAG, "---");
}

}  // namespace axcl::daemon
