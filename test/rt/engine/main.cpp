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
#include <cstdlib>
#include <string>

#include "axcl.h"
#include "axcl_rt_engine.h"

#include "randomizer.hpp"

#include "net.h"
#include "net_io.h"

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    cmdline::parser parser;
    parser.add<uint32_t>("device", 'd', "device index", false, 0);
    parser.parse_check(argc, argv);
    uint32_t device_index = parser.get<uint32_t>("device");

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

    const int32_t device_id = device_list.devices[device_index];
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

class axclrtDeviceTest : public testing::Test {
protected:
    static void SetUpTestSuite() {
    }

    static void TearDownTestSuite() {
    }

    static axcl::randomizer m_random;
};

axcl::randomizer axclrtDeviceTest::m_random;

TEST_F(axclrtDeviceTest, Case01_AXCL_EngineInitFinal) {
    EXPECT_EQ(AXCL_SUCC, axclrtEngineInit(axclrtEngineVNpuKind{}));
    EXPECT_EQ(AXCL_SUCC, axclrtEngineFinalize());
}

TEST_F(axclrtDeviceTest, Case02_axclrtEngineGetVNpuKind) {
    for (int i = AXCL_VNPU_DISABLE; i <= AXCL_VNPU_LITTLE_BIG; i++) {
        ASSERT_EQ(AXCL_SUCC, axclrtEngineInit(static_cast<axclrtEngineVNpuKind>(i)));

        axclrtEngineVNpuKind kind_got{};
        EXPECT_EQ(AXCL_SUCC, axclrtEngineGetVNpuKind(&kind_got));
        EXPECT_EQ(static_cast<axclrtEngineVNpuKind>(i), kind_got);
        EXPECT_EQ(AXCL_SUCC, axclrtEngineFinalize());
    }
}

TEST_F(axclrtDeviceTest, Case03_axclrtEngineGetModelTypeFromMem) {
    for (int i = AXCL_VNPU_DISABLE; i <= AXCL_VNPU_LITTLE_BIG; i++) {
        // 1. init engine
        ASSERT_EQ(AXCL_SUCC, axclrtEngineInit(static_cast<axclrtEngineVNpuKind>(i)));

        // 2. get engine attribute and check
        axclrtEngineVNpuKind kind_got{};
        EXPECT_EQ(AXCL_SUCC, axclrtEngineGetVNpuKind(&kind_got));
        EXPECT_EQ(static_cast<axclrtEngineVNpuKind>(i), kind_got);

        // 3. get malloc model size
        void* devMem = nullptr;
        ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem, compiled_axmodel_size, AXCL_MEM_MALLOC_NORMAL_ONLY));

        // 4. copy model to device
        EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem, compiled_axmodel, compiled_axmodel_size, AXCL_MEMCPY_HOST_TO_DEVICE));

        // 5. get model type and check
        axclrtEngineModelKind model_kind;
        EXPECT_EQ(AXCL_SUCC, axclrtEngineGetModelTypeFromMem(devMem, compiled_axmodel_size, &model_kind));
        EXPECT_EQ(AXCL_MODEL_TYPE_1CORE, model_kind);

        // 6. free device memory
        EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem));

        // 7. deinit engine
        EXPECT_EQ(AXCL_SUCC, axclrtEngineFinalize());
    }
}

TEST_F(axclrtDeviceTest, Case04_axclrtEngineLoadFromMem) {
    for (int i = AXCL_VNPU_DISABLE; i <= AXCL_VNPU_LITTLE_BIG; i++) {
        // 1. init engine
        ASSERT_EQ(AXCL_SUCC, axclrtEngineInit(static_cast<axclrtEngineVNpuKind>(i)));

        // 2. get engine attribute and check
        axclrtEngineVNpuKind kind_got{};
        EXPECT_EQ(AXCL_SUCC, axclrtEngineGetVNpuKind(&kind_got));
        EXPECT_EQ(static_cast<axclrtEngineVNpuKind>(i), kind_got);

        // 3. get malloc model size
        void* devMem = nullptr;
        ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem, compiled_axmodel_size, AXCL_MEM_MALLOC_NORMAL_ONLY));

        // 4. copy model to device
        EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem, compiled_axmodel, compiled_axmodel_size, AXCL_MEMCPY_HOST_TO_DEVICE));

        // 5. create engine handle
        uint64_t id{0};
        ASSERT_EQ(AXCL_SUCC, axclrtEngineLoadFromMem(devMem, compiled_axmodel_size, &id));
        EXPECT_NE(0u, id);

        // 6. destroy engine handle
        EXPECT_EQ(AXCL_SUCC, axclrtEngineUnload(id));

        // 7. free device memory
        EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem));

        // 8. deinit engine
        EXPECT_EQ(AXCL_SUCC, axclrtEngineFinalize());
    }
}

