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

#include <cstdint>

namespace axcl::pkg::runtime::p2p {

enum class API : int {
    P2P_SETUP_CONFIG = 0,
    P2P_START_BANDWIDTH_TEST = 1,
    P2P_REMOVE_CONFIG = 2,
};

struct p2p_config {
    int32_t target_id;
    uint64_t dma_pci_addr;
    uint64_t mbox_pci_addr;
    uint64_t cmm_phy_addr;
    uint32_t cmm_size;
};

struct bwt_config {
    int32_t target_id;
    uint32_t loop_count;
    uint32_t loop_interval;
    uint32_t chunk_size;
};

struct bwt_result {
    int32_t target_id;
    uint64_t avg;
    uint64_t min;
    uint64_t max;
};

}  // namespace axcl::pkg::runtime::p2p