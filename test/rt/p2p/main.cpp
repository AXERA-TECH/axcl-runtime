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
#include "axcl_rt_p2p_type.h"
#include "axcl_rt_p2p.h"
#include "axcl_rt_p2p_internal.h"

#include "elapser.hpp"

#define MEM_SIZE_MALLOC  (8 * 1024 * 1024)  // default is 8M
#define TIMEOUT_MS  (10 * 1000)

int type = 0;                               // test type: 0-P2P/1-RING/2-BROADCAST
int src = 0;                                // source ep index
int tar = 1;                                // target ep index
int p2p_num = 4;                            // p2p num
unsigned int loop_count = 1;                // c
unsigned int loop_interval = 0;             // i
unsigned long chunk_size = 1 * 1024 * 1024; // s
axclrtDeviceList device_list;


void print_help() {
    printf("Usage: program_name [OPTIONS]\n");
    printf("Options:\n");
    printf("  -t, --type <int>     Test type: 0-P2P/1-RING/2-BROADCAST/3-RING_NOE2E. Default is 0.\n");
    printf("  -s, --src <int>      Source ep index. Default is 0.\n");
    printf("  -r, --tar <int>      Target ep index. Default is 1.\n");
    printf("  -n, --num <int>      p2p number. Default is 4.\n");
    printf("  -c, --count <uint>   Loop count. Default is 1.\n");
    printf("  -i, --interval <uint> Loop interval. Default is 0.\n");
    printf("  -z, --size <ulong>   Chunk size in bytes. Default is 1MB.\n");
    printf("  -h, --help           Show this help message and exit.\n");
}

int findIndex(int arr[], int size, int target) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;
}

int* malloc_p2p_ring(int n, int start, int length) {
    start = start % n;
    if (start < 0) {
        start += n;
    }
    if (length > n) {
        length = n;
    }
    int* result = (int*)malloc(length * sizeof(int));
    if (result == NULL) {
        fprintf(stderr, "error mnalloc\n");
        return NULL;
    }
    for (int i = 0; i < length; i++) {
        result[i] = (start + i) % n;
    }

    printf("p2p_ring: ");
    if (result != NULL) {
        for (int i = 0; i < length; i++) {
            printf("%d ", result[i]);
        }
        printf("\n");
    }

    return result;
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    cmdline::parser parser;
    parser.add<int>("type", 't', "Test type: 0-P2P/1-RING/2-BROADCAST/3-RING_NOE2E", false, 0);
    parser.add<int>("src", 's', "Source ep index", false, 0);
    parser.add<int>("tar", 'r', "Target ep index", false, 1);
    parser.add<int>("num", 'n', "p2p number", false, 4);
    parser.add<unsigned int>("count", 'c', "Loop count", false, 1);
    parser.add<unsigned int>("interval", 'i', "Loop interval", false, 0);
    parser.add<unsigned long>("size", 'z', "Chunk size in bytes", false, 1 * 1024 * 1024);

    parser.parse_check(argc, argv);
    if (parser.exist("help") || parser.exist("h")) {
        print_help();
        return 0;
    }

    type = parser.get<int>("type");
    src = parser.get<int>("src");
    tar = parser.get<int>("tar");
    p2p_num = parser.get<int>("num");
    loop_count = parser.get<unsigned int>("count");
    loop_interval = parser.get<unsigned int>("interval");
    chunk_size = parser.get<unsigned long>("size");

    printf("test type: %d\n", type);
    printf("source ep index: %d\n", src);
    printf("target ep index: %d\n", tar);
    printf("p2p number: %d\n", p2p_num);
    printf("loop count: %u\n", loop_count);
    printf("loop interval: %u\n", loop_interval);
    printf("chunk size: %lu\n", chunk_size);

    /* init axcl */
    axclError ax_ret = axclInit("");
    if (AXCL_SUCC != ax_ret) {
        printf("axcl init fail, ret = 0x%x\n", ax_ret);
        return 1;
    }

    axclrtGetDeviceList(&device_list);
    std::cout << "device list:" << std::endl;
    for (uint32_t i = 0; i < device_list.num; ++i) {
        std::cout << "  " << i << ":" << device_list.devices[i] << std::endl;
    }


    /* run all tests */
    int ret = RUN_ALL_TESTS();
    if (0 == ret) {
        std::cout << "============= UT PASS =============" << std::endl;
    } else {
        std::cout << "============= UT FAIL =============" << std::endl;
    }

    /* deinit axcl */
    axclFinalize();
    return ret;
}

