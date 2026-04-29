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
#include <thread>
#include <random>
#include "../def.hpp"
#include "axcl_comm.hpp"
#include <algorithm>
#include "elapser.hpp"
#include <iomanip>

using namespace axcl::comm;

class CommTest : public testing::Test {
protected:
    void SetUp() override {
        m_socket = std::move(axcl_create_comm_instance(COMM_TYPE::COMM_TYPE_SOCKET));
        if (m_socket) {
            comm_args_t args;
            args.socket.type = SOCKET_TYPE::SOCKET_TYPE_SERVER;
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
        char send_buf[1024];
        sprintf(send_buf, "this is a message sent by server, msg id = %d", i);
        size_t len = strlen(send_buf);
        ASSERT_EQ(m_socket->send(send_buf, len, 0, 3000), len);
    }

    for (int i = 0; i < TOTAL_MSG_NUM; ++i) {
        char exp[1024] = {0};
        sprintf(exp, "this is a message sent by client, msg id = %d", i);
        size_t len = strlen(exp);

        void *recv_buf;
        ASSERT_EQ(m_socket->recv(&recv_buf, 0, 3000), len);
        ASSERT_TRUE(std::equal((uint8_t *)recv_buf, (uint8_t *)recv_buf + len, exp));
        m_socket->free(recv_buf);
    }

}

TEST_F(CommTest, Case02_SendRecvMsgThread) {
    ASSERT_NE(m_socket, nullptr);

    std::thread t1([this]() {
        for (int i = 0; i < TOTAL_MSG_NUM; ++i) {
            char exp[1024] = {0};
            sprintf(exp, "this is a message sent by client, msg id = %d", i);
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
            sprintf(send_buf, "this is a message sent by server, msg id = %d", i);
            size_t len = strlen(send_buf);
            ASSERT_EQ(m_socket->send(send_buf, len, 0, 3000), len);
        }
    });

    t1.join();
    t2.join();
}

TEST_F(CommTest, Case03_LoopTestDma) {
    ASSERT_NE(m_socket, nullptr);

    sleep(10);

    constexpr size_t LEN = NORM_DMA_BUF_SIZE;
    uint8_t *send_buf = (uint8_t *)malloc(LEN);
    ASSERT_NE(send_buf, nullptr);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint8_t> dis(0x00, 0xFF);
    for (size_t i = 0; i < LEN; ++i) {
        send_buf[i] = dis(gen);
    }

    void *recv_buf;
    for (int i = 0; i < NORM_DMA_LOOP_CNT; ++i) {
        ASSERT_EQ(m_socket->send((const void *)send_buf, LEN, 0, 3000), (size_t)LEN);
        ASSERT_EQ(m_socket->recv(&recv_buf, 0, 3000), LEN);
        ASSERT_TRUE(std::equal((uint8_t *)recv_buf, (uint8_t *)recv_buf + LEN, send_buf)) << "echo data are not equal";
        m_socket->free(recv_buf);
    }
    free(send_buf);
}

TEST_F(CommTest, Case04_LoopTestDmaHuge) {
    ASSERT_NE(m_socket, nullptr);

    constexpr size_t LEN = HUGE_DMA_BUF_SIZE;
    uint8_t *send_buf = (uint8_t *)malloc(LEN);
    ASSERT_NE(send_buf, nullptr);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint8_t> dis(0x00, 0xFF);
    for (size_t i = 0; i < LEN; ++i) {
        send_buf[i] = dis(gen);
    }

    void *recv_buf;
    for (int i = 0; i < HUGE_DMA_LOOP_CNT; ++i) {
        ASSERT_EQ(m_socket->send((const void *)send_buf, LEN, 0, 3000), (size_t)LEN);
        ASSERT_EQ(m_socket->recv(&recv_buf, 0, 3000), (size_t)LEN);
        ASSERT_TRUE(std::equal((uint8_t *)recv_buf, (uint8_t *)recv_buf + LEN, send_buf)) << "echo data are not equal";
        m_socket->free(recv_buf);
    }
}

TEST_F(CommTest, Case05_LatencyTest) {
    ASSERT_NE(m_socket, nullptr);

    uint8_t *send_buf = (uint8_t *)malloc(MAX_LATENCY_MSG_LEN);
    ASSERT_NE(send_buf, nullptr);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint8_t> dis(0x00, 0xFF);
    for (size_t i = 0; i < MAX_LATENCY_MSG_LEN; ++i) {
        send_buf[i] = dis(gen);
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
            ASSERT_EQ(m_socket->send((const void *)send_buf, len, 0, 3000), len);
            ASSERT_EQ(m_socket->recv(&recv_buf, 0, 3000), len);
            cost_time += elapse.cost(axcl::elapser::UNIT::microseconds);
            ASSERT_TRUE(std::equal((uint8_t *)recv_buf, (uint8_t *)recv_buf + len, send_buf)) << "echo data are not equal";
            m_socket->free(recv_buf);
        }

        std::cout << (double)cost_time / LATENCY_LOOP_CNT << " us" << std::endl;
    }

    free(send_buf);
}
