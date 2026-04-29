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
#include <atomic>
#include <vector>
#include "threadx.hpp"

class counter {
public:
    counter() : m_cash(0) {
    }

    void start(int32_t increment) {
        m_thread.start("count", &counter::thread_entry, this, increment);
    }

    void wait() {
        m_thread.join();
    }

    int32_t get_cash() const {
        return m_cash;
    }

protected:
    void thread_entry(int32_t increment) {
        for (int i = 0; i < 1000; ++i) {
            m_cash += increment;
        }
    }

private:
    axcl::threadx m_thread;
    int32_t m_cash;
};

TEST(ThreadTest, ClassMemberStartEntry) {
    counter count;
    count.start(2);
    count.wait();
    EXPECT_EQ(count.get_cash(), 2000);
}

TEST(ThreadTest, LamdaFuncEntry) {
    std::atomic<int32_t> counter = {0};
    int num = 5;
    int increment = 1000;

    std::vector<axcl::threadx> threads;

    for (int i = 0; i < num; ++i) {
        threads.emplace_back("cash" + std::to_string(i), [&counter, increment]() {
            for (int j = 0; j < increment; ++j) {
                ++counter;
            }
        });
    }

    for (auto& m : threads) {
        m.join();
    }

    EXPECT_EQ(counter.load(), num * increment);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}