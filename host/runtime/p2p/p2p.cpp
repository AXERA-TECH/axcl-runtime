/**************************************************************************************************
 *
 * Copyright (c) 2019-2025 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "p2p.hpp"
#include "axcl_rt.h"
#include "axcl_rt_internal.h"
#include "axcl_rt_memory.h"
#include "device/device.hpp"
#include "axcl_logger.hpp"
#include "os.hpp"
#include "axcl.h"

#include <stdio.h>
#include <cstdlib>
#include <utility>

#define TAG "p2p"

#define TIMEOUT_3S  (3000)

// #define FOR_ADDR_TEST

axclError axcl::rt::p2p::create(const axclrtP2PUnitInfo *info, AXCL_P2P_UNIT_HANDLE *handle) {
    LOG_MM_D(TAG, "+++");

    axclError nRet = AXCL_SUCC;
    bool bP2PConfigured = false; /* Flag to track P2P configuration status */
    p2p_info_t *pHandleInfo = nullptr;
    const uint32_t u32Num = info->u32DeviceNum;

    /* Allocate and initialize handle structure */
    pHandleInfo = static_cast<p2p_info_t*>(malloc(sizeof(p2p_info_t)));
    if (!pHandleInfo) {
        LOG_MM_E(TAG, "Failed to allocate pHandleInfo");
        return AXCL_ERR_P2P_INIT;
    }
    memset(pHandleInfo, 0, sizeof(p2p_info_t));
    memcpy(&pHandleInfo->p2p_unit, info, sizeof(axclrtP2PUnitInfo)); /* Copy to correct struct member */

    /* Arrays to track resource addresses */
    uint64_t phyAddr[AXCL_MAX_P2P_DEVICE_COUNT] = {0};
    uint64_t dmaAddr[AXCL_MAX_P2P_DEVICE_COUNT] = {0};
    uint64_t mboxAddr[AXCL_MAX_P2P_DEVICE_COUNT] = {0};

    /* Step 1: Initialize PCIe communication for devices */
    for (uint32_t i = 0; i < u32Num; ++i) {
        nRet = axclrtSetDevice(info->n32DeviceId[i]);
        if (AXCL_SUCC != nRet) {
            LOG_MM_E(TAG, "axclrtSetDevice failed for dev_id={}", info->n32DeviceId[i]);
            goto CLEANUP;
        }
        pHandleInfo->bDevInited[i] = true; /* Mark device as initialized */
    }

    /* Step 2: Allocate device memory and get physical addresses */
    for (uint32_t i = 0; i < u32Num; ++i) {
        nRet = malloc_device_cmm(info->n32DeviceId[i], info->u32DeviceMemSize[i], &phyAddr[i]);
        if (AXCL_SUCC != nRet) {
            LOG_MM_E(TAG, "malloc_device_cmm failed for dev_id={}", info->n32DeviceId[i]);
            goto CLEANUP;
        }
        pHandleInfo->bDevMemMalloced[i] = true; /* Mark memory as allocated */
        pHandleInfo->pAlignedAddr[i] = phyAddr[i];
    }

    /* Step 3: Get P2P configuration (DMA and Mailbox addresses) */
    p2p_device_config_t p2pCfg;
    memset(&p2pCfg, 0, sizeof(p2pCfg));
    nRet = get_p2p_config(&p2pCfg);
    if (AXCL_SUCC != nRet) {
        LOG_MM_E(TAG, "get_p2p_config failed");
        goto CLEANUP;
    }

    /* Map PCI addresses for each device */
    for (uint32_t i = 0; i < u32Num; ++i) {
        for (uint32_t j = 0; j < p2pCfg.device_num; ++j) {
            if (info->n32DeviceId[i] == (int32_t)p2pCfg.devices[j].target_id) {
                dmaAddr[i] = p2pCfg.devices[j].dma_pci_addr;
                mboxAddr[i] = p2pCfg.devices[j].mbox_pci_addr;
                break;
            }
        }
    }

    /* Step 4: Configure P2P parameters and submit request */
    struct axcl::pkg::runtime::p2p::p2p_config config[AXCL_MAX_P2P_DEVICE_COUNT];
    for (uint32_t i = 0; i < u32Num; ++i) {
        config[i].target_id = info->n32DeviceId[i];
        config[i].dma_pci_addr = dmaAddr[i];
        config[i].mbox_pci_addr = mboxAddr[i];
        config[i].cmm_phy_addr = phyAddr[i];
        config[i].cmm_size = info->u32DeviceMemSize[i];
        LOG_MM_I(TAG, "[{}] config: dev_id={}, dma={:#X}, mbox={:#X}, cmm={:#X}, size={}",
                 i,
                 config[i].target_id,
                 config[i].dma_pci_addr,
                 config[i].mbox_pci_addr,
                 config[i].cmm_phy_addr,
                 config[i].cmm_size);
    }
    m_req.setup_p2p_config(config, u32Num);

    /* Step 5: Apply P2P configuration via ioctl */
    for (uint32_t i = 0; i < u32Num; ++i) {
        nRet = setup_device_p2pcfg(info->n32DeviceId[i], TIMEOUT_3S);
        if (AXCL_SUCC != nRet) {
            LOG_MM_E(TAG, "setup_device_p2pcfg failed for dev_id={}", info->n32DeviceId[i]);
            goto CLEANUP;
        }
    }
    bP2PConfigured = true; /* Mark P2P configuration as applied */

    /* Success path */
    *handle = static_cast<AXCL_P2P_UNIT_HANDLE>(pHandleInfo);
    LOG_MM_D(TAG, "---");
    return AXCL_SUCC;

