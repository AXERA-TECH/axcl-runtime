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
#include <cstdlib>
#include <string>

#include "axcl.h"
#include "axcl_rt_memory.h"
#include "axcl_npu.h"

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

TEST_F(axclrtDeviceTest, Case01_AXCL_ENGINE_NPUReset) {
    AXCL_ENGINE_NPUReset();
}

TEST_F(axclrtDeviceTest, Case02_AXCL_ENGINE_GetVersion) {
    EXPECT_NE(nullptr, AXCL_ENGINE_GetVersion());
}

TEST_F(axclrtDeviceTest, Case03_AXCL_EngineInitDeinit) {
    AX_ENGINE_NPU_ATTR_T attr{};
    EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_Init(&attr));
    EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_Deinit());
}

TEST_F(axclrtDeviceTest, Case04_AXCL_ENGINE_GetVNPUAttr) {
    for (int i = 0; i < AX_ENGINE_VIRTUAL_NPU_BUTT; i++) {
        AX_ENGINE_NPU_ATTR_T attr_set{static_cast<AX_ENGINE_NPU_MODE_T>(i)};
        ASSERT_EQ(AXCL_SUCC, AXCL_ENGINE_Init(&attr_set));

        AX_ENGINE_NPU_ATTR_T attr_got{};
        EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_GetVNPUAttr(&attr_got));
        EXPECT_EQ(attr_set.eHardMode, attr_got.eHardMode);
        EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_Deinit());
    }
}

TEST_F(axclrtDeviceTest, Case05_AXCL_ENGINE_GetModelType) {
    for (int i = 0; i < AX_ENGINE_VIRTUAL_NPU_BUTT; i++) {
        // 1. init engine
        AX_ENGINE_NPU_ATTR_T attr_set{static_cast<AX_ENGINE_NPU_MODE_T>(i)};
        ASSERT_EQ(AXCL_SUCC, AXCL_ENGINE_Init(&attr_set));

        // 2. get engine attribute and check
        AX_ENGINE_NPU_ATTR_T attr_got{};
        EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_GetVNPUAttr(&attr_got));
        EXPECT_EQ(attr_set.eHardMode, attr_got.eHardMode);

        // 3. get malloc model size
        void* devMem = nullptr;
        ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem, compiled_axmodel_size, AXCL_MEM_MALLOC_NORMAL_ONLY));

        // 4. copy model to device
        EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem, compiled_axmodel, compiled_axmodel_size, AXCL_MEMCPY_HOST_TO_DEVICE));

        // 5. get model type and check
        AX_ENGINE_MODEL_TYPE_T model_type;
        EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_GetModelType(devMem, compiled_axmodel_size, &model_type));
        EXPECT_EQ(AX_ENGINE_MODEL_TYPE0, model_type);

        // 6. free device memory
        EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem));

        // 7. deinit engine
        EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_Deinit());
    }
}

TEST_F(axclrtDeviceTest, Case06_AXCL_ENGINE_CreateHandle) {
    for (int i = 0; i < AX_ENGINE_VIRTUAL_NPU_BUTT; i++) {
        // 1. init engine
        AX_ENGINE_NPU_ATTR_T attr_set{static_cast<AX_ENGINE_NPU_MODE_T>(i)};
        ASSERT_EQ(AXCL_SUCC, AXCL_ENGINE_Init(&attr_set));

        // 2. get engine attribute and check
        AX_ENGINE_NPU_ATTR_T attr_got{};
        EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_GetVNPUAttr(&attr_got));
        EXPECT_EQ(attr_set.eHardMode, attr_got.eHardMode);

        // 3. get malloc model size
        void* devMem = nullptr;
        ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem, compiled_axmodel_size, AXCL_MEM_MALLOC_NORMAL_ONLY));

        // 4. copy model to device
        EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem, compiled_axmodel, compiled_axmodel_size, AXCL_MEMCPY_HOST_TO_DEVICE));

        // 5. create engine handle
        AX_ENGINE_HANDLE handle{};
        EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_CreateHandle(&handle, devMem, compiled_axmodel_size));
        EXPECT_NE(nullptr, handle);

        // 6. destroy engine handle
        EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_DestroyHandle(handle));

        // 7. free device memory
        EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem));

        // 8. deinit engine
        EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_Deinit());
    }
}

