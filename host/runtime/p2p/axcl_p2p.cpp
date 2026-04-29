/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "axcl_rt_p2p.h"
#include "axcl_rt_p2p_type.h"
#include "axcl_rt_p2p_internal.h"
#include "p2p.hpp"

AXCL_EXPORT axclError axclrtCreateP2PUnit(const axclrtP2PUnitInfo *info, AXCL_P2P_UNIT_HANDLE *handle) {
    axcl::rt::p2p api;
    return api.create(info, handle);
}

AXCL_EXPORT axclError axclrtP2PTriggerTest(AXCL_P2P_UNIT_HANDLE handle, axclrtP2PTestConfig *cfg) {
    axcl::rt::p2p api;
    return api.test(handle, cfg);
}

AXCL_EXPORT axclError axclrtDestoryP2PUnit(AXCL_P2P_UNIT_HANDLE handle) {
    axcl::rt::p2p api;
    return api.destory(handle);
}