CLEANUP:
    if (pHandleInfo) {
        /* Step 1: Remove P2P configuration (only for configured devices) */
        if (bP2PConfigured) {
            for (uint32_t i = 0; i < u32Num; ++i) {
                if (pHandleInfo->bDevInited[i]) {
                    m_req.remove_p2p_config();
                    nRet = remove_device_p2pcfg(info->n32DeviceId[i], TIMEOUT_3S);
                    if (AXCL_SUCC != nRet) {
                        LOG_MM_E(TAG, "remove_device_p2pcfg failed for dev_id={}", info->n32DeviceId[i]);
                    }
                }
            }
        }

        /* Step 2: Free device memory (only allocated ones) */
        for (uint32_t i = 0; i < u32Num; ++i) {
            if (pHandleInfo->bDevMemMalloced[i] && pHandleInfo->pAlignedAddr[i]) {
                free_device_cmm(info->n32DeviceId[i], pHandleInfo->pAlignedAddr[i]);
                pHandleInfo->pAlignedAddr[i] = 0; /* Prevent double-free */
            }
        }

        /* Step 3: Reset devices (only initialized ones) */
        for (uint32_t i = 0; i < u32Num; ++i) {
            if (pHandleInfo->bDevInited[i]) {
                nRet = axclrtResetDevice(info->n32DeviceId[i]);
                if (AXCL_SUCC != nRet) {
                    LOG_MM_E(TAG, "axclrtResetDevice failed for dev_id={}", info->n32DeviceId[i]);
                }
            }
        }

        /* Free handle structure */
        free(pHandleInfo);
        *handle = nullptr;
    }

    LOG_MM_D(TAG, "---");
    return AXCL_ERR_P2P_INIT;
}

