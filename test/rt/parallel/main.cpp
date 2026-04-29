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
#include "axcl_rt_p2p.h"

#include "res_guard.hpp"

#include <cmdline.h>

#include <cstdlib>
#include <iomanip>
#include <string>
#include <thread>
#include <filesystem>
#include <fstream>

struct sched {
    bool& ready;
    std::mutex& mutex;
    std::condition_variable& cond;

    std::vector<bool>& flags;
};

constexpr size_t p2p_cmm_size = 8 * 1024 * 1024;

std::vector<uint32_t> ids;
std::string name_format;
std::string verify_folder;
int group_index = 0;

bool thread_work(const sched& sched, const uint64_t modelId, uint64_t contextId, axclrtEngineIO io) {
    {
        std::unique_lock lock(sched.mutex);
        sched.cond.wait(lock, [&] { return sched.ready; });
    }

    std::cout << "[INFO] run model id: " << modelId;
    const auto ret = axclrtEngineExecute(modelId, contextId, group_index, io);
    std::cout << "... done." << std::endl;
    std::fflush(stdout);

    // wait for log
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    return 0 == ret;
}

void thread_func(const sched& sched, const size_t index, const std::string &name) {
    const auto id = ids[index];
    auto ctx_guard = res_guard<axclrtContext>(
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
    if (nullptr == ctx_guard.get()) {
        sched.flags[index] = false;
        return;
    }

    auto engine_guard = res_guard<bool>(
        [&] {
            bool flag = true;
            if (const axclError ret = axclrtEngineInit(AXCL_VNPU_DISABLE); AXCL_SUCC != ret) {
                printf("[ERROR] engine init failed, ret = 0x%x\n", ret);
                flag = false;
            }
            return flag;
        },
        [](bool flag) {
            if (flag) {
                axclrtEngineFinalize();
            }
            flag = false;
        }
    );
    ASSERT_TRUE(engine_guard.get());

    std::cout << "[INFO] model: " << name << " loading..." << std::endl;
    auto model_guard = res_guard<uint64_t>(
        [&] {
            uint64_t model = 0;
            if (const axclError ret = axclrtEngineLoadFromFile(name.c_str(), &model); AXCL_SUCC != ret) {
                std::cout << "[ERROR] thread id: " << std::this_thread::get_id() << " load model: " << name << " failed: " << ret << std::endl;
            }
            return model;
        },
        [](uint64_t model) {
            if (0 != model) {
                axclrtEngineUnload(model);
                model = 0;
            }
        }
    );
    if (0 == model_guard.get()) {
        sched.flags[index] = false;
        return;
    }
    std::cout << "[INFO] model id: " << model_guard.get() << " was created on device: " << id << std::endl;

    uint64_t context = 0;
    if (const axclError ret = axclrtEngineCreateContext(model_guard.get(), &context); AXCL_SUCC != ret) {
        std::cout << "[ERROR] thread id: " << std::this_thread::get_id() << " create context failed: " << ret << std::endl;
        sched.flags[index] = false;
        return;
    }

    auto info_guard = res_guard<axclrtEngineIOInfo>(
        [&] {
            axclrtEngineIOInfo io_info;
            if (const axclError ret = axclrtEngineGetIOInfo(model_guard.get(), &io_info); AXCL_SUCC != ret) {
                std::cout << "[ERROR] thread id: " << std::this_thread::get_id() << " create io info failed: " << ret << std::endl;
            }
            return io_info;
        },
        [](axclrtEngineIOInfo io_info) {
            if (nullptr != io_info) {
                axclrtEngineDestroyIOInfo(io_info);
                io_info = nullptr;
            }
        }
    );
    if (nullptr == info_guard.get()) {
        sched.flags[index] = false;
        return;
    }

    auto io_guard = res_guard<axclrtEngineIO>(
        [&] {
            axclrtEngineIO io;
            if (const axclError ret = axclrtEngineCreateIO(info_guard.get(), &io); AXCL_SUCC != ret) {
                std::cout << "[ERROR] thread id: " << std::this_thread::get_id() << " create io failed: " << ret << std::endl;
            }
            return io;
        },
        [](axclrtEngineIO io) {
            if (nullptr != io) {
                axclrtEngineDestroyIO(io);
                io = nullptr;
            }
        }
    );
    if (nullptr == io_guard.get()) {
        sched.flags[index] = false;
        return;
    }

    int32_t model_shape_count = 0;
    if (const auto ret = axclrtEngineGetShapeGroupsCount(info_guard.get(), &model_shape_count); AXCL_SUCC != ret) {
        std::cout << "[ERROR] thread id: " << std::this_thread::get_id() << " get shape group count failed: " << ret << std::endl;
        sched.flags[index] = false;
        return;
    }

    const auto shape_index = std::min(group_index, model_shape_count);

    const auto input_count = axclrtEngineGetNumInputs(info_guard.get());
    const auto output_count = axclrtEngineGetNumOutputs(info_guard.get());

    auto inputs_guard = res_vector_guard<void*>(
        [&] {
            std::vector<void*> io_buffers(input_count, nullptr);
            for (uint32_t i = 0; i < input_count; ++i) {
                void* buffer = nullptr;
                const auto size = axclrtEngineGetInputSizeByIndex(info_guard.get(), shape_index, i);
                if (0 == size) {
                    std::cout << "[ERROR] thread id: " << std::this_thread::get_id() << " get input size failed." << std::endl;
                }
                if (const axclError ret = axclrtMalloc(&buffer, size, {}); AXCL_SUCC != ret) {
                    std::cout << "[ERROR] thread id: " << std::this_thread::get_id() << " malloc input buffer: " << size <<  "failed: " << ret << std::endl;
                }
                io_buffers[i] = buffer;
            }
            return io_buffers;
        },
        [](void* buffer) {
            if (nullptr != buffer) {
                axclrtFree(buffer);
                buffer = nullptr;
            }
        }
    );
    if (inputs_guard.get().empty()) {
        sched.flags[index] = false;
        return;
    }
    for (uint32_t i = 0; i < input_count; ++i) {
        if (nullptr == inputs_guard.get()[i]) {
            sched.flags[index] = false;
            return;
        }
        const auto this_io_size = axclrtEngineGetInputSizeByIndex(info_guard.get(), shape_index, i);
        if (const auto ret = axclrtEngineSetInputBufferByIndex(io_guard.get(), i, inputs_guard.get()[i], this_io_size); AXCL_SUCC != ret) {
            std::cout << "[ERROR] thread id: " << std::this_thread::get_id() << " set input buffer failed: " << ret << std::endl;
            sched.flags[index] = false;
            return;
        }
    }

    auto outputs_guard = res_vector_guard<void*>(
        [&] {
            std::vector<void*> io_buffers(output_count, nullptr);
            for (uint32_t i = 0; i < output_count; ++i) {
                void* buffer = nullptr;
                const auto size = axclrtEngineGetOutputSizeByIndex(info_guard.get(), shape_index, i);
                if (0 == size) {
                    std::cout << "[ERROR] thread id: " << std::this_thread::get_id() << " get output size failed." << std::endl;
                }
                if (const axclError ret = axclrtMalloc(&buffer, size, {}); AXCL_SUCC != ret) {
                    std::cout << "[ERROR] thread id: " << std::this_thread::get_id() << " malloc output buffer: " << size <<  "failed: " << ret << std::endl;
                }
                io_buffers[i] = buffer;
            }
            return io_buffers;
        },
        [](void* buffer) {
            if (nullptr != buffer) {
                axclrtFree(buffer);
                buffer = nullptr;
            }
        }
    );
    if (outputs_guard.get().empty()) {
        sched.flags[index] = false;
        return;
    }
    for (uint32_t i = 0; i < output_count; ++i) {
        if (nullptr == outputs_guard.get()[i]) {
            sched.flags[index] = false;
            return;
        }
        const auto this_io_size = axclrtEngineGetOutputSizeByIndex(info_guard.get(), shape_index, i);
        if (const auto ret = axclrtEngineSetOutputBufferByIndex(io_guard.get(), i, outputs_guard.get()[i], this_io_size); AXCL_SUCC != ret) {
            std::cout << "[ERROR] thread id: " << std::this_thread::get_id() << " set output buffer failed: " << ret << std::endl;
            sched.flags[index] = false;
            return;
        }
    }

    bool input_feed = !verify_folder.empty();
    if (input_feed) {
        for (uint32_t i = 0; i < input_count; ++i) {
            const std::string tensor_name{axclrtEngineGetInputNameByIndex(info_guard.get(), i)};
            if (tensor_name.empty()) {
                std::cout << "[WARNING] thread id: " << std::this_thread::get_id() << " get input name for index: " << i << " failed." << std::endl;
                std::cout << "          verify canceled." << std::endl;
                input_feed = false;
                break;
            }
            const auto tensor_bin_file = std::filesystem::path(verify_folder) / "input" / (tensor_name + ".bin");
            if (!std::filesystem::exists(tensor_bin_file)) {
                std::cout << "[WARNING] thread id: " << std::this_thread::get_id() << " verify file: " << tensor_bin_file << " not exist." << std::endl;
                std::cout << "          verify canceled." << std::endl;
                input_feed = false;
                break;
            }
            const auto tensor_bin_file_size = std::filesystem::file_size(tensor_bin_file);

            void* tensor_data_ptr = nullptr;
            uint64_t tensor_data_size = 0;
            if (const auto ret = axclrtEngineGetInputBufferByIndex(io_guard.get(), i, &tensor_data_ptr, &tensor_data_size); AXCL_SUCC != ret) {
                std::cout << "[WARNING] thread id: " << std::this_thread::get_id() << " get input buffer for index: " << i << " failed: " << ret << std::endl;
                std::cout << "          verify canceled." << std::endl;
                input_feed = false;
                break;
            }

            if (tensor_bin_file_size != tensor_data_size) {
                std::cout << "[WARNING] thread id: " << std::this_thread::get_id() << " verify file: " << tensor_bin_file << " size not match." << std::endl;
                std::cout << "          file size : " << tensor_bin_file_size << ", buffer size: " << tensor_data_size << std::endl;
                std::cout << "          verify canceled." << std::endl;
                input_feed = false;
                break;
            }
            std::vector<char> tensor_binary_buffer(tensor_bin_file_size, 0);
            auto fs = std::ifstream(tensor_bin_file, std::ios::binary);
            if (!fs.is_open()) {
                std::cout << "[WARNING] thread id: " << std::this_thread::get_id() << " open file: " << tensor_bin_file << " failed." << std::endl;
                std::cout << "          verify canceled." << std::endl;
                input_feed = false;
                break;
            }
            fs.read(tensor_binary_buffer.data(), static_cast<std::streamsize>(tensor_bin_file_size));
            fs.close();

            if (const auto ret = axclrtMemcpy(tensor_data_ptr, tensor_binary_buffer.data(), tensor_data_size, AXCL_MEMCPY_HOST_TO_DEVICE); AXCL_SUCC != ret) {
                std::cout << "[WARNING] thread id: " << std::this_thread::get_id() << " memcpy input buffer for index: " << i << " failed: " << ret << std::endl;
                std::cout << "          verify canceled." << std::endl;
                input_feed = false;
                break;
            }
        }
    } else {
        input_feed = false;
    }

    std::cout << "[INFO] model id: " << model_guard.get() << " ready to running on device: " << id << std::endl;
    sched.flags[index] = thread_work(sched, model_guard.get(), context, io_guard.get());
    ASSERT_TRUE(sched.flags[index]);


    if (input_feed) {
        for (uint32_t i = 0; i < output_count; ++i) {
            void* tensor_data_ptr = nullptr;
            uint64_t tensor_data_size = 0;
            if (const auto ret = axclrtEngineGetOutputBufferByIndex(io_guard.get(), i, &tensor_data_ptr, &tensor_data_size); AXCL_SUCC != ret) {
                std::cout << "[WARNING] thread id: " << std::this_thread::get_id() << " get output buffer for index: " << i << " failed: " << ret << std::endl;
                std::cout << "          verify skipped." << std::endl;
                continue;
            }
            std::vector<char> tensor_data(tensor_data_size, 0);
            if (const auto ret = axclrtMemcpy(tensor_data.data(), tensor_data_ptr, tensor_data_size, AXCL_MEMCPY_DEVICE_TO_HOST); AXCL_SUCC != ret) {
                std::cout << "[WARNING] thread id: " << std::this_thread::get_id() << " memcpy output buffer for index: " << i << " failed: " << ret << std::endl;
                std::cout << "          verify skipped." << std::endl;
                continue;
            }
            const std::string tensor_name{axclrtEngineGetOutputNameByIndex(info_guard.get(), i)};
            if (tensor_name.empty()) {
                std::cout << "[ERROR] thread id: " << std::this_thread::get_id() << " get output name for index: " << i << " failed." << std::endl;
                std::cout << "          verify skipped." << std::endl;
                continue;
            }

            // dump output
            {
                const auto hw_tensor_bin_file = std::filesystem::path(verify_folder) / "output_hw" / (tensor_name + ".bin");
                auto fs = std::ofstream(hw_tensor_bin_file, std::ios::binary);
                if (!fs.is_open()) {
                    std::cout << "[WARNING] thread id: " << std::this_thread::get_id() << " open file: " << hw_tensor_bin_file << " failed." << std::endl;
                    std::cout << "           file dump skipped." << std::endl;
                }
                fs.write(tensor_data.data(), static_cast<std::streamsize>(tensor_data_size));
                fs.close();
            }

            // read verify file
            const auto tensor_bin_file = std::filesystem::path(verify_folder) / "output" / (tensor_name + ".bin");
            if (!std::filesystem::exists(tensor_bin_file)) {
                std::cout << "[WARNING] thread id: " << std::this_thread::get_id() << " verify file: " << tensor_bin_file << " not exist." << std::endl;
                std::cout << "          verify skipped." << std::endl;
                continue;
            }
            const auto tensor_bin_file_size = std::filesystem::file_size(tensor_bin_file);
            if (tensor_bin_file_size != tensor_data_size) {
                std::cout << "[WARNING] thread id: " << std::this_thread::get_id() << " verify file: " << tensor_bin_file << " size not match." << std::endl;
                std::cout << "          file size : " << tensor_bin_file_size << ", buffer size: " << tensor_data_size << std::endl;
                std::cout << "          verify skipped." << std::endl;
                continue;
            }
            std::vector<char> tensor_binary_buffer(tensor_bin_file_size, 0);
            auto fs = std::ifstream(tensor_bin_file, std::ios::binary);
            if (!fs) {
                std::cout << "[WARNING] thread id: " << std::this_thread::get_id() << " open file: " << tensor_bin_file << " failed." << std::endl;
                std::cout << "          verify skipped." << std::endl;
                continue;
            }
            fs.read(tensor_binary_buffer.data(), static_cast<std::streamsize>(tensor_bin_file_size));
            fs.close();

            sched.flags[index] = (0 == memcmp(tensor_data.data(), tensor_binary_buffer.data(), tensor_data_size));
            if (!sched.flags[index]) {
                std::cout << "[ERROR] thread id: " << std::this_thread::get_id() << " verify file: " << tensor_bin_file << " data not match." << std::endl;
            } else {
                std::cout << "[INFO] thread id: " << std::this_thread::get_id() << " verify file: " << tensor_bin_file << " data match." << std::endl;
            }
        }
    }
}

std::vector<std::string> generate_filenames(const std::string& filename_pattern, const int count) {
    std::vector<std::string> filenames(count);

    std::stringstream ss;
    ss << filename_pattern;
    const std::size_t pos = ss.str().find("%d");

    for (int i = 0; i < count; ++i) {
        std::string filename;
        if (pos != std::string::npos) {
            filename = ss.str().substr(0, pos);
            filename += std::to_string(i);
            filename += ss.str().substr(pos + 2); //  + 2 to skip "%d"
        } else {
            filename = ss.str();
        }

        filenames[i] = filename;
    }
    return filenames;
}

class axclrtDeviceTest : public testing::Test {
protected:
    static void SetUpTestSuite() {
    }

    static void TearDownTestSuite() {
    }
};

TEST_F(axclrtDeviceTest, Case01_AXCL_EngineTensorParallel) {
    std::vector results(ids.size(), true);
    std::vector<std::string> filenames = generate_filenames(name_format, static_cast<int>(ids.size()));

    for (const auto& filename : filenames) {
        std::cout << "[INFO] test file: " << filename << std::endl;
    }

    bool ready = false;
    std::mutex mutex;
    std::condition_variable cond;
    sched sched{ready, mutex, cond, results};

    std::vector<std::thread> threads;
    threads.reserve(ids.size());
    for (size_t i = 0; i < ids.size(); ++i) {
        threads.emplace_back(thread_func, sched, i, filenames[i]);
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

    for (size_t i = 0; i < results.size(); ++i) {
        EXPECT_EQ(results[i], true) << "device: { index: " << i << ", id: " << ids[i] << " } failed.";
    }
    for (const auto& one : results) {
        if (!one) {
            std::cout << "[ERROR] test failed." << std::endl;
            return;
        }
    }
    std::cout << "[INFO] test done." << std::endl;
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
                printf("[INFO] axcl finalize.\n");
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
    args.add<uint32_t>("world-size", 'w', "world size", true, 2u, cmdline::range(2u, device_list.num));
    args.add<std::string>("model-format-path", 'm', "split model path with %d placeholder", true);
    args.add<std::string>("verify-folder", 'v', "verify folder", false);
    args.add<int>("group-index", 'g', "shape group index, default is 0", false, 0, cmdline::range(0, 32));
    args.parse_check(argc, argv);

    const auto world_size = args.get<uint32_t>("world-size");
    name_format = args.get<std::string>("model-format-path");
    verify_folder = args.get<std::string>("verify-folder");
    group_index = args.get<int>("group-index");

    // try mkdir for model real output
    if (!verify_folder.empty()) {
        std::filesystem::create_directories(std::filesystem::path(verify_folder) / "output_hw");
    }

    auto p2p_guard = res_guard<AXCL_P2P_UNIT_HANDLE>(
        [&] {
            axclrtP2PUnitInfo p2p_unit;
            p2p_unit.u32DeviceNum = world_size;
            for (uint32_t i = 0; i < world_size; ++i) {
                p2p_unit.n32DeviceId[i] = device_list.devices[i];
                p2p_unit.u32DeviceMemSize[i] = p2p_cmm_size;
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

    ids.resize(world_size);
    for (uint32_t i = 0; i < world_size; ++i) {
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
