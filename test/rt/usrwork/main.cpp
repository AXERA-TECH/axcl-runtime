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
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <thread>
#include <vector>
#include "axcl.h"
#include "axcl_rt_usrwork.h"
#include "elapser.hpp"

#if defined(WINDOWS)
    #include <windows.h>
#endif

#define USRWORKER_UT "usrworker_ut"
#define SAMPLE_USRWORKER "sample_usrworker"

static std::string get_default_usrworker_path() {
#if defined(WINDOWS)
    return ".\\usrworker";
#else
    return "/opt/bin/axcl/usrworker";
#endif
}

static int32_t device_id = -1;
std::string usrworker_path = get_default_usrworker_path();
constexpr uint32_t KB = 1024;
constexpr uint32_t MB = 1024 * 1024;

static std::string get_temp_dir() {
    #if defined(WINDOWS)
        return usrworker_path + "/temp/";
    #else
        return "/tmp/";
    #endif
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    cmdline::parser parser;
    parser.add<uint32_t>("device", 'd', "device index", false, 0);
    parser.add<std::string>("usrworker", 'u', "usrworker path", false, get_default_usrworker_path());
    parser.parse_check(argc, argv);
    uint32_t device_index = parser.get<uint32_t>("device");
    usrworker_path = parser.get<std::string>("usrworker");

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

class axclrtUsrworkTest : public testing::Test {
protected:
    static void SetUpTestSuite() {
    }

    static void TearDownTestSuite() {
    }

    void SetUp() override {
        std::cout << std::endl;
    }

    void TearDown() override {
        std::cout << std::endl;
    }
};

TEST_F(axclrtUsrworkTest, Case01_axclrtTransferFile) {
    std::string bin = usrworker_path + std::string(1, std::filesystem::path::preferred_separator) + USRWORKER_UT;
    const char* src1 = bin.c_str();
    const char* dst1 = "/opt/bin/axclSlave/bin1";

    std::cout << "transfer file from host to device  : " << src1 << " to " << dst1 << std::endl;
    ASSERT_EQ(AXCL_SUCC, axclrtTransferFile(src1, dst1, FILE_TRANSFER_FROM_HOST_TO_DEVICE));

    const char* dst2 = "/opt/bin/axclSlave/bin2";
    std::cout << "transfer file from device to device: " << dst1 << " to " << dst2 << std::endl;
    ASSERT_EQ(AXCL_SUCC, axclrtTransferFile(dst1, dst2, FILE_TRANSFER_FROM_DEVICE_TO_DEVICE));

    std::string temp_dir = get_temp_dir();
    std::string src2 = temp_dir + "axcl_ut_rt_usrwork2_" + std::to_string(time(nullptr));
    std::cout << "transfer file from device to host  : " << dst2 << " to " << src2 << std::endl;
    ASSERT_EQ(AXCL_SUCC, axclrtTransferFile(dst2, src2.c_str(), FILE_TRANSFER_FROM_DEVICE_TO_HOST));

    std::ifstream file1(src1, std::ios::binary);
    std::ifstream file2(src2, std::ios::binary);
    std::string content1, content2;
    file1 >> content1;
    file2 >> content2;
    EXPECT_EQ(content1, content2);

    file1.close();
    file2.close();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::filesystem::remove(src2);
    EXPECT_EQ(AXCL_SUCC, axclrtTransferFile(dst1, nullptr, FILE_TRANSFER_REMOVE_DEVICE_FILE));
    EXPECT_EQ(AXCL_SUCC, axclrtTransferFile(dst2, nullptr, FILE_TRANSFER_REMOVE_DEVICE_FILE));
}

TEST_F(axclrtUsrworkTest, Case02_axclrtWorkerNormalMode) {
    std::string bin = usrworker_path + std::string(1, std::filesystem::path::preferred_separator) + USRWORKER_UT;
    const char* src = bin.c_str();
    const char* dst = "/opt/bin/usrworker_ut";

    std::cout << "Step 1: Transfer " << src << " to " << dst << std::endl;
    ASSERT_EQ(AXCL_SUCC, axclrtTransferFile(src, dst, FILE_TRANSFER_FROM_HOST_TO_DEVICE));

    const char* argv[] = {"-t", "0"};
    int32_t argc = sizeof(argv) / sizeof(argv[0]);
    uint32_t pid = 0;
    std::cout << "Step 2: Launch " << dst << std::endl;
    ASSERT_EQ(AXCL_SUCC, axclrtExecWorker(dst, &argc, argv, &pid));

    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "Step 3: Kill " << dst << std::endl;
    EXPECT_EQ(AXCL_SUCC, axclrtKillWorker(pid));
    EXPECT_EQ(AXCL_SUCC, axclrtTransferFile(dst, nullptr, FILE_TRANSFER_REMOVE_DEVICE_FILE));
}

TEST_F(axclrtUsrworkTest, Case03_axclrtWorkerKillNormalMode) {
    std::string bin = usrworker_path + std::string(1, std::filesystem::path::preferred_separator) + USRWORKER_UT;
    const char* src = bin.c_str();
    const char* dst = "/opt/bin/usrworker_ut";

    std::cout << "Step 1: Transfer " << src << " to " << dst << std::endl;
    ASSERT_EQ(AXCL_SUCC, axclrtTransferFile(src, dst, FILE_TRANSFER_FROM_HOST_TO_DEVICE));

    const char* argv[] = {"-t", "0", "-s", "10"};
    int32_t argc = sizeof(argv) / sizeof(argv[0]);
    uint32_t pid = 0;
    std::cout << "Step 2: Launch " << dst << std::endl;
    ASSERT_EQ(AXCL_SUCC, axclrtExecWorker(dst, &argc, argv, &pid));

    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "Step 3: Kill " << dst << std::endl;
    EXPECT_EQ(AXCL_SUCC, axclrtKillWorker(pid));
    EXPECT_EQ(AXCL_SUCC, axclrtTransferFile(dst, nullptr, FILE_TRANSFER_REMOVE_DEVICE_FILE));
}

TEST_F(axclrtUsrworkTest, Case04_axclrtWorkerInteractiveMode) {
    // Test complete worker flow: exec -> send -> recv -> kill
    // 1. Transfer file to device
    std::string bin = usrworker_path + std::string(1, std::filesystem::path::preferred_separator) + USRWORKER_UT;
    const char* src = bin.c_str();
    const char* dst = "/opt/bin/usrworker_ut";
    std::cout << "Step 1: Transfer " << src << " to " << dst << std::endl;
    ASSERT_EQ(AXCL_SUCC, axclrtTransferFile(src, dst, FILE_TRANSFER_FROM_HOST_TO_DEVICE));

    // 2. Exec worker
    const char* argv[] = {"-t", "2"};
    int32_t argc = sizeof(argv) / sizeof(argv[0]);
    uint32_t pid = 0;
    std::cout << "Step 2: Launch " << dst << std::endl;
    ASSERT_EQ(AXCL_SUCC, axclrtExecWorker(dst, &argc, argv, &pid));

    // 3. Send data to worker
    constexpr uint32_t timeout = 3000;
    const char* test_data = "This is a test message.";
    uint32_t data_size = static_cast<uint32_t>(strlen(test_data));
    std::cout << "Step 3: --> " << test_data << std::endl;
    EXPECT_EQ(AXCL_SUCC, axclrtWorkerSend(pid, test_data, data_size, timeout));

    // 4. Receive data from worker
    char recv_buffer[1024] = {0};
    uint32_t buffer_size = sizeof(recv_buffer);
    uint32_t recv_len = 0;
    EXPECT_EQ(AXCL_SUCC, axclrtWorkerRecv(pid, recv_buffer, buffer_size, &recv_len, timeout));
    std::cout << "Step 4: <-- " << recv_buffer << std::endl;

    EXPECT_EQ(0, strcmp(test_data, recv_buffer));

    // 5. Terminate worker process
    std::cout << "Step 5: Kill " << dst << std::endl;
    EXPECT_EQ(AXCL_SUCC, axclrtKillWorker(pid));
    EXPECT_EQ(AXCL_SUCC, axclrtTransferFile(dst, nullptr, FILE_TRANSFER_REMOVE_DEVICE_FILE));
}

TEST_F(axclrtUsrworkTest, Case05_axclrtWorkerCrashMode) {
    std::string bin = usrworker_path + std::string(1, std::filesystem::path::preferred_separator) + USRWORKER_UT;
    const char* src = bin.c_str();
    const char* dst = "/opt/bin/usrworker_ut";

    std::cout << "Step 1: Transfer " << src << " to " << dst << std::endl;
    ASSERT_EQ(AXCL_SUCC, axclrtTransferFile(src, dst, FILE_TRANSFER_FROM_HOST_TO_DEVICE));

    const char* argv[] = {"-t", "1", "-s", "3"};
    int32_t argc = sizeof(argv) / sizeof(argv[0]);
    uint32_t pid = 0;
    ASSERT_EQ(AXCL_SUCC, axclrtExecWorker(dst, &argc, argv, &pid));

    std::this_thread::sleep_for(std::chrono::seconds(5));

    EXPECT_EQ(AXCL_SUCC, axclrtKillWorker(pid));
    EXPECT_EQ(AXCL_SUCC, axclrtTransferFile(dst, nullptr, FILE_TRANSFER_REMOVE_DEVICE_FILE));
}

TEST_F(axclrtUsrworkTest, Case10_LaunchSampleUsrWorker) {
    std::string bin = usrworker_path + std::string(1, std::filesystem::path::preferred_separator) + SAMPLE_USRWORKER;
    const char* src = bin.c_str();
    const char* dst = "/opt/bin/sample_usrworker";
    std::cout << "Step 1: Transfer " << src << " to " << dst << std::endl;
    ASSERT_EQ(AXCL_SUCC, axclrtTransferFile(src, dst, FILE_TRANSFER_FROM_HOST_TO_DEVICE));

    std::cout << "Step 2: Launch " << dst << std::endl;
    const char* argv[] = {"-arg1", "1", "-arg2", "abc", "-arg3", "1.23"};
    int32_t argc = sizeof(argv) / sizeof(argv[0]);
    uint32_t pid = 0;
    ASSERT_EQ(AXCL_SUCC, axclrtExecWorker(dst, &argc, argv, &pid));

    char recv_msg[4096];
    uint32_t recv_len = 0;
    constexpr uint32_t timeout = 3000;
    std::cout << "Step 3: Loop messages " << std::endl;
    for (int i = 0; i < 10; i++) {
        std::string send_msg = "This is " + std::to_string(i) + " test message";
        uint32_t data_size = static_cast<uint32_t>(send_msg.size());
        ASSERT_EQ(AXCL_SUCC, axclrtWorkerSend(pid, send_msg.c_str(), data_size, timeout));
        std::cout << "--> " << send_msg << std::endl;
        memset(recv_msg, 0, sizeof(recv_msg));
        EXPECT_EQ(AXCL_SUCC, axclrtWorkerRecv(pid, recv_msg, sizeof(recv_msg), &recv_len, timeout));
        std::cout << "<-- " << recv_msg << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "Step 4: Kill " << dst << std::endl;
    EXPECT_EQ(AXCL_SUCC, axclrtWorkerSend(pid, "quit", 4, timeout));
    EXPECT_EQ(AXCL_SUCC, axclrtKillWorker(pid));
    EXPECT_EQ(AXCL_SUCC, axclrtTransferFile(dst, nullptr, FILE_TRANSFER_REMOVE_DEVICE_FILE));
}