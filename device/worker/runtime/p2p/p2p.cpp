/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "p2p.hpp"
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "dispatcher.hpp"
#include "axcl_logger.hpp"
#include "p2p_info.h"

#define TAG "p2p"

namespace axcl::worker {

p2p::p2p(dispatcher *dispatcher) : base(dispatcher), m_api(TAG) {
    m_api.append({
        RUNTIME_P2P_API_APPEND(P2P_SETUP_CONFIG),
        RUNTIME_P2P_API_APPEND(P2P_START_BANDWIDTH_TEST),
        RUNTIME_P2P_API_APPEND(P2P_REMOVE_CONFIG),
    });
}

void p2p::run(const void *data, const size_t &size) {
    m_api.run(get_dispatcher(), data, size);
}

RUNTIME_P2P_API_IMPL(P2P_SETUP_CONFIG) {
    AX_S32 nRet = AXCL_SUCC;

    /* ioctl config */
    auto config = response->get_p2p_config();

    p2p_device_config_t io_cfg;
    io_cfg.device_num = config.size();
    for (uint32_t i = 0; i < io_cfg.device_num; ++i) {
        io_cfg.devices[i].target_id = config[i].target_id;
        io_cfg.devices[i].dma_pci_addr = config[i].dma_pci_addr;
        io_cfg.devices[i].mbox_pci_addr = config[i].mbox_pci_addr;
        io_cfg.devices[i].cmm_phy_addr = config[i].cmm_phy_addr;
        io_cfg.devices[i].cmm_size = config[i].cmm_size;
    }

    int fd = ::open(AX_P2P_DEV, O_RDWR);
    if (fd < 0) {
        LOG_MM_E(TAG, "open p2p dev ({}) failed.", AX_P2P_DEV);
        nRet = -1;
        return nRet;
    }

    /* ioctl */
    nRet = ::ioctl(fd, AX_P2P_IOC_CONFIG_SET, &io_cfg);
    if (nRet < 0) {
        LOG_MM_E(TAG, "ioctl AX_P2P_IOC_CONFIG_SET failed, {}", ::strerror(errno));
        nRet = -1;
    }

    /* close fd */
    ::close(fd);

    /* return nRet, set request data: status */
    return nRet;
}

RUNTIME_P2P_API_IMPL(P2P_START_BANDWIDTH_TEST) {
    AX_S32 nRet = AXCL_SUCC;

    int fd = -1;
    fd = ::open(AX_P2P_DEV, O_RDWR);
    if (fd < 0) {
        LOG_MM_E(TAG, "open p2p dev ({}) failed.", AX_P2P_DEV);
        nRet = -1;
        return nRet;
    }

    /* src target id */
    const uint32_t target_id = response->get_target_id();
    // int32_t timeout_ms = response->get_bandwidth_test_timeout();

    /* ioctl config */
    auto config = response->get_bandwidth_test_config();
    p2p_bwt_infos_t bwt_cfg = {};
    bwt_cfg.target_num = config.size();
    for (uint32_t i = 0; i < config.size(); ++i) {
        bwt_cfg.infos[i].target_id = config[i].target_id;
        bwt_cfg.infos[i].chunk_size = config[i].chunk_size;
        bwt_cfg.infos[i].loop_interval = config[i].loop_interval;
        bwt_cfg.infos[i].loop_count = config[i].loop_count;
    }

    /* preconfig mailbox outbound address with 1st target id */
    p2p_device_config_t io_cfg;
    nRet = ::ioctl(fd, AX_P2P_IOC_CONFIG_GET, &io_cfg);
    if (nRet < 0) {
        LOG_MM_E(TAG, "ioctl AX_P2P_IOC_CONFIG_GET failed, {}", ::strerror(errno));
        ::close(fd);
        return nRet;
    }

    for (uint32_t i = 0; i < io_cfg.device_num; ++i) {
        if (io_cfg.devices[i].target_id == bwt_cfg.infos[0].target_id) {
            ::ioctl(fd, AX_P2P_IOC_MAILBOX_OUTBOUND, &io_cfg.devices[i].mbox_pci_addr);
            break;
        }
    }

    /* start bandwidth test */
    nRet = ::ioctl(fd, AX_P2P_IOC_START_BWT, &bwt_cfg);
    if (nRet < 0) {
        LOG_MM_E(TAG, "ioctl AX_P2P_IOC_START_BWT failed, {}", ::strerror(errno));
    }

    /* set request data: band width test result */
    struct axcl::pkg::runtime::p2p::bwt_result result;
    result.target_id = target_id;
    result.avg = bwt_cfg.result.avg;
    result.min = bwt_cfg.result.min;
    result.max = bwt_cfg.result.max;

    request->set_bandwidth_test_result(result);

    /* close fd */
    ::close(fd);

    /* return nRet, set request data: status */
    return nRet;
}

RUNTIME_P2P_API_IMPL(P2P_REMOVE_CONFIG) {
    AX_S32 nRet = AXCL_SUCC;

    int fd = ::open(AX_P2P_DEV, O_RDWR);
    if (fd < 0) {
        LOG_MM_E(TAG, "open p2p dev ({}) failed.", AX_P2P_DEV);
        nRet = -1;
        return nRet;
    }

    /* ioctl */
    nRet = ::ioctl(fd, AX_P2P_IOC_DISABLE);
    if (nRet < 0) {
        LOG_MM_E(TAG, "ioctl AX_P2P_IOC_DISABLE failed, {}", ::strerror(errno));
        nRet = -1;
    }

    /* close fd */
    ::close(fd);
    /* return nRet, set request data: status */
    return nRet;
}

}  // namespace axcl::worker
