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
#include <string.h>
#include <iostream>
#include <thread>
#include "../def.hpp"
#include "ax_sys_api.h"
#include "axcl_comm.hpp"
#include "axcl_package.hpp"

using namespace axcl::comm;

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

class CommTest : public testing::Test {
protected:
    void SetUp() override {
        m_pcie = axcl_create_comm_instance(COMM_TYPE::COMM_TYPE_PCIE);
        if (m_pcie) {
            comm_args_t args;
            args.pcie.handshake = true;
            args.pcie.target = 0;
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
        char exp[1024] = {0};
        sprintf(exp, "this is a message sent by rc, msg id = %d", i);
        size_t len = strlen(exp);

        void *recv_buf;
        ASSERT_EQ(m_pcie->recv(&recv_buf, 0, -1), len);
        ASSERT_TRUE(std::equal((uint8_t *)recv_buf, (uint8_t *)recv_buf + len, exp));
        m_pcie->free(recv_buf);
    }

    for (int i = 0; i < TOTAL_MSG_NUM; ++i) {
        char send_buf[1024];
        sprintf(send_buf, "this is a message sent by ep, msg id = %d", i);
        size_t len = strlen(send_buf);
        ASSERT_EQ(m_pcie->send(send_buf, len, axcl::comm::icomm::FLAG_RAWDATA, -1), len);
    }
}

TEST_F(CommTest, Case02_SendRecvMsgThread) {
    ASSERT_NE(m_pcie, nullptr);

    std::thread t1([this]() {
        for (int i = 0; i < TOTAL_MSG_NUM; ++i) {
            char exp[1024] = {0};
            sprintf(exp, "this is a message sent by rc, msg id = %d", i);
            size_t len = strlen(exp);

            void *recv_buf;
            ASSERT_EQ(m_pcie->recv(&recv_buf, 0, -1), len);
            ASSERT_TRUE(std::equal((uint8_t *)recv_buf, (uint8_t *)recv_buf + len, exp));
            m_pcie->free(recv_buf);
        }
    });

    std::thread t2([this]() {
        for (int i = 0; i < TOTAL_MSG_NUM; ++i) {
            char send_buf[1024];
            sprintf(send_buf, "this is a message sent by ep, msg id = %d", i);
            size_t len = strlen(send_buf);
            ASSERT_EQ(m_pcie->send(send_buf, len, axcl::comm::icomm::FLAG_RAWDATA, -1), len);
        }
    });

    t1.join();
    t2.join();
}

TEST_F(CommTest, Case03_LoopTestDma) {
    ASSERT_NE(m_pcie, nullptr);

    constexpr size_t LEN = NORM_DMA_BUF_SIZE;
    void *recv_buf;
    for (int i = 0; i < NORM_DMA_LOOP_CNT; ++i) {
        ASSERT_EQ(m_pcie->recv(&recv_buf, 0, -1), LEN);
        ASSERT_EQ(m_pcie->send((const void *)recv_buf, LEN, axcl::comm::icomm::FLAG_RAWDATA, -1), LEN);
        m_pcie->free(recv_buf);
    }
}

TEST_F(CommTest, Case04_LoopTestDmaHuge) {
    ASSERT_NE(m_pcie, nullptr);

    constexpr size_t LEN = HUGE_DMA_BUF_SIZE;

    void *recv_buf;
    for (int i = 0; i < HUGE_DMA_LOOP_CNT; ++i) {
        ASSERT_EQ(m_pcie->recv(&recv_buf, 0, -1), LEN);
        ASSERT_EQ(m_pcie->send((const void *)recv_buf, LEN, axcl::comm::icomm::FLAG_RAWDATA, -1), LEN);
        m_pcie->free(recv_buf);
    }
}

TEST_F(CommTest, Case05_LatencyTest) {
    ASSERT_NE(m_pcie, nullptr);

    void *recv_buf;
    size_t len = 1;
    while (1) {
        len *= 2;
        if (len > MAX_LATENCY_MSG_LEN) {
            break;
        }

        for (int i = 0; i < LATENCY_LOOP_CNT; ++i) {
            ASSERT_EQ(m_pcie->recv(&recv_buf, 0, -1), len);
            ASSERT_EQ(m_pcie->send((const void *)recv_buf, len, axcl::comm::icomm::FLAG_RAWDATA, -1), len);
            m_pcie->free(recv_buf);
        }
    };
}

TEST_F(CommTest, Case06_TestMemcpyHandlerFromHostToDevice) {
    ASSERT_NE(m_pcie, nullptr);

    ASSERT_EQ(0, AX_SYS_Init());

    /* 01: alloc CMM buffer and send to host */
    AX_U64 dev_phy;
    AX_VOID *dev_vir;
    constexpr AX_U32 mem_size = 8 * 1024 * 1024;
    ASSERT_EQ(0, AX_SYS_MemAlloc(&dev_phy, &dev_vir, mem_size, 128, (const AX_S8 *)"test"));
    ::memset(dev_vir, 0, mem_size);

    msg_data_t msg;
    msg.msg = MSG_DEVICE_CMM_MEMORY;
    *((uint64_t *)&msg.data[0]) = dev_phy;
    *((uint32_t *)&msg.data[sizeof(uint64_t)]) = mem_size;
    ASSERT_EQ(sizeof(msg), m_pcie->send((const void *)&msg, sizeof(msg), axcl::comm::icomm::FLAG_RAWDATA, -1));

    /* 02: recv memcpy package */
    void *rcv_buf;
    ASSERT_EQ(sizeof(axcl::pkg::memcpy_package), m_pcie->recv(&rcv_buf, 0, -1));
    axcl::pkg::memcpy_package pkg = *(reinterpret_cast<axcl::pkg::memcpy_package *>(rcv_buf));
    m_pcie->free(rcv_buf);
    ASSERT_EQ(static_cast<uint32_t>(RUNTIME_MEMCPY_HANDLER_TYPE), pkg.hd.type);
    ASSERT_EQ(dev_phy, pkg.dst_phy);
    ASSERT_EQ(mem_size, pkg.size);

    /* 03: check memory */
    uint8_t *data = (uint8_t *)dev_vir;
    for (AX_U32 i = 0; i < mem_size; ++i) {
        ASSERT_EQ(0xAA, data[i]);
    }

    ASSERT_EQ(0, AX_SYS_MemFree(dev_phy, dev_vir));
    ASSERT_EQ(0, AX_SYS_Deinit());
}

TEST_F(CommTest, Case7_TestMemcpyHandlerFromDeviceToHost) {
    ASSERT_NE(m_pcie, nullptr);

    ASSERT_EQ(0, AX_SYS_Init());

    /* 01: alloc CMM buffer and send to host */
    AX_U64 dev_phy;
    AX_VOID *dev_vir;
    constexpr AX_U32 mem_size = 8 * 1024 * 1024;
    ASSERT_EQ(0, AX_SYS_MemAlloc(&dev_phy, &dev_vir, mem_size, 128, (const AX_S8 *)"test"));
    ::memset(dev_vir, 0xBB, mem_size);

    msg_data_t msg;
    msg.msg = MSG_DEVICE_CMM_MEMORY;
    *((uint64_t *)&msg.data[0]) = dev_phy;
    *((uint32_t *)&msg.data[sizeof(uint64_t)]) = mem_size;
    ASSERT_EQ(sizeof(msg), m_pcie->send((const void *)&msg, sizeof(msg), axcl::comm::icomm::FLAG_RAWDATA, -1));

    /* 02: recv memcpy package */
    void *rcv_buf;
    ASSERT_EQ(sizeof(axcl::pkg::memcpy_package), m_pcie->recv(&rcv_buf, 0, -1));
    axcl::pkg::memcpy_package pkg = *(reinterpret_cast<axcl::pkg::memcpy_package *>(rcv_buf));
    m_pcie->free(rcv_buf);
    ASSERT_EQ(static_cast<uint32_t>(RUNTIME_MEMCPY_HANDLER_TYPE), pkg.hd.type);
    ASSERT_EQ(dev_phy, pkg.src_phy);
    ASSERT_EQ(mem_size, pkg.size);

    ASSERT_EQ(0, AX_SYS_MemFree(dev_phy, dev_vir));
    ASSERT_EQ(0, AX_SYS_Deinit());
}

