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
#include <thread>
#include "axcl.h"
#include "dma_buffer.hpp"
#include "elapser.hpp"
#include "randomizer.hpp"

static int32_t device_id = -1;
constexpr uint32_t KB = 1024;
constexpr uint32_t MB = 1024 * 1024;

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

class axclrtDeviceTest : public testing::Test {
protected:
    static void SetUpTestSuite() {
    }

    static void TearDownTestSuite() {
    }

    static axcl::randomizer m_random;
};

axcl::randomizer axclrtDeviceTest::m_random;

TEST_F(axclrtDeviceTest, Case01_axclrtMalloc) {
    axclrtMemMallocPolicy policy = AXCL_MEM_MALLOC_NORMAL_ONLY;
    uint32_t step = 1;
    while (1) {
        size_t size = step++ * 64 * KB;
        std::cout << "size: " << std::right << std::setw(8) << size << "(0x" << std::hex << size << ")" << std::dec << ": " << std::endl;

        void* devMem = nullptr;
        ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem, size, policy));
        ASSERT_NE(nullptr, devMem);
        EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem));

        if (size >= 8 * MB) {
            break;
        }
    }
}

TEST_F(axclrtDeviceTest, Case02_axclrtMallocCached) {
    axclrtMemMallocPolicy policy = AXCL_MEM_MALLOC_NORMAL_ONLY;
    uint32_t step = 1;
    while (1) {
        size_t size = step++ * 64 * KB;
        std::cout << "size: " << std::right << std::setw(8) << size << "(0x" << std::hex << size << ")" << std::dec << ": " << std::endl;

        void* devMem = nullptr;
        ASSERT_EQ(AXCL_SUCC, axclrtMallocCached(&devMem, size, policy));
        ASSERT_NE(nullptr, devMem);
        EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem));

        if (size >= 8 * MB) {
            break;
        }
    }
}

TEST_F(axclrtDeviceTest, Case03_axclrtMemcpy) {
    axclrtMemMallocPolicy policy = AXCL_MEM_MALLOC_NORMAL_ONLY;
    uint32_t step = 1;
    while (1) {
        size_t size = step++ * MB;
        std::cout << "size: " << std::right << std::setw(8) << size << "(0x" << std::hex << size << ")" << std::dec << ": " << std::endl;

        void* hostMem[2] = {nullptr, nullptr};
        void* devMem[2] = {nullptr, nullptr};

        /* 01: alloc 1st host memory and memset randomize data */
        ASSERT_EQ(AXCL_SUCC, axclrtMallocHost(&hostMem[0], size));
        m_random.randomize(static_cast<uint8_t*>(hostMem[0]), size);

        /* 02: alloc 1st device memory and memset to 0 */
        ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem[0], size, policy));
        EXPECT_EQ(AXCL_SUCC, axclrtMemset(devMem[0], 0, size));

        /* 03: memcpy 1st host memory to 1st device memory */
        EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem[0], hostMem[0], size, AXCL_MEMCPY_HOST_TO_DEVICE));

        /* 04: alloc 2nd host memory and memset to 0 */
        ASSERT_EQ(AXCL_SUCC, axclrtMallocHost(&hostMem[1], size));
        ::memset(hostMem[1], 0, size);

        /* 05: alloc 2nd device memory and copy 1st device memory to 2nd device memory */
        ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem[1], size, policy));
        EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem[1], devMem[0], size, AXCL_MEMCPY_DEVICE_TO_DEVICE));

        /* 06: memcpy 2nd device memory to 2nd host memory */
        EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(hostMem[1], devMem[1], size, AXCL_MEMCPY_DEVICE_TO_HOST));

        /* 07: compare 1st and 2nd host memory */
        EXPECT_EQ(0, ::memcmp(hostMem[0], hostMem[1], size));

        /* 08: free all memory */
        EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem[0]));
        EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem[1]));
        EXPECT_EQ(AXCL_SUCC, axclrtFreeHost(hostMem[0]));
        EXPECT_EQ(AXCL_SUCC, axclrtFreeHost(hostMem[1]));

        if (size >= 8 * MB) {
            break;
        }
    }
}

TEST_F(axclrtDeviceTest, Case04_axclrtMemcpyCached) {
    axclrtMemMallocPolicy policy = AXCL_MEM_MALLOC_NORMAL_ONLY;
    uint32_t step = 1;
    while (1) {
        size_t size = step++ * MB;
        std::cout << "size: " << std::right << std::setw(8) << size << "(0x" << std::hex << size << ")" << std::dec << ": " << std::endl;

        void* hostMem[2] = {nullptr, nullptr};
        void* devMem[2] = {nullptr, nullptr};

        /* 01: alloc 1st host memory and memset randomize data */
        ASSERT_EQ(AXCL_SUCC, axclrtMallocHost(&hostMem[0], size));
        m_random.randomize(static_cast<uint8_t*>(hostMem[0]), size);

        /* 02: alloc 1st device memory and memset to 0 */
        ASSERT_EQ(AXCL_SUCC, axclrtMallocCached(&devMem[0], size, policy));
        EXPECT_EQ(AXCL_SUCC, axclrtMemset(devMem[0], 0, size));
        EXPECT_EQ(AXCL_SUCC, axclrtMemInvalidate(devMem[0], size));

        /* 03: memcpy 1st host memory to 1st device memory */
        EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem[0], hostMem[0], size, AXCL_MEMCPY_HOST_TO_DEVICE));
        EXPECT_EQ(AXCL_SUCC, axclrtMemInvalidate(devMem[0], size));

        /* 04: alloc 2nd host memory and memset to 0 */
        ASSERT_EQ(AXCL_SUCC, axclrtMallocHost(&hostMem[1], size));
        ::memset(hostMem[1], 0, size);

        /* 05: alloc 2nd device memory and copy 1st device memory to 2nd device memory */
        ASSERT_EQ(AXCL_SUCC, axclrtMallocCached(&devMem[1], size, policy));
        EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem[1], devMem[0], size, AXCL_MEMCPY_DEVICE_TO_DEVICE));
        EXPECT_EQ(AXCL_SUCC, axclrtMemInvalidate(devMem[1], size));

        /* 06: memcpy 2nd device memory to 2nd host memory */
        EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(hostMem[1], devMem[1], size, AXCL_MEMCPY_DEVICE_TO_HOST));

        /* 07: compare 1st and 2nd host memory */
        EXPECT_EQ(0, ::memcmp(hostMem[0], hostMem[1], size));

        /* 08: free all memory */
        EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem[0]));
        EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem[1]));
        EXPECT_EQ(AXCL_SUCC, axclrtFreeHost(hostMem[0]));
        EXPECT_EQ(AXCL_SUCC, axclrtFreeHost(hostMem[1]));

        if (size >= 8 * MB) {
            break;
        }
    }
}

