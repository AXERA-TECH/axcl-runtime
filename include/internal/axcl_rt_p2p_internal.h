/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#ifndef __AXCL_RT_P2P_INTERNAL_H__
#define __AXCL_RT_P2P_INTERNAL_H__

#include "axcl_rt_p2p_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct p2p_info_t {
    struct axclrtP2PUnitInfo p2p_unit;                  /* input, configured by the user */
    bool bDevInited[AXCL_MAX_P2P_DEVICE_COUNT];         /* initialization of the device comm by axclrtSetDevice */
    bool bDevMemMalloced[AXCL_MAX_P2P_DEVICE_COUNT];    /* initialization of the device mem by cmm malloc api */
    uint64_t pAlignedAddr[AXCL_MAX_P2P_DEVICE_COUNT];     /* aligned addr */
} p2p_info_t;

typedef struct p2p_bwt_info_t {
    unsigned int target_id;
    unsigned int loop_count;
    unsigned int loop_interval;
    unsigned long chunk_size;
} p2p_bwt_info_t;

typedef struct axclrtP2PSrc2Dst {
    int device_id;
    unsigned int target_num;
    int timeout_ms;
	struct p2p_bwt_info_t target_info[AXCL_MAX_P2P_DEVICE_COUNT - 1];
} axclrtP2PSrc2Dst;

typedef struct axclrtBwtRes {
    unsigned int target_id;
    unsigned long long avg;
    unsigned long long min;
    unsigned long long max;
} axclrtBwtRes;

typedef struct axclrtP2PTestRes {
    struct axclrtBwtRes result;
} axclrtP2PTestRes;

typedef struct axclrtP2PTestConfig{
	int num;
  	struct axclrtP2PSrc2Dst info[AXCL_MAX_P2P_DEVICE_COUNT];
    struct axclrtP2PTestRes res[AXCL_MAX_P2P_DEVICE_COUNT];
} axclrtP2PTestConfig;

AXCL_EXPORT axclError axclrtP2PTriggerTest(AXCL_P2P_UNIT_HANDLE handle, axclrtP2PTestConfig *cfg);

#ifdef __cplusplus
}
#endif

#endif /* __AXCL_RT_P2P_INTERNAL_H__ */