TEST_F(axclrtDeviceTest, Case07_AXCL_ENGINE_CreateHandleV2) {
    for (int i = 0; i < AX_ENGINE_VIRTUAL_NPU_BUTT; i++) {
        const auto model_name = "compiled_axmodel";

        // 1. init engine
        AX_ENGINE_NPU_ATTR_T attr_set{static_cast<AX_ENGINE_NPU_MODE_T>(i)};
        ASSERT_EQ(AXCL_SUCC, AXCL_ENGINE_Init(&attr_set));

        // 2. get engine attribute and check
        AX_ENGINE_NPU_ATTR_T attr_got{};
        EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_GetVNPUAttr(&attr_got));
        EXPECT_EQ(attr_set.eHardMode, attr_got.eHardMode);

        // 3. get malloc model size
        void* devMem = nullptr;
        ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem, compiled_axmodel_size, AXCL_MEM_MALLOC_NORMAL_ONLY));

        // 4. copy model to device
        EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem, compiled_axmodel, compiled_axmodel_size, AXCL_MEMCPY_HOST_TO_DEVICE));

        // 5. create engine handle
        AX_ENGINE_HANDLE handle{};
        AX_ENGINE_HANDLE_EXTRA_T extra{};
        extra.nNpuSet = 0b111;
        extra.pName = reinterpret_cast<AX_S8 *>(const_cast<char *>(model_name));
        EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_CreateHandleV2(&handle, devMem, compiled_axmodel_size, &extra));
        EXPECT_NE(nullptr, handle);

        // 6. destroy engine handle
        EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_DestroyHandle(handle));

        // 7. free device memory
        EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem));

        // 8. deinit engine
        EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_Deinit());
    }
}

TEST_F(axclrtDeviceTest, Case08_AXCL_ENGINE_CreateContext) {
    for (int i = 0; i < AX_ENGINE_VIRTUAL_NPU_BUTT; i++) {
        // 1. init engine
        AX_ENGINE_NPU_ATTR_T attr_set{static_cast<AX_ENGINE_NPU_MODE_T>(i)};
        ASSERT_EQ(AXCL_SUCC, AXCL_ENGINE_Init(&attr_set));

        // 2. get engine attribute and check
        AX_ENGINE_NPU_ATTR_T attr_got{};
        EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_GetVNPUAttr(&attr_got));
        EXPECT_EQ(attr_set.eHardMode, attr_got.eHardMode);

        // 3. get malloc model size
        void* devMem = nullptr;
        ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem, compiled_axmodel_size, AXCL_MEM_MALLOC_NORMAL_ONLY));

        // 4. copy model to device
        EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem, compiled_axmodel, compiled_axmodel_size, AXCL_MEMCPY_HOST_TO_DEVICE));

        // 5. create engine handle
        AX_ENGINE_HANDLE handle{};
        EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_CreateHandle(&handle, devMem, compiled_axmodel_size));
        EXPECT_NE(nullptr, handle);

        // 6. create context
        EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_CreateContext(handle));

        // 7. destroy engine handle
        EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_DestroyHandle(handle));

        // 8. free device memory
        EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem));

        // 9. deinit engine
        EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_Deinit());
    }
}

TEST_F(axclrtDeviceTest, Case09_AXCL_ENGINE_CreateContextV2) {
    for (int i = 0; i < AX_ENGINE_VIRTUAL_NPU_BUTT; i++) {
        const auto model_name = "compiled_axmodel";

        // 1. init engine
        AX_ENGINE_NPU_ATTR_T attr_set{static_cast<AX_ENGINE_NPU_MODE_T>(i)};
        ASSERT_EQ(AXCL_SUCC, AXCL_ENGINE_Init(&attr_set));

        // 2. get engine attribute and check
        AX_ENGINE_NPU_ATTR_T attr_got{};
        EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_GetVNPUAttr(&attr_got));
        EXPECT_EQ(attr_set.eHardMode, attr_got.eHardMode);

        // 3. get malloc model size
        void* devMem = nullptr;
        ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem, compiled_axmodel_size, AXCL_MEM_MALLOC_NORMAL_ONLY));

        // 4. copy model to device
        EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem, compiled_axmodel, compiled_axmodel_size, AXCL_MEMCPY_HOST_TO_DEVICE));

        // 5. create engine handle
        AX_ENGINE_HANDLE handle{};
        AX_ENGINE_HANDLE_EXTRA_T extra{};
        extra.nNpuSet = 0b111;
        extra.pName = reinterpret_cast<AX_S8 *>(const_cast<char *>(model_name));
        EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_CreateHandleV2(&handle, devMem, compiled_axmodel_size, &extra));
        EXPECT_NE(nullptr, handle);

        // 6. create context
        AX_ENGINE_CONTEXT_T context{};
        EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_CreateContextV2(handle, &context));
        EXPECT_NE(nullptr, context);

        // 7. destroy engine handle
        EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_DestroyHandle(handle));

        // 8. free device memory
        EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem));

        // 9. deinit engine
        EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_Deinit());
    }
}

