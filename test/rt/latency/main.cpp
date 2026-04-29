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
#include <stdlib.h>
#include <string.h>
#include <iomanip>
#include <iostream>

#include "axcl.h"
#include "axcl_rt_internal.h"
#include "elapser.hpp"
#include "randomizer.hpp"

#define PCIE_MSG_MODE 0
#define PCIE_DMA_MODE 1
#define PCIE_AUTO_MODE -1

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

    const int32_t device_id = device_list.devices[device_index];
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

class axclrtLatencyTest : public testing::Test {
protected:
    static void SetUpTestSuite() {
    }

    static void TearDownTestSuite() {
    }

    static axcl::randomizer m_random;

};

axcl::randomizer axclrtLatencyTest::m_random;

TEST_F(axclrtLatencyTest, Case01_axclrtLoopBackRateWithoutCheck) {

    constexpr uint32_t KB = 1024;
    constexpr uint32_t MB = KB * KB;
    constexpr uint32_t LOOP_COUNT = 100;
    axcl::elapser elapse;
    double cost;
    double rate;
    uint64_t cost_time;

    uint32_t step = 1;
    size_t size;
    bool performCheck = false;
    while (1) {
        size = step++ * KB * KB;

        void *data = ::malloc(size);
        cost_time = 0;
        for (uint32_t i = 0; i < LOOP_COUNT; ++i) {
            elapse.start();
            EXPECT_EQ(AXCL_SUCC, axclrtLatencyLoopback(data, static_cast<uint32_t>(size), performCheck, PCIE_DMA_MODE));
            cost_time += elapse.cost(axcl::elapser::UNIT::microseconds);
        }

        cost = (double)cost_time / LOOP_COUNT;
        rate = (size / (cost / 1000000)) / MB;

        std::cout << "latency loopback: "
              << std::setw(10) << std::right << size << " bytes "
              << std::setw(10) << std::right << std::fixed << std::setprecision(2) << cost << " us "
              << std::setw(10) << std::right << std::fixed << std::setprecision(2) << rate << " MB/s"
              << std::endl;

        ::free(data);

        if (size >= 8 * KB * KB) {
            break;
        }
    }
}

TEST_F(axclrtLatencyTest, Case02_axclrtLoopBackWithCheck) {

    constexpr uint32_t KB = 1024;
    uint32_t step = 1;
    size_t size;
    bool performCheck = true;

    while (1) {
        size = step++ * KB * KB;

        void *data = ::malloc(size);

        m_random.randomize(static_cast<uint8_t*>(data), size);
        {
            unsigned char *byte_data = (unsigned char *)data;
            printf("random Hex of the first 10 bytes: ");
            for (int i = 0; i < 10 && i < (int)size; i++) {
                printf("%02x ", byte_data[i]);
            }
            printf("\n");
        }

        EXPECT_EQ(AXCL_SUCC, axclrtLatencyLoopback(data, static_cast<uint32_t>(size), performCheck, PCIE_DMA_MODE));

        memset(data, 0xAC, size);
        {
            unsigned char *byte_data = (unsigned char *)data;
            printf("memset Hex of the first 10 bytes: ");
            for (int i = 0; i < 10 && i < (int)size; i++) {
                printf("%02x ", byte_data[i]);
            }
            printf("\n");
        }
        EXPECT_EQ(AXCL_SUCC, axclrtLatencyLoopback(data, static_cast<uint32_t>(size), performCheck, PCIE_DMA_MODE));

        ::free(data);

        if (size >= 8 * KB * KB) {
            break;
        }
    }
}

