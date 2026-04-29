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

#include <memory>
#include <mutex>
#include <unordered_map>
#include "axcl_rt_device_internal.h"
#include "axcl_rt_type.h"
#include "device_info.h"
#include "singleton.hpp"
#include "threadx.hpp"
#include "os.hpp"

namespace axcl::rt {

class device;
class device_manager : public singleton<device_manager> {
    friend class singleton<device_manager>;

public:
    int32_t init();
    int32_t deinit();

    int32_t active(int32_t device_id);
    int32_t deactive(int32_t device_id, bool force = false);
    bool is_actived(int32_t device_id);

    int32_t sync();

    device *get_device(int32_t device_id) {
        return m_devices[device_id].first.get();
    }

    uint32_t get_device_count() const {
        return m_devices.size();
    }

    uint32_t get_device_list(axclrtDeviceList *list) const;

    /* get device id of current thread */
    int32_t get_current_thread_device_id();

    int32_t get_bus_info(int32_t device_id, axclrtDeviceBusInfo &info);
    int32_t get_pid_info(int32_t device_id, axclrtDevicePidInfo &info);

    /* deactive -> reboot -> active */
    int32_t reboot(int32_t device_id);

    void bind_thread(uint32_t tid, int32_t device_id);
    void unbind_thread(uint32_t tid);

    int32_t request_usr_ports(int32_t device_id, uint32_t port_num, uint32_t *ports);
    int32_t destroy_usr_ports(int32_t device_id, uint32_t port_num, const uint32_t *ports);

protected:
    device_manager() = default;

    int32_t probe();
    void listen();

    int32_t request_ports(int32_t device_id, struct device_info &info);
    int32_t destroy_ports(struct device_info info);

private:
    xp_dev_t m_fd = XP_INVALID_DEV;
    int32_t m_ref_cnt = 0;
    axcl::threadx m_listen;
    std::mutex m_mtx_mgr;

    /* device id -> device */
    std::mutex m_mtx_devices;
    std::unordered_map<int32_t /* device id */, std::pair<std::shared_ptr<device>, uint32_t /* ref cnt */>> m_devices;

    /* thread id -> device id */
    std::mutex m_mtx_thread_device_map;
    std::unordered_map<uint32_t /* thread id */, int32_t /* device id */> m_thread_device_map;
};

}  // namespace axcl::rt

#define DEVICE_MANAGER() axcl::rt::device_manager::get_instance()
