/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include <cmdline.h>
#include <gtest/gtest.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>
#include <random>
#include <thread>
#include "../def.hpp"
#include "axcl_comm.hpp"
#include "axcl_package.hpp"
#include "elapser.hpp"

using namespace axcl::comm;

static int32_t device_id = 0x81;

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    cmdline::parser parser;
    parser.add<int32_t>("device", 'd', "device index [1-256]", false, 0x81, cmdline::range(1, 256));

    parser.parse_check(argc, argv);

    device_id = parser.get<int32_t>("device");

    if (device_id <= 0 || device_id > 256) {
        printf("usage: -d, --device=xxx [1 - 256]\n");
        return 1;
    }

    return RUN_ALL_TESTS();
}

class CommTest : public testing::Test {
protected:
    void SetUp() override {
        m_pcie = axcl_create_comm_instance(COMM_TYPE::COMM_TYPE_PCIE);
        if (m_pcie) {
            axcl::elapser::sleep(1);

            comm_args_t args;
            args.pcie.handshake = true;
            args.pcie.target = device_id;
            args.pcie.port[0] = PCIE_PORT;
            args.pcie.port[1] = PCIE_PORT + 1;
            args.pcie.port[2] = PCIE_PORT + 2;
            args.pcie.port[3] = PCIE_PORT + 3;
            if (!m_pcie->open(args)) {
                m_pcie.reset();
            }
        }
    }

    void TearDown() override {
        if (m_pcie) {
            m_pcie->close();
            m_pcie.reset();
        }
    }

    std::shared_ptr<icomm> m_pcie;
};

TEST_F(CommTest, Case01_SendRecvMsg) {
    ASSERT_NE(m_pcie, nullptr);

    for (int i = 0; i < TOTAL_MSG_NUM; ++i) {
        char send_buf[1024];
        sprintf(send_buf, "this is a message sent by rc, msg id = %d", i);
        size_t len = strlen(send_buf);
        ASSERT_EQ(m_pcie->send(send_buf, len, axcl::comm::icomm::FLAG_RAWDATA, -1), len);
    }

    for (int i = 0; i < TOTAL_MSG_NUM; ++i) {
        char exp[1024] = {0};
        sprintf(exp, "this is a message sent by ep, msg id = %d", i);
        size_t len = strlen(exp);

        void *recv_buf;
        ASSERT_EQ(m_pcie->recv(&recv_buf, 0, -1), len);
        ASSERT_TRUE(std::equal((uint8_t *)recv_buf, (uint8_t *)recv_buf + len, exp));
        m_pcie->free(recv_buf);
    }
}

TEST_F(CommTest, Case02_SendRecvMsgThread) {
    ASSERT_NE(m_pcie, nullptr);

    std::thread t1([this]() {
        for (int i = 0; i < TOTAL_MSG_NUM; ++i) {
            char send_buf[1024];
            sprintf(send_buf, "this is a message sent by rc, msg id = %d", i);
            size_t len = strlen(send_buf);
            ASSERT_EQ(m_pcie->send(send_buf, len, axcl::comm::icomm::FLAG_RAWDATA, -1), len);
        }
    });

    std::thread t2([this]() {
        for (int i = 0; i < TOTAL_MSG_NUM; ++i) {
            char exp[1024] = {0};
            sprintf(exp, "this is a message sent by ep, msg id = %d", i);
            size_t len = strlen(exp);

            void *recv_buf;
            ASSERT_EQ(m_pcie->recv(&recv_buf, 0, -1), len);
            ASSERT_TRUE(std::equal((uint8_t *)recv_buf, (uint8_t *)recv_buf + len, exp));
            m_pcie->free(recv_buf);
        }
    });

    t1.join();
    t2.join();
}

TEST_F(CommTest, Case03_LoopTestDma) {
    ASSERT_NE(m_pcie, nullptr);

    constexpr size_t LEN = NORM_DMA_BUF_SIZE;
    std::array<uint8_t, LEN> send_buf = {0};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0x00, 0xFF);
    for (auto &m : send_buf) {
        m = static_cast<uint8_t>(dis(gen));
    }

    void *recv_buf;
    for (int i = 0; i < NORM_DMA_LOOP_CNT; ++i) {
        ASSERT_EQ(m_pcie->send((const void *)send_buf.data(), LEN, axcl::comm::icomm::FLAG_RAWDATA, -1), (size_t)LEN);
        ASSERT_EQ(m_pcie->recv(&recv_buf, 0, -1), LEN);
        ASSERT_TRUE(std::equal((uint8_t *)recv_buf, (uint8_t *)recv_buf + LEN, send_buf.data())) << "echo data are not equal";
        m_pcie->free(recv_buf);
    }
}

TEST_F(CommTest, Case04_LoopTestDmaHuge) {
    ASSERT_NE(m_pcie, nullptr);

    constexpr size_t LEN = HUGE_DMA_BUF_SIZE;
    uint8_t *send_buf = (uint8_t *)malloc(LEN);
    ASSERT_NE(send_buf, nullptr);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0x00, 0xFF);
    for (size_t i = 0; i < LEN; ++i) {
        send_buf[i] = static_cast<uint8_t>(dis(gen));
    }

    void *recv_buf;
    for (int i = 0; i < HUGE_DMA_LOOP_CNT; ++i) {
        ASSERT_EQ(m_pcie->send((const void *)send_buf, LEN, axcl::comm::icomm::FLAG_RAWDATA, -1), (size_t)LEN);
        ASSERT_EQ(m_pcie->recv(&recv_buf, 0, -1), (size_t)LEN);
        ASSERT_TRUE(std::equal((uint8_t *)recv_buf, (uint8_t *)recv_buf + LEN, send_buf)) << "echo data are not equal";
        m_pcie->free(recv_buf);
    }

    free(send_buf);
}