TEST_F(axclrtDeviceTest, Case05_axclrtMemcmp) {
    axclrtMemMallocPolicy policy = AXCL_MEM_MALLOC_NORMAL_ONLY;
    constexpr uint32_t size = 1 * MB;
    void* devMem[2] = {nullptr, nullptr};
    ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem[0], size, policy));
    ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem[1], size, policy));

    EXPECT_EQ(AXCL_SUCC, axclrtMemset(devMem[0], 0xAA, size));
    EXPECT_EQ(AXCL_SUCC, axclrtMemset(devMem[1], 0xBB, size));
    EXPECT_NE(AXCL_SUCC, axclrtMemcmp(devMem[0], devMem[1], size));
    EXPECT_EQ(AXCL_SUCC, axclrtMemset(devMem[1], 0xAA, size));
    EXPECT_EQ(AXCL_SUCC, axclrtMemcmp(devMem[0], devMem[1], size));

    EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem[0]));
    EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem[1]));
}

TEST_F(axclrtDeviceTest, Case06_axclrtMemcpyFromHostPhyToDevice) {
    uint32_t max_size_mb = 8 * MB;
    bool no_cma = false;
    const auto cma_free_in_mb = dma_buffer::get_cma_free_size() * KB;
    if (cma_free_in_mb < 8 * MB) {
        max_size_mb = 4 * MB; /* try to alloc 2^10 from buddy */
        no_cma = true;
    }

    axclrtMemMallocPolicy policy = AXCL_MEM_MALLOC_NORMAL_ONLY;
    uint32_t step = 1;
    while (1) {
        size_t size = step++ * MB;
        std::cout << "size: " << std::right << std::setw(8) << size << "(0x" << std::hex << size << ")" << std::dec << ": " << std::endl;

        /* 01: alloc 1st host memory and memset randomize data */
        dma_buffer allocator1(device_id);
        dma_buffer allocator2(device_id);
        if (!allocator1.alloc(size)) {
            if (no_cma) {
                break;
            } else {
                ASSERT_TRUE(false) << "allocate " << size << " (0x" << std::hex << size << ")" << std::dec << " bytes fail";
            }
        }

        if (!allocator2.alloc(size)) {
            allocator1.free();
            if (no_cma) {
                break;
            } else {
                ASSERT_TRUE(false) << "allocate " << size << " (0x" << std::hex << size << ")" << std::dec << " bytes fail";
            }
        }

        const struct dma_mem& host_mem1 = allocator1.get();
        const struct dma_mem& host_mem2 = allocator2.get();
        m_random.randomize(static_cast<uint8_t*>(host_mem1.blks[0].vir), size);
        m_random.randomize(static_cast<uint8_t*>(host_mem2.blks[0].vir), size);

        /* 02: alloc device memory and memset to 0 */
        void* dev_mem;
        ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&dev_mem, size, policy));
        EXPECT_EQ(AXCL_SUCC, axclrtMemset(dev_mem, 0, size));

        /* 03: memcpy host memory1 to device memory */
        EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(dev_mem, reinterpret_cast<void*>(host_mem1.blks[0].phy), size, AXCL_MEMCPY_HOST_PHY_TO_DEVICE));

        /* 04: memcpy device memory to host memory2 */
        EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(reinterpret_cast<void*>(host_mem2.blks[0].phy), dev_mem, size, AXCL_MEMCPY_DEVICE_TO_HOST_PHY));

        /* 05: compare host memory1 and host memory2 */
        EXPECT_EQ(0, ::memcmp(host_mem1.blks[0].vir, host_mem2.blks[0].vir, size));

        /* 06: free all memory */
        EXPECT_EQ(AXCL_SUCC, axclrtFree(dev_mem));
        allocator1.free();
        allocator2.free();

        if (size >= max_size_mb) {
            break;
        }
    }
}

TEST_F(axclrtDeviceTest, Case07_axclrtMemcpyFromHostToDeviceLatency) {
    constexpr uint32_t max_size_mb = 8 * MB;
    const auto cma_free_in_mb = dma_buffer::get_cma_free_size() * KB;

    axclrtMemMallocPolicy policy = AXCL_MEM_MALLOC_NORMAL_ONLY;
    uint32_t step = 1;
    constexpr uint32_t UHD = 3840 * 2160 * 3 / 2;
    constexpr uint32_t FHD = 1920 * 1080 * 3 / 2;
    constexpr uint32_t _540P = 960 * 540 * 3 / 2;
    constexpr uint32_t _360P = 640 * 360 * 3 / 2;
    constexpr uint32_t SPECIAL_SIZE[] = {UHD, FHD, _540P, _360P};
    constexpr uint32_t KB = 1024;
    constexpr uint32_t MB = KB * KB;
    constexpr uint32_t LOOP_COUNT = 100;
    axcl::elapser elapse;
    double cost;
    double rate;
    dma_buffer allocator(device_id);
    struct dma_mem cma_mem;
    uint64_t cost_time;

    int32_t special_count = static_cast<int32_t>(sizeof(SPECIAL_SIZE) / sizeof(SPECIAL_SIZE[0]));
    size_t size;
    while (1) {
        if (special_count > 0) {
            size = SPECIAL_SIZE[--special_count];
        } else {
            size = step++ * MB;
            if (size > max_size_mb) {
                break;
            }
        }

        void* hostMem = nullptr;
        void* devMem = nullptr;

        /* 01: memcpy host malloc to device cm */
        ASSERT_EQ(AXCL_SUCC, axclrtMallocHost(&hostMem, size));
        ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem, size, policy));
        cost_time = 0;
        for (uint32_t i = 0; i < LOOP_COUNT; ++i) {
            elapse.start();
            EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem, hostMem, size, AXCL_MEMCPY_HOST_TO_DEVICE));
            cost_time += elapse.cost(axcl::elapser::UNIT::microseconds);
        }
        cost = (double)cost_time / LOOP_COUNT;
        rate = (size / (cost / 1000000)) / MB;
        std::cout << "host malloc  -> device cmm       size: " << std::setw(10) << std::right << size << std::setw(10) << std::right
                  << std::hex << size << std::dec << std::setw(10) << std::right << std::fixed << std::setprecision(2) << cost << " us "
                  << std::setw(10) << std::right << std::fixed << std::setprecision(2) << rate << " MB/s" << std::endl;

        EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem));
        EXPECT_EQ(AXCL_SUCC, axclrtFreeHost(hostMem));

        if (cma_free_in_mb < max_size_mb) {
            continue;
        }

        /* 02: memcpy from host cma memory to device cmm */
        ASSERT_EQ(true, allocator.alloc(size));
        cma_mem = allocator.get();
        ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem, size, policy));
        cost_time = 0;
        for (uint32_t i = 0; i < LOOP_COUNT; ++i) {
            elapse.start();
            EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem, reinterpret_cast<void*>(cma_mem.blks[0].phy), size, AXCL_MEMCPY_HOST_PHY_TO_DEVICE));
            cost_time += elapse.cost(axcl::elapser::UNIT::microseconds);
        }
        cost = (double)cost_time / LOOP_COUNT;
        rate = (size / (cost / 1000000)) / MB;
        std::cout << "host cma phy -> device cmm       size: " << std::setw(10) << std::right << size << std::setw(10) << std::right
                  << std::hex << size << std::dec << std::setw(10) << std::right << std::fixed << std::setprecision(2) << cost << " us "
                  << std::setw(10) << std::right << std::fixed << std::setprecision(2) << rate << " MB/s" << std::endl;
        allocator.free();
        EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem));

        /* 03: memcpy malloc to host noncached cma */
        ASSERT_EQ(true, allocator.alloc(size));
        cma_mem = allocator.get();
        ASSERT_EQ(AXCL_SUCC, axclrtMallocHost(&hostMem, size));
        cost_time = 0;
        for (uint32_t i = 0; i < LOOP_COUNT; ++i) {
            elapse.start();
            ::memcpy(reinterpret_cast<void*>(cma_mem.blks[0].vir), hostMem, size);
            cost_time += elapse.cost(axcl::elapser::UNIT::microseconds);
        }
        cost = (double)cost_time / LOOP_COUNT;
        rate = (size / (cost / 1000000)) / MB;
        std::cout << "host malloc  -> host   cma       size: " << std::setw(10) << std::right << size << std::setw(10) << std::right
                  << std::hex << size << std::dec << std::setw(10) << std::right << std::fixed << std::setprecision(2) << cost << " us "
                  << std::endl;

        allocator.free();
        EXPECT_EQ(AXCL_SUCC, axclrtFreeHost(hostMem));