axclError axcl::rt::p2p::destory(AXCL_P2P_UNIT_HANDLE handle) {
    axclError nRet = AXCL_SUCC;
    if (handle == nullptr) {
        return nRet;
    }

    p2p_info_t *pHandleInfo = static_cast<p2p_info_t*>(handle);
    uint32_t u32Num = pHandleInfo->p2p_unit.u32DeviceNum;

     /* free all allocated resources */
    /* step 0. deinit p2p config for pci domain(dma and mailbox) */
    for (uint32_t i = 0; i < u32Num; ++i) {
        m_req.remove_p2p_config();
        nRet = remove_device_p2pcfg(pHandleInfo->p2p_unit.n32DeviceId[i], TIMEOUT_3S);
        if (AXCL_SUCC != nRet) {
            LOG_MM_E(TAG, "remove_device_p2pcfg failed, dev_id={}", pHandleInfo->p2p_unit.n32DeviceId[i]);
        }
    }

    /* step 1. free device mem */
    for (uint32_t i = 0; i < u32Num; ++i) {
        if (pHandleInfo->bDevMemMalloced[i] && pHandleInfo->pAlignedAddr[i]) {
            free_device_cmm(pHandleInfo->p2p_unit.n32DeviceId[i], pHandleInfo->pAlignedAddr[i]);
        }
    }

    /* step 2. reset device */
    for (uint32_t i = 0; i < u32Num; ++i) {
        if (pHandleInfo->bDevInited[i]) {
            nRet = axclrtResetDevice(pHandleInfo->p2p_unit.n32DeviceId[i]);
            if (AXCL_SUCC != nRet) {
                LOG_MM_E(TAG, "axclrtResetDevice failed, dev_id={}", pHandleInfo->p2p_unit.n32DeviceId[i]);
            }
        }
    }

    /* free handle */
    if (handle != nullptr) {
        free(handle);
        handle = nullptr;
    }

    return nRet;
}

axclError axcl::rt::p2p::test(AXCL_P2P_UNIT_HANDLE handle, axclrtP2PTestConfig *cfg) {
    axclError nRet = AXCL_SUCC;

    /* create bwt thread */
    axclError rets[AXCL_MAX_P2P_DEVICE_COUNT];
    for (int i = 0; i < cfg->num;  ++ i) {
        m_thread[i].start("p2pbwt_" + std::to_string(i), SCHED_FIFO, 99, &axcl::rt::p2p::test_thread, this,
            static_cast<void*>(&(cfg->info[i])),
            static_cast<void*>(&(cfg->res[i])),
            &rets[i]);
    }

    /* join and stop */
    for (int i = 0; i < cfg->num;  ++ i) {
        m_thread[i].stop();
        m_thread[i].join();
        if (rets[i] != AXCL_SUCC) {
            nRet = rets[i];
        }
    }

    return nRet;
}

axclError axcl::rt::p2p::get_p2p_config(p2p_device_config_t *p2p_cfg) {
    axclError nRet = AXCL_SUCC;

    static xp_dev_t fd = XP_INVALID_DEV;
    fd = xp_io_open(AX_P2P_DEV);
    if (fd == XP_INVALID_DEV) {
        LOG_MM_E(TAG, "open p2p dev ({}) failed.", AX_P2P_DEV);
        nRet = -1;
        return nRet;
    }

    nRet = xp_io_ioctl(fd, AX_P2P_IOC_CONFIG_GET, p2p_cfg, sizeof(*p2p_cfg), p2p_cfg, sizeof(*p2p_cfg));
    if (nRet < 0) {
        LOG_MM_E(TAG, "ioctl IOC_AX_P2P_CONFIG_GET failed, {}", ::strerror(xp_io_error()));
    }

    xp_io_close(fd);

    return nRet;
}

axclError axcl::rt::p2p::free_device_cmm(int32_t device_id, uint64_t original_addr) {
    axclError nRet = AXCL_SUCC;

    axclrtContext context;
    axclrtGetDefaultContext(&context, device_id);
    axclrtSetCurrentContext(context);

    nRet = axclrtFree(reinterpret_cast<void*>(original_addr));
    if (AXCL_SUCC != nRet) {
        LOG_MM_E(TAG, "axclrtFree failed, dev_id={}, addr={:#X}", device_id, original_addr);
    }

    return nRet;
}

