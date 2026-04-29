/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "device_manager.hpp"
#include <errno.h>
#include <string.h>
#include <algorithm>
#include "axcl_logger.hpp"
#include "axcl_rt_config.h"
#include "axcl_rt_device.h"
#include "context/context_manager.hpp"
#include "device/device.hpp"
#include "device_info.h"
#include "os.hpp"
#include "res_guard.hpp"

// clang-format off
#define TAG "device manager"

#if defined(WINDOWS)
    #define DEV_PATH "axcl_host"
#else
    #define DEV_PATH "/dev/axcl_host"
#endif

#if defined(WINDOWS)
    #define IOC_AXCL_MAGIC      0x00
#else
    #define IOC_AXCL_MAGIC      'A'
#endif

#define IOC_AXCL_PORT_MANAGE    AX_IOWR(IOC_AXCL_MAGIC,  1, struct device_info)
#define IOC_AXCL_CONN_STATUS    AX_IOR( IOC_AXCL_MAGIC,  2, struct device_connect_status)
#define IOC_AXCL_HEART_BEATS    AX_IOW( IOC_AXCL_MAGIC,  3, struct device_heart_beat)
#define IOC_AXCL_DEVICE_LIST    AX_IOWR(IOC_AXCL_MAGIC,  4, struct device_list)
#define IOC_AXCL_WAKEUP_POLL    AX_IOWR(IOC_AXCL_MAGIC,  5, struct device_info)
#define IOC_AXCL_DEVICE_RESET   AX_IOWR(IOC_AXCL_MAGIC,  6, struct device_info)
#define IOC_AXCL_DEVICE_BOOT    AX_IOWR(IOC_AXCL_MAGIC,  7, struct device_info)
#define IOC_AXCL_BUS_INFO       AX_IOWR(IOC_AXCL_MAGIC,  8, struct bus_info_t)
#define IOC_AXCL_PID_NUM        AX_IOWR(IOC_AXCL_MAGIC,  9, struct pid_num_t)
#define IOC_AXCL_PID_INFO       AX_IOWR(IOC_AXCL_MAGIC, 10, struct pid_info_t)
// clang-format on