#ifdef AXCL_CMA_CACHED
        /* 04: memcpy malloc to host cached cma */
        ASSERT_EQ(true, allocator.alloc(size, true));
        cma_mem = allocator.get();
        ASSERT_EQ(AXCL_SUCC, axclrtMallocHost(&hostMem, size));
        cost_time = 0;
        for (uint32_t i = 0; i < LOOP_COUNT; ++i) {
            elapse.start();
            ::memcpy(reinterpret_cast<void*>(cma_mem.blks[0].vir), hostMem, size);
            cma_mem.ops.flush(cma_mem.blks[0].phy, cma_mem.blks[0].vir, size);
            cost_time += elapse.cost(axcl::elapser::UNIT::microseconds);
        }
        cost = (double)cost_time / LOOP_COUNT;
        rate = (size / (cost / 1000000)) / MB;
        std::cout << "host malloc  -> host   cma cache size: " << std::setw(10) << std::right << size << std::setw(10) << std::right
                  << std::hex << size << std::dec << std::setw(10) << std::right << std::fixed << std::setprecision(2) << cost << " us "
                  << std::endl;
        allocator.free();
        EXPECT_EQ(AXCL_SUCC, axclrtFreeHost(hostMem));
#endif

        /* 05: memcpy malloc to host sglist cached memory */
        ASSERT_EQ(true, allocator.alloc(size, true, true));
        const struct dma_mem& sg_mem = allocator.get();
        ASSERT_EQ(AXCL_SUCC, axclrtMallocHost(&hostMem, size));
        cost_time = 0;
        for (uint32_t i = 0; i < LOOP_COUNT; ++i) {
            elapse.start();
            ::memcpy(reinterpret_cast<void*>(sg_mem.blks[0].vir), hostMem, size);
            sg_mem.ops.flush(sg_mem.blks[0].phy, sg_mem.blks[0].vir, (uint32_t)size);
            cost_time += elapse.cost(axcl::elapser::UNIT::microseconds);
        }
        cost = (double)cost_time / LOOP_COUNT;
        rate = (size / (cost / 1000000)) / MB;
        std::cout << "host malloc  -> host   sg  cache size: " << std::setw(10) << std::right << size << std::setw(10) << std::right
                  << std::hex << size << std::dec << std::setw(10) << std::right << std::fixed << std::setprecision(2) << cost << " us "
                  << std::endl;
        allocator.free();
        EXPECT_EQ(AXCL_SUCC, axclrtFreeHost(hostMem));
    }
}