class axclrtP2pTest : public testing::Test {
protected:
    static void SetUpTestSuite() {
    }

    static void TearDownTestSuite() {
    }

};

TEST_F(axclrtP2pTest, Case01_axclrtTestP2P) {

    if (0 != type) {
        printf("Case01_axclrtTestP2P ignore\n");
        return;
    }

    axclError ret;

    /* create p2p unit: devices and cmm size*/
    axclrtP2PUnitInfo p2p_unit;
    p2p_unit.u32DeviceNum = p2p_num;
    for (uint32_t i = 0; i < p2p_unit.u32DeviceNum; ++i) {
        p2p_unit.n32DeviceId[i] = device_list.devices[i];
        p2p_unit.u32DeviceMemSize[i] = MEM_SIZE_MALLOC;
    }

    AXCL_P2P_UNIT_HANDLE handle = 0;
    ret = axclrtCreateP2PUnit(&p2p_unit, &handle);
    EXPECT_EQ(AXCL_SUCC, ret);
    EXPECT_NE(nullptr, handle);

    {

        int src_id = device_list.devices[src];
        int tar_id = device_list.devices[tar];
        printf("route from EP[%d:%d] to EP[%d:%d]\n", src, src_id, tar, tar_id);

        axclrtP2PTestConfig config;
        memset(&config, 0x0, sizeof(axclrtP2PTestConfig));
        config.num = 1;

        auto peer = &config.info[0];
        peer->device_id = src_id;
        peer->target_num = 1;

        peer->timeout_ms = TIMEOUT_MS;
        peer->target_info[0].target_id = tar_id;
        peer->target_info[0].loop_count = loop_count;
        peer->target_info[0].loop_interval = loop_interval;
        peer->target_info[0].chunk_size = chunk_size;

        ret = axclrtP2PTriggerTest(handle, &config);
        EXPECT_EQ(AXCL_SUCC, ret);

        for (int32_t i = 0; i < config.num; ++i) {
            auto peer = &config.info[i];
            auto res = &config.res[i];
            double size_all = ((double)chunk_size * (double)loop_count) / (double)(1024 * 1024);
            double spend_all = ((double)res->result.avg * (double)loop_count) / (double)(1000 * 1000);
            double rate = size_all / spend_all;

            std::cout << "p2p test" << std::dec
                << " src:" << findIndex(device_list.devices, device_list.num, peer->device_id) << ":" << peer->device_id
                << " to tar:" <<  findIndex(device_list.devices, device_list.num, peer->target_info[0].target_id) << ":"  << peer->target_info[0].target_id << ", "
                << " count: " << loop_count << ", "
                << " size: "<< chunk_size << ", "
                << " max: " << res->result.max << "us,"
                << " avg: " << res->result.avg << "us,"
                << " min: " << res->result.min << "us,"
                << " rate: " << rate << " MB/s" << ". "
                << std::endl;
        }
    }

    /* destory p2p unit: devices and cmm size*/
    ret = axclrtDestoryP2PUnit(handle);
    EXPECT_EQ(AXCL_SUCC, ret);
}