TEST_F(axclrtDeviceTest, Case10_AXCL_ENGINE_GetHandleModelType) {
    // 1. init engine
    AX_ENGINE_NPU_ATTR_T attr_set{};
    ASSERT_EQ(AXCL_SUCC, AXCL_ENGINE_Init(&attr_set));

    // 2. get malloc model size
    void* devMem = nullptr;
    ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem, compiled_axmodel_size, AXCL_MEM_MALLOC_NORMAL_ONLY));

    // 3. copy model to device
    EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem, compiled_axmodel, compiled_axmodel_size, AXCL_MEMCPY_HOST_TO_DEVICE));

    // 4. create engine handle
    AX_ENGINE_HANDLE handle{};
    EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_CreateHandle(&handle, devMem, compiled_axmodel_size));
    EXPECT_NE(nullptr, handle);

    // 5. get & check model type
    AX_ENGINE_MODEL_TYPE_T model_type{};
    EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_GetHandleModelType(handle, &model_type));
    EXPECT_EQ(AX_ENGINE_MODEL_TYPE0, model_type);

    // 6. destroy engine handle
    EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_DestroyHandle(handle));

    // 7. free device memory
    EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem));

    // 8. deinit engine
    EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_Deinit());
}

TEST_F(axclrtDeviceTest, Case11_AXCL_ENGINE_GetModelToolsVersion) {
    const auto model_ver_ref = "2.4-beta3 bd2851d6";

    // 1. init engine
    AX_ENGINE_NPU_ATTR_T attr_set{AX_ENGINE_VIRTUAL_NPU_STD};
    ASSERT_EQ(AXCL_SUCC, AXCL_ENGINE_Init(&attr_set));

    // 2. get malloc model size
    void* devMem = nullptr;
    ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem, compiled_axmodel_size, AXCL_MEM_MALLOC_NORMAL_ONLY));

    // 3. copy model to device
    EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem, compiled_axmodel, compiled_axmodel_size, AXCL_MEMCPY_HOST_TO_DEVICE));

    // 4. create engine handle
    AX_ENGINE_HANDLE handle{};
    EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_CreateHandle(&handle, devMem, compiled_axmodel_size));
    EXPECT_NE(nullptr, handle);

    // 5. get & check model version
    const auto model_ver_got = AXCL_ENGINE_GetModelToolsVersion(handle);
    EXPECT_NE(nullptr, model_ver_got);
    EXPECT_EQ(0, strcmp(model_ver_ref, model_ver_got));

    // 6. destroy engine handle
    EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_DestroyHandle(handle));

    // 7. free device memory
    EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem));

    // 8. deinit engine
    EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_Deinit());
}

TEST_F(axclrtDeviceTest, Case12_AXCL_ENGINE_GetCMMUsage) {
    // 1. init engine
    AX_ENGINE_NPU_ATTR_T attr_set{AX_ENGINE_VIRTUAL_NPU_STD};
    ASSERT_EQ(AXCL_SUCC, AXCL_ENGINE_Init(&attr_set));

    // 2. get malloc model size
    void* devMem = nullptr;
    ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem, compiled_axmodel_size, AXCL_MEM_MALLOC_NORMAL_ONLY));

    // 3. copy model to device
    EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem, compiled_axmodel, compiled_axmodel_size, AXCL_MEMCPY_HOST_TO_DEVICE));

    // 4. create engine handle
    AX_ENGINE_HANDLE handle{};
    EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_CreateHandle(&handle, devMem, compiled_axmodel_size));
    EXPECT_NE(nullptr, handle);

    // 5. check model shape group count
    AX_ENGINE_CMM_INFO cmm_info{};
    EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_GetCMMUsage(handle, &cmm_info));
    EXPECT_EQ(20618160u, cmm_info.nCMMSize);

    // 6. destroy engine handle
    EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_DestroyHandle(handle));

    // 7. free device memory
    EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem));

    // 8. deinit engine
    EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_Deinit());
}

TEST_F(axclrtDeviceTest, Case13_AXCL_ENGINE_GetGroupIOInfoCount) {
    // 1. init engine
    AX_ENGINE_NPU_ATTR_T attr_set{AX_ENGINE_VIRTUAL_NPU_STD};
    ASSERT_EQ(AXCL_SUCC, AXCL_ENGINE_Init(&attr_set));

    // 2. get malloc model size
    void* devMem = nullptr;
    ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem, compiled_axmodel_size, AXCL_MEM_MALLOC_NORMAL_ONLY));

    // 3. copy model to device
    EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem, compiled_axmodel, compiled_axmodel_size, AXCL_MEMCPY_HOST_TO_DEVICE));

    // 4. create engine handle
    AX_ENGINE_HANDLE handle{};
    EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_CreateHandle(&handle, devMem, compiled_axmodel_size));
    EXPECT_NE(nullptr, handle);

    // 5. check model shape group count
    AX_U32 count{};
    EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_GetGroupIOInfoCount(handle, &count));
    EXPECT_EQ(3u, count);

    // 6. destroy engine handle
    EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_DestroyHandle(handle));

    // 7. free device memory
    EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem));

    // 8. deinit engine
    EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_Deinit());
}