TEST_F(CommTest, Case05_LatencyTest) {
    ASSERT_NE(m_pcie, nullptr);

    uint8_t *send_buf = (uint8_t *)malloc(MAX_LATENCY_MSG_LEN);
    ASSERT_NE(send_buf, nullptr);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0x00, 0xFF);
    for (size_t i = 0; i < MAX_LATENCY_MSG_LEN; ++i) {
        send_buf[i] = static_cast<uint8_t>(dis(gen));
    }

    axcl::elapser elapse;
    elapse.sleep(1); /* make sure EP is ready */

    void *recv_buf;
    uint64_t cost_time;
    size_t len = 1;
    while (1) {
        len *= 2;
        if (len > MAX_LATENCY_MSG_LEN) {
            break;
        }

        std::cout << "len: " << std::right << std::setw(8) << len << ": ";

        cost_time = 0;
        for (int i = 0; i < LATENCY_LOOP_CNT; ++i) {
            elapse.start();
            ASSERT_EQ(m_pcie->send((const void *)send_buf, len, axcl::comm::icomm::FLAG_RAWDATA, -1), len);
            ASSERT_EQ(m_pcie->recv(&recv_buf, 0, -1), len);
            cost_time += elapse.cost(axcl::elapser::UNIT::microseconds);
            ASSERT_TRUE(std::equal((uint8_t *)recv_buf, (uint8_t *)recv_buf + len, send_buf)) << "echo data are not equal";
            m_pcie->free(recv_buf);
        }

        std::cout << (double)cost_time / LATENCY_LOOP_CNT << " us" << std::endl;
    }

    free(send_buf);
}

TEST_F(CommTest, Case06_TestMemcpyHandlerFromHostToDevice) {
    ASSERT_NE(m_pcie, nullptr);

    /* 01: receive device memory to copy */
    void *rcv_buf;
    constexpr size_t msg_len = sizeof(msg_data_t);
    ASSERT_EQ(msg_len, m_pcie->recv(&rcv_buf, axcl::comm::icomm::FLAG_RAWDATA, -1));
    msg_data_t msg = *(reinterpret_cast<msg_data_t *>(rcv_buf));
    m_pcie->free(rcv_buf);

    uint64_t dev_mem = *((uint64_t *)&msg.data[0]);
    uint32_t mem_size = *((uint32_t *)&msg.data[sizeof(uint64_t)]);
    ASSERT_EQ(msg.msg, MSG_DEVICE_CMM_MEMORY);
    ASSERT_NE(0ULL, dev_mem);
    ASSERT_NE(0L, mem_size);

    /* 02: malloc host memory */
    void *host_mem = ::malloc(mem_size);
    ::memset(host_mem, 0xAA, mem_size);

    /* 03: trig memcpy from host to device */
    axcl::pkg::memcpy_package pkg;
    pkg.hd.type = RUNTIME_MEMCPY_HANDLER_TYPE;
    pkg.kind = axcl::pkg::HOST_VIR_TO_DEVICE;
    pkg.dst_phy = dev_mem;
    pkg.size = mem_size;
    pkg.data = host_mem;

    axcl::elapser elapse;
    ASSERT_EQ(sizeof(pkg), m_pcie->send((const void *)&pkg, sizeof(pkg), 0, -1));
    std::cout << "size 0x" << std::hex << mem_size << ": " << std::dec << elapse.cost(axcl::elapser::UNIT::microseconds) << " us"
              << std::endl;

    ::free(host_mem);
}

TEST_F(CommTest, Case07_TestMemcpyHandlerFromDeviceToHost) {
    ASSERT_NE(m_pcie, nullptr);

    /* 01: receive device memory to copy */
    void *rcv_buf;
    constexpr size_t msg_len = sizeof(msg_data_t);
    ASSERT_EQ(msg_len, m_pcie->recv(&rcv_buf, axcl::comm::icomm::FLAG_RAWDATA, -1));
    msg_data_t msg = *(reinterpret_cast<msg_data_t *>(rcv_buf));
    m_pcie->free(rcv_buf);

    uint64_t dev_mem = *((uint64_t *)&msg.data[0]);
    uint32_t mem_size = *((uint32_t *)&msg.data[sizeof(uint64_t)]);
    ASSERT_EQ(msg.msg, MSG_DEVICE_CMM_MEMORY);
    ASSERT_NE(0ULL, dev_mem);
    ASSERT_NE(0L, mem_size);

    /* 02: malloc host memory */
    void *host_mem = ::malloc(mem_size);
    ::memset(host_mem, 0x00, mem_size);

    /* 03: trig memcpy from host to device */
    axcl::pkg::memcpy_package pkg;
    pkg.hd.type = RUNTIME_MEMCPY_HANDLER_TYPE;
    pkg.kind = axcl::pkg::DEVICE_TO_HOST_VIR;
    pkg.src_phy = dev_mem;
    pkg.size = mem_size;
    pkg.data = host_mem;

    axcl::elapser elapse;
    ASSERT_EQ(sizeof(pkg), m_pcie->send((const void *)&pkg, sizeof(pkg), 0, -1));
    std::cout << "size 0x" << std::hex << mem_size << ": " << std::dec << elapse.cost(axcl::elapser::UNIT::microseconds) << " us"
              << std::endl;

    /* 04: compare memcpy result */
    uint8_t *data = (uint8_t *)host_mem;
    for (uint32_t i = 0; i < mem_size; ++i) {
        ASSERT_EQ(0xBB, data[i]);
    }

    ::free(host_mem);
}
