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

#include "axcl.h"
#include "axcl_rt_engine_internal.h"
#include "axcl_rt_p2p.h"

#include "res_guard.hpp"

#include <cmdline.h>

#include <array>
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

enum class P2P_MODE : uint32_t {
    P2P_PARALLEL_NONE_BLOCK = 1,
    P2P_PARALLEL_BLOCK = 2,
    P2P_SEQUENTIAL_NONE_BLOCK = 4,
    P2P_SEQUENTIAL_BLOCK = 8,
    P2P_INTERLACE_NONE_BLOCK = 16,
    P2P_INTERLACE_BLOCK = 32,
    P2P_ALL_MODE = P2P_PARALLEL_NONE_BLOCK |
        P2P_PARALLEL_BLOCK |
        P2P_SEQUENTIAL_NONE_BLOCK |
        P2P_SEQUENTIAL_BLOCK |
        P2P_INTERLACE_NONE_BLOCK |
        P2P_INTERLACE_BLOCK,
};

struct sched {
    bool& ready;
    std::mutex& mutex;
    std::condition_variable& cond;

    std::vector<bool>& flags;
    std::vector<std::array<float, 10>>& bws;
};

constexpr size_t size = 8 * 1024 * 1024;

std::vector<uint32_t> ids;
auto mode = P2P_MODE::P2P_INTERLACE_BLOCK;

bool thread_work(const sched& sched, const P2P_MODE mode, const size_t size, axclrtP2PTestRes& bw) {
    {
        std::unique_lock lock(sched.mutex);
        sched.cond.wait(lock, [&] { return sched.ready; });
    }

    if (const auto ret = axclrtEngineP2pBwTest(&bw, static_cast<uint32_t>(mode), size); AXCL_SUCC != ret) {
        std::cout << "[ERROR] thread: " << std::this_thread::get_id() << " thread bw test size: " << size << " failed: " << ret << "." << std::endl;
        return false;
    }
    return true;
}

void thread_func(const sched& sched, const size_t index, const P2P_MODE mode, const size_t size) {
    //std::cout << "[DEBUG] thread id: " << std::this_thread::get_id() << std::endl;

    const auto id = ids[index];
    auto ctx_holder = res_guard<axclrtContext>(
        [&] {
            axclrtContext context = nullptr;
            if (const axclError ret = axclrtCreateContext(&context, static_cast<int32_t>(id)); AXCL_SUCC != ret) {
                std::cout << "[ERROR] thread id: " << std::this_thread::get_id() << " create device: " << id << " context failed: " << ret << std::endl;
            }
            return context;
        },
        [](axclrtContext ctx) {
            if (ctx) {
                axclrtDestroyContext(ctx);
            }
        }
    );
    if (nullptr == ctx_holder.get()) {
        sched.flags[index] = false;
        return;
    }

    axclrtP2PTestRes bw{};
    sched.flags[index] = thread_work(sched, mode, size, bw);
    if (sched.flags[index]) {
        for (size_t i = 0; i < bw.count; ++i) {
            sched.bws[index][i] = bw.elapsed[i];
        }
    }

    //std::cout << "[DEBUG] thread id: " << std::this_thread::get_id() << " exit." << std::endl;
}

class axclrtDeviceTest : public testing::Test {
protected:
    static void SetUpTestSuite() {
    }

    static void TearDownTestSuite() {
    }
};