TEST_F(axclrtDeviceTest, Case08_axclrtMemcpyFromDeviceToHostLatency) {
    constexpr uint32_t max_size_mb = 8 * MB;
    const auto cma_free_in_mb = dma_buffer::get_cma_free_size() * KB;

    axclrtMemMallocPolicy policy = AXCL_MEM_MALLOC_NORMAL_ONLY;
    uint32_t step = 1;
    constexpr uint32_t UHD = 3840 * 2160 * 3 / 2;
    constexpr uint32_t FHD = 1920 * 1080 * 3 / 2;
    constexpr uint32_t _540P = 960 * 540 * 3 / 2;
    constexpr uint32_t _360P = 640 * 360 * 3 / 2;
    constexpr uint32_t SPECIAL_SIZE[] = {UHD, FHD, _540P, _360P};
    constexpr uint32_t KB = 1024;
    constexpr uint32_t MB = KB * KB;
    constexpr uint32_t LOOP_COUNT = 100;
    axcl::elapser elapse;
    double cost;
    double rate;
    uint64_t cost_time;
    dma_buffer allocator(device_id);
    struct dma_mem cma_mem;

    int32_t special_count = static_cast<int32_t>(sizeof(SPECIAL_SIZE) / sizeof(SPECIAL_SIZE[0]));
    size_t size;
    while (1) {
        if (special_count > 0) {
            size = SPECIAL_SIZE[--special_count];
        } else {
            size = step++ * MB;
            if (size > max_size_mb) {
                break;
            }
        }

        void* hostMem = nullptr;
        void* devMem = nullptr;

        /* 01: memcpy device cmm to host malloc memory */
        ASSERT_EQ(AXCL_SUCC, axclrtMallocHost(&hostMem, size));
        ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem, size, policy));
        cost_time = 0;
        for (uint32_t i = 0; i < LOOP_COUNT; ++i) {
            elapse.start();
            EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(hostMem, devMem, size, AXCL_MEMCPY_DEVICE_TO_HOST));
            cost_time += elapse.cost(axcl::elapser::UNIT::microseconds);
        }
        cost = (double)cost_time / LOOP_COUNT;
        rate = (size / (cost / 1000000)) / MB;
        std::cout << "device cmm      -> host malloc size: " << std::setw(10) << std::right << size << std::setw(10) << std::right
                  << std::hex << size << std::dec << std::setw(10) << std::right << std::fixed << std::setprecision(2) << cost << " us "
                  << std::setw(10) << std::right << std::fixed << std::setprecision(2) << rate << " MB/s" << std::endl;
        EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem));
        EXPECT_EQ(AXCL_SUCC, axclrtFreeHost(hostMem));

        if (cma_free_in_mb < max_size_mb) {
            continue;
        }

        /* 02: memcpy from device cmm to host cma memory */
        ASSERT_EQ(true, allocator.alloc(size));
        cma_mem = allocator.get();
        ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem, size, policy));
        cost_time = 0;
        for (uint32_t i = 0; i < LOOP_COUNT; ++i) {
            elapse.start();
            EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(reinterpret_cast<void*>(cma_mem.blks[0].phy), devMem, size, AXCL_MEMCPY_DEVICE_TO_HOST_PHY));
            cost_time += elapse.cost(axcl::elapser::UNIT::microseconds);
        }

        cost = (double)cost_time / LOOP_COUNT;
        rate = (size / (cost / 1000000)) / MB;
        std::cout << "device cmm      -> host cma    size: " << std::setw(10) << std::right << size << std::setw(10) << std::right
                  << std::hex << size << std::dec << std::setw(10) << std::right << std::fixed << std::setprecision(2) << cost << " us "
                  << std::setw(10) << std::right << std::fixed << std::setprecision(2) << rate << " MB/s" << std::endl;
        allocator.free();
        EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem));

        /* 03: memcpy host noncached cma to host malloc memory */
        ASSERT_EQ(true, allocator.alloc(size));
        cma_mem = allocator.get();
        ASSERT_EQ(AXCL_SUCC, axclrtMallocHost(&hostMem, size));
        cost_time = 0;
        for (uint32_t i = 0; i < LOOP_COUNT; ++i) {
            elapse.start();
            ::memcpy(hostMem, reinterpret_cast<void*>(cma_mem.blks[0].vir), size);
            cost_time += elapse.cost(axcl::elapser::UNIT::microseconds);
        }
        cost = (double)cost_time / LOOP_COUNT;
        rate = (size / (cost / 1000000)) / MB;
        std::cout << "host cma        -> host malloc size: " << std::setw(10) << std::right << size << std::setw(10) << std::right
                  << std::hex << size << std::dec << std::setw(10) << std::right << std::fixed << std::setprecision(2) << cost << " us "
                  << std::endl;
        allocator.free();
        EXPECT_EQ(AXCL_SUCC, axclrtFreeHost(hostMem));

#ifdef AXCL_CMA_CACHED
        /* 04: memcpy host cached cma to host malloc memory */
        ASSERT_EQ(true, allocator.alloc(size, true));
        cma_mem = allocator.get();
        ASSERT_EQ(AXCL_SUCC, axclrtMallocHost(&hostMem, size));
        cost_time = 0;
        for (uint32_t i = 0; i < LOOP_COUNT; ++i) {
            elapse.start();
            ::memcpy(hostMem, reinterpret_cast<void*>(cma_mem.blks[0].vir), size);
            cost_time += elapse.cost(axcl::elapser::UNIT::microseconds);
        }
        cost = (double)cost_time / LOOP_COUNT;
        rate = (size / (cost / 1000000)) / MB;
        std::cout << "host cma cached -> host malloc size: " << std::setw(10) << std::right << size << std::setw(10) << std::right
                  << std::hex << size << std::dec << std::setw(10) << std::right << std::fixed << std::setprecision(2) << cost << " us "
                  << std::endl;
        allocator.free();
        EXPECT_EQ(AXCL_SUCC, axclrtFreeHost(hostMem));
#endif

        /* 05: memcpy host sg cached to host malloc memory */
        ASSERT_EQ(true, allocator.alloc(size, true, true));
        const struct dma_mem& sg_mem = allocator.get();
        ASSERT_EQ(AXCL_SUCC, axclrtMallocHost(&hostMem, size));
        cost_time = 0;
        for (uint32_t i = 0; i < LOOP_COUNT; ++i) {
            elapse.start();
            ::memcpy(hostMem, reinterpret_cast<void*>(sg_mem.blks[0].vir), size);
            cost_time += elapse.cost(axcl::elapser::UNIT::microseconds);
        }
        cost = (double)cost_time / LOOP_COUNT;
        rate = (size / (cost / 1000000)) / MB;
        std::cout << "host sg  cached -> host malloc size: " << std::setw(10) << std::right << size << std::setw(10) << std::right
                  << std::hex << size << std::dec << std::setw(10) << std::right << std::fixed << std::setprecision(2) << cost << " us "
                  << std::endl;
        allocator.free();
        EXPECT_EQ(AXCL_SUCC, axclrtFreeHost(hostMem));
    }
}

TEST_F(axclrtDeviceTest, Case09_axclrtMemcpyFromDeviceToDeviceLatency) {
    axclrtMemMallocPolicy policy = AXCL_MEM_MALLOC_NORMAL_ONLY;
    uint32_t step = 1;
    constexpr uint32_t LOOP_COUNT = 100;
    axcl::elapser elapse;
    double cost;

    while (1) {
        size_t size = step++ * MB;

        void* devMem[2] = {nullptr, nullptr};

        /* 01: alloc device memory0 */
        ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem[0], size, policy));

        /* 02: alloc device memory1  */
        ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem[1], size, policy));

        /* 03: memcpy device memory0 to device memory1 */
        uint64_t cost_time = 0;
        for (uint32_t i = 0; i < LOOP_COUNT; ++i) {
            elapse.start();
            EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem[1], devMem[0], size, AXCL_MEMCPY_DEVICE_TO_DEVICE));
            cost_time += elapse.cost(axcl::elapser::UNIT::milliseconds);
        }

        cost = (double)cost_time / LOOP_COUNT;
        std::cout << "size: " << std::right << std::setw(10) << size << std::hex << std::right << std::setw(10) << size << std::dec
                  << std::right << std::setw(10) << std::fixed << std::setprecision(2) << cost << " us " << std::endl;

        /* 04: free all memory */
        EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem[0]));
        EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem[1]));

        if (size >= 8 * MB) {
            break;
        }
    }
}