TEST_F(axclrtDeviceTest, Case14_AXCL_ENGINE_SetAffinity) {
    // 1. init engine
    AX_ENGINE_NPU_ATTR_T attr_set{AX_ENGINE_VIRTUAL_NPU_STD};
    ASSERT_EQ(AXCL_SUCC, AXCL_ENGINE_Init(&attr_set));

    // 2. get malloc model size
    void* devMem = nullptr;
    ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem, compiled_axmodel_size, AXCL_MEM_MALLOC_NORMAL_ONLY));

    // 3. copy model to device
    EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem, compiled_axmodel, compiled_axmodel_size, AXCL_MEMCPY_HOST_TO_DEVICE));

    // 4. create engine handle
    AX_ENGINE_HANDLE handle{};
    EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_CreateHandle(&handle, devMem, compiled_axmodel_size));
    EXPECT_NE(nullptr, handle);

    // 5. get & check model affinity
    for (int i = 1; i < 0b111; i++) {
        AX_ENGINE_NPU_SET_T mask_set{static_cast<AX_ENGINE_NPU_SET_T>(i)}, mask_got{};
        EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_SetAffinity(handle, mask_set));
        EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_GetAffinity(handle, &mask_got));
        EXPECT_EQ(mask_set, mask_got);
    }

    // 6. destroy engine handle
    EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_DestroyHandle(handle));

    // 7. free device memory
    EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem));

    // 8. deinit engine
    EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_Deinit());
}

TEST_F(axclrtDeviceTest, Case15_AXCL_ENGINE_GetAffinity) {
    for (int i = 1; i <= 0b111; i++) {
        const auto model_name = "compiled_axmodel";

        // 1. init engine
        AX_ENGINE_NPU_ATTR_T attr_set{AX_ENGINE_VIRTUAL_NPU_STD};
        ASSERT_EQ(AXCL_SUCC, AXCL_ENGINE_Init(&attr_set));

        // 2. get malloc model size
        void* devMem = nullptr;
        ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem, compiled_axmodel_size, AXCL_MEM_MALLOC_NORMAL_ONLY));

        // 3. copy model to device
        EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem, compiled_axmodel, compiled_axmodel_size, AXCL_MEMCPY_HOST_TO_DEVICE));

        // 4. create engine handle
        AX_ENGINE_HANDLE handle{};
        AX_ENGINE_HANDLE_EXTRA_T extra{};
        extra.nNpuSet = i;
        extra.pName = reinterpret_cast<AX_S8 *>(const_cast<char *>(model_name));
        EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_CreateHandleV2(&handle, devMem, compiled_axmodel_size, &extra));
        EXPECT_NE(nullptr, handle);

        // 5. check model affinity
        AX_ENGINE_NPU_SET_T mask_got{};
        EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_GetAffinity(handle, &mask_got));
        EXPECT_EQ(static_cast<AX_ENGINE_NPU_SET_T>(i), mask_got);

        // 6. destroy engine handle
        EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_DestroyHandle(handle));

        // 7. free device memory
        EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem));

        // 8. deinit engine
        EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_Deinit());
    }
}