TEST_F(axclrtDeviceTest, Case05_axclrtEngineCreateContext) {
    for (int i = AXCL_VNPU_DISABLE; i <= AXCL_VNPU_LITTLE_BIG; i++) {
        // 1. init engine
        ASSERT_EQ(AXCL_SUCC, axclrtEngineInit(static_cast<axclrtEngineVNpuKind>(i)));

        // 2. get engine attribute and check
        axclrtEngineVNpuKind kind_got{};
        EXPECT_EQ(AXCL_SUCC, axclrtEngineGetVNpuKind(&kind_got));
        EXPECT_EQ(static_cast<axclrtEngineVNpuKind>(i), kind_got);

        // 3. get malloc model size
        void* devMem = nullptr;
        ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem, compiled_axmodel_size, AXCL_MEM_MALLOC_NORMAL_ONLY));

        // 4. copy model to device
        EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem, compiled_axmodel, compiled_axmodel_size, AXCL_MEMCPY_HOST_TO_DEVICE));

        // 5. create engine handle
        uint64_t id{0};
        ASSERT_EQ(AXCL_SUCC, axclrtEngineLoadFromMem(devMem, compiled_axmodel_size, &id));
        EXPECT_NE(0u, id);

        // 6. create context
        uint64_t ctx_id{0};
        EXPECT_EQ(AXCL_SUCC, axclrtEngineCreateContext(id, &ctx_id));
        EXPECT_NE(0u, ctx_id);

        // 7. destroy engine handle
        EXPECT_EQ(AXCL_SUCC, axclrtEngineUnload(id));

        // 8. free device memory
        EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem));

        // 9. deinit engine
        EXPECT_EQ(AXCL_SUCC, axclrtEngineFinalize());
    }
}

TEST_F(axclrtDeviceTest, Case06_axclrtEngineGetModelTypeFromModelId) {
    for (int i = AXCL_VNPU_DISABLE; i <= AXCL_VNPU_LITTLE_BIG; i++) {
        // 1. init engine
        ASSERT_EQ(AXCL_SUCC, axclrtEngineInit(static_cast<axclrtEngineVNpuKind>(i)));

        // 2. get engine attribute and check
        axclrtEngineVNpuKind kind_got{};
        EXPECT_EQ(AXCL_SUCC, axclrtEngineGetVNpuKind(&kind_got));
        EXPECT_EQ(static_cast<axclrtEngineVNpuKind>(i), kind_got);

        // 3. get malloc model size
        void* devMem = nullptr;
        ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem, compiled_axmodel_size, AXCL_MEM_MALLOC_NORMAL_ONLY));

        // 4. copy model to device
        EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem, compiled_axmodel, compiled_axmodel_size, AXCL_MEMCPY_HOST_TO_DEVICE));

        // 5. create engine handle
        uint64_t id{0};
        ASSERT_EQ(AXCL_SUCC, axclrtEngineLoadFromMem(devMem, compiled_axmodel_size, &id));
        EXPECT_NE(0u, id);

        // 6. get & check model type
        axclrtEngineModelKind model_kind{};
        EXPECT_EQ(AXCL_SUCC, axclrtEngineGetModelTypeFromModelId(id, &model_kind));
        EXPECT_EQ(AXCL_MODEL_TYPE_1CORE, model_kind);

        // 7. destroy engine handle
        EXPECT_EQ(AXCL_SUCC, axclrtEngineUnload(id));

        // 8. free device memory
        EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem));

        // 9. deinit engine
        EXPECT_EQ(AXCL_SUCC, axclrtEngineFinalize());
    }
}

