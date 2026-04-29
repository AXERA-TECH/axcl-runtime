/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include <gtest/gtest.h>
#include <iostream>
#include "axcl.h"

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    if (0 == ret) {
        std::cout << "============= UT PASS =============" << std::endl;
    } else {
        std::cout << "============= UT FAIL =============" << std::endl;
    }

    return ret;
}

TEST(axclTest, Case01_axclInit_null) {
    ASSERT_EQ(AXCL_SUCC, axclInit(NULL));
    ASSERT_EQ(AXCL_SUCC, axclFinalize());

    ASSERT_EQ(AXCL_SUCC, axclInit(""));
    ASSERT_EQ(AXCL_SUCC, axclFinalize());
}

TEST(axclTest, Case02_axclInit_json) {
    ASSERT_EQ(AXCL_SUCC, axclInit("/opt/bin/axcl/axcl.json"));
    ASSERT_EQ(AXCL_SUCC, axclFinalize());
}

TEST(axclTest, Case03_axclrtGetVersion) {
    int32_t major = 0;
    int32_t minor = 0;
    int32_t patch = 0;
    ASSERT_EQ(AXCL_SUCC, axclrtGetVersion(&major, &minor, &patch));
    ASSERT_NE(0, major);
}

TEST(axclTest, Case03_axclrtGetFullVersion) {
    const char* version = axclrtGetFullVersion();
    EXPECT_NE(nullptr, version);
    if (version) {
        std::cout << "full version: " << version << std::endl;
        EXPECT_STRNE(version, "0.0.0");
    }
}

TEST(axclTest, Case04_axclrtGetSocName) {
    ASSERT_EQ(AXCL_SUCC, axclInit(NULL));

    axclrtDeviceList device_list;
    axclError ret = axclrtGetDeviceList(&device_list);
    EXPECT_EQ(AXCL_SUCC, ret);
    if (ret == AXCL_SUCC) {
        EXPECT_GE(device_list.num, 1u);
        for (uint32_t i = 0; i < device_list.num; ++i) {
            ret = axclrtSetDevice(device_list.devices[i]);
            EXPECT_EQ(AXCL_SUCC, ret);
            if (ret != AXCL_SUCC) {
                continue;
            }

            const char* name = axclrtGetSocName();
            EXPECT_NE(nullptr, name);
            if (name) {
                std::cout << "device " << device_list.devices[i] << " soc name: " << name << std::endl;
                EXPECT_STRNE(name, "UNKNOWN");
            }

            ret = axclrtResetDevice(device_list.devices[i]);
            EXPECT_EQ(AXCL_SUCC, ret);
        }
    }

    ASSERT_EQ(AXCL_SUCC, axclFinalize());
}
