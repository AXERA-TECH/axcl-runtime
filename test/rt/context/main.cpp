/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "cmdline.h"
#include <gtest/gtest.h>
#include <iostream>
#include <thread>
#include "axcl.h"

static uint32_t device_id = -1;

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    cmdline::parser parser;
    parser.add<uint32_t>("device", 'd', "device index", false, 0);
    parser.parse_check(argc, argv);
    uint32_t device_index = parser.get<uint32_t>("device");
    if (axclError ret = axclInit(""); AXCL_SUCC != ret) {
        printf("axcl init fail, ret = 0x%x\n", ret);
        return 1;
    }

    axclrtDeviceList device_list;
    if (axclError ret = axclrtGetDeviceList(&device_list); AXCL_SUCC != ret || 0 == device_list.num) {
        printf("no device is connected\n");
        axclFinalize();
        return 1;
    }

    if (device_index >= device_list.num) {
        printf("device index %d is out of connected device num %d\n", device_index, device_list.num);
        axclFinalize();
        return 1;
    }

    device_id = device_list.devices[device_index];
    printf("device index: %d, bus number: %d\n", device_index, device_id);

    if (axclError ret = axclrtSetDevice(device_id); AXCL_SUCC != ret) {
        printf("active device, ret = 0x%x\n", ret);
        axclFinalize();
        return 1;
    }

    int ret = RUN_ALL_TESTS();
    if (0 == ret) {
        std::cout << "============= UT PASS =============" << std::endl;
    } else {
        std::cout << "============= UT FAIL =============" << std::endl;
    }

    axclrtResetDevice(device_id);
    axclFinalize();
    return ret;
}

class axclrtContextTest : public testing::Test {
protected:
    static void SetUpTestSuite() {
    }

    static void TearDownTestSuite() {
    }
};

TEST_F(axclrtContextTest, Case01_axclrtCreateAndDestoryContext) {
    axclrtContext context;
    ASSERT_EQ(AXCL_SUCC, axclrtCreateContext(&context, device_id));

    constexpr int THREAD_NUM = 16;
    std::thread t[THREAD_NUM];
    for (int i = 0; i < THREAD_NUM; ++i) {
        t[i] = std::thread([]() {
            axclrtContext context;
            EXPECT_EQ(AXCL_SUCC, axclrtCreateContext(&context, device_id));
            EXPECT_EQ(AXCL_SUCC, axclrtDestroyContext(context));
        });
    }

    for (auto&& m : t) {
        m.join();
    }

    EXPECT_EQ(AXCL_SUCC, axclrtDestroyContext(context));
}

TEST_F(axclrtContextTest, Case02_axclrtSetCurrentContext) {
    axclrtContext context1;
    ASSERT_EQ(AXCL_SUCC, axclrtGetDefaultContext(&context1, device_id));
    ASSERT_EQ(AXCL_SUCC, axclrtSetCurrentContext(context1));
    axclrtContext context2;
    ASSERT_EQ(AXCL_SUCC, axclrtGetCurrentContext(&context2));
    EXPECT_EQ(context1, context2);
}