TEST_F(axclrtDeviceTest, Case16_AX_ENGINE_GetIOInfo) {
    // 1. init engine
    AX_ENGINE_NPU_ATTR_T attr_set{};
    ASSERT_EQ(AXCL_SUCC, AXCL_ENGINE_Init(&attr_set));

    // 2. get malloc model size
    void* devMem = nullptr;
    ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem, compiled_axmodel_size, AXCL_MEM_MALLOC_NORMAL_ONLY));

    // 3. copy model to device
    EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem, compiled_axmodel, compiled_axmodel_size, AXCL_MEMCPY_HOST_TO_DEVICE));

    // 4. create engine handle
    AX_ENGINE_HANDLE handle{};
    EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_CreateHandle(&handle, devMem, compiled_axmodel_size));
    EXPECT_NE(nullptr, handle);

    // 5. get engine io info
    AX_ENGINE_IO_INFO_T* io_info{};
    EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_GetIOInfo(handle, &io_info));
    EXPECT_NE(nullptr, io_info);
    // dump_io_info(io_info);

    // 6. check io info
    if (nullptr != io_info) {
        EXPECT_EQ(1u, io_info->nInputSize);
        EXPECT_EQ(1u, io_info->nOutputSize);
        if (nullptr != io_info->pInputs) {
            EXPECT_NE(nullptr, io_info->pInputs);
            if (1u == io_info->nInputSize && nullptr != io_info->pInputs) {
                const auto model_input_name = "data";
                EXPECT_EQ(0, strncmp(model_input_name, io_info->pInputs[0].pName, strlen(model_input_name)));
                EXPECT_EQ(AX_ENGINE_TENSOR_LAYOUT_NHWC, io_info->pInputs[0].eLayout);
                EXPECT_EQ(AX_ENGINE_DT_UINT8, io_info->pInputs[0].eDataType);
                EXPECT_NE(nullptr, io_info->pInputs[0].pExtraMeta);
                if (nullptr != io_info->pInputs[0].pExtraMeta) {
                    EXPECT_EQ(AX_ENGINE_CS_BGR, io_info->pInputs[0].pExtraMeta->eColorSpace);
                }
                EXPECT_EQ(static_cast<AX_U32>(1*224*224*3), io_info->pInputs[0].nSize);
                EXPECT_EQ(4u, io_info->pInputs[0].nShapeSize);
                EXPECT_NE(nullptr, io_info->pInputs[0].pShape);
                if (nullptr != io_info->pInputs[0].pShape) {
                    EXPECT_EQ(1, io_info->pInputs[0].pShape[0]);
                    EXPECT_EQ(224, io_info->pInputs[0].pShape[1]);
                    EXPECT_EQ(224, io_info->pInputs[0].pShape[2]);
                    EXPECT_EQ(3, io_info->pInputs[0].pShape[3]);
                }
                EXPECT_EQ(nullptr, io_info->pInputs[0].pStride);
            }
        }

        EXPECT_NE(nullptr, io_info->pOutputs);
        if (nullptr != io_info->pOutputs) {
            if (1u == io_info->nOutputSize && nullptr != io_info->pOutputs) {
                const auto model_output_name = "prob";
                EXPECT_EQ(0, strncmp(model_output_name, io_info->pOutputs[0].pName, strlen(model_output_name)));
                EXPECT_EQ(AX_ENGINE_TENSOR_LAYOUT_UNKNOWN, io_info->pOutputs[0].eLayout);
                EXPECT_EQ(AX_ENGINE_DT_FLOAT32, io_info->pOutputs[0].eDataType);
                EXPECT_NE(nullptr, io_info->pOutputs[0].pExtraMeta);
                if (nullptr != io_info->pOutputs[0].pExtraMeta) {
                    EXPECT_EQ(AX_ENGINE_CS_FEATUREMAP, io_info->pOutputs[0].pExtraMeta->eColorSpace);
                }
                EXPECT_EQ(static_cast<AX_U32>(sizeof(float) * 1000), io_info->pOutputs[0].nSize);
                EXPECT_EQ(4u, io_info->pOutputs[0].nShapeSize);
                EXPECT_NE(nullptr, io_info->pOutputs[0].pShape);
                if (nullptr != io_info->pOutputs[0].pShape) {
                    EXPECT_EQ(1, io_info->pOutputs[0].pShape[0]);
                    EXPECT_EQ(1000, io_info->pOutputs[0].pShape[1]);
                }
                EXPECT_EQ(nullptr, io_info->pOutputs[0].pStride);
            }
        }
    }

    // 7. destroy engine handle
    EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_DestroyHandle(handle));

    // 8. free device memory
    EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem));

    // 9. deinit engine
    EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_Deinit());
}