TEST_F(axclrtDeviceTest, Case10_axclrtMemcpyRandom) {
    axclrtMemMallocPolicy policy = AXCL_MEM_MALLOC_NORMAL_ONLY;
    uint32_t step = 1;

    std::random_device sd;
    std::minstd_rand linear(sd());
    std::uniform_int_distribution<int> dis(1, KB);

    while (1) {
        size_t size = step++ * MB + dis(linear);
        std::cout << "size: " << std::right << std::setw(8) << size << "(0x" << std::hex << size << ")" << std::dec << ": " << std::endl;

        void* hostMem[2] = {nullptr, nullptr};
        void* devMem[2] = {nullptr, nullptr};

        /* 01: alloc 1st host memory and memset randomize data */
        ASSERT_EQ(AXCL_SUCC, axclrtMallocHost(&hostMem[0], size));
        m_random.randomize(static_cast<uint8_t*>(hostMem[0]), size);

        /* 02: alloc 1st device memory and memset to 0 */
        ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem[0], size, policy));
        EXPECT_EQ(AXCL_SUCC, axclrtMemset(devMem[0], 0, size));

        /* 03: memcpy 1st host memory to 1st device memory */
        EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem[0], hostMem[0], size, AXCL_MEMCPY_HOST_TO_DEVICE));

        /* 04: alloc 2nd host memory and memset to 0 */
        ASSERT_EQ(AXCL_SUCC, axclrtMallocHost(&hostMem[1], size));
        ::memset(hostMem[1], 0, size);

        /* 05: alloc 2nd device memory and copy 1st device memory to 2nd device memory */
        ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem[1], size, policy));
        EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem[1], devMem[0], size, AXCL_MEMCPY_DEVICE_TO_DEVICE));

        /* 06: memcpy 2nd device memory to 2nd host memory */
        EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(hostMem[1], devMem[1], size, AXCL_MEMCPY_DEVICE_TO_HOST));

        /* 07: compare 1st and 2nd host memory */
        EXPECT_EQ(0, ::memcmp(hostMem[0], hostMem[1], size));

        /* 08: free all memory */
        EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem[0]));
        EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem[1]));
        EXPECT_EQ(AXCL_SUCC, axclrtFreeHost(hostMem[0]));
        EXPECT_EQ(AXCL_SUCC, axclrtFreeHost(hostMem[1]));

        if (size >= 8 * MB) {
            break;
        }
    }
}

TEST_F(axclrtDeviceTest, Case11_axclrtMemcpyCachedRandom) {
    axclrtMemMallocPolicy policy = AXCL_MEM_MALLOC_NORMAL_ONLY;
    uint32_t step = 1;

    std::random_device sd;
    std::minstd_rand linear(sd());
    std::uniform_int_distribution<int> dis(1, KB);

    while (1) {
        size_t size = step++ * MB + dis(linear);
        std::cout << "size: " << std::right << std::setw(8) << size << "(0x" << std::hex << size << ")" << std::dec << ": " << std::endl;

        void* hostMem[2] = {nullptr, nullptr};
        void* devMem[2] = {nullptr, nullptr};

        /* 01: alloc 1st host memory and memset randomize data */
        ASSERT_EQ(AXCL_SUCC, axclrtMallocHost(&hostMem[0], size));
        m_random.randomize(static_cast<uint8_t*>(hostMem[0]), size);

        /* 02: alloc 1st device memory and memset to 0 */
        ASSERT_EQ(AXCL_SUCC, axclrtMallocCached(&devMem[0], size, policy));
        EXPECT_EQ(AXCL_SUCC, axclrtMemset(devMem[0], 0, size));
        EXPECT_EQ(AXCL_SUCC, axclrtMemInvalidate(devMem[0], size));

        /* 03: memcpy 1st host memory to 1st device memory */
        EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem[0], hostMem[0], size, AXCL_MEMCPY_HOST_TO_DEVICE));
        EXPECT_EQ(AXCL_SUCC, axclrtMemInvalidate(devMem[0], size));

        /* 04: alloc 2nd host memory and memset to 0 */
        ASSERT_EQ(AXCL_SUCC, axclrtMallocHost(&hostMem[1], size));
        ::memset(hostMem[1], 0, size);

        /* 05: alloc 2nd device memory and copy 1st device memory to 2nd device memory */
        ASSERT_EQ(AXCL_SUCC, axclrtMallocCached(&devMem[1], size, policy));
        EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem[1], devMem[0], size, AXCL_MEMCPY_DEVICE_TO_DEVICE));
        EXPECT_EQ(AXCL_SUCC, axclrtMemInvalidate(devMem[1], size));

        /* 06: memcpy 2nd device memory to 2nd host memory */
        EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(hostMem[1], devMem[1], size, AXCL_MEMCPY_DEVICE_TO_HOST));

        /* 07: compare 1st and 2nd host memory */
        EXPECT_EQ(0, ::memcmp(hostMem[0], hostMem[1], size));

        /* 08: free all memory */
        EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem[0]));
        EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem[1]));
        EXPECT_EQ(AXCL_SUCC, axclrtFreeHost(hostMem[0]));
        EXPECT_EQ(AXCL_SUCC, axclrtFreeHost(hostMem[1]));

        if (size >= 8 * MB) {
            break;
        }
    }
}

TEST_F(axclrtDeviceTest, Case12_axclrtMemAsync) {
    axclrtMemMallocPolicy policy = AXCL_MEM_MALLOC_NORMAL_ONLY;
    uint32_t step = 1;

    while (1) {
        size_t size = step * MB;
        std::cout << "size: " << std::right << std::setw(8) << size << "(0x" << std::hex << size << ")" << std::dec << ": " << std::endl;

        void* hostMem[2] = {nullptr, nullptr};
        void* devMem[2] = {nullptr, nullptr};

        ASSERT_EQ(AXCL_SUCC, axclrtMallocHost(&hostMem[0], size));
        m_random.randomize(static_cast<uint8_t*>(hostMem[0]), size);

        ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem[0], size, policy));
        ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem[1], size, policy));

        axclrtStream stream;
        ASSERT_EQ(AXCL_SUCC, axclrtCreateStream(&stream));

        /* dev mem 0 */
        EXPECT_EQ(AXCL_SUCC, axclrtMemsetAsync(devMem[0], 0, size, stream));
        EXPECT_EQ(AXCL_SUCC, axclrtMemcpyAsync(devMem[0], hostMem[0], size, AXCL_MEMCPY_HOST_TO_DEVICE, stream));

        /* dev mem 1 */
        EXPECT_EQ(AXCL_SUCC, axclrtMemsetAsync(devMem[1], 0, size, stream));
        EXPECT_EQ(AXCL_SUCC, axclrtMemcpyAsync(devMem[1], hostMem[0], size, AXCL_MEMCPY_HOST_TO_DEVICE, stream));

        /* Compare dev mem 0 and 1 */
        EXPECT_EQ(AXCL_SUCC, axclrtMemcmpAsync(devMem[0], devMem[1], size, stream));
        if (0 == step % 2) {
            EXPECT_EQ(AXCL_SUCC, axclrtSynchronizeStream(stream));
        } else {
            EXPECT_EQ(AXCL_SUCC, axclrtSynchronizeStreamWithTimeout(stream, 1000));
        }

        /* destroy stream */
        EXPECT_EQ(AXCL_SUCC, axclrtDestroyStream(stream));

        /* free all memory */
        EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem[0]));
        EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem[1]));
        EXPECT_EQ(AXCL_SUCC, axclrtFreeHost(hostMem[0]));

        step++;
        if (size >= 8 * MB) {
            break;
        }
    }
}

