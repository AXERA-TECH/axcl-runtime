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

#include <sys/ioctl.h>
#include "ax_base_type.h"

#ifdef __cplusplus
extern "C" {
#endif
#define AXCL_MAX_P2P_DEVICE_COUNT       (8)
#define AX_P2P_DEV                      ("/dev/p2p")

#define AX_P2P_IOC_BASE					'P'
#define AX_P2P_IOC_CONFIG_SET			_IOW(AX_P2P_IOC_BASE, 1, p2p_device_config_t)       /* EP, map dma_pci_addr, mailbox_pci_addr and cmm_phy_addr */
#define AX_P2P_IOC_CONFIG_GET			_IOR(AX_P2P_IOC_BASE, 2, p2p_device_config_t)       /* EP, get dma_pci_addr and mailbox_pci_addr */
#define AX_P2P_IOC_START_BWT			_IOW(AX_P2P_IOC_BASE, 3, p2p_bwt_infos_t)           /* EP, start bandwidth test */
#define AX_P2P_IOC_FETCH_BWT_RESULT		_IOR(AX_P2P_IOC_BASE, 4, p2p_bwt_result_t)          /* EP, fetch bandwidth result */
#define AX_P2P_IOC_ITEM_SEND			_IOR(AX_P2P_IOC_BASE, 5, p2p_item_send_t)
#define AX_P2P_IOC_ITEM_GET				_IOR(AX_P2P_IOC_BASE, 6, p2p_item_get_t)
#define AX_P2P_IOC_DISABLE				_IO(AX_P2P_IOC_BASE, 7)                             /* EP, disable addr mapping */
#define AX_P2P_IOC_GET_LOCAL_ID			_IOR(AX_P2P_IOC_BASE, 14, int)                      /* EP, get local id */
#define AX_P2P_IOC_MAILBOX_OUTBOUND	    _IOW(AX_P2P_IOC_BASE, 15, unsigned long long)       /* EP, mailbox outbound address */

typedef struct p2p_device_info {
    unsigned int target_id;                                                                 /* EP bus number */
    unsigned long long dma_pci_addr;                                                        /* dma pci address */
    unsigned long long mbox_pci_addr;                                                       /* mailbox pci address */
    unsigned long long cmm_phy_addr;                                                        /* cmm physical address */
    unsigned long cmm_size;                                                                 /* cmm space size */
} p2p_device_info_t;

typedef struct p2p_device_config {
    unsigned int device_num;
    p2p_device_info_t devices[AXCL_MAX_P2P_DEVICE_COUNT];
} p2p_device_config_t;

typedef struct p2p_item_send {
    AX_S32 timeout;
    AX_U32 time_consume;                                                                    /* unit is us */
    AX_U64 src_phy_addr;
    AX_U64 dst_phy_addr;
    AX_U32 transfer_size;
    AX_U32 dst_target_id;                                                                   /* bus number */
    AX_U32 customer_data[7];                                                                /* 32 bytes send */
} p2p_item_send_t;

typedef struct p2p_item_msg {
    AX_U32 src_target_id;
    AX_U32 customer_data[7]; /* 32 bytes receive */
} p2p_item_msg_t;

typedef struct p2p_item_get {
    p2p_item_msg_t msg;
} p2p_item_get_t;

#ifdef __cplusplus
}
#endif
