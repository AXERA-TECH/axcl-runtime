/**************************************************************************************************
 *
 * Copyright (c) 2019-2025 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#pragma once

#include "axcl_rt_type.h"
#include "axcl_rt_type_internal.h"
#include "axcl_rt_p2p_type.h"
#include "axcl_rt_p2p_internal.h"

#include "package/host/runtime/p2p/p2p_request.hpp"
#include "package/host/runtime/p2p/p2p_response.hpp"
#include "package/include/runtime/p2p/type.hpp"
#include "threadx.hpp"
#include "os.hpp"

// clang-format off
#if defined(WINDOWS)
    #define AX_P2P_DEV                  "p2p"
    #define AX_P2P_IOC_BASE             0x60
#else
    #define AX_P2P_DEV                  "/dev/p2p"
    #define AX_P2P_IOC_BASE             'P'
#endif

#define AX_P2P_IOC_CONFIG_GET           AX_IOR(AX_P2P_IOC_BASE, 2, p2p_device_config_t)       /* HOST, get dma_pci_addr and mailbox_pci_addr */
// clang-format on

typedef struct p2p_device_info {
	unsigned int target_id;			    /* EP bus number */
	unsigned long long dma_pci_addr;	/* dma pci address */
	unsigned long long mbox_pci_addr;	/* mailbox pci address */
	unsigned long long cmm_phy_addr;	/* cmm physical address */
	unsigned long long cmm_size;        /* cmm space size */
} p2p_device_info_t;

typedef struct p2p_device_config {
    unsigned int device_num;
    p2p_device_info_t devices[AXCL_MAX_P2P_DEVICE_COUNT];
} p2p_device_config_t;

namespace axcl::rt {

class p2p {
public:
    p2p() = default;
    axclError create(const axclrtP2PUnitInfo *info, AXCL_P2P_UNIT_HANDLE *handle);
    axclError destory(AXCL_P2P_UNIT_HANDLE handle);
    axclError test(AXCL_P2P_UNIT_HANDLE handle, axclrtP2PTestConfig *cfg);

protected:
    axclError get_p2p_config(p2p_device_config_t *p2p_cfg);
    axclError malloc_device_cmm(int32_t device_id, uint32_t mem_size, uint64_t *algin_addr);
    axclError free_device_cmm(int32_t device_id, uint64_t real_addr);
    axclError setup_device_p2pcfg(int32_t device_id, int32_t timeout);
    axclError remove_device_p2pcfg(int32_t device_id, int32_t timeout);
    axclError get_runtime_environment(axclrtStream &stream, axclrtEnvironment &env);
    void test_thread(void* config, void* result, axclError *ret);


private:
    axcl::threadx m_thread[AXCL_MAX_P2P_DEVICE_COUNT];

    axcl::pkg::host::p2p_request m_req;
    axcl::pkg::host::p2p_response m_rsp;
};

}