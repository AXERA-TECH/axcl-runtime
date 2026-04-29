/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include <iostream>
#include <gtest/gtest.h>
#include "def.hpp"
#include "mem_helper.hpp"
#include "elapser.hpp"
#include "randomizer.hpp"

static axcl::randomizer m_random;

#define MemCpyFunc(case_name, copy_size) \
TEST(MemoryCopyTest, case_name) { \
    constexpr size_t COPY_SIZE = copy_size; \
    uint8_t *src = (uint8_t *)malloc(COPY_SIZE); \
    m_random.randomize(src, COPY_SIZE); \
    uint64_t cost_time; \
    axcl::elapser elapse; \
    uint8_t *dst1 = (uint8_t *)malloc(COPY_SIZE); \
    uint8_t *dst2 = (uint8_t *)malloc(COPY_SIZE); \
    cost_time = 0; \
    for (int i = 0; i < MEMCPY_LOOP_CNT; ++i) { \
        elapse.start(); \
        memcpy(dst1, src, COPY_SIZE); \
        cost_time += elapse.cost(axcl::elapser::UNIT::microseconds); \
    } \
    std::cout << "posix memcpy cost: " << (double)cost_time / MEMCPY_LOOP_CNT << " us" << std::endl; \
    cost_time = 0; \
    for (int i = 0; i < MEMCPY_LOOP_CNT; ++i) { \
        elapse.start(); \
        axcl::mem_helper::memcpy(dst2, src, COPY_SIZE); \
        cost_time += elapse.cost(axcl::elapser::UNIT::microseconds); \
    } \
    std::cout << "axcl memcpy cost: " << (double)cost_time / MEMCPY_LOOP_CNT << " us" << std::endl; \
    EXPECT_EQ(0, memcmp(dst1, dst2, COPY_SIZE)); \
} \

MemCpyFunc(Case01_32K,  COPY_SIZE_32K)
MemCpyFunc(Case02_64K,  COPY_SIZE_64K)
MemCpyFunc(Case03_128K, COPY_SIZE_128K)
MemCpyFunc(Case04_256K, COPY_SIZE_256K)
MemCpyFunc(Case05_4M,   COPY_SIZE_4M)

int main(int argc, char** argv) {
    axcl::mem_helper::init_check_simd();
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