namespace axcl::rt {

int32_t device_manager::init() {
    std::lock_guard<std::mutex> lck(m_mtx_mgr);

    if (0 == m_ref_cnt) {
        m_fd = xp_io_open(DEV_PATH);
        if (m_fd == XP_INVALID_DEV) {
            LOG_MM_E(TAG, "open {} fail, errno: {} {}", DEV_PATH, xp_io_error(), ::strerror(xp_io_error()));
            return AXCL_ERR_DEVICE_OPEN;
        }

        if (int32_t ret = probe(); AXCL_SUCC != ret) {
            xp_io_close(m_fd);
            m_fd = XP_INVALID_DEV;
            return ret;
        }

        m_listen.start("rt-listen", &device_manager::listen, this);
        LOG_MM_I(TAG, "device manager is initialized");
    }

    ++m_ref_cnt;

    return AXCL_SUCC;
}

int32_t device_manager::deinit() {
    std::lock_guard<std::mutex> lck(m_mtx_mgr);

    --m_ref_cnt;

    if (m_ref_cnt > 0) {
        return AXCL_SUCC;
    }

    if (m_ref_cnt < 0) {
        m_ref_cnt = 0;
        return AXCL_SUCC;
    }

    LOG_MM_I(TAG, "stop and uninitialze ...");
    m_listen.stop();

    /* WR: make sure listen::poll has runned */
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    struct device_info info;
    xp_io_ioctl(m_fd, IOC_AXCL_WAKEUP_POLL, &info, sizeof(info), &info, sizeof(info));

    LOG_MM_I(TAG, "wait listen thread join +++");
    m_listen.join();
    LOG_MM_I(TAG, "wait listen thread join ---");

    /* make sure to close all devices */
    for (auto &&m : m_devices) {
        if (m.second.second > 0) {
            m.second.first->close();
        }
    }

    xp_io_close(m_fd);
    m_fd = XP_INVALID_DEV;

    m_devices.clear();
    m_thread_device_map.clear();

    axclrtConfigUnLoad();
    LOG_MM_I(TAG, "device manager is uninitialzed");
    return AXCL_SUCC;
}

int32_t device_manager::probe() {
    struct device_list dev_list = {0};
    if (int32_t ret = xp_io_ioctl(m_fd, IOC_AXCL_DEVICE_LIST, &dev_list, sizeof(dev_list), &dev_list, sizeof(dev_list)); ret < 0) {
        LOG_MM_E(TAG, "query device list fail, errno: {} {}", xp_io_error(), ::strerror(xp_io_error()));
        return AXCL_ERR_DEVICE_QUERY_DEVICE;
    }

    if (0 == dev_list.num) {
        LOG_MM_E(TAG, "no device connected");
        return AXCL_ERR_DEVICE_NO_CONNECT;
    }

    axcl::comm::COMM_TYPE dev_type;
    switch (dev_list.type) {
        case PCIE_DEVICE:
            dev_type = axcl::comm::COMM_TYPE::COMM_TYPE_PCIE;
            break;
        default:
            LOG_MM_E(TAG, "device type {} is not supported yet", dev_list.type);
            return AXCL_ERR_DEVICE_UNSUPPORT;
    }

    std::lock_guard<std::mutex> lck(m_mtx_devices);
    m_devices.clear();
    for (unsigned int i = 0; i < dev_list.num; ++i) {
        const int32_t device_id = static_cast<int32_t>(dev_list.devices[i]);
        LOG_MM_I(TAG, "device[{}]: {}", i + 1, device_id);
        m_devices.emplace(device_id, std::make_pair(std::make_shared<device>(this, dev_type), 0));
    }

    return AXCL_SUCC;
}

int32_t device_manager::active(int32_t device_id) {
    std::lock_guard<std::mutex> lck(m_mtx_devices);

    auto it = m_devices.find(device_id);
    if (it == m_devices.end()) {
        LOG_MM_E(TAG, "device {} is not connected", device_id);
        return AXCL_ERR_DEVICE_INVALID_ID;
    }

    auto &dev_pair = m_devices[device_id];
    if (0 == dev_pair.second) {
        struct device_info info;
        if (int32_t ret = request_ports(device_id, info); ret != AXCL_SUCC) {
            return ret;
        }

        if (!dev_pair.first->open(info)) {
            return AXCL_ERR_DEVICE_OPEN;
        }

        dev_pair.first->get_context_manager()->create_default();
    }

    ++dev_pair.second;
    bind_thread(gettid(), device_id);

    return AXCL_SUCC;
}

void device_manager::bind_thread(uint32_t tid, int32_t device_id) {
    LOG_MM_I(TAG, "bind device {} to thread {}", device_id, tid);
    std::lock_guard<std::mutex> lck(m_mtx_thread_device_map);
    m_thread_device_map[tid] = device_id;
}

void device_manager::unbind_thread(uint32_t tid) {
    std::lock_guard<std::mutex> lck(m_mtx_thread_device_map);
    m_thread_device_map.erase(tid);
}

int32_t device_manager::deactive(int32_t device_id, bool force) {
    std::lock_guard<std::mutex> lck(m_mtx_devices);

    auto it = m_devices.find(device_id);
    if (it == m_devices.end()) {
        LOG_MM_E(TAG, "device {} is not connected", device_id);
        return AXCL_ERR_DEVICE_INVALID_ID;
    }

    auto &dev_pair = m_devices[device_id];
    if (0 == dev_pair.second) {
        if (force) {
            return AXCL_SUCC;
        }

        LOG_MM_E(TAG, "device {} is not actived", device_id);
        return AXCL_ERR_DEVICE_NOT_ACTIVE;
    }

    if (force || --dev_pair.second <= 0) {
        if (int32_t ret = destroy_ports(dev_pair.first->get_device_info()); ret != AXCL_SUCC) {
            return ret;
        }

        dev_pair.first->close();
        dev_pair.second = 0;
    }

    unbind_thread(gettid());
    return AXCL_SUCC;
}

bool device_manager::is_actived(int32_t device_id) {
    std::lock_guard<std::mutex> lck(m_mtx_devices);
    const auto it = m_devices.find(device_id);
    if (it == m_devices.end()) {
        LOG_MM_E(TAG, "device {} is not connected", device_id);
        return false;
    }

    return it->second.second > 0;
}

int32_t device_manager::get_current_thread_device_id() {
    std::lock_guard<std::mutex> lck(m_mtx_thread_device_map);
    auto it = m_thread_device_map.find(gettid());
    if (it == m_thread_device_map.end()) {
        return AXCL_ERR_DEVICE_INVALID_ID;
    }

    return it->second;
}

int32_t device_manager::sync() {
    std::lock_guard<std::mutex> lck(m_mtx_devices);
    for (auto &&m : m_devices) {
        if (m.second.second > 0) {
            m.second.first->sync();
        }
    }

    return AXCL_SUCC;
}

void device_manager::listen() {
    LOG_MM_I(TAG, "+++");

#if defined(WINDOWS)
    UCHAR inputbuffer[32] = {0};

    DWORD statusMask = 0;
    ssize_t ret;

    while (m_listen.running()) {
        LOG_MM_I(TAG, "select +++");
        ret = xp_io_poll(m_fd, &inputbuffer, sizeof(inputbuffer), statusMask, INFINITE);
        LOG_MM_I(TAG, "select --- ret = {}, quit: {}", ret, !m_listen.running());

        if (!m_listen.running()) {
            break;
        }

        if (ret < 0) {
            LOG_MM_E(TAG, "select fail, errno: {} {}", errno, ::strerror(errno));
            break;
        } else {
            UCHAR InputBuffer[32] = {0};
            size_t InputBufferLen = sizeof(InputBuffer);

            struct device_connect_status status = {0};
            if (ret = ::xp_io_ioctl(m_fd, IOC_AXCL_CONN_STATUS, InputBuffer, InputBufferLen, &status, sizeof(status)); ret < 0) {
                LOG_MM_E(TAG, "query device connection status fail, errno: {} {}", errno, ::strerror(errno));
                continue;
            }

            std::lock_guard<std::mutex> lck(m_mtx_devices);
            for (auto &&m : m_devices) {
                if (DEVICE_OFFLINE == status.status[m.first]) {
                    LOG_MM_E(TAG, "device {} is dead", m.first);
                    if (m.second.second > 0) {
                        /*
                         * As device is dead, nothing can do by host side, just set device to offline
                         */
                        m.second.first->offline();
                    }
                }
            }
        }
    }

#else
    const int fd_max = m_fd + 1;
    fd_set set;
    int ret;

    while (m_listen.running()) {
        FD_ZERO(&set);
        FD_SET(m_fd, &set);

        LOG_MM_I(TAG, "select +++");
        ret = ::select(fd_max, &set, NULL, NULL, NULL);
        LOG_MM_I(TAG, "select --- ret = {}, quit: {}", ret, !m_listen.running());

        if (!m_listen.running()) {
            break;
        }

        if (ret < 0) {
            LOG_MM_E(TAG, "select fail, errno: {} {}", errno, ::strerror(errno));
            break;
        } else if (0 == ret) {
            /* timeout */
            continue;
        } else {
            if (FD_ISSET(m_fd, &set)) {
                struct device_connect_status status = {0};
                if (ret = ::ioctl(m_fd, IOC_AXCL_CONN_STATUS, &status); ret < 0) {
                    LOG_MM_E(TAG, "query device connection status fail, errno: {} {}", errno, ::strerror(errno));
                    continue;
                }

                std::lock_guard<std::mutex> lck(m_mtx_devices);
                for (auto &&m : m_devices) {
                    if (DEVICE_OFFLINE == status.status[m.first]) {
                        LOG_MM_E(TAG, "device {} is dead", m.first);
                        if (m.second.second > 0) {
                            /*
                             * As device is dead, nothing can do by host side, just set device to offline
                             */
                            m.second.first->offline();
                        }
                    }
                }
            }
        }
    }
#endif
    LOG_MM_I(TAG, "---");
}

int32_t device_manager::reboot(int32_t device_id) {
    struct device_info info = {};
    info.device = device_id;
    if (int ret = xp_io_ioctl(m_fd, IOC_AXCL_DEVICE_BOOT, &info, sizeof(info), &info, sizeof(info)); ret < 0) {
        LOG_MM_E(TAG, "reboot device {} fail, errno: {} {}", device_id, xp_io_error(), ::strerror(xp_io_error()));
        return AXCL_ERR_DEVICE_REBOOT;
    }

    LOG_MM_I(TAG, "reboot device {} success", device_id);
    return AXCL_SUCC;
}

uint32_t device_manager::get_device_list(axclrtDeviceList *list) const {
    list->num = 0;
    for (auto &&m : m_devices) {
        list->devices[list->num++] = m.first;
    }

    std::sort(list->devices, list->devices + list->num);
    return list->num;
}

int32_t device_manager::get_bus_info(int32_t device_id, axclrtDeviceBusInfo &info) {
    memset(&info, 0, sizeof(info));

    struct bus_info_t bus_info = {0};
    bus_info.device = device_id;
    if (int32_t ret = xp_io_ioctl(m_fd, IOC_AXCL_BUS_INFO, &bus_info, sizeof(bus_info), &bus_info, sizeof(bus_info)); ret < 0) {
        LOG_MM_E(TAG, "query bus info fail, errno: {} {}", xp_io_error(), ::strerror(xp_io_error()));
        return AXCL_ERR_DEVICE_QUERY_DEVICE;
    }

    info.domain = bus_info.domain;
    info.bus = bus_info.device;
    info.slot = bus_info.slot;
    info.func = bus_info.func;

    return AXCL_SUCC;
}

int32_t device_manager::get_pid_info(int32_t device_id, axclrtDevicePidInfo &info) {
    memset(&info, 0, sizeof(info));
    info.device = device_id;

    struct pid_num_t pid_num = {0};
    pid_num.device = device_id;
    if (int32_t ret = xp_io_ioctl(m_fd, IOC_AXCL_PID_NUM, &pid_num, sizeof(pid_num), &pid_num, sizeof(pid_num)); ret < 0) {
        LOG_MM_E(TAG, "query pid num fail, errno: {} {}", xp_io_error(), ::strerror(xp_io_error()));
        return AXCL_ERR_DEVICE_QUERY_DEVICE;
    }

    info.num = pid_num.num;
    if (0 == info.num) {
        return AXCL_SUCC;
    }

    auto mem_holder =
        res_guard<unsigned int *>([&info]() -> unsigned int * { return (unsigned int *)::malloc(sizeof(unsigned int) * info.num); },
                                  [](unsigned int *&p) {
                                      if (p) ::free(p);
                                  });

    unsigned int *pids = mem_holder.get();
    if (!pids) {
        LOG_MM_E(TAG, "malloc pid info(num: {}) memory fail, errno: {} {}", info.num, xp_io_error(), ::strerror(xp_io_error()));
        return AXCL_DEF_DEVICE_ERR(AXCL_ERR_NO_MEMORY);
    }

    struct pid_info_t pid_info = {0};
    pid_info.device = device_id;
    pid_info.num = info.num;
    pid_info.pid = static_cast<unsigned long long>(reinterpret_cast<uintptr_t>(pids));
    if (int32_t ret = xp_io_ioctl(m_fd, IOC_AXCL_PID_INFO, &pid_info, sizeof(pid_info), &pid_info, sizeof(pid_info)); ret < 0) {
        LOG_MM_E(TAG, "query pid info fail, errno: {} {}", xp_io_error(), ::strerror(xp_io_error()));
        return AXCL_ERR_DEVICE_QUERY_DEVICE;
    }

    info.pid = (unsigned int *)::malloc(sizeof(unsigned int) * info.num);
    if (!info.pid) {
        LOG_MM_E(TAG, "malloc pid info(num: {}) memory fail, errno: {} {}", info.num, xp_io_error(), ::strerror(xp_io_error()));
        return AXCL_DEF_DEVICE_ERR(AXCL_ERR_NO_MEMORY);
    }

    ::memcpy(info.pid, pids, sizeof(unsigned int) * info.num);

    return AXCL_SUCC;
}

int32_t device_manager::request_ports(int32_t device_id, struct device_info &info) {
    memset(&info, 0, sizeof(info));
    info.device = device_id;
    info.cmd = AXCL_PORT_CREATE;
    info.pid = getpid();
    info.dma_buf_size = axclrtConfigGetDmaBufSize();
    info.log_level = axclrtConfigGetLogLevel(1);
    if (int32_t ret = xp_io_ioctl(m_fd, IOC_AXCL_PORT_MANAGE, &info, sizeof(info), &info, sizeof(info)); ret < 0) {
        LOG_MM_E(TAG, "request ports from device {} fail, errno: {} {}", info.device, xp_io_error(), ::strerror(xp_io_error()));
        return AXCL_ERR_DEVICE_PORT_ALLOCATE;
    }

    if (0 == info.port_num) {
        LOG_MM_E(TAG, "request 0 port from device {}", info.device);
        return AXCL_ERR_DEVICE_PORT_ALLOCATE;
    }

    for (unsigned int j = 0; j < info.port_num; ++j) {
        LOG_MM_I(TAG, "device {} => port[{}]: {}", info.device, j + 1, info.ports[j]);
    }

    return AXCL_SUCC;
}

int32_t device_manager::destroy_ports(struct device_info info) {
    LOG_MM_I(TAG, "destroy ports from device {}", info.device);

    info.cmd = AXCL_PORT_DESTROY;
    info.pid = getpid();
    if (int32_t ret = xp_io_ioctl(m_fd, IOC_AXCL_PORT_MANAGE, &info, sizeof(info), &info, sizeof(info)); ret < 0) {
        LOG_MM_E(TAG, "destroy ports from device {} fail, errno: {} {}", info.device, xp_io_error(), ::strerror(xp_io_error()));
        return AXCL_ERR_DEVICE_PORT_DESTROY;
    }

    return AXCL_SUCC;
}

int32_t device_manager::request_usr_ports(int32_t device_id, uint32_t port_num, uint32_t *ports) {
    struct device_info info = {};
    info.device = device_id;
    info.cmd = AXCL_PORT_REQUEST_USR_PORT;
    info.port_num = port_num;
    info.pid = getpid();
    if (int32_t ret = xp_io_ioctl(m_fd, IOC_AXCL_PORT_MANAGE, &info, sizeof(info), &info, sizeof(info)); ret < 0) {
        LOG_MM_E(TAG, "request usr ports from device {} fail, errno: {} {}", info.device, xp_io_error(), ::strerror(xp_io_error()));
        return AXCL_ERR_DEVICE_PORT_ALLOCATE;
    }

    for (unsigned int j = 0; j < info.port_num; ++j) {
        ports[j] = info.ports[j];
        LOG_MM_I(TAG, "device {} => usr port[{}]: {}", info.device, j + 1, info.ports[j]);
    }

    return AXCL_SUCC;
}

int32_t device_manager::destroy_usr_ports(int32_t device_id, uint32_t port_num, const uint32_t *ports) {
    struct device_info info = {};
    info.device = device_id;
    info.cmd = AXCL_PORT_RECYCLE_USR_PORT;
    info.pid = getpid();
    info.port_num = port_num;
    for (unsigned int j = 0; j < port_num; ++j) {
        info.ports[j] = ports[j];
    }

    if (int32_t ret = xp_io_ioctl(m_fd, IOC_AXCL_PORT_MANAGE, &info, sizeof(info), &info, sizeof(info)); ret < 0) {
        LOG_MM_E(TAG, "destroy usr ports from device {} fail, errno: {} {}", info.device, xp_io_error(), ::strerror(xp_io_error()));
        return AXCL_ERR_DEVICE_PORT_DESTROY;
    }

    return AXCL_SUCC;
}

}  // namespace axcl::rt