TEST_F(axclrtLatencyTest, Case03_axclrtLoopBackRandomSizeWithCheck) {

    constexpr uint32_t KB = 1024;

    size_t ranges[][2] = {
        {1, 10},
        {10, 100},
        {100, 512},
        {512, KB},
        {KB, 2 * KB},
        {2 * KB, 4 * KB},
        {4 * KB, 8 * KB},
        {8 * KB, 16 * KB},
        {16 * KB, 32 * KB},
        {32 * KB, 64 * KB},
        {64 * KB, 128 * KB},
        {128 * KB, 256 * KB},
        {256 * KB, 512 * KB},
        {512 * KB, KB * KB},
        {KB * KB, 2 * KB * KB},
        {2 * KB * KB, 4 * KB * KB},
        {4 * KB * KB, 8 * KB * KB}
    };

    size_t size;
    bool performCheck = true;
    for (size_t  i = 0; i < sizeof(ranges) / sizeof(ranges[0]); ++i) {
        size_t min = ranges[i][0];
        size_t max = ranges[i][1];
        size = m_random.randomize_range(min, max);
        void *data = ::malloc(size);
        printf("randomize size %zu\n", size);
        m_random.randomize(static_cast<uint8_t*>(data), size);
        EXPECT_EQ(AXCL_SUCC, axclrtLatencyLoopback(data, static_cast<uint32_t>(size), performCheck, PCIE_AUTO_MODE));

        memset(data, 0xAC, size);
        EXPECT_EQ(AXCL_SUCC, axclrtLatencyLoopback(data, static_cast<uint32_t>(size), performCheck, PCIE_AUTO_MODE));

        ::free(data);
    }

}

TEST_F(axclrtLatencyTest, Case04_axclrtLoopBackMSGWithoutCheck) {
    constexpr uint32_t KB = 1024;
    constexpr uint32_t LOOP_COUNT = 100;
    axcl::elapser elapse;
    double cost;
    double rate;
    uint64_t cost_time;

    std::vector<uint32_t> msg_sizes = {4, 8, 16, 32, 64, 128, 256, 512};
    bool performCheck = false;

    for (auto size : msg_sizes) {
        void *data = ::malloc(size);
        m_random.randomize(static_cast<uint8_t*>(data), size);

        cost_time = 0;
        for (uint32_t i = 0; i < LOOP_COUNT; ++i) {
            elapse.start();
            EXPECT_EQ(AXCL_SUCC, axclrtLatencyLoopback(data, size, performCheck, PCIE_MSG_MODE));
            cost_time += elapse.cost(axcl::elapser::UNIT::microseconds);
        }

        cost = (double)cost_time / LOOP_COUNT;
        rate = (size / (cost / 1000000)) / KB;

        std::cout << "MSG latency loopback: "
              << std::setw(10) << std::right << size << " bytes "
              << std::setw(10) << std::right << std::fixed << std::setprecision(2) << cost << " us "
              << std::setw(10) << std::right << std::fixed << std::setprecision(2) << rate << " KB/s"
              << std::endl;

        ::free(data);
    }
}

TEST_F(axclrtLatencyTest, Case05_axclrtLoopBackMSGWithCheck) {
    constexpr uint32_t KB = 1024;
    constexpr uint32_t LOOP_COUNT = 100;
    axcl::elapser elapse;
    double cost;
    double rate;
    uint64_t cost_time;

    std::vector<uint32_t> msg_sizes = {4, 8, 16, 32, 64, 128, 256, 512};
    bool performCheck = true;

    for (auto size : msg_sizes) {
        void *data = ::malloc(size);
        m_random.randomize(static_cast<uint8_t*>(data), size);

        cost_time = 0;
        for (uint32_t i = 0; i < LOOP_COUNT; ++i) {
            elapse.start();
            EXPECT_EQ(AXCL_SUCC, axclrtLatencyLoopback(data, size, performCheck, PCIE_MSG_MODE));
            cost_time += elapse.cost(axcl::elapser::UNIT::microseconds);
        }

        cost = (double)cost_time / LOOP_COUNT;
        rate = (size / (cost / 1000000)) / KB;

        std::cout << "MSG latency loopback: "
              << std::setw(10) << std::right << size << " bytes "
              << std::setw(10) << std::right << std::fixed << std::setprecision(2) << cost << " us "
              << std::setw(10) << std::right << std::fixed << std::setprecision(2) << rate << " KB/s"
              << std::endl;

        ::free(data);
    }
}

