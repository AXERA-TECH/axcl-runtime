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
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>
#include <thread>
#include "../def.hpp"
#include "axcl_comm.hpp"
#include "elapser.hpp"

using namespace axcl::comm;

class CommTest : public testing::Test {
protected:
    void SetUp() override {
        m_socket = std::move(axcl_create_comm_instance(COMM_TYPE::COMM_TYPE_SOCKET));
        if (m_socket) {
            comm_args_t args;
            args.socket.type = SOCKET_TYPE::SOCKET_TYPE_CLIENT;
            args.socket.port = SOCKET_PORT;
            char ip[] = "127.0.0.1";
            memcpy(args.socket.ip, ip, strlen(ip));
            if (!m_socket->open(args)) {
                m_socket.reset();
            }
        }
    }

    void TearDown() override {
        if (m_socket) {
            m_socket->close();
            m_socket.reset();
        }
    }

    std::shared_ptr<icomm> m_socket;
};

TEST_F(CommTest, Case01_SendRecvMsg) {
    ASSERT_NE(m_socket, nullptr);

    for (int i = 0; i < TOTAL_MSG_NUM; ++i) {
        char exp[1024] = {0};
        sprintf(exp, "this is a message sent by server, msg id = %d", i);
        size_t len = strlen(exp);

        void *recv_buf;
        ASSERT_EQ(m_socket->recv(&recv_buf, 0, 3000), len);
        ASSERT_TRUE(std::equal((uint8_t *)recv_buf, (uint8_t *)recv_buf + len, exp));
        m_socket->free(recv_buf);
    }

    for (int i = 0; i < TOTAL_MSG_NUM; ++i) {
        char send_buf[1024];
        sprintf(send_buf, "this is a message sent by client, msg id = %d", i);
        size_t len = strlen(send_buf);
        ASSERT_EQ(m_socket->send(send_buf, len, 0, 3000), len);
    }
}

TEST_F(CommTest, Case02_SendRecvMsgThread) {
    ASSERT_NE(m_socket, nullptr);

    std::thread t1([this]() {
        for (int i = 0; i < TOTAL_MSG_NUM; ++i) {
            char exp[1024] = {0};
            sprintf(exp, "this is a message sent by server, msg id = %d", i);
            size_t len = strlen(exp);

            void *recv_buf;
            ASSERT_EQ(m_socket->recv(&recv_buf, 0, 3000), len);
            ASSERT_TRUE(std::equal((uint8_t *)recv_buf, (uint8_t *)recv_buf + len, exp));
            m_socket->free(recv_buf);
        }
    });

    std::thread t2([this]() {
        for (int i = 0; i < TOTAL_MSG_NUM; ++i) {
            char send_buf[1024];
            sprintf(send_buf, "this is a message sent by client, msg id = %d", i);
            size_t len = strlen(send_buf);
            ASSERT_EQ(m_socket->send(send_buf, len, 0, 3000), len);
        }
    });

    t1.join();
    t2.join();
}

TEST_F(CommTest, Case03_LoopTestDma) {
    ASSERT_NE(m_socket, nullptr);

    constexpr size_t LEN = NORM_DMA_BUF_SIZE;
    void *recv_buf;
    for (int i = 0; i < NORM_DMA_LOOP_CNT; ++i) {
        ASSERT_EQ(m_socket->recv(&recv_buf, 0, 3000), LEN);
        ASSERT_EQ(m_socket->send((const void *)recv_buf, LEN, 0, 3000), LEN);
        m_socket->free(recv_buf);
    }
}

TEST_F(CommTest, Case04_LoopTestDmaHuge) {
    ASSERT_NE(m_socket, nullptr);

    constexpr size_t LEN = HUGE_DMA_BUF_SIZE;

    void *recv_buf;
    for (int i = 0; i < HUGE_DMA_LOOP_CNT; ++i) {
        ASSERT_EQ(m_socket->recv(&recv_buf, 0, 3000), LEN);
        ASSERT_EQ(m_socket->send((const void *)recv_buf, LEN, 0, 3000), LEN);
        m_socket->free(recv_buf);
    }
}

TEST_F(CommTest, Case05_LatencyTest) {
    ASSERT_NE(m_socket, nullptr);

    void *recv_buf;
    size_t len = 1;
    while (1) {
        len *= 2;
        if (len > MAX_LATENCY_MSG_LEN) {
            break;
        }

        for (int i = 0; i < LATENCY_LOOP_CNT; ++i) {
            ASSERT_EQ(m_socket->recv(&recv_buf, 0, 3000), len);
            ASSERT_EQ(m_socket->send((const void *)recv_buf, len, 0, 3000), len);
            m_socket->free(recv_buf);
        }
    };
}