TEST_F(axclrtDeviceTest, Case13_SingleThreadMultiStream) {
    axclrtMemMallocPolicy policy = AXCL_MEM_MALLOC_NORMAL_ONLY;
    uint32_t step = 1;

    while (1) {
        size_t size = step * MB;
        std::cout << "size: " << std::right << std::setw(8) << size << "(0x" << std::hex << size << ")" << std::dec << ": " << std::endl;

        void* hostMem[2] = {nullptr, nullptr};
        void* devMem[4] = {nullptr, nullptr, nullptr, nullptr};

        /* Allocate host memory and initialize with random data */
        ASSERT_EQ(AXCL_SUCC, axclrtMallocHost(&hostMem[0], size));
        m_random.randomize(static_cast<uint8_t*>(hostMem[0]), size);

        /* Allocate device memory for four buffers */
        for (int i = 0; i < 4; i++) {
            ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem[i], size, policy));
        }

        /* Create two streams for parallel execution */
        axclrtStream stream1, stream2;
        ASSERT_EQ(AXCL_SUCC, axclrtCreateStream(&stream1));
        ASSERT_EQ(AXCL_SUCC, axclrtCreateStream(&stream2));

        /* Execute operations on stream1 */
        EXPECT_EQ(AXCL_SUCC, axclrtMemsetAsync(devMem[0], 0, size, stream1));
        EXPECT_EQ(AXCL_SUCC, axclrtMemcpyAsync(devMem[0], hostMem[0], size, AXCL_MEMCPY_HOST_TO_DEVICE, stream1));
        EXPECT_EQ(AXCL_SUCC, axclrtMemsetAsync(devMem[1], 0, size, stream1));
        EXPECT_EQ(AXCL_SUCC, axclrtMemcpyAsync(devMem[1], hostMem[0], size, AXCL_MEMCPY_HOST_TO_DEVICE, stream1));

        /* Execute operations on stream2 */
        EXPECT_EQ(AXCL_SUCC, axclrtMemsetAsync(devMem[2], 0, size, stream2));
        EXPECT_EQ(AXCL_SUCC, axclrtMemcpyAsync(devMem[2], hostMem[0], size, AXCL_MEMCPY_HOST_TO_DEVICE, stream2));
        EXPECT_EQ(AXCL_SUCC, axclrtMemsetAsync(devMem[3], 0, size, stream2));
        EXPECT_EQ(AXCL_SUCC, axclrtMemcpyAsync(devMem[3], hostMem[0], size, AXCL_MEMCPY_HOST_TO_DEVICE, stream2));

        /* Compare memory between different streams */
        EXPECT_EQ(AXCL_SUCC, axclrtMemcmpAsync(devMem[0], devMem[1], size, stream1));
        EXPECT_EQ(AXCL_SUCC, axclrtMemcmpAsync(devMem[2], devMem[3], size, stream2));

        /* Synchronize both streams */
        EXPECT_EQ(AXCL_SUCC, axclrtSynchronizeStream(stream1));
        EXPECT_EQ(AXCL_SUCC, axclrtSynchronizeStream(stream2));

        /* Destroy streams */
        EXPECT_EQ(AXCL_SUCC, axclrtDestroyStream(stream1));
        EXPECT_EQ(AXCL_SUCC, axclrtDestroyStream(stream2));

        /* Free all memory */
        for (int i = 0; i < 4; i++) {
            EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem[i]));
        }
        EXPECT_EQ(AXCL_SUCC, axclrtFreeHost(hostMem[0]));

        step++;
        if (size >= 8 * MB) {
            break;
        }
    }
}

TEST_F(axclrtDeviceTest, Case14_MultiThreadMultiStream) {
    axclrtMemMallocPolicy policy = AXCL_MEM_MALLOC_NORMAL_ONLY;
    uint32_t step = 1;
    constexpr int THREAD_NUM = 2;

    while (1) {
        size_t size = step * MB;
        std::cout << "size: " << std::right << std::setw(8) << size << "(0x" << std::hex << size << ")" << std::dec << ": " << std::endl;

        /* Pre-allocate memory for all threads */
        std::vector<void*> hostMem(THREAD_NUM, nullptr);
        std::vector<std::array<void*, 2>> devMem(THREAD_NUM);

        /* Allocate and initialize host memory for each thread */
        for (int i = 0; i < THREAD_NUM; i++) {
            ASSERT_EQ(AXCL_SUCC, axclrtMallocHost(&hostMem[i], size));
            m_random.randomize(static_cast<uint8_t*>(hostMem[i]), size);
        }

        /* Allocate device memory for each thread */
        for (int i = 0; i < THREAD_NUM; i++) {
            for (int j = 0; j < 2; j++) {
                ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem[i][j], size, policy));
            }
        }

        /* Create thread function */
        auto threadFunc = [&](int threadId) {
            /* Create context */
            axclrtContext context;
            EXPECT_EQ(AXCL_SUCC, axclrtCreateContext(&context, device_id));

            /* Create stream */
            axclrtStream stream;
            EXPECT_EQ(AXCL_SUCC, axclrtCreateStream(&stream));

            /* Execute operations on stream */
            EXPECT_EQ(AXCL_SUCC, axclrtMemsetAsync(devMem[threadId][0], 0, size, stream));
            EXPECT_EQ(AXCL_SUCC, axclrtMemcpyAsync(devMem[threadId][0], hostMem[threadId], size, AXCL_MEMCPY_HOST_TO_DEVICE, stream));
            EXPECT_EQ(AXCL_SUCC, axclrtMemsetAsync(devMem[threadId][1], 0, size, stream));
            EXPECT_EQ(AXCL_SUCC, axclrtMemcpyAsync(devMem[threadId][1], hostMem[threadId], size, AXCL_MEMCPY_HOST_TO_DEVICE, stream));

            /* Compare memory */
            EXPECT_EQ(AXCL_SUCC, axclrtMemcmpAsync(devMem[threadId][0], devMem[threadId][1], size, stream));

            /* Synchronize stream */
            EXPECT_EQ(AXCL_SUCC, axclrtSynchronizeStream(stream));

            /* Destroy stream */
            EXPECT_EQ(AXCL_SUCC, axclrtDestroyStream(stream));

            /* Destroy context */
            EXPECT_EQ(AXCL_SUCC, axclrtDestroyContext(context));
        };

        /* Create and start threads */
        std::vector<std::thread> threads;
        for (int i = 0; i < THREAD_NUM; i++) {
            threads.emplace_back(threadFunc, i);
        }

        /* Wait for all threads to complete */
        for (auto& thread : threads) {
            thread.join();
        }

        /* Free all memory */
        for (int i = 0; i < THREAD_NUM; i++) {
            for (int j = 0; j < 2; j++) {
                EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem[i][j]));
            }
            EXPECT_EQ(AXCL_SUCC, axclrtFreeHost(hostMem[i]));
        }

        step++;
        if (size >= 8 * MB) {
            break;
        }
    }
}