TEST_F(axclrtDeviceTest, Case07_axclrtEngineGetModelCompilerVersion) {
    for (int i = AXCL_VNPU_DISABLE; i <= AXCL_VNPU_LITTLE_BIG; i++) {
        const auto model_ver_ref = "2.4-beta3 bd2851d6";

        // 1. init engine
        ASSERT_EQ(AXCL_SUCC, axclrtEngineInit(static_cast<axclrtEngineVNpuKind>(i)));

        // 2. get engine attribute and check
        axclrtEngineVNpuKind kind_got{};
        EXPECT_EQ(AXCL_SUCC, axclrtEngineGetVNpuKind(&kind_got));
        EXPECT_EQ(static_cast<axclrtEngineVNpuKind>(i), kind_got);

        // 3. get malloc model size
        void* devMem = nullptr;
        ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem, compiled_axmodel_size, AXCL_MEM_MALLOC_NORMAL_ONLY));

        // 4. copy model to device
        EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem, compiled_axmodel, compiled_axmodel_size, AXCL_MEMCPY_HOST_TO_DEVICE));

        // 5. create engine handle
        uint64_t id{0};
        ASSERT_EQ(AXCL_SUCC, axclrtEngineLoadFromMem(devMem, compiled_axmodel_size, &id));
        EXPECT_NE(0u, id);

        // 6. get & check model version
        const auto model_ver_got = axclrtEngineGetModelCompilerVersion(id);
        EXPECT_NE(nullptr, model_ver_got);
        EXPECT_EQ(0, strcmp(model_ver_ref, model_ver_got));

        // 7. destroy engine handle
        EXPECT_EQ(AXCL_SUCC, axclrtEngineUnload(id));

        // 8. free device memory
        EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem));

        // 9. deinit engine
        EXPECT_EQ(AXCL_SUCC, axclrtEngineFinalize());
    }
}

TEST_F(axclrtDeviceTest, Case08_axclrtEngineGetUsageFromModelId) {
    for (int i = AXCL_VNPU_DISABLE; i <= AXCL_VNPU_LITTLE_BIG; i++) {
        // 1. init engine
        ASSERT_EQ(AXCL_SUCC, axclrtEngineInit(static_cast<axclrtEngineVNpuKind>(i)));

        // 2. get engine attribute and check
        axclrtEngineVNpuKind kind_got{};
        EXPECT_EQ(AXCL_SUCC, axclrtEngineGetVNpuKind(&kind_got));
        EXPECT_EQ(static_cast<axclrtEngineVNpuKind>(i), kind_got);

        // 3. get malloc model size
        void* devMem = nullptr;
        ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem, compiled_axmodel_size, AXCL_MEM_MALLOC_NORMAL_ONLY));

        // 4. copy model to device
        EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem, compiled_axmodel, compiled_axmodel_size, AXCL_MEMCPY_HOST_TO_DEVICE));

        // 5. create engine handle
        uint64_t id{0};
        ASSERT_EQ(AXCL_SUCC, axclrtEngineLoadFromMem(devMem, compiled_axmodel_size, &id));
        EXPECT_NE(0u, id);

        // 6. check model shape group count
        int64_t sys_usage{}, cmm_usage{};
        EXPECT_EQ(AXCL_SUCC, axclrtEngineGetUsageFromModelId(id, &sys_usage, &cmm_usage));
        EXPECT_EQ(0u, sys_usage);
        EXPECT_EQ(20618160u, cmm_usage);

        // 7. destroy engine handle
        EXPECT_EQ(AXCL_SUCC, axclrtEngineUnload(id));

        // 8. free device memory
        EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem));

        // 9. deinit engine
        EXPECT_EQ(AXCL_SUCC, axclrtEngineFinalize());
    }
}