axclError axcl::rt::p2p::malloc_device_cmm(int32_t device_id, uint32_t mem_size, uint64_t *aligned_addr) {
    axclError nRet = AXCL_SUCC;

    axclrtContext context;
    axclrtGetDefaultContext(&context, device_id);
    axclrtSetCurrentContext(context);

    void* _original = nullptr;
    nRet = axclrtMalloc(&_original, mem_size, AXCL_MEM_MALLOC_SIZE_ALIGN);
    if (AXCL_SUCC != nRet) {
        LOG_MM_E(TAG, "axclrtMalloc failed, dev_id={}", device_id);
        return AXCL_ERR_P2P_NO_MEMORY;
    }
    uint64_t _original_addr = reinterpret_cast<uint64_t>(_original);
    *aligned_addr = _original_addr;

    return nRet;
}

axclError axcl::rt::p2p::setup_device_p2pcfg(int32_t device_id, int32_t timeout) {
    axclError nRet = AXCL_SUCC;

    axclrtContext context;
    axclrtGetDefaultContext(&context, device_id);
    axclrtSetCurrentContext(context);

    axclrtStream stream;
    axclrtEnvironment env;
    nRet = get_runtime_environment(stream, env);
    if (AXCL_SUCC != nRet) {
        LOG_MM_E(TAG, "get_runtime_environment failed, dev_id={}", device_id);
        return AXCL_ERR_P2P_INIT;
    }

    /* pkg and encode */
    /* head */
    axcl::pkg::head hd;
    hd.seq_num = axclrtGetToken();
    hd.device = env.device;
    hd.context = env.context;
    hd.stream = env.stream;
    hd.type = CREATE_PACKAGE_HEAD_TYPE(axcl::pkg::PROTO,
                            axcl::pkg::module::RUNTIME,
                            axcl::pkg::runtime::module::P2P,
                            axcl::pkg::runtime::p2p::API::P2P_SETUP_CONFIG);
    hd.command = 0;

    /* pid and tid */
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
        LOG_MM_E(TAG, "encode failed, dev_id={}", device_id);
        return AXCL_ERR_P2P_INIT;
    }

    /* create task */
    axclrtTask task;
    nRet = axclrtCreateTask(&task, stream, hd.seq_num, pkg, len, timeout, true/* has response */, axcl::comm::PCIE_MSG);
    if ( AXCL_SUCC != nRet) {
        LOG_MM_E(TAG, "axclrtCreateTask failed, dev_id={}", device_id);
        return AXCL_ERR_P2P_INIT;
    }

    /* wait task finish */
    auto deleter = [](axclrtTask task) { axclrtDestoryTask(task); };
    std::unique_ptr<void, decltype(deleter)> sp(task, deleter);
    if (nRet = axclrtWaitTaskFinish(task); AXCL_SUCC != nRet) {
        LOG_MM_E(TAG, "axclrtWaitTaskFinish failed, dev_id={}", device_id);
        return AXCL_ERR_P2P_INIT;
    }

    /* get response */
    nRet = axclrtGetTaskResponse(task, &pkg, &len);
    if (AXCL_SUCC != nRet) {
        LOG_MM_E(TAG, "axclrtGetTaskResponse failed, dev_id={}", device_id);
        return AXCL_ERR_P2P_INIT;
    }

    /* decode and check status */
    if (!m_rsp.decode(pkg, len)) {
        LOG_MM_E(TAG, "decode failed, dev_id={}", device_id);
        return AXCL_ERR_P2P_INIT;
    }

    int32_t nStatus = m_rsp.get_status();
    if (0 != nStatus) {
        LOG_MM_E(TAG, "status code ({}) of response is incorrect, dev_id={}", nStatus, device_id);
        return AXCL_ERR_P2P_INIT;
    }

    return AXCL_SUCC;
}