TEST_F(axclrtP2pTest, Case01_axclrtTestRing) {

    if (1 != type) {
        printf("Case01_axclrtTestRing ignore\n");
        return;
    }

    int timeout_ms = 10 * 1000;
    axclError ret;

    /* create p2p unit: devices and cmm size*/
    axclrtP2PUnitInfo p2p_unit;
    p2p_unit.u32DeviceNum = p2p_num;

    for (uint32_t i = 0; i < p2p_unit.u32DeviceNum; ++i) {
        p2p_unit.n32DeviceId[i] = device_list.devices[i];
        p2p_unit.u32DeviceMemSize[i] = MEM_SIZE_MALLOC;
    }

    AXCL_P2P_UNIT_HANDLE handle = 0;
    ret = axclrtCreateP2PUnit(&p2p_unit, &handle);
    EXPECT_EQ(AXCL_SUCC, ret);
    EXPECT_NE(nullptr, handle);

    {

        int* p2p_index_arr = malloc_p2p_ring(device_list.num, src, p2p_num);

        axclrtP2PTestConfig config;
        memset(&config, 0x0, sizeof(axclrtP2PTestConfig));
        config.num = p2p_num;

        int ii = 0;
        for (int32_t i = 0; i < config.num; ++i) {
            auto peer = &config.info[i];


            peer->target_num = 1;

            peer->timeout_ms = timeout_ms;

            ii = p2p_index_arr[i];
            int index = p2p_index_arr[i+1];
            if (i + 1 >= config.num) {
                index = p2p_index_arr[0];
            }

            peer->device_id = device_list.devices[ii];
            peer->target_info[0].target_id = device_list.devices[index];
            peer->target_info[0].loop_count = loop_count;
            peer->target_info[0].loop_interval = loop_interval;
            peer->target_info[0].chunk_size = chunk_size;
            printf("route from EP[%d:%d] to EP[%d:%d]\n", ii, device_list.devices[ii], index, device_list.devices[index]);
        }

        free(p2p_index_arr);
        ret = axclrtP2PTriggerTest(handle, &config);
        EXPECT_EQ(AXCL_SUCC, ret);

        for (int32_t i = 0; i < config.num; ++i) {
            auto peer = &config.info[i];
            auto res = &config.res[i];
            double size_all = ((double)chunk_size * (double)loop_count) / (double)(1024 * 1024);
            double spend_all = ((double)res->result.avg * (double)loop_count) / (double)(1000 * 1000);
            double rate = size_all / spend_all;

            std::cout << "p2p test" << std::dec
                << " src:" << findIndex(device_list.devices, device_list.num, peer->device_id) << ":" << peer->device_id
                << " to tar:" <<  findIndex(device_list.devices, device_list.num, peer->target_info[0].target_id) << ":"  << peer->target_info[0].target_id << ", "
                << " count: " << loop_count << ", "
                << " size: "<< chunk_size << ", "
                << " max: " << res->result.max << "us,"
                << " avg: " << res->result.avg << "us,"
                << " min: " << res->result.min << "us,"
                << " rate: " << rate << " MB/s" << ". "
                << std::endl;
        }
    }

    ret = axclrtDestoryP2PUnit(handle);
    EXPECT_EQ(AXCL_SUCC, ret);
}

TEST_F(axclrtP2pTest, Case01_axclrtTestRingNOE2E) {

    if (3 != type) {
        printf("Case01_axclrtTestRing ignore\n");
        return;
    }

    int timeout_ms = 10 * 1000;
    axclError ret;

    /* create p2p unit: devices and cmm size*/
    axclrtP2PUnitInfo p2p_unit;
    p2p_unit.u32DeviceNum = p2p_num;

    for (uint32_t i = 0; i < p2p_unit.u32DeviceNum; ++i) {
        p2p_unit.n32DeviceId[i] = device_list.devices[i];
        p2p_unit.u32DeviceMemSize[i] = MEM_SIZE_MALLOC;
    }

    AXCL_P2P_UNIT_HANDLE handle = 0;
    ret = axclrtCreateP2PUnit(&p2p_unit, &handle);
    EXPECT_EQ(AXCL_SUCC, ret);
    EXPECT_NE(nullptr, handle);

    {
        axclrtP2PTestConfig config;
        memset(&config, 0x0, sizeof(axclrtP2PTestConfig));
        config.num = p2p_num - 1;
        int* p2p_index_arr = malloc_p2p_ring(device_list.num, src, p2p_num);
        int ii = 0;

        for (int32_t i = 0; i < p2p_num; ++i) {
            auto peer = &config.info[i];
            peer->target_num = 1;
            peer->timeout_ms = timeout_ms;

            ii = p2p_index_arr[i];
            int index = p2p_index_arr[i+1];
            if (i + 1 >= p2p_num) {
                break;
            }

            peer->device_id = device_list.devices[ii];
            peer->target_info[0].target_id = device_list.devices[index];
            peer->target_info[0].loop_count = loop_count;
            peer->target_info[0].loop_interval = loop_interval;
            peer->target_info[0].chunk_size = chunk_size;

            printf("route from EP[%d:%d] to EP[%d:%d]\n", ii, device_list.devices[ii], index, device_list.devices[index]);
        }

        ret = axclrtP2PTriggerTest(handle, &config);
        EXPECT_EQ(AXCL_SUCC, ret);

        for (int32_t i = 0; i < config.num; ++i) {
            auto peer = &config.info[i];
            auto res = &config.res[i];
            double size_all = ((double)chunk_size * (double)loop_count) / (double)(1024 * 1024);
            double spend_all = ((double)res->result.avg * (double)loop_count) / (double)(1000 * 1000);
            double rate = size_all / spend_all;

            std::cout << "p2p test" << std::dec
                << " src:" << findIndex(device_list.devices, device_list.num, peer->device_id) << ":" << peer->device_id
                << " to tar:" <<  findIndex(device_list.devices, device_list.num, peer->target_info[0].target_id) << ":"  << peer->target_info[0].target_id << ", "
                << " count: " << loop_count << ", "
                << " size: "<< chunk_size << ", "
                << " max: " << res->result.max << "us,"
                << " avg: " << res->result.avg << "us,"
                << " min: " << res->result.min << "us,"
                << " rate: " << rate << " MB/s" << ". "
                << std::endl;
        }
    }

    ret = axclrtDestoryP2PUnit(handle);
    EXPECT_EQ(AXCL_SUCC, ret);
}

