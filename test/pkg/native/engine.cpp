/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "native.hpp"
#include "package/device/native/module/npu_request.hpp"
#include "package/device/native/module/npu_response.hpp"
#include "package/host/native/module/npu_request.hpp"
#include "package/host/native/module/npu_response.hpp"

#include "ax_engine_api.h"

class EnginePkgTest : public NativeTest<axcl::pkg::host::npu_request, axcl::pkg::device::npu_response, axcl::pkg::device::npu_request,
                                        axcl::pkg::host::npu_response> {
protected:
    EnginePkgTest()
        : NativeTest<axcl::pkg::host::npu_request, axcl::pkg::device::npu_response, axcl::pkg::device::npu_request,
                     axcl::pkg::host::npu_response>(axcl::pkg::native::module::ENGINE){};
};

TEST_F(EnginePkgTest, AX_ENGINE_Init) {
    /**
     * AX_S32 AX_ENGINE_Init(AX_ENGINE_NPU_ATTR_T* pNpuAttr);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_ENGINE_NPU_ATTR_T mod0, mod1;
    m_random.randomize(mod0);
    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::ENGINE_API::AX_ENGINE_Init, &mod0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::ENGINE_API::AX_ENGINE_Init, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::ENGINE_API::AX_ENGINE_Init, &mod1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(true, struct_equal<AX_ENGINE_NPU_ATTR_T>(mod0, mod1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::ENGINE_API::AX_ENGINE_Init, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::ENGINE_API::AX_ENGINE_Init, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::ENGINE_API::AX_ENGINE_Init, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(EnginePkgTest, AX_ENGINE_Deinit) {
    /**
     * AX_S32 AX_ENGINE_Deinit();
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::ENGINE_API::AX_ENGINE_Deinit));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::ENGINE_API::AX_ENGINE_Deinit, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::ENGINE_API::AX_ENGINE_Deinit));
    CheckRequestPackageHead(msg);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::ENGINE_API::AX_ENGINE_Deinit, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::ENGINE_API::AX_ENGINE_Deinit, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::ENGINE_API::AX_ENGINE_Deinit, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(EnginePkgTest, AX_ENGINE_GetVNPUAttr) {
    /**
     * AX_S32 AX_ENGINE_GetVNPUAttr(AX_ENGINE_NPU_ATTR_T* pNpuAttr);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_ENGINE_NPU_ATTR_T NpuAttr0, NpuAttr1, NpuAttr2;
    m_random.randomize(NpuAttr0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::ENGINE_API::AX_ENGINE_GetVNPUAttr, &NpuAttr0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::ENGINE_API::AX_ENGINE_GetVNPUAttr, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::ENGINE_API::AX_ENGINE_GetVNPUAttr, &NpuAttr1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(true, struct_equal<AX_ENGINE_NPU_ATTR_T>(NpuAttr0, NpuAttr1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::ENGINE_API::AX_ENGINE_GetVNPUAttr, m_req_status, &NpuAttr1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::ENGINE_API::AX_ENGINE_GetVNPUAttr, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::ENGINE_API::AX_ENGINE_GetVNPUAttr, &m_ack_status, &NpuAttr2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(true, struct_equal<AX_ENGINE_NPU_ATTR_T>(NpuAttr0, NpuAttr2));
}