TEST_F(axclrtDeviceTest, Case15_MultiThreadSharedStream) {
    /*
     * This test case demonstrates multi-threaded execution with a shared stream.
     * Multiple threads share a single stream for asynchronous operations.
     *
     * Key characteristics:
     * 1. Thread synchronization: No explicit synchronization between threads when submitting operations
     * 2. Execution order: Operations are executed in the order they are queued to the stream
     * 3. Completion guarantee: Each thread calls axclrtSynchronizeStream at the end of its operations
     * 4. Resource management: The shared stream is created before thread creation and destroyed after all threads complete
     *
     * This pattern is useful when multiple threads need to submit work to the same stream
     * but don't need to coordinate the exact order of their operations.
     */
    axclrtMemMallocPolicy policy = AXCL_MEM_MALLOC_NORMAL_ONLY;
    uint32_t step = 1;
    constexpr int THREAD_NUM = 2;

    while (1) {
        size_t size = step * MB;
        std::cout << "size: " << std::right << std::setw(8) << size << "(0x" << std::hex << size << ")" << std::dec << ": " << std::endl;

        /* Pre-allocate memory for all threads */
        std::vector<void*> hostMem(THREAD_NUM, nullptr);
        std::vector<std::array<void*, 2>> devMem(THREAD_NUM);

        /* Allocate host memory */
        for (int i = 0; i < THREAD_NUM; i++) {
            ASSERT_EQ(AXCL_SUCC, axclrtMallocHost(&hostMem[i], size));
            m_random.randomize(static_cast<uint8_t*>(hostMem[i]), size);
        }

        /* Allocate device memory */
        for (int i = 0; i < THREAD_NUM; i++) {
            for (int j = 0; j < 2; j++) {
                ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem[i][j], size, policy));
            }
        }

        /* Create a shared stream */
        axclrtStream sharedStream;
        ASSERT_EQ(AXCL_SUCC, axclrtCreateStream(&sharedStream));

        /* Thread function */
        auto threadFunc = [&](int threadId) {
            /* Execute operations on shared stream */
            EXPECT_EQ(AXCL_SUCC, axclrtMemsetAsync(devMem[threadId][0], 0, size, sharedStream));
            EXPECT_EQ(AXCL_SUCC, axclrtMemcpyAsync(devMem[threadId][0], hostMem[threadId], size, AXCL_MEMCPY_HOST_TO_DEVICE, sharedStream));
            EXPECT_EQ(AXCL_SUCC, axclrtMemsetAsync(devMem[threadId][1], 0, size, sharedStream));
            EXPECT_EQ(AXCL_SUCC, axclrtMemcpyAsync(devMem[threadId][1], hostMem[threadId], size, AXCL_MEMCPY_HOST_TO_DEVICE, sharedStream));

            /* Compare memory */
            EXPECT_EQ(AXCL_SUCC, axclrtMemcmpAsync(devMem[threadId][0], devMem[threadId][1], size, sharedStream));

            /* Synchronize shared stream */
            EXPECT_EQ(AXCL_SUCC, axclrtSynchronizeStream(sharedStream));
        };

        /* Create and start threads */
        std::vector<std::thread> threads;
        for (int i = 0; i < THREAD_NUM; i++) {
            threads.emplace_back(threadFunc, i);
        }

        /* Wait for all threads to complete */
        for (auto& thread : threads) {
            thread.join();
        }

        /* Destroy shared stream */
        EXPECT_EQ(AXCL_SUCC, axclrtDestroyStream(sharedStream));

        /* Free all memory */
        for (int i = 0; i < THREAD_NUM; i++) {
            for (int j = 0; j < 2; j++) {
                EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem[i][j]));
            }
            EXPECT_EQ(AXCL_SUCC, axclrtFreeHost(hostMem[i]));
        }

        step++;
        if (size >= 8 * MB) {
            break;
        }
    }
}