void print(const std::vector<size_t>& sizes, const std::vector<std::vector<std::array<float, 10>>>& elapses, const std::vector<std::vector<bool>>& results) {
    printf("-----------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("|  size   |  id  |    1       2       3       4      5        6       7       8       9      10    |   min   |   max   | all avg | MB/s |\n");
    printf("-----------------------------------------------------------------------------------------------------------------------------------------\n");

    std::vector<std::vector<float>> min_elapse(sizes.size(), std::vector<float>(ids.size(), 0.f));
    std::vector<std::vector<float>> max_elapse(sizes.size(), std::vector<float>(ids.size(), 0.f));
    std::vector<float> avg_elapse(sizes.size(), 0.f);
    std::vector<float> avg_bw(sizes.size(), 0.f);
    for (size_t size_index = 0; size_index < sizes.size(); ++size_index) {
        const auto current_size = sizes[size_index];
        auto total_avg_elapse_for_size = 0.f;
        for (size_t id_index = 0; id_index < ids.size(); ++id_index) {
            const auto& elapse_array = elapses[size_index][id_index];

            min_elapse[size_index][id_index] = elapse_array[0];
            max_elapse[size_index][id_index] = elapse_array[0];
            auto sum_elapse = 0.f;
            for (float elapse : elapse_array) {
                min_elapse[size_index][id_index] = std::min(min_elapse[size_index][id_index], elapse);
                max_elapse[size_index][id_index] = std::max(max_elapse[size_index][id_index], elapse);
                sum_elapse += elapse;
            }
            total_avg_elapse_for_size += sum_elapse / static_cast<float>(elapse_array.size());
        }

        avg_elapse[size_index] = total_avg_elapse_for_size / static_cast<float>(ids.size());
        avg_bw[size_index] = static_cast<float>(current_size * (ids.size() - 1) * 1000) / (1024.f * 1024.f) / avg_elapse[size_index];
    }

    for (size_t size_index = 0; size_index < sizes.size(); ++size_index) {
        const auto current_size = sizes[size_index];
        const auto& dev_flags = results[size_index];
        for (size_t id_index = 0; id_index < ids.size(); ++id_index) {
            const auto dev_flag = dev_flags[id_index];

            // size, dev id
            if (0 == id_index) {
                printf("| %7zu | %4u | ", current_size, ids[id_index]);
            }
            else {
                printf("|         | %4u | ", ids[id_index]);
            }
            // elapses on dev
            if (dev_flag) {
                for (const float i : elapses[size_index][id_index]) {
                    if (999.f < i) {
                        printf("TIMEOUT ");
                    } else {
                        printf("%7.3f ", i);
                    }
                }
                printf("| %7.3f | %7.3f |", min_elapse[size_index][id_index], max_elapse[size_index][id_index]);
            } else {
                printf(" --.--   --.--   --.--   --.--   --.--   --.--   --.--   --.--   --.--   --.--  |  --.--  |  --.--  |");
            }
            // avg elapse, bw
            if (0 == id_index) {
                auto dev_all_flag = true;
                for (const auto one : dev_flags) {
                    if (!one) {
                        dev_all_flag = false;
                        break;
                    }
                }
                if (dev_all_flag) {
                    printf(" %7.3f |  %3u |\n", avg_elapse[size_index], static_cast<uint32_t>(avg_bw[size_index]));
                } else {
                    printf("   N/A   |  N/A |\n");
                }
            }
            else {
                printf("         |      |\n");
            }
        }
        printf("-----------------------------------------------------------------------------------------------------------------------------------------\n");
    }
}

TEST_F(axclrtDeviceTest, Case01_AXCL_EngineAllGather) {
    std::vector<size_t> sizes;
    for (size_t new_size = 256; new_size <= (size / ids.size()); new_size *= 2) {
        sizes.push_back(new_size);
    }

    std::vector<std::vector<std::array<float, 10>>> elapses(sizes.size(), std::vector<std::array<float, 10>>(ids.size(), std::array<float, 10>{}));
    std::vector<std::vector<bool>> results(sizes.size(), std::vector<bool>(ids.size(), true));
    for (size_t size_index = 0; size_index < sizes.size(); ++size_index) {
        std::cout << "[INFO] testing size: " << sizes[size_index] << std::endl;

        bool ready = false;
        std::mutex mutex;
        std::condition_variable cond;
        sched sched{ready, mutex, cond, results[size_index], elapses[size_index]};

        std::vector<std::thread> threads;
        threads.reserve(ids.size());
        for (size_t i = 0; i < ids.size(); ++i) {
            threads.emplace_back(thread_func, sched, i, mode, sizes[size_index]);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        {
            std::lock_guard lock(mutex);
            ready = true;
        }
        cond.notify_all();

        for (auto& thread : threads) {
            thread.join();
        }

        for (const auto& one : results[size_index]) {
            EXPECT_TRUE(one);
            if (!one) {
                std::cout << "[ERROR] bw test failed." << std::endl;
                break;
            }
        }
    }
    print(sizes, elapses, results);
    std::cout << "[INFO] all bw test done." << std::endl;
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    auto axcl_guard = res_guard<bool>(
        [&] {
            bool flag = true;
            if (const axclError ret = axclInit(""); AXCL_SUCC != ret) {
                printf("[ERROR] axcl init fail, ret = 0x%x\n", ret);
                flag = false;
            }
            return flag;
        },
        [](bool flag) {
            if (flag) {
                axclFinalize();
            }
            flag = false;
        }
    );

    EXPECT_TRUE(axcl_guard.get());
    if (!axcl_guard.get()) {
        return 1;
    }

    axclrtDeviceList device_list;
    if (const axclError ret = axclrtGetDeviceList(&device_list); AXCL_SUCC != ret || 0 == device_list.num) {
        printf("[ERROR] no device is connected.\n");
    }
    printf("[INFO] device num: %d.\n", device_list.num);
    if (2 > device_list.num) {
        printf("[ERROR] device num is less than 2.\n");
        return 2;
    }

    cmdline::parser args;
    args.add<uint32_t>("count", 'c', "world size", false,
        2u, cmdline::range(2u, device_list.num));
    args.add<uint32_t>("mode", 'm', "test mode", false,
        static_cast<uint32_t>(P2P_MODE::P2P_INTERLACE_BLOCK),
        cmdline::range(static_cast<uint32_t>(P2P_MODE::P2P_PARALLEL_NONE_BLOCK), static_cast<uint32_t>(P2P_MODE::P2P_ALL_MODE)));
    args.parse_check(argc, argv);

    const auto count = args.get<uint32_t>("count");
    mode = static_cast<P2P_MODE>(args.get<uint32_t>("mode"));

    auto p2p_guard = res_guard<AXCL_P2P_UNIT_HANDLE>(
        [&] {
            axclrtP2PUnitInfo p2p_unit;
            p2p_unit.u32DeviceNum = count;
            for (uint32_t i = 0; i < count; ++i) {
                p2p_unit.n32DeviceId[i] = device_list.devices[i];
                p2p_unit.u32DeviceMemSize[i] = size;
            }
            AXCL_P2P_UNIT_HANDLE handle = nullptr;
            if (const auto ret= axclrtCreateP2PUnit(&p2p_unit, &handle); AXCL_SUCC != ret) {
                printf("[ERROR] axcl init p2p unit fail, ret = 0x%x\n", ret);
            } else {
                std::cout << "[INFO] p2p unit created." << std::endl;
            }
            return handle;
        },
        [](AXCL_P2P_UNIT_HANDLE handle) {
            if (nullptr != handle) {
                axclrtDestoryP2PUnit(handle);
                printf("[INFO] p2p unit destroyed.\n");
            }
        }
    );

    EXPECT_TRUE(p2p_guard.get());
    if (!p2p_guard.get()) {
        return 3;
    }

    ids.resize(count);
    for (uint32_t i = 0; i < count; ++i) {
        ids[i] = device_list.devices[i];
    }

    const int ret = RUN_ALL_TESTS();
    if (0 == ret) {
        std::cout << "============= UT PASS =============" << std::endl;
    } else {
        std::cout << "============= UT FAIL =============" << std::endl;
    }

    return ret;
}