axclError axcl::rt::p2p::remove_device_p2pcfg(int32_t device_id, int32_t timeout) {
    axclError nRet = AXCL_SUCC;

    axclrtContext context;
    axclrtGetDefaultContext(&context, device_id);
    axclrtSetCurrentContext(context);

    axclrtStream stream;
    axclrtEnvironment env;
    nRet = get_runtime_environment(stream, env);
    if (AXCL_SUCC != nRet) {
        LOG_MM_E(TAG, "get_runtime_environment failed, dev_id={}", device_id);
        return AXCL_ERR_P2P_INIT;
    }

    /* pkg and encode */
    /* head */
    axcl::pkg::head hd;
    hd.seq_num = axclrtGetToken();
    hd.device = env.device;
    hd.context = env.context;
    hd.stream = env.stream;
    hd.type = CREATE_PACKAGE_HEAD_TYPE(axcl::pkg::PROTO,
                            axcl::pkg::module::RUNTIME,
                            axcl::pkg::runtime::module::P2P,
                            axcl::pkg::runtime::p2p::API::P2P_REMOVE_CONFIG);
    hd.command = 0;

    /* pid and tid */
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
        LOG_MM_E(TAG, "encode failed, dev_id={}", device_id);
        return AXCL_ERR_P2P_INIT;
    }

    /* create task */
    axclrtTask task;
    nRet = axclrtCreateTask(&task, stream, hd.seq_num, pkg, len, timeout, true/* has response */, axcl::comm::PCIE_MSG);
    if ( AXCL_SUCC != nRet) {
        LOG_MM_E(TAG, "axclrtCreateTask failed, dev_id={}", device_id);
        return AXCL_ERR_P2P_INIT;
    }

    /* wait task finish */
    auto deleter = [](axclrtTask task) { axclrtDestoryTask(task); };
    std::unique_ptr<void, decltype(deleter)> sp(task, deleter);
    if (nRet = axclrtWaitTaskFinish(task); AXCL_SUCC != nRet) {
        LOG_MM_E(TAG, "axclrtWaitTaskFinish failed, dev_id={}", device_id);
        return AXCL_ERR_P2P_INIT;
    }

    /* get response */
    nRet = axclrtGetTaskResponse(task, &pkg, &len);
    if (AXCL_SUCC != nRet) {
        LOG_MM_E(TAG, "axclrtGetTaskResponse failed, dev_id={}", device_id);
        return AXCL_ERR_P2P_INIT;
    }

    /* decode and check status */
    if (!m_rsp.decode(pkg, len)) {
        LOG_MM_E(TAG, "decode failed, dev_id={}", device_id);
        return AXCL_ERR_P2P_INIT;
    }

    int32_t nStatus = m_rsp.get_status();
    if (0 != nStatus) {
        LOG_MM_E(TAG, "status code ({}) of response is incorrect, dev_id={}", nStatus, device_id);
        return AXCL_ERR_P2P_INIT;
    }

    return AXCL_SUCC;
}

