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

#define PACKAGE_HEAD_SIZE (sizeof(axcl::pkg::head))

namespace axcl::pkg {

/**
 * ----------------------------------------
 * |    head    |         package         |
 * ----------------------------------------
 */
enum : uint8_t { PROTO = 0, RAW = 1 };

#define GET_PACKAGE_MODULE(mask)          ((mask) & 0xFF)
#define GET_PACKAGE_SUB_MODULE(mask)      (((mask) >> 8) & 0xFF)
#define GET_PACKAGE_SUB_MODULE_API(mask)  (((mask) >> 16) & 0xFF)
#define GET_PACKAGE_TYPE(mask)            (((mask) >> 30) & 0x3)
#define CREATE_PACKAGE_HEAD_TYPE(type, mod, sub, api) \
    (((static_cast<uint8_t>(type) & 0x3) << 30) | ((static_cast<uint8_t>(api) & 0xFF) << 16) | ((static_cast<uint8_t>(sub) & 0xFF) << 8) | (static_cast<uint8_t>(mod) & 0xFF))

#define RUNTIME_MEMCPY_HANDLER_TYPE (CREATE_PACKAGE_HEAD_TYPE(axcl::pkg::RAW, 0/* runtime */, 1 /* memory */, 5 /* read/write */) | (0x1 /* memcpy */ << 24))
#define RUNTIME_MEMCPY_ASYNC_HANDLER_TYPE (CREATE_PACKAGE_HEAD_TYPE(axcl::pkg::RAW, 0/* runtime */, 1 /* memory */, 6 /* read/write */) | (0x1 /* memcpy */ << 24))

#pragma pack(push, 1)

struct head {
    /*
     * |31 30| 29 ... ... 24 | 23 ... ... 16 | 15 ... ... 8 | 7 ... ... 0 |
     *  0: proto
     *  1: raw
     *                                                      |    module   |
     *                                                      | 0: runtime  |
     *                                                      | 1: native   |
     *                                       | sub module   |
     *                                       | vdec,venc..  |
     *                       |      api      |
     *       |    handler    |
     *       | 01: memcpy    |
     */
    uint32_t type;
    uint64_t seq_num;
    uint32_t device;
    uint32_t context;
    uint32_t stream;
    uint32_t command;  // for control-cmd
};

/**
 * memcpy between host and device
*/
enum memcpy_kind : uint8_t {
    DEVICE_TO_HOST_VIR = 0,
    HOST_VIR_TO_DEVICE = 1,
    DEVICE_TO_HOST_PHY = 2,
    HOST_PHY_TO_DEVICE = 3,
};

struct memcpy_package {
    struct head hd;
    memcpy_kind kind;
    uint64_t src_phy;
    uint64_t dst_phy;
    void *data;
    uint32_t size;
};

#pragma pack(pop)

}  // namespace axcl::pkg