TEST_F(axclrtDeviceTest, Case17_AXCL_ENGINE_GetGroupIOInfo) {
    // 1. init engine
    AX_ENGINE_NPU_ATTR_T attr_set{};
    ASSERT_EQ(AXCL_SUCC, AXCL_ENGINE_Init(&attr_set));

    // 2. get malloc model size
    void* devMem = nullptr;
    ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem, compiled_axmodel_size, AXCL_MEM_MALLOC_NORMAL_ONLY));

    // 3. copy model to device
    EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem, compiled_axmodel, compiled_axmodel_size, AXCL_MEMCPY_HOST_TO_DEVICE));

    // 4. create engine handle
    AX_ENGINE_HANDLE handle{};
    EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_CreateHandle(&handle, devMem, compiled_axmodel_size));
    EXPECT_NE(nullptr, handle);

    // 5. get engine io info
    AX_ENGINE_IO_INFO_T* io_info{};
    EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_GetGroupIOInfo(handle, 0, &io_info));
    EXPECT_NE(nullptr, io_info);
    // dump_io_info(io_info);

    // 6. check io info
    if (nullptr != io_info) {
        EXPECT_EQ(1u, io_info->nInputSize);
        EXPECT_EQ(1u, io_info->nOutputSize);
        if (nullptr != io_info->pInputs) {
            EXPECT_NE(nullptr, io_info->pInputs);
            if (1u == io_info->nInputSize && nullptr != io_info->pInputs) {
                const auto model_input_name = "data";
                EXPECT_EQ(0, strncmp(model_input_name, io_info->pInputs[0].pName, strlen(model_input_name)));
                EXPECT_EQ(AX_ENGINE_TENSOR_LAYOUT_NHWC, io_info->pInputs[0].eLayout);
                EXPECT_EQ(AX_ENGINE_DT_UINT8, io_info->pInputs[0].eDataType);
                EXPECT_NE(nullptr, io_info->pInputs[0].pExtraMeta);
                if (nullptr != io_info->pInputs[0].pExtraMeta) {
                    EXPECT_EQ(AX_ENGINE_CS_BGR, io_info->pInputs[0].pExtraMeta->eColorSpace);
                }
                EXPECT_EQ(static_cast<AX_U32>(1*224*224*3), io_info->pInputs[0].nSize);
                EXPECT_EQ(4u, io_info->pInputs[0].nShapeSize);
                EXPECT_NE(nullptr, io_info->pInputs[0].pShape);
                if (nullptr != io_info->pInputs[0].pShape) {
                    EXPECT_EQ(1, io_info->pInputs[0].pShape[0]);
                    EXPECT_EQ(224, io_info->pInputs[0].pShape[1]);
                    EXPECT_EQ(224, io_info->pInputs[0].pShape[2]);
                    EXPECT_EQ(3, io_info->pInputs[0].pShape[3]);
                }
                EXPECT_EQ(nullptr, io_info->pInputs[0].pStride);
            }
        }

        EXPECT_NE(nullptr, io_info->pOutputs);
        if (nullptr != io_info->pOutputs) {
            if (1u == io_info->nOutputSize && nullptr != io_info->pOutputs) {
                const auto model_output_name = "prob";
                EXPECT_EQ(0, strncmp(model_output_name, io_info->pOutputs[0].pName, strlen(model_output_name)));
                EXPECT_EQ(AX_ENGINE_TENSOR_LAYOUT_UNKNOWN, io_info->pOutputs[0].eLayout);
                EXPECT_EQ(AX_ENGINE_DT_FLOAT32, io_info->pOutputs[0].eDataType);
                EXPECT_NE(nullptr, io_info->pOutputs[0].pExtraMeta);
                if (nullptr != io_info->pOutputs[0].pExtraMeta) {
                    EXPECT_EQ(AX_ENGINE_CS_FEATUREMAP, io_info->pOutputs[0].pExtraMeta->eColorSpace);
                }
                EXPECT_EQ(static_cast<AX_U32>(sizeof(float) * 1000), io_info->pOutputs[0].nSize);
                EXPECT_EQ(4u, io_info->pOutputs[0].nShapeSize);
                EXPECT_NE(nullptr, io_info->pOutputs[0].pShape);
                if (nullptr != io_info->pOutputs[0].pShape) {
                    EXPECT_EQ(1, io_info->pOutputs[0].pShape[0]);
                    EXPECT_EQ(1000, io_info->pOutputs[0].pShape[1]);
                }
                EXPECT_EQ(nullptr, io_info->pOutputs[0].pStride);
            }
        }
    }

    // 7. destroy engine handle
    EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_DestroyHandle(handle));

    // 8. free device memory
    EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem));

    // 9. deinit engine
    EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_Deinit());
}

TEST_F(axclrtDeviceTest, Case18_AX_ENGINE_RunSync) {
    // 1. init engine
    AX_ENGINE_NPU_ATTR_T attr_set{AX_ENGINE_VIRTUAL_NPU_DISABLE};
    ASSERT_EQ(AXCL_SUCC, AXCL_ENGINE_Init(&attr_set));

    // 3. get malloc model size
    void* devMem = nullptr;
    ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem, compiled_axmodel_size, AXCL_MEM_MALLOC_NORMAL_ONLY));

    // 4. copy model to device
    EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem, compiled_axmodel, compiled_axmodel_size, AXCL_MEMCPY_HOST_TO_DEVICE));

    // 5. create engine handle
    AX_ENGINE_HANDLE handle{};
    ASSERT_EQ(AXCL_SUCC, AXCL_ENGINE_CreateHandle(&handle, devMem, compiled_axmodel_size));
    ASSERT_NE(nullptr, handle);
    //GTEST_LOG_(INFO) << "handle: 0x" << handle;

    // 6. malloc input & output buffer
    void* input = nullptr;
    void* output = nullptr;
    ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&input, data_0_bin_size, AXCL_MEM_MALLOC_NORMAL_ONLY));
    ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&output, prob_0_bin_size, AXCL_MEM_MALLOC_NORMAL_ONLY));
    ASSERT_NE(nullptr, input);
    ASSERT_NE(nullptr, output);

    // 7. copy input data to device
    EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(input, data_0_bin, data_0_bin_size, AXCL_MEMCPY_HOST_TO_DEVICE));

    // 8. create model io
    AX_ENGINE_IO_T io{};
    std::vector<AX_ENGINE_IO_BUFFER_T> input_buffers(1);
    std::vector<AX_ENGINE_IO_BUFFER_T> output_buffers(1);
    input_buffers[0] = AX_ENGINE_IO_BUFFER_T{};
    output_buffers[0] = AX_ENGINE_IO_BUFFER_T{};

    io.pInputs = input_buffers.data();
    io.pOutputs = output_buffers.data();
    io.nInputSize = 1;
    io.nOutputSize = 1;
    io.pInputs[0].phyAddr = reinterpret_cast<AX_U64>(input);
    io.pInputs[0].nSize = data_0_bin_size;
    io.pOutputs[0].phyAddr = reinterpret_cast<AX_U64>(output);
    io.pOutputs[0].nSize = prob_0_bin_size;

    // 9. run model
    ASSERT_EQ(AXCL_SUCC, AXCL_ENGINE_RunSync(handle, &io));

    // 10. copy output data to host
    std::vector<uint8_t> hw_prob_0_bin(prob_0_bin_size);
    EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(hw_prob_0_bin.data(), output, prob_0_bin_size, AXCL_MEMCPY_DEVICE_TO_HOST));

    // 11. compare output data
    EXPECT_EQ(0, memcmp(prob_0_bin, hw_prob_0_bin.data(), prob_0_bin_size));

    // 12. free input & output buffer
    EXPECT_EQ(AXCL_SUCC, axclrtFree(input));
    EXPECT_EQ(AXCL_SUCC, axclrtFree(output));

    // 13. destroy engine handle
    EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_DestroyHandle(handle));

    // 14. free device memory
    EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem));

    // 15. deinit engine
    EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_Deinit());
}