void axcl::rt::p2p::test_thread(void* config, void* res, axclError *nRet) {
    constexpr int API_P2P_BWT_TEST = 1;

    axclrtP2PSrc2Dst* pInfo = static_cast<axclrtP2PSrc2Dst*>(config);
    axclrtP2PTestRes* pResult = static_cast<axclrtP2PTestRes*>(res);
    int32_t device_id = pInfo->device_id; /* src target id */
    int32_t timeout_ms = pInfo->timeout_ms;

    axclrtContext context;
    *nRet = axclrtCreateContext(&context, device_id);
    if (AXCL_SUCC == *nRet) {
        do {
            axcl::pkg::host::p2p_request req;
            axcl::pkg::host::p2p_response rsp;

            /* config request for band width test(bwt) */
            uint32_t num = pInfo->target_num;
            struct axcl::pkg::runtime::p2p::bwt_config bwt_config[AXCL_MAX_P2P_DEVICE_COUNT - 1];
            for (uint32_t i = 0; i < num; ++i) {
                auto &item = pInfo->target_info[i];
                bwt_config[i].target_id = item.target_id;
                bwt_config[i].chunk_size = item.chunk_size;
                bwt_config[i].loop_count = item.loop_count;
                bwt_config[i].loop_interval = item.loop_interval;
            }

            req.start_bandwidth_test(device_id, bwt_config, num, timeout_ms);

            axclrtTask task;
            axclrtStream stream;
            axclrtEnvironment env;
            *nRet = get_runtime_environment(stream, env);
            if (AXCL_SUCC != *nRet) {
                LOG_MM_E(TAG, "get_runtime_environment failed, dev_id={}", device_id);
                break;
            }

            /* pkg and encode */
            /* head */
            axcl::pkg::head hd;
            hd.seq_num = axclrtGetToken();
            hd.device = env.device;
            hd.context = env.context;
            hd.stream = env.stream;
            hd.type = CREATE_PACKAGE_HEAD_TYPE(axcl::pkg::PROTO, axcl::pkg::module::RUNTIME, axcl::pkg::runtime::module::P2P, API_P2P_BWT_TEST);
            hd.command = 0;

            /* pid and tid */
            static uint32_t pid = 0;
            if (0 == pid) {
                pid = static_cast<uint32_t>(getpid());
            }
            static thread_local uint32_t tid = 0;
            if (0 == tid) {
                tid = gettid();
            }
            req.set_env({pid, tid});

            uint32_t len;
            void *pkg = req.encode(hd, len);
            if (!pkg) {
                LOG_MM_E(TAG, "encode failed, dev_id={}", device_id);
                *nRet = AXCL_ERR_P2P_BWT;
                break;
            }

            /* create task */
            *nRet = axclrtCreateTask(&task, stream, hd.seq_num, pkg, len, timeout_ms + TIMEOUT_3S, true/* has response */, axcl::comm::PCIE_MSG);
            if (AXCL_SUCC != *nRet) {
                LOG_MM_E(TAG, "axclrtCreateTask failed, dev_id={}", device_id);
                break;
            }

            /* wait task finish */
            auto deleter = [](axclrtTask task) { axclrtDestoryTask(task); };
            std::unique_ptr<void, decltype(deleter)> sp(task, deleter);
            *nRet = axclrtWaitTaskFinish(task);
            if (AXCL_SUCC != *nRet) {
                LOG_MM_E(TAG, "axclrtWaitTaskFinish failed, dev_id={}", device_id);
                break;
            }

            /* get response */
            *nRet = axclrtGetTaskResponse(task, &pkg, &len);
            if (AXCL_SUCC != *nRet) {
                LOG_MM_E(TAG, "axclrtGetTaskResponse failed, dev_id={}", device_id);
                break;
            }

            /* decode and check status */
            if (!rsp.decode(pkg, len)) {
                LOG_MM_E(TAG, "decode failed, dev_id={}", device_id);
                *nRet = AXCL_ERR_P2P_BWT;
                break;

            }

            if (0 != rsp.get_status()) {
                LOG_MM_E(TAG, "get_status failed, dev_id={}", device_id);
                *nRet = AXCL_ERR_P2P_BWT;
                break;
            }

            auto result = rsp.get_bandwidth_test_result();
            pResult->result.target_id = result.target_id;
            pResult->result.avg = result.avg;
            pResult->result.min = result.min;
            pResult->result.max = result.max;

            *nRet = AXCL_SUCC;
        } while (0);

        axclrtDestroyContext(context);
    }
}

axclError axcl::rt::p2p::get_runtime_environment(axclrtStream &stream, axclrtEnvironment &env) {
    axclError nRet;
    axclrtContext context;
    if (nRet = axclrtGetCurrentContext(&context); AXCL_SUCC != nRet) {
        return nRet;
    }

    if (nRet = axclrtGetDefaultStream(context, &stream); AXCL_SUCC != nRet) {
        return nRet;
    }

    if (nRet = axclrtGetEnviromentByStream(stream, &env); AXCL_SUCC != nRet) {
        return nRet;
    }

    return AXCL_SUCC;
}