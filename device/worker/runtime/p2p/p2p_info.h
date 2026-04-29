/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/
#ifndef __P2P_INFO_H__
#define __P2P_INFO_H__

#include <sys/ioctl.h>
#include "ax_base_type.h"

#ifdef __cplusplus
extern "C" {
#endif
#define AXCL_MAX_P2P_DEVICE_COUNT       (8)
#define AX_P2P_DEV                      ("/dev/p2p")

#define AX_P2P_IOC_BASE			        'P'
#define AX_P2P_IOC_CONFIG_SET		    _IOW(AX_P2P_IOC_BASE, 1, p2p_device_config_t)       /* EP, map dma_pci_addr, mailbox_pci_addr and cmm_phy_addr */
#define AX_P2P_IOC_CONFIG_GET		    _IOR(AX_P2P_IOC_BASE, 2, p2p_device_config_t)       /* EP, get dma_pci_addr and mailbox_pci_addr */
#define AX_P2P_IOC_START_BWT		    _IOW(AX_P2P_IOC_BASE, 3, p2p_bwt_infos_t)           /* EP, start band width test */
#define AX_P2P_IOC_DISABLE              _IO(AX_P2P_IOC_BASE, 7)                      		/* EP, disable addr mapping */
#define AX_P2P_IOC_MAILBOX_OUTBOUND     _IOW(AX_P2P_IOC_BASE, 15, unsigned long long)     /* EP, set mailbox outbound address */

typedef struct p2p_device_info {
	unsigned int target_id;			    /* EP bus number */
	unsigned long long dma_pci_addr;	/* dma pci address */
	unsigned long long mbox_pci_addr;	/* mailbox pci address */
	unsigned long long cmm_phy_addr;	/* cmm physical address */
	unsigned long long cmm_size;	    /* cmm space size */
} p2p_device_info_t;

typedef struct p2p_device_config {
    unsigned int device_num;
    p2p_device_info_t devices[AXCL_MAX_P2P_DEVICE_COUNT];
} p2p_device_config_t;

typedef struct p2p_bwt_info {
	unsigned int target_id;			// bus number of the target device
	unsigned int loop_count;		// test count
	unsigned int loop_interval;		// time between transmissions, us unit
	unsigned long chunk_size;		// size of the bytes sent, which is less than the allocated CMM size
} p2p_bwt_info_t;

typedef struct p2p_bwt_infos_t  {
    unsigned int target_num;
    p2p_bwt_info_t infos[AXCL_MAX_P2P_DEVICE_COUNT - 1];
	struct {
		unsigned long long max;
		unsigned long long min;
		unsigned long long avg;
	} result;
} p2p_bwt_infos_t;

#ifdef __cplusplus
}
#endif

#endif /* __P2P_INFO_H__ */