TEST_F(axclrtDeviceTest, Case19_AX_ENGINE_RunSyncV2) {
    // 1. init engine
    AX_ENGINE_NPU_ATTR_T attr_set{AX_ENGINE_VIRTUAL_NPU_DISABLE};
    ASSERT_EQ(AXCL_SUCC, AXCL_ENGINE_Init(&attr_set));

    // 3. get malloc model size
    void* devMem = nullptr;
    ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem, compiled_axmodel_size, AXCL_MEM_MALLOC_NORMAL_ONLY));

    // 4. copy model to device
    EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem, compiled_axmodel, compiled_axmodel_size, AXCL_MEMCPY_HOST_TO_DEVICE));

    // 5. create engine handle
    AX_ENGINE_HANDLE handle{};
    ASSERT_EQ(AXCL_SUCC, AXCL_ENGINE_CreateHandle(&handle, devMem, compiled_axmodel_size));
    ASSERT_NE(nullptr, handle);

    // 6. create context
    AX_ENGINE_CONTEXT_T context{};
    ASSERT_EQ(AXCL_SUCC, AXCL_ENGINE_CreateContextV2(handle, &context));
    ASSERT_NE(nullptr, context);
    //GTEST_LOG_(INFO) << "handle: 0x" << handle << ", context: 0x" << context;

    // 7. malloc input & output buffer
    void* input = nullptr;
    void* output = nullptr;
    ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&input, data_0_bin_size, AXCL_MEM_MALLOC_NORMAL_ONLY));
    ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&output, prob_0_bin_size, AXCL_MEM_MALLOC_NORMAL_ONLY));
    ASSERT_NE(nullptr, input);
    ASSERT_NE(nullptr, output);

    //GTEST_LOG_(INFO) << "data_0_bin_size: " << data_0_bin_size << ", prob_0_bin_size: " << prob_0_bin_size;
    //GTEST_LOG_(INFO) << "input: " << input << ", output: " << output;

    // 8. copy input data to device
    EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(input, data_0_bin, data_0_bin_size, AXCL_MEMCPY_HOST_TO_DEVICE));

    // 9. create model io
    AX_ENGINE_IO_T io{};
    std::vector<AX_ENGINE_IO_BUFFER_T> input_buffers(1);
    std::vector<AX_ENGINE_IO_BUFFER_T> output_buffers(1);
    input_buffers[0] = AX_ENGINE_IO_BUFFER_T{};
    output_buffers[0] = AX_ENGINE_IO_BUFFER_T{};

    io.pInputs = input_buffers.data();
    io.pOutputs = output_buffers.data();
    io.nInputSize = 1;
    io.nOutputSize = 1;
    io.pInputs[0].phyAddr = reinterpret_cast<AX_U64>(input);
    io.pInputs[0].nSize = data_0_bin_size;
    io.pOutputs[0].phyAddr = reinterpret_cast<AX_U64>(output);
    io.pOutputs[0].nSize = prob_0_bin_size;

    // 10. run model
    ASSERT_EQ(AXCL_SUCC, AXCL_ENGINE_RunSyncV2(handle, context, &io));

    // 11. copy output data to host
    std::vector<uint8_t> hw_prob_0_bin(prob_0_bin_size);
    EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(hw_prob_0_bin.data(), output, prob_0_bin_size, AXCL_MEMCPY_DEVICE_TO_HOST));

    // 12. compare output data
    EXPECT_EQ(0, memcmp(prob_0_bin, hw_prob_0_bin.data(), prob_0_bin_size));

    // 13. free input & output buffer
    EXPECT_EQ(AXCL_SUCC, axclrtFree(input));
    EXPECT_EQ(AXCL_SUCC, axclrtFree(output));

    // 14. destroy engine handle
    EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_DestroyHandle(handle));

    // 15. free device memory
    EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem));

    // 16. deinit engine
    EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_Deinit());
}