TEST_F(axclrtP2pTest, Case01_axclrtTestBroadcast) {
    if (2 != type) {
        printf("Case01_axclrtTestBroadcast ignore\n");
        return;
    }

    /* test type, source ep index */
    axclError ret;

    /* create p2p unit: devices and cmm size*/
    axclrtP2PUnitInfo p2p_unit;
    p2p_unit.u32DeviceNum = p2p_num;
    for (uint32_t i = 0; i < p2p_unit.u32DeviceNum; ++i) {
        p2p_unit.n32DeviceId[i] = device_list.devices[i];
        p2p_unit.u32DeviceMemSize[i] = MEM_SIZE_MALLOC;
    }

    AXCL_P2P_UNIT_HANDLE handle = 0;
    ret = axclrtCreateP2PUnit(&p2p_unit, &handle);
    EXPECT_EQ(AXCL_SUCC, ret);
    EXPECT_NE(nullptr, handle);

    {

        axclrtP2PTestConfig config;
        memset(&config, 0x0, sizeof(axclrtP2PTestConfig));
        config.num = 1;
        auto peer = &config.info[0];
        peer->device_id = device_list.devices[src];
        peer->target_num = p2p_num  - 1;
        peer->timeout_ms = TIMEOUT_MS;

        int index = 0;
        for (int32_t j = 0; j < p2p_num; ++j) {
            if (peer->device_id == device_list.devices[j]) {
                continue;
            }
            peer->target_info[index].target_id = device_list.devices[j];
            peer->target_info[index].loop_count = loop_count;
            peer->target_info[index].loop_interval = loop_interval;
            peer->target_info[index].chunk_size = chunk_size;
            printf("route from EP[%d:%d] to EP[%d:%d]\n", src, peer->device_id, j, peer->target_info[index].target_id);
            index++;
        }

        ret = axclrtP2PTriggerTest(handle, &config);
        EXPECT_EQ(AXCL_SUCC, ret);

        for (int32_t i = 0; i < config.num; ++i) {
            auto peer = &config.info[i];
            auto res = &config.res[i];
            double size_all = ((double)chunk_size * (double)loop_count) / (double)(1024 * 1024);
            double spend_all = ((double)res->result.avg * (double)loop_count) / (double)(1000 * 1000);
            double rate = size_all / spend_all;

            std::string target_info = "[";
            for (uint32_t j = 0; j < peer->target_num; ++j) {
                target_info += std::to_string(findIndex(device_list.devices, device_list.num, peer->target_info[j].target_id));
                target_info += ":";
                target_info += std::to_string(peer->target_info[j].target_id);
                if (j < peer->target_num - 1) {
                    target_info += " ";
                }
            }
            target_info += "]";

            std::cout << "p2p test" << std::dec
                << " src:" << findIndex(device_list.devices, device_list.num, peer->device_id) << ":" << peer->device_id
                << " to tar:" << target_info << ", "
                << " count: " << loop_count << ", "
                << " size: "<< chunk_size << ", "
                << " max: " << res->result.max << "us,"
                << " avg: " << res->result.avg << "us,"
                << " min: " << res->result.min << "us,"
                << " rate: " << rate << " MB/s" << ". "
                << std::endl;
        }
    }

    ret = axclrtDestoryP2PUnit(handle);
    EXPECT_EQ(AXCL_SUCC, ret);
}
