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
#include <chrono>
#include <iostream>
#include <thread>
#include <atomic>
#include "axcl.h"

int main(int argc, char **argv) {
    if (axclError ret = axclInit(""); AXCL_SUCC != ret) {
        printf("axcl init fail, ret = 0x%x\n", ret);
        return 1;
    }

    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    if (0 == ret) {
        std::cout << "============= UT PASS =============" << std::endl;
    } else {
        std::cout << "============= UT FAIL =============" << std::endl;
    }

    axclFinalize();
    return ret;
}

class axclrtDeviceTest : public testing::Test {
protected:
    static void SetUpTestSuite() {
    }

    static void TearDownTestSuite() {
    }
};

TEST_F(axclrtDeviceTest, Case01_axclrtGetDeviceCount) {
    uint32_t count = 0;
    ASSERT_EQ(AXCL_SUCC, (uint32_t)axclrtGetDeviceCount(&count));
    EXPECT_GE(count, 1u);
    EXPECT_LE(count, static_cast<uint32_t>(AXCL_MAX_DEVICE_COUNT));
}

TEST_F(axclrtDeviceTest, Case02_axclrtGetDeviceList) {
    axclrtDeviceList lst;
    ASSERT_EQ(AXCL_SUCC, (uint32_t)axclrtGetDeviceList(&lst));
    EXPECT_GE(lst.num, 1u);
    for (uint32_t i = 0; i < lst.num; ++i) {
        EXPECT_GE(lst.devices[i], 1);
        EXPECT_LE(lst.devices[i], 256);
    }
}

TEST_F(axclrtDeviceTest, Case03_axclrtSetAndResetDevice) {
    axclrtDeviceList lst;
    ASSERT_EQ(AXCL_SUCC, (uint32_t)axclrtGetDeviceList(&lst));
    EXPECT_GE(lst.num, 1u);
    for (uint32_t i = 0; i < lst.num; ++i) {
        for (int j = 0; j < 5; ++j) {
            ASSERT_EQ(AXCL_SUCC, (uint32_t)axclrtSetDevice(lst.devices[i]));
            ASSERT_EQ(AXCL_SUCC, (uint32_t)axclrtResetDevice(lst.devices[i]));
        }
    }
}