TEST_F(axclrtDeviceTest, Case09_axclrtEngineSetGetAffinity) {
    // 1. init engine
    ASSERT_EQ(AXCL_SUCC, axclrtEngineInit(AXCL_VNPU_ENABLE));

    // 2. get malloc model size
    void* devMem = nullptr;
    ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem, compiled_axmodel_size, AXCL_MEM_MALLOC_NORMAL_ONLY));

    // 3. copy model to device
    EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem, compiled_axmodel, compiled_axmodel_size, AXCL_MEMCPY_HOST_TO_DEVICE));

    // 4. create engine handle
    uint64_t id{0};
    ASSERT_EQ(AXCL_SUCC, axclrtEngineLoadFromMem(devMem, compiled_axmodel_size, &id));
    EXPECT_NE(0u, id);

    // 5. get & check model affinity
    for (int i = 1; i < 0b111; i++) {
        axclrtEngineSet mask_set{static_cast<axclrtEngineSet>(i)}, mask_got{};
        EXPECT_EQ(AXCL_SUCC, axclrtEngineSetAffinity(id, mask_set));
        EXPECT_EQ(AXCL_SUCC, axclrtEngineGetAffinity(id, &mask_got));
        EXPECT_EQ(mask_set, mask_got);
    }

    // 6. destroy engine handle
    EXPECT_EQ(AXCL_SUCC, axclrtEngineUnload(id));

    // 7. free device memory
    EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem));

    // 8. deinit engine
    EXPECT_EQ(AXCL_SUCC, axclrtEngineFinalize());
}

TEST_F(axclrtDeviceTest, Case10_axclrtEngineGetIOInfo) {
    // 1. init engine
    ASSERT_EQ(AXCL_SUCC, axclrtEngineInit(AXCL_VNPU_ENABLE));

    // 2. get malloc model size
    void* devMem = nullptr;
    ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem, compiled_axmodel_size, AXCL_MEM_MALLOC_NORMAL_ONLY));

    // 3. copy model to device
    EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem, compiled_axmodel, compiled_axmodel_size, AXCL_MEMCPY_HOST_TO_DEVICE));

    // 4. create engine handle
    uint64_t id{0};
    ASSERT_EQ(AXCL_SUCC, axclrtEngineLoadFromMem(devMem, compiled_axmodel_size, &id));
    EXPECT_NE(0u, id);

    // 5. get engine io info
    axclrtEngineIOInfo io_info{};
    EXPECT_EQ(AXCL_SUCC, axclrtEngineGetIOInfo(id, &io_info));
    EXPECT_NE(nullptr, io_info);

    // 6. check io info
    if (nullptr != io_info) {
        int32_t count{};
        EXPECT_EQ(0, axclrtEngineGetShapeGroupsCount(io_info, &count));

        EXPECT_EQ(1u, axclrtEngineGetNumInputs(io_info));
        if (1u == axclrtEngineGetNumInputs(io_info)) {
            const auto input_name = axclrtEngineGetInputNameByIndex(io_info, 0);
            EXPECT_NE(nullptr, input_name);
            if (nullptr != input_name) {
                const auto model_input_name = "data";
                EXPECT_EQ(0, strcmp(model_input_name, input_name));
            }
            EXPECT_EQ(static_cast<uint64_t>(1 * 224 * 224 * 3), axclrtEngineGetInputSizeByIndex(io_info, 0, 0));

            axclrtEngineIODims dims{};
            EXPECT_EQ(0, axclrtEngineGetInputDims(io_info, 0, 0, &dims));
            if (0 == axclrtEngineGetInputDims(io_info, 0, 0, &dims)) {
                EXPECT_EQ(4, dims.dimCount);
                if (4 == dims.dimCount) {
                    EXPECT_EQ(1, dims.dims[0]);
                    EXPECT_EQ(224, dims.dims[1]);
                    EXPECT_EQ(224, dims.dims[2]);
                    EXPECT_EQ(3, dims.dims[3]);
                }
            }
        }

        EXPECT_EQ(1u, axclrtEngineGetNumOutputs(io_info));
        if (1u != axclrtEngineGetNumOutputs(io_info)) {
            const auto output_name = axclrtEngineGetOutputNameByIndex(io_info, 0);
            EXPECT_NE(nullptr, output_name);
            if (nullptr != output_name) {
                const auto model_output_name = "prob";
                EXPECT_EQ(0, strcmp(model_output_name, output_name));
            }
            EXPECT_EQ(sizeof(float) * 1000, axclrtEngineGetOutputSizeByIndex(io_info, 0, 0));

            axclrtEngineIODims dims{};
            EXPECT_EQ(0, axclrtEngineGetOutputDims(io_info, 0, 0, &dims));
            if (0 == axclrtEngineGetOutputDims(io_info, 0, 0, &dims)) {
                EXPECT_EQ(2, dims.dimCount);
                if (2 == dims.dimCount) {
                    EXPECT_EQ(1, dims.dims[0]);
                    EXPECT_EQ(1000, dims.dims[1]);
                }
            }
        }
    }

    // 7. destroy io info
    EXPECT_EQ(AXCL_SUCC, axclrtEngineDestroyIOInfo(io_info));

    // 8. destroy engine handle
    EXPECT_EQ(AXCL_SUCC, axclrtEngineUnload(id));

    // 9. free device memory
    EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem));

    // 10. deinit engine
    EXPECT_EQ(AXCL_SUCC, axclrtEngineFinalize());
}

