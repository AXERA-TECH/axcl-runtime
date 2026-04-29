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
#include <chrono>
#include <thread>
#include "axcl_logger.hpp"

#define TAG ("tag")

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(LoggerTest, Logger) {
    auto* logger_instance = axcl::logger::get_instance("./log/sub/a.log");
    ASSERT_NE(logger_instance, nullptr);


    LOG_MM_D(TAG, "debug logger  : with function and line {}", 0);
    LOG_MM_I(TAG, "info logger   : with function and line {}", 1);
    LOG_MM_W(TAG, "warning logger: with function and line {}", 2);
    LOG_MM_C(TAG, "critial logger: with function and line {}", 3);
    LOG_MM_E(TAG, "error logger  : with function and line {}", 4);

    LOG_M_D(TAG, "debug logger  : {}", 0);
    LOG_M_I(TAG, "info logger   : {}", 1);
    LOG_M_W(TAG, "warning logger: {}", 2);
    LOG_M_C(TAG, "critial logger: {}", 3);
    LOG_M_E(TAG, "error   logger: {}", 4);

    LOG_M_C(TAG, "change logger level to warning");
    AXCL_LOGGER->set_level(spdlog::level::warn);

    LOG_M_D(TAG, "debug   logger: NOT DISPLAYED ==== If you see this, it indicates a failure");
    LOG_M_I(TAG, "info    logger: NOT DISPLAYED ==== If you see this, it indicates a failure");
    LOG_M_W(TAG, "warning logger: is shown 0");
    LOG_M_C(TAG, "critial logger: is shown 1");
    LOG_M_E(TAG, "error   logger: is shown 2");

    AXCL_LOGGER->set_level(spdlog::level::debug);

    axcl::logger::flush_every(3);

    for (int i = 1; i <= 100; ++i) {
        LOG_M_I(TAG, "test logger in seconds {}", i);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    AXCL_LOGGER->flush();
}