TEST_F(axclrtDeviceTest, Case20_AXCL_ENGINE_RunGroupIOSync) {
    // 1. init engine
    AX_ENGINE_NPU_ATTR_T attr_set{AX_ENGINE_VIRTUAL_NPU_DISABLE};
    ASSERT_EQ(AXCL_SUCC, AXCL_ENGINE_Init(&attr_set));

    // 3. get malloc model size
    void* devMem = nullptr;
    ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&devMem, compiled_axmodel_size, AXCL_MEM_MALLOC_NORMAL_ONLY));

    // 4. copy model to device
    EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(devMem, compiled_axmodel, compiled_axmodel_size, AXCL_MEMCPY_HOST_TO_DEVICE));

    // 5. create engine handle
    AX_ENGINE_HANDLE handle{};
    ASSERT_EQ(AXCL_SUCC, AXCL_ENGINE_CreateHandle(&handle, devMem, compiled_axmodel_size));
    ASSERT_NE(nullptr, handle);

    // 6. create context
    AX_ENGINE_CONTEXT_T context{};
    ASSERT_EQ(AXCL_SUCC, AXCL_ENGINE_CreateContextV2(handle, &context));
    ASSERT_NE(nullptr, context);
    //GTEST_LOG_(INFO) << "handle" << handle << ", context: " << context;

    // 7. malloc input & output buffer
    void* input = nullptr;
    void* output = nullptr;
    ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&input, data_0_bin_size, AXCL_MEM_MALLOC_NORMAL_ONLY));
    ASSERT_EQ(AXCL_SUCC, axclrtMalloc(&output, prob_0_bin_size, AXCL_MEM_MALLOC_NORMAL_ONLY));
    ASSERT_NE(nullptr, input);
    ASSERT_NE(nullptr, output);

    //GTEST_LOG_(INFO) << "data_0_bin_size: " << data_0_bin_size << ", prob_0_bin_size: " << prob_0_bin_size;
    //GTEST_LOG_(INFO) << "input: " << input << ", output: " << output;

    // 8. copy input data to device
    EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(input, data_0_bin, data_0_bin_size, AXCL_MEMCPY_HOST_TO_DEVICE));

    // 9. create model io
    AX_ENGINE_IO_T io{};
    std::vector<AX_ENGINE_IO_BUFFER_T> input_buffers(1);
    std::vector<AX_ENGINE_IO_BUFFER_T> output_buffers(1);
    input_buffers[0] = AX_ENGINE_IO_BUFFER_T{};
    output_buffers[0] = AX_ENGINE_IO_BUFFER_T{};

    io.pInputs = input_buffers.data();
    io.pOutputs = output_buffers.data();
    io.nInputSize = 1;
    io.nOutputSize = 1;
    io.pInputs[0].phyAddr = reinterpret_cast<AX_U64>(input);
    io.pInputs[0].nSize = data_0_bin_size;
    io.pOutputs[0].phyAddr = reinterpret_cast<AX_U64>(output);
    io.pOutputs[0].nSize = prob_0_bin_size;

    // 10. run model
    ASSERT_EQ(AXCL_SUCC, AXCL_ENGINE_RunSyncV2(handle, context, &io));
    ASSERT_EQ(AXCL_SUCC, AXCL_ENGINE_RunGroupIOSync(handle, context, 0, &io));

    // 11. copy output data to host
    std::vector<uint8_t> hw_prob_0_bin(prob_0_bin_size);
    EXPECT_EQ(AXCL_SUCC, axclrtMemcpy(hw_prob_0_bin.data(), output, prob_0_bin_size, AXCL_MEMCPY_DEVICE_TO_HOST));

    // 12. compare output data
    EXPECT_EQ(0, memcmp(prob_0_bin, hw_prob_0_bin.data(), prob_0_bin_size));

    // 13. free input & output buffer
    EXPECT_EQ(AXCL_SUCC, axclrtFree(input));
    EXPECT_EQ(AXCL_SUCC, axclrtFree(output));

    // 14. destroy engine handle
    EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_DestroyHandle(handle));

    // 15. free device memory
    EXPECT_EQ(AXCL_SUCC, axclrtFree(devMem));

    // 16. deinit engine
    EXPECT_EQ(AXCL_SUCC, AXCL_ENGINE_Deinit());
}