TEST_F(axclrtDeviceTest, Case16_MultiThreadEventSync) {
    /*
     * This test case demonstrates how to synchronize asynchronous operations between multiple threads using events
     * Key features:
     * 1. Main thread creates the event
     * 2. Thread 1 records event after its async operations
     * 3. Thread 2 waits for event before its async operations
     * 4. Ensures Thread 2's async operations start after Thread 1's operations complete
     */
    axclrtMemMallocPolicy policy = AXCL_MEM_MALLOC_NORMAL_ONLY;
    uint32_t step = 1;
    constexpr int THREAD_NUM = 2;

    while (1) {
        size_t size = step * MB;
        std::cout << "********* size: " << std::right << std::setw(8) << size << "(0x" << std::hex << size << ")" << std::dec << ": "
                  << std::endl;

        /* Pre-allocate memory for all threads */
        std::vector<void*> hostMem(THREAD_NUM, nullptr);
        std::vector<std::array<void*, 2>> devMem(THREAD_NUM);

        /* Allocate and initialize host memory */
        for (int i = 0; i < THREAD_NUM; i++) {
            ASSERT_EQ(AXCL_SUCC, axclrtMallocHost(&hostMem[i], size));
            m_random.randomize(static_cast<uint8_t*>(hostMem[i]), size);
        }

        /* Allocate device memory */
        for (int i = 0; i < THREAD_NUM; i++) {
            for (int j = 0; j < 2; j++) {
                ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem[i][j], size, policy));
            }
        }

        /* Create event for thread synchronization */
        axclrtEvent syncEvent;
        ASSERT_EQ(AXCL_SUCC, axclrtCreateEvent(&syncEvent));

        /* Thread 1 execution function */
        auto thread1Func = [&]() {
            axclrtContext context;
            EXPECT_EQ(AXCL_SUCC, axclrtCreateContext(&context, device_id));

            /* Create stream */
            axclrtStream stream1;
            EXPECT_EQ(AXCL_SUCC, axclrtCreateStream(&stream1));

            /* Execute async operations */
            EXPECT_EQ(AXCL_SUCC, axclrtMemsetAsync(devMem[0][0], 0, size, stream1));
            EXPECT_EQ(AXCL_SUCC, axclrtMemcpyAsync(devMem[0][0], hostMem[0], size, AXCL_MEMCPY_HOST_TO_DEVICE, stream1));
            EXPECT_EQ(AXCL_SUCC, axclrtMemsetAsync(devMem[0][1], 0, size, stream1));
            EXPECT_EQ(AXCL_SUCC, axclrtMemcpyAsync(devMem[0][1], hostMem[0], size, AXCL_MEMCPY_HOST_TO_DEVICE, stream1));
            /* Record event to mark Thread 1's operations completion */
            EXPECT_EQ(AXCL_SUCC, axclrtRecordEvent(syncEvent, stream1));

            /* Synchronize stream */
            EXPECT_EQ(AXCL_SUCC, axclrtSynchronizeStream(stream1));

            /* Cleanup stream */
            EXPECT_EQ(AXCL_SUCC, axclrtDestroyStream(stream1));
            EXPECT_EQ(AXCL_SUCC, axclrtDestroyContext(context));
        };

        /* Thread 2 execution function */
        auto thread2Func = [&]() {
            axclrtContext context;
            EXPECT_EQ(AXCL_SUCC, axclrtCreateContext(&context, device_id));

            /* Create stream */
            axclrtStream stream2;
            EXPECT_EQ(AXCL_SUCC, axclrtCreateStream(&stream2));

            /* Wait for Thread 1's operations to complete */
            EXPECT_EQ(AXCL_SUCC, axclrtStreamWaitEvent(stream2, syncEvent));

            /* Execute async operations */
            EXPECT_EQ(AXCL_SUCC, axclrtMemsetAsync(devMem[1][0], 0, size, stream2));
            EXPECT_EQ(AXCL_SUCC, axclrtMemcpyAsync(devMem[1][0], hostMem[1], size, AXCL_MEMCPY_HOST_TO_DEVICE, stream2));
            EXPECT_EQ(AXCL_SUCC, axclrtMemsetAsync(devMem[1][1], 0, size, stream2));
            EXPECT_EQ(AXCL_SUCC, axclrtMemcpyAsync(devMem[1][1], hostMem[1], size, AXCL_MEMCPY_HOST_TO_DEVICE, stream2));

            /* Synchronize stream */
            EXPECT_EQ(AXCL_SUCC, axclrtSynchronizeStream(stream2));

            /* Cleanup stream */
            EXPECT_EQ(AXCL_SUCC, axclrtDestroyStream(stream2));
            EXPECT_EQ(AXCL_SUCC, axclrtDestroyContext(context));
        };

        /* Create and start threads */
        std::thread t1(thread1Func);
        std::thread t2(thread2Func);

        /* Wait for threads to complete */
        t1.join();
        t2.join();

        /* Destroy event */
        EXPECT_EQ(AXCL_SUCC, axclrtDestroyEvent(syncEvent));

        /* Free memory */
        for (int i = 0; i < THREAD_NUM; i++) {
            for (int j = 0; j < 2; j++) {
                EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem[i][j]));
            }
            EXPECT_EQ(AXCL_SUCC, axclrtFreeHost(hostMem[i]));
        }

        step++;
        if (size >= 8 * MB) {
            break;
        }
        break;
    }
}

TEST_F(axclrtDeviceTest, Case17_axclrtMemcpyScatterList8M) {
    /**
     * Set scatter list memory size from 4MB to 8.5MB
     */
    axclrtMemMallocPolicy policy = AXCL_MEM_MALLOC_NORMAL_ONLY;
    ASSERT_EQ(AXCL_SUCC, axclrtResetDevice(device_id));
    ASSERT_EQ(AXCL_SUCC, axclFinalize());
    constexpr const char* json = R"({"dma buf size": "0x880000"})";
    ASSERT_EQ(AXCL_SUCC, axclInit(json));
    ASSERT_EQ(AXCL_SUCC, axclrtSetDevice(device_id));

    uint32_t step = 1;
    while (1) {
        size_t size = step++ * MB;
        std::cout << "size: " << std::right << std::setw(8) << size << "(0x" << std::hex << size << ")" << std::dec << ": " << std::endl;

        void* hostMem[2] = {nullptr, nullptr};
        void* devMem[2] = {nullptr, nullptr};

        /* 01: alloc 1st host memory and memset randomize data */
        ASSERT_EQ(AXCL_SUCC, axclrtMallocHost(&hostMem[0], size));
        m_random.randomize(static_cast<uint8_t*>(hostMem[0]), size);

        /* 02: alloc 1st device memory and memset to 0 */
        ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem[0], size, policy));
        EXPECT_EQ(AXCL_SUCC, axclrtMemset(devMem[0], 0, size));

        /* 03: memcpy 1st host memory to 1st device memory */
        EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem[0], hostMem[0], size, AXCL_MEMCPY_HOST_TO_DEVICE));

        /* 04: alloc 2nd host memory and memset to 0 */
        ASSERT_EQ(AXCL_SUCC, axclrtMallocHost(&hostMem[1], size));
        ::memset(hostMem[1], 0, size);

        /* 05: alloc 2nd device memory and copy 1st device memory to 2nd device memory */
        ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem[1], size, policy));
        EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem[1], devMem[0], size, AXCL_MEMCPY_DEVICE_TO_DEVICE));

        /* 06: memcpy 2nd device memory to 2nd host memory */
        EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(hostMem[1], devMem[1], size, AXCL_MEMCPY_DEVICE_TO_HOST));

        /* 07: compare 1st and 2nd host memory */
        EXPECT_EQ(0, ::memcmp(hostMem[0], hostMem[1], size));

        /* 08: free all memory */
        EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem[0]));
        EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem[1]));
        EXPECT_EQ(AXCL_SUCC, axclrtFreeHost(hostMem[0]));
        EXPECT_EQ(AXCL_SUCC, axclrtFreeHost(hostMem[1]));

        if (size >= 8 * MB) {
            break;
        }
    }
}