TEST_F(axclrtDeviceTest, Case11_axclrtEngineExecute) {
    // 1. init engine
    ASSERT_EQ(AXCL_SUCC, axclrtEngineInit(AXCL_VNPU_ENABLE));

    // 2. get malloc model size
    void* devMem = nullptr;
    ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem, compiled_axmodel_size, AXCL_MEM_MALLOC_NORMAL_ONLY));

    // 3. copy model to device
    EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem, compiled_axmodel, compiled_axmodel_size, AXCL_MEMCPY_HOST_TO_DEVICE));

    // 4. create engine handle
    uint64_t id{0};
    ASSERT_EQ(AXCL_SUCC, axclrtEngineLoadFromMem(devMem, compiled_axmodel_size, &id));
    ASSERT_NE(0u, id);

    // 5. create engine context
    uint64_t ctx_id{0};
    ASSERT_EQ(AXCL_SUCC, axclrtEngineCreateContext(id, &ctx_id));
    ASSERT_NE(0u, ctx_id);
    ;

    // 6. get engine io info
    axclrtEngineIOInfo io_info{};
    ASSERT_EQ(AXCL_SUCC, axclrtEngineGetIOInfo(id, &io_info));
    ASSERT_NE(nullptr, io_info);

    // 7. create engine io
    axclrtEngineIO io{};
    ASSERT_EQ(AXCL_SUCC, axclrtEngineCreateIO(io_info, &io));
    ASSERT_NE(nullptr, io);

    // 8. malloc input & output buffer
    void* input = nullptr;
    void* output = nullptr;
    ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&input, data_0_bin_size, AXCL_MEM_MALLOC_NORMAL_ONLY));
    ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&output, prob_0_bin_size, AXCL_MEM_MALLOC_NORMAL_ONLY));
    ASSERT_NE(nullptr, input);
    ASSERT_NE(nullptr, output);

    // 9. copy input data to device
    EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(input, data_0_bin, data_0_bin_size, AXCL_MEMCPY_HOST_TO_DEVICE));

    // 10. set model io
    EXPECT_EQ(AXCL_SUCC, axclrtEngineSetInputBufferByIndex(io, 0, input, data_0_bin_size));
    EXPECT_EQ(AXCL_SUCC, axclrtEngineSetOutputBufferByIndex(io, 0, output, prob_0_bin_size));

    // 11. run model
    EXPECT_EQ(AXCL_SUCC, axclrtEngineExecute(id, ctx_id, 0, io));

    // 12. copy output data to host
    std::vector<uint8_t> hw_prob_0_bin(prob_0_bin_size);
    EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(hw_prob_0_bin.data(), output, prob_0_bin_size, AXCL_MEMCPY_DEVICE_TO_HOST));

    // 13. compare output data
    EXPECT_EQ(0, memcmp(prob_0_bin, hw_prob_0_bin.data(), prob_0_bin_size));

    // 14. free input & output buffer
    EXPECT_EQ(AXCL_SUCC, axclrtFree(input));
    EXPECT_EQ(AXCL_SUCC, axclrtFree(output));

    // 15. destroy engine io
    EXPECT_EQ(AXCL_SUCC, axclrtEngineDestroyIO(io));

    // 16. destroy io info
    EXPECT_EQ(AXCL_SUCC, axclrtEngineDestroyIOInfo(io_info));

    // 17. destroy engine handle
    EXPECT_EQ(AXCL_SUCC, axclrtEngineUnload(id));

    // 18. free device memory
    EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem));

    // 19. deinit engine
    EXPECT_EQ(AXCL_SUCC, axclrtEngineFinalize());
}
