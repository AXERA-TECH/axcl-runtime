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
#include "package/device/native/module/ive_request.hpp"
#include "package/device/native/module/ive_response.hpp"
#include "package/host/native/module/ive_request.hpp"
#include "package/host/native/module/ive_response.hpp"
#include "ax_ive_type.h"

class IvePkgTest : public NativeTest<axcl::pkg::host::ive_request, axcl::pkg::device::ive_response, axcl::pkg::device::ive_request,
                                     axcl::pkg::host::ive_response> {
protected:
    IvePkgTest()
        : NativeTest<axcl::pkg::host::ive_request, axcl::pkg::device::ive_response, axcl::pkg::device::ive_request,
                     axcl::pkg::host::ive_response>(axcl::pkg::native::module::IVE) {};
};

TEST_F(IvePkgTest, AX_IVE_Init) {
    /**
     * AX_S32 AX_IVE_Init();
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_Init));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_Init, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_Init));
    CheckRequestPackageHead(msg);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_Init, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_Init, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_Init, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(IvePkgTest, AX_IVE_Exit) {
    /**
     * AX_S32 AX_IVE_Exit();
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_Exit));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_Exit, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_Exit));
    CheckRequestPackageHead(msg);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_Exit, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_Exit, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_Exit, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(IvePkgTest, AX_IVE_Query) {
    /**
     * AX_S32 AX_IVE_Query(AX_IVE_HANDLE IveHandle, AX_BOOL *pbFinish, AX_BOOL bBlock);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_IVE_HANDLE IveHandle0, IveHandle1;
    m_random.randomize(IveHandle0);

    AX_BOOL bFinish0, bFinish1, bFinish2;
    m_random.randomize(bFinish0);

    AX_BOOL bBlock0, bBlock1;
    m_random.randomize(bBlock0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_Query, IveHandle0, bFinish0, bBlock0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_Query, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_Query, &IveHandle1, &bFinish1, &bBlock1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IveHandle0, IveHandle1);
    EXPECT_EQ(bFinish0, bFinish1);
    EXPECT_EQ(bBlock0, bBlock1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_Query, m_req_status, bFinish1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_Query, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_Query, &m_ack_status, &bFinish2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(bFinish0, bFinish2);
}

TEST_F(IvePkgTest, AX_IVE_DMA) {
    /**
     * AX_S32 AX_IVE_DMA(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_DATA_T *pstSrc, AX_IVE_DST_DATA_T *pstDst, AX_IVE_DMA_CTRL_T *pstDmaCtrl,
     * AX_BOOL bInstant);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_IVE_HANDLE IveHandle0, IveHandle1, IveHandle2;
    m_random.randomize(IveHandle0);

    AX_IVE_SRC_DATA_T stSrc0, stSrc1;
    m_random.randomize(stSrc0);

    AX_IVE_DST_DATA_T stDst0, stDst1, stDst2;
    m_random.randomize(stDst0);

    AX_IVE_DMA_CTRL_T stDmaCtrl0, stDmaCtrl1;
    m_random.randomize(stDmaCtrl0);

    AX_BOOL bInstant0, bInstant1;
    m_random.randomize(bInstant0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_DMA, IveHandle0, &stSrc0, &stDst0, &stDmaCtrl0, bInstant0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_DMA, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_DMA, &IveHandle1, &stSrc1, &stDst1, &stDmaCtrl1, &bInstant1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IveHandle0, IveHandle1);
    EXPECT_EQ(true, struct_equal<AX_IVE_SRC_DATA_T>(stSrc0, stSrc1));
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_DATA_T>(stDst0, stDst1));
    EXPECT_EQ(true, struct_equal<AX_IVE_DMA_CTRL_T>(stDmaCtrl0, stDmaCtrl1));
    EXPECT_EQ(bInstant0, bInstant1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_DMA, m_req_status, IveHandle1, &stDst1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_DMA, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_DMA, &m_ack_status, &IveHandle2, &stDst2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(IveHandle0, IveHandle2);
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_DATA_T>(stDst0, stDst2));
}

TEST_F(IvePkgTest, AX_IVE_Add) {
    /**
     * AX_S32 AX_IVE_Add(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc1, AX_IVE_SRC_IMAGE_T *pstSrc2,
     AX_IVE_DST_IMAGE_T *pstDst, AX_IVE_ADD_CTRL_T *pstAddCtrl, AX_BOOL bInstant);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_IVE_HANDLE IveHandle0, IveHandle1, IveHandle2;
    m_random.randomize(IveHandle0);

    AX_IVE_SRC_IMAGE_T stSrc10, stSrc11;
    m_random.randomize(stSrc10);

    AX_IVE_SRC_IMAGE_T stSrc20, stSrc21;
    m_random.randomize(stSrc20);

    AX_IVE_DST_IMAGE_T stDst0, stDst1, stDst2;
    m_random.randomize(stDst0);

    AX_IVE_ADD_CTRL_T stAddCtrl0, stAddCtrl1;
    m_random.randomize(stAddCtrl0);

    AX_BOOL bInstant0, bInstant1;
    m_random.randomize(bInstant0);

    EXPECT_EQ(true,
              m_req_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_Add, IveHandle0, &stSrc10, &stSrc20, &stDst0, &stAddCtrl0, bInstant0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_Add, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_Add, &IveHandle1, &stSrc11, &stSrc21, &stDst1, &stAddCtrl1,
                                        &bInstant1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IveHandle0, IveHandle1);
    EXPECT_EQ(true, struct_equal<AX_IVE_SRC_IMAGE_T>(stSrc10, stSrc11));
    EXPECT_EQ(true, struct_equal<AX_IVE_SRC_IMAGE_T>(stSrc20, stSrc21));
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDst0, stDst1));
    EXPECT_EQ(true, struct_equal<AX_IVE_ADD_CTRL_T>(stAddCtrl0, stAddCtrl1));
    EXPECT_EQ(bInstant0, bInstant1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_Add, m_req_status, IveHandle1, &stDst1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_Add, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_Add, &m_ack_status, &IveHandle2, &stDst2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(IveHandle0, IveHandle2);
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDst0, stDst2));
}

TEST_F(IvePkgTest, AX_IVE_Sub) {
    /**
     * AX_S32 AX_IVE_Sub(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc1, AX_IVE_SRC_IMAGE_T *pstSrc2,
    AX_IVE_DST_IMAGE_T *pstDst, AX_IVE_SUB_CTRL_T *pstSubCtrl, AX_BOOL bInstant);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_IVE_HANDLE IveHandle0, IveHandle1, IveHandle2;
    m_random.randomize(IveHandle0);

    AX_IVE_SRC_IMAGE_T stSrc10, stSrc11;
    m_random.randomize(stSrc10);

    AX_IVE_SRC_IMAGE_T stSrc20, stSrc21;
    m_random.randomize(stSrc20);

    AX_IVE_DST_IMAGE_T stDst0, stDst1, stDst2;
    m_random.randomize(stDst0);

    AX_IVE_SUB_CTRL_T stSubCtrl0, stSubCtrl1;
    m_random.randomize(stSubCtrl0);

    AX_BOOL bInstant0, bInstant1;
    m_random.randomize(bInstant0);

    EXPECT_EQ(true,
              m_req_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_Sub, IveHandle0, &stSrc10, &stSrc20, &stDst0, &stSubCtrl0, bInstant0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_Sub, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_Sub, &IveHandle1, &stSrc11, &stSrc21, &stDst1, &stSubCtrl1,
                                        &bInstant1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IveHandle0, IveHandle1);
    EXPECT_EQ(true, struct_equal<AX_IVE_SRC_IMAGE_T>(stSrc10, stSrc11));
    EXPECT_EQ(true, struct_equal<AX_IVE_SRC_IMAGE_T>(stSrc20, stSrc21));
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDst0, stDst1));
    EXPECT_EQ(true, struct_equal<AX_IVE_SUB_CTRL_T>(stSubCtrl0, stSubCtrl1));
    EXPECT_EQ(bInstant0, bInstant1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_Sub, m_req_status, IveHandle1, &stDst1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_Sub, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_Sub, &m_ack_status, &IveHandle2, &stDst2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(IveHandle0, IveHandle2);
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDst0, stDst2));
}

TEST_F(IvePkgTest, AX_IVE_And) {
    /**
     * AX_S32 AX_IVE_And(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc1, AX_IVE_SRC_IMAGE_T *pstSrc2,
    AX_IVE_DST_IMAGE_T *pstDst, AX_BOOL bInstant);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_IVE_HANDLE IveHandle0, IveHandle1, IveHandle2;
    m_random.randomize(IveHandle0);

    AX_IVE_SRC_IMAGE_T stSrc10, stSrc11;
    m_random.randomize(stSrc10);

    AX_IVE_SRC_IMAGE_T stSrc20, stSrc21;
    m_random.randomize(stSrc20);

    AX_IVE_DST_IMAGE_T stDst0, stDst1, stDst2;
    m_random.randomize(stDst0);

    AX_BOOL bInstant0, bInstant1;
    m_random.randomize(bInstant0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_And, IveHandle0, &stSrc10, &stSrc20, &stDst0, bInstant0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_And, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_And, &IveHandle1, &stSrc11, &stSrc21, &stDst1, &bInstant1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IveHandle0, IveHandle1);
    EXPECT_EQ(true, struct_equal<AX_IVE_SRC_IMAGE_T>(stSrc10, stSrc11));
    EXPECT_EQ(true, struct_equal<AX_IVE_SRC_IMAGE_T>(stSrc20, stSrc21));
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDst0, stDst1));
    EXPECT_EQ(bInstant0, bInstant1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_And, m_req_status, IveHandle1, &stDst1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_And, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_And, &m_ack_status, &IveHandle2, &stDst2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(IveHandle0, IveHandle2);
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDst0, stDst2));
}

TEST_F(IvePkgTest, AX_IVE_Or) {
    /**
     * AX_S32 AX_IVE_Or(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc1, AX_IVE_SRC_IMAGE_T *pstSrc2,
    AX_IVE_DST_IMAGE_T *pstDst, AX_BOOL bInstant);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_IVE_HANDLE IveHandle0, IveHandle1, IveHandle2;
    m_random.randomize(IveHandle0);

    AX_IVE_SRC_IMAGE_T stSrc10, stSrc11;
    m_random.randomize(stSrc10);

    AX_IVE_SRC_IMAGE_T stSrc20, stSrc21;
    m_random.randomize(stSrc20);

    AX_IVE_DST_IMAGE_T stDst0, stDst1, stDst2;
    m_random.randomize(stDst0);

    AX_BOOL bInstant0, bInstant1;
    m_random.randomize(bInstant0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_Or, IveHandle0, &stSrc10, &stSrc20, &stDst0, bInstant0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_Or, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_Or, &IveHandle1, &stSrc11, &stSrc21, &stDst1, &bInstant1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IveHandle0, IveHandle1);
    EXPECT_EQ(true, struct_equal<AX_IVE_SRC_IMAGE_T>(stSrc10, stSrc11));
    EXPECT_EQ(true, struct_equal<AX_IVE_SRC_IMAGE_T>(stSrc20, stSrc21));
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDst0, stDst1));
    EXPECT_EQ(bInstant0, bInstant1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_Or, m_req_status, IveHandle1, &stDst1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_Or, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_Or, &m_ack_status, &IveHandle2, &stDst2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(IveHandle0, IveHandle2);
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDst0, stDst2));
}

TEST_F(IvePkgTest, AX_IVE_Xor) {
    /**
     * AX_S32 AX_IVE_Xor(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc1, AX_IVE_SRC_IMAGE_T *pstSrc2,
    AX_IVE_DST_IMAGE_T *pstDst, AX_BOOL bInstant);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_IVE_HANDLE IveHandle0, IveHandle1, IveHandle2;
    m_random.randomize(IveHandle0);

    AX_IVE_SRC_IMAGE_T stSrc10, stSrc11;
    m_random.randomize(stSrc10);

    AX_IVE_SRC_IMAGE_T stSrc20, stSrc21;
    m_random.randomize(stSrc20);

    AX_IVE_DST_IMAGE_T stDst0, stDst1, stDst2;
    m_random.randomize(stDst0);

    AX_BOOL bInstant0, bInstant1;
    m_random.randomize(bInstant0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_Xor, IveHandle0, &stSrc10, &stSrc20, &stDst0, bInstant0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_Xor, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_Xor, &IveHandle1, &stSrc11, &stSrc21, &stDst1, &bInstant1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IveHandle0, IveHandle1);
    EXPECT_EQ(true, struct_equal<AX_IVE_SRC_IMAGE_T>(stSrc10, stSrc11));
    EXPECT_EQ(true, struct_equal<AX_IVE_SRC_IMAGE_T>(stSrc20, stSrc21));
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDst0, stDst1));
    EXPECT_EQ(bInstant0, bInstant1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_Xor, m_req_status, IveHandle1, &stDst1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_Xor, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_Xor, &m_ack_status, &IveHandle2, &stDst2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(IveHandle0, IveHandle2);
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDst0, stDst2));
}

TEST_F(IvePkgTest, AX_IVE_Mse) {
    /**
     * AX_S32 AX_IVE_Mse(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc1, AX_IVE_SRC_IMAGE_T *pstSrc2,
    AX_IVE_DST_IMAGE_T *pstDst, AX_IVE_MSE_CTRL_T *pstMseCtrl, AX_BOOL bInstant);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_IVE_HANDLE IveHandle0, IveHandle1, IveHandle2;
    m_random.randomize(IveHandle0);

    AX_IVE_SRC_IMAGE_T stSrc10, stSrc11;
    m_random.randomize(stSrc10);

    AX_IVE_SRC_IMAGE_T stSrc20, stSrc21;
    m_random.randomize(stSrc20);

    AX_IVE_DST_IMAGE_T stDst0, stDst1, stDst2;
    m_random.randomize(stDst0);

    AX_IVE_MSE_CTRL_T stMseCtrl0, stMseCtrl1;
    m_random.randomize(stMseCtrl0);

    AX_BOOL bInstant0, bInstant1;
    m_random.randomize(bInstant0);

    EXPECT_EQ(true,
              m_req_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_Mse, IveHandle0, &stSrc10, &stSrc20, &stDst0, &stMseCtrl0, bInstant0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_Mse, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_Mse, &IveHandle1, &stSrc11, &stSrc21, &stDst1, &stMseCtrl1,
                                        &bInstant1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IveHandle0, IveHandle1);
    EXPECT_EQ(true, struct_equal<AX_IVE_SRC_IMAGE_T>(stSrc10, stSrc11));
    EXPECT_EQ(true, struct_equal<AX_IVE_SRC_IMAGE_T>(stSrc20, stSrc21));
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDst0, stDst1));
    EXPECT_EQ(true, struct_equal<AX_IVE_MSE_CTRL_T>(stMseCtrl0, stMseCtrl1));
    EXPECT_EQ(bInstant0, bInstant1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_Mse, m_req_status, IveHandle1, &stDst1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_Mse, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_Mse, &m_ack_status, &IveHandle2, &stDst2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(IveHandle0, IveHandle2);
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDst0, stDst2));
}

TEST_F(IvePkgTest, AX_IVE_CannyHysEdge) {
    /**
     * AX_S32 AX_IVE_CannyHysEdge(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc1, AX_IVE_SRC_IMAGE_T *pstSrc2,
    AX_IVE_DST_IMAGE_T *pstDst, AX_IVE_HYS_EDGE_CTRL_T *pstCannyHysEdgeCtrl, AX_BOOL bInstant);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_IVE_HANDLE IveHandle0, IveHandle1, IveHandle2;
    m_random.randomize(IveHandle0);

    AX_IVE_SRC_IMAGE_T stSrc10, stSrc11;
    m_random.randomize(stSrc10);

    AX_IVE_SRC_IMAGE_T stSrc20, stSrc21;
    m_random.randomize(stSrc20);

    AX_IVE_DST_IMAGE_T stDst0, stDst1, stDst2;
    m_random.randomize(stDst0);

    AX_IVE_HYS_EDGE_CTRL_T stCannyHysEdgeCtrl0, stCannyHysEdgeCtrl1;
    m_random.randomize(stCannyHysEdgeCtrl0);

    AX_BOOL bInstant0, bInstant1;
    m_random.randomize(bInstant0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_CannyHysEdge, IveHandle0, &stSrc10, &stSrc20, &stDst0,
                                      &stCannyHysEdgeCtrl0, bInstant0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_CannyHysEdge, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_CannyHysEdge, &IveHandle1, &stSrc11, &stSrc21, &stDst1,
                                        &stCannyHysEdgeCtrl1, &bInstant1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IveHandle0, IveHandle1);
    EXPECT_EQ(true, struct_equal<AX_IVE_SRC_IMAGE_T>(stSrc10, stSrc11));
    EXPECT_EQ(true, struct_equal<AX_IVE_SRC_IMAGE_T>(stSrc20, stSrc21));
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDst0, stDst1));
    EXPECT_EQ(true, struct_equal<AX_IVE_HYS_EDGE_CTRL_T>(stCannyHysEdgeCtrl0, stCannyHysEdgeCtrl1));
    EXPECT_EQ(bInstant0, bInstant1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_CannyHysEdge, m_req_status, IveHandle1, &stDst1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_CannyHysEdge, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_CannyHysEdge, &m_ack_status, &IveHandle2, &stDst2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(IveHandle0, IveHandle2);
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDst0, stDst2));
}

TEST_F(IvePkgTest, AX_IVE_CannyEdge) {
    /**
     * AX_S32 AX_IVE_CannyEdge(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pstDst,
    AX_IVE_CANNY_EDGE_CTRL_T *pstCannyEdgeCtrl, AX_BOOL bInstant);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_IVE_HANDLE IveHandle0, IveHandle1, IveHandle2;
    m_random.randomize(IveHandle0);

    AX_IVE_SRC_IMAGE_T stSrc0, stSrc1;
    m_random.randomize(stSrc0);

    AX_IVE_DST_IMAGE_T stDst0, stDst1, stDst2;
    m_random.randomize(stDst0);

    AX_IVE_CANNY_EDGE_CTRL_T stCannyEdgeCtrl0, stCannyEdgeCtrl1;
    m_random.randomize(stCannyEdgeCtrl0);

    AX_BOOL bInstant0, bInstant1;
    m_random.randomize(bInstant0);

    EXPECT_EQ(true,
              m_req_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_CannyEdge, IveHandle0, &stSrc0, &stDst0, &stCannyEdgeCtrl0, bInstant0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_CannyEdge, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_CannyEdge, &IveHandle1, &stSrc1, &stDst1, &stCannyEdgeCtrl1,
                                        &bInstant1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IveHandle0, IveHandle1);
    EXPECT_EQ(true, struct_equal<AX_IVE_SRC_IMAGE_T>(stSrc0, stSrc1));
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDst0, stDst1));
    EXPECT_EQ(true, struct_equal<AX_IVE_CANNY_EDGE_CTRL_T>(stCannyEdgeCtrl0, stCannyEdgeCtrl1));
    EXPECT_EQ(bInstant0, bInstant1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_CannyEdge, m_req_status, IveHandle1, &stDst1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_CannyEdge, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_CannyEdge, &m_ack_status, &IveHandle2, &stDst2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(IveHandle0, IveHandle2);
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDst0, stDst2));
}

TEST_F(IvePkgTest, AX_IVE_CCL) {
    /**
     * AX_S32 AX_IVE_CCL(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pstDst,
    AX_IVE_DST_MEM_INFO_T *pstBlob, AX_IVE_CCL_CTRL_T *pstCclCtrl, AX_BOOL bInstant);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_IVE_HANDLE IveHandle0, IveHandle1, IveHandle2;
    m_random.randomize(IveHandle0);

    AX_IVE_SRC_IMAGE_T stSrc0, stSrc1;
    m_random.randomize(stSrc0);

    AX_IVE_DST_IMAGE_T stDst0, stDst1, stDst2;
    m_random.randomize(stDst0);

    AX_IVE_DST_MEM_INFO_T stBlo0, stBlo1, stBlo2;
    m_random.randomize(stBlo0);

    AX_IVE_CCL_CTRL_T stCclCtrl0, stCclCtrl1;
    m_random.randomize(stCclCtrl0);

    AX_BOOL bInstant0, bInstant1;
    m_random.randomize(bInstant0);

    EXPECT_EQ(true,
              m_req_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_CCL, IveHandle0, &stSrc0, &stDst0, &stBlo0, &stCclCtrl0, bInstant0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_CCL, m_req_decode.get_api());
    EXPECT_EQ(true,
              m_req_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_CCL, &IveHandle1, &stSrc1, &stDst1, &stBlo1, &stCclCtrl1, &bInstant1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IveHandle0, IveHandle1);
    EXPECT_EQ(true, struct_equal<AX_IVE_SRC_IMAGE_T>(stSrc0, stSrc1));
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDst0, stDst1));
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_MEM_INFO_T>(stBlo0, stBlo1));
    EXPECT_EQ(true, struct_equal<AX_IVE_CCL_CTRL_T>(stCclCtrl0, stCclCtrl1));
    EXPECT_EQ(bInstant0, bInstant1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_CCL, m_req_status, IveHandle1, &stDst1, &stBlo1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_CCL, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_CCL, &m_ack_status, &IveHandle2, &stDst2, &stBlo2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(IveHandle0, IveHandle2);
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDst0, stDst2));
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_MEM_INFO_T>(stBlo0, stBlo2));
}

TEST_F(IvePkgTest, AX_IVE_Erode) {
    /**
     * AX_S32 AX_IVE_Erode(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pstDst,
    AX_IVE_ERODE_CTRL_T *pstErodeCtrl, AX_BOOL bInstant);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_IVE_HANDLE IveHandle0, IveHandle1, IveHandle2;
    m_random.randomize(IveHandle0);

    AX_IVE_SRC_IMAGE_T stSrc0, stSrc1;
    m_random.randomize(stSrc0);

    AX_IVE_DST_IMAGE_T stDst0, stDst1, stDst2;
    m_random.randomize(stDst0);

    AX_IVE_ERODE_CTRL_T stErodeCtrl0, stErodeCtrl1;
    m_random.randomize(stErodeCtrl0);

    AX_BOOL bInstant0, bInstant1;
    m_random.randomize(bInstant0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_Erode, IveHandle0, &stSrc0, &stDst0, &stErodeCtrl0, bInstant0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_Erode, m_req_decode.get_api());
    EXPECT_EQ(true,
              m_req_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_Erode, &IveHandle1, &stSrc1, &stDst1, &stErodeCtrl1, &bInstant1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IveHandle0, IveHandle1);
    EXPECT_EQ(true, struct_equal<AX_IVE_SRC_IMAGE_T>(stSrc0, stSrc1));
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDst0, stDst1));
    EXPECT_EQ(true, struct_equal<AX_IVE_ERODE_CTRL_T>(stErodeCtrl0, stErodeCtrl1));
    EXPECT_EQ(bInstant0, bInstant1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_Erode, m_req_status, IveHandle1, &stDst1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_Erode, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_Erode, &m_ack_status, &IveHandle2, &stDst2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(IveHandle0, IveHandle2);
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDst0, stDst2));
}

TEST_F(IvePkgTest, AX_IVE_Dilate) {
    /**
     * AX_S32 AX_IVE_Dilate(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pstDst,
    AX_IVE_DILATE_CTRL_T *pstDilateCtrl, AX_BOOL bInstant);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_IVE_HANDLE IveHandle0, IveHandle1, IveHandle2;
    m_random.randomize(IveHandle0);

    AX_IVE_SRC_IMAGE_T stSrc0, stSrc1;
    m_random.randomize(stSrc0);

    AX_IVE_DST_IMAGE_T stDst0, stDst1, stDst2;
    m_random.randomize(stDst0);

    AX_IVE_DILATE_CTRL_T stDilateCtrl0, stDilateCtrl1;
    m_random.randomize(stDilateCtrl0);

    AX_BOOL bInstant0, bInstant1;
    m_random.randomize(bInstant0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_Dilate, IveHandle0, &stSrc0, &stDst0, &stDilateCtrl0, bInstant0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_Dilate, m_req_decode.get_api());
    EXPECT_EQ(true,
              m_req_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_Dilate, &IveHandle1, &stSrc1, &stDst1, &stDilateCtrl1, &bInstant1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IveHandle0, IveHandle1);
    EXPECT_EQ(true, struct_equal<AX_IVE_SRC_IMAGE_T>(stSrc0, stSrc1));
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDst0, stDst1));
    EXPECT_EQ(true, struct_equal<AX_IVE_DILATE_CTRL_T>(stDilateCtrl0, stDilateCtrl1));
    EXPECT_EQ(bInstant0, bInstant1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_Dilate, m_req_status, IveHandle1, &stDst1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_Dilate, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_Dilate, &m_ack_status, &IveHandle2, &stDst2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(IveHandle0, IveHandle2);
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDst0, stDst2));
}

TEST_F(IvePkgTest, AX_IVE_Filter) {
    /**
     * AX_S32 AX_IVE_Filter(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pstDst,
    AX_IVE_FILTER_CTRL_T *pstFltCtrl, AX_BOOL bInstant);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_IVE_HANDLE IveHandle0, IveHandle1, IveHandle2;
    m_random.randomize(IveHandle0);

    AX_IVE_SRC_IMAGE_T stSrc0, stSrc1;
    m_random.randomize(stSrc0);

    AX_IVE_DST_IMAGE_T stDst0, stDst1, stDst2;
    m_random.randomize(stDst0);

    AX_IVE_FILTER_CTRL_T stFltCtrl0, stFltCtrl1;
    m_random.randomize(stFltCtrl0);

    AX_BOOL bInstant0, bInstant1;
    m_random.randomize(bInstant0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_Filter, IveHandle0, &stSrc0, &stDst0, &stFltCtrl0, bInstant0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_Filter, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_Filter, &IveHandle1, &stSrc1, &stDst1, &stFltCtrl1, &bInstant1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IveHandle0, IveHandle1);
    EXPECT_EQ(true, struct_equal<AX_IVE_SRC_IMAGE_T>(stSrc0, stSrc1));
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDst0, stDst1));
    EXPECT_EQ(true, struct_equal<AX_IVE_FILTER_CTRL_T>(stFltCtrl0, stFltCtrl1));
    EXPECT_EQ(bInstant0, bInstant1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_Filter, m_req_status, IveHandle1, &stDst1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_Filter, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_Filter, &m_ack_status, &IveHandle2, &stDst2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(IveHandle0, IveHandle2);
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDst0, stDst2));
}

TEST_F(IvePkgTest, AX_IVE_Hist) {
    /**
     * AX_S32 AX_IVE_Hist(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_MEM_INFO_T *pstDst,
    AX_BOOL bInstant);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_IVE_HANDLE IveHandle0, IveHandle1, IveHandle2;
    m_random.randomize(IveHandle0);

    AX_IVE_SRC_IMAGE_T stSrc0, stSrc1;
    m_random.randomize(stSrc0);

    AX_IVE_DST_MEM_INFO_T stDst0, stDst1, stDst2;
    m_random.randomize(stDst0);

    AX_BOOL bInstant0, bInstant1;
    m_random.randomize(bInstant0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_Hist, IveHandle0, &stSrc0, &stDst0, bInstant0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_Hist, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_Hist, &IveHandle1, &stSrc1, &stDst1, &bInstant1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IveHandle0, IveHandle1);
    EXPECT_EQ(true, struct_equal<AX_IVE_SRC_IMAGE_T>(stSrc0, stSrc1));
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_MEM_INFO_T>(stDst0, stDst1));
    EXPECT_EQ(bInstant0, bInstant1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_Hist, m_req_status, IveHandle1, &stDst1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_Hist, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_Hist, &m_ack_status, &IveHandle2, &stDst2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(IveHandle0, IveHandle2);
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_MEM_INFO_T>(stDst0, stDst2));
}

TEST_F(IvePkgTest, AX_IVE_EqualizeHist) {
    /**
     * AX_S32 AX_IVE_EqualizeHist(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_MEM_INFO_T *pstDst,
    AX_IVE_EQUALIZE_HIST_CTRL_T *pstEqualizeHistCtrl, AX_BOOL bInstant);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_IVE_HANDLE IveHandle0, IveHandle1, IveHandle2;
    m_random.randomize(IveHandle0);

    AX_IVE_SRC_IMAGE_T stSrc0, stSrc1;
    m_random.randomize(stSrc0);

    AX_IVE_DST_MEM_INFO_T stDst0, stDst1, stDst2;
    m_random.randomize(stDst0);

    AX_IVE_EQUALIZE_HIST_CTRL_T stEqualizeHistCtrl0, stEqualizeHistCtrl1;
    m_random.randomize(stEqualizeHistCtrl0);

    AX_BOOL bInstant0, bInstant1;
    m_random.randomize(bInstant0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_EqualizeHist, IveHandle0, &stSrc0, &stDst0, &stEqualizeHistCtrl0,
                                      bInstant0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_EqualizeHist, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_EqualizeHist, &IveHandle1, &stSrc1, &stDst1,
                                        &stEqualizeHistCtrl1, &bInstant1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IveHandle0, IveHandle1);
    EXPECT_EQ(true, struct_equal<AX_IVE_SRC_IMAGE_T>(stSrc0, stSrc1));
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_MEM_INFO_T>(stDst0, stDst1));
    EXPECT_EQ(true, struct_equal<AX_IVE_EQUALIZE_HIST_CTRL_T>(stEqualizeHistCtrl0, stEqualizeHistCtrl1));
    EXPECT_EQ(bInstant0, bInstant1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_EqualizeHist, m_req_status, IveHandle1, &stDst1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_EqualizeHist, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_EqualizeHist, &m_ack_status, &IveHandle2, &stDst2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(IveHandle0, IveHandle2);
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_MEM_INFO_T>(stDst0, stDst2));
}

TEST_F(IvePkgTest, AX_IVE_Integ) {
    /**
     * AX_S32 AX_IVE_Integ(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pstDst,
    AX_IVE_INTEG_CTRL_T *pstIntegCtrl, AX_BOOL bInstant);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_IVE_HANDLE IveHandle0, IveHandle1, IveHandle2;
    m_random.randomize(IveHandle0);

    AX_IVE_SRC_IMAGE_T stSrc0, stSrc1;
    m_random.randomize(stSrc0);

    AX_IVE_DST_IMAGE_T stDst0, stDst1, stDst2;
    m_random.randomize(stDst0);

    AX_IVE_INTEG_CTRL_T stIntegCtrl0, stIntegCtrl1;
    m_random.randomize(stIntegCtrl0);

    AX_BOOL bInstant0, bInstant1;
    m_random.randomize(bInstant0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_Integ, IveHandle0, &stSrc0, &stDst0, &stIntegCtrl0, bInstant0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_Integ, m_req_decode.get_api());
    EXPECT_EQ(true,
              m_req_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_Integ, &IveHandle1, &stSrc1, &stDst1, &stIntegCtrl1, &bInstant1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IveHandle0, IveHandle1);
    EXPECT_EQ(true, struct_equal<AX_IVE_SRC_IMAGE_T>(stSrc0, stSrc1));
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDst0, stDst1));
    EXPECT_EQ(true, struct_equal<AX_IVE_INTEG_CTRL_T>(stIntegCtrl0, stIntegCtrl1));
    EXPECT_EQ(bInstant0, bInstant1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_Integ, m_req_status, IveHandle1, &stDst1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_Integ, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_Integ, &m_ack_status, &IveHandle2, &stDst2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(IveHandle0, IveHandle2);
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDst0, stDst2));
}

TEST_F(IvePkgTest, AX_IVE_MagAndAng) {
    /**
     * AX_S32 AX_IVE_MagAndAng(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc1, AX_IVE_SRC_IMAGE_T *pstSrc2,
    AX_IVE_DST_IMAGE_T *pstDstMag, AX_IVE_DST_IMAGE_T *pstDstAng, AX_BOOL bInstant);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_IVE_HANDLE IveHandle0, IveHandle1, IveHandle2;
    m_random.randomize(IveHandle0);

    AX_IVE_SRC_IMAGE_T stSrc10, stSrc11;
    m_random.randomize(stSrc10);

    AX_IVE_SRC_IMAGE_T stSrc20, stSrc21;
    m_random.randomize(stSrc20);

    AX_IVE_DST_IMAGE_T stDstMag0, stDstMag1, stDstMag2;
    m_random.randomize(stDstMag0);

    AX_IVE_DST_IMAGE_T stDstAng0, stDstAng1, stDstAng2;
    m_random.randomize(stDstAng0);

    AX_BOOL bInstant0, bInstant1;
    m_random.randomize(bInstant0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_MagAndAng, IveHandle0, &stSrc10, &stSrc20, &stDstMag0, &stDstAng0,
                                      bInstant0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_MagAndAng, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_MagAndAng, &IveHandle1, &stSrc11, &stSrc21, &stDstMag1,
                                        &stDstAng1, &bInstant1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IveHandle0, IveHandle1);
    EXPECT_EQ(true, struct_equal<AX_IVE_SRC_IMAGE_T>(stSrc10, stSrc11));
    EXPECT_EQ(true, struct_equal<AX_IVE_SRC_IMAGE_T>(stSrc20, stSrc21));
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDstMag0, stDstMag1));
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDstAng0, stDstAng1));
    EXPECT_EQ(bInstant0, bInstant1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_MagAndAng, m_req_status, IveHandle1, &stDstMag0, &stDstAng0));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_MagAndAng, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_MagAndAng, &m_ack_status, &IveHandle2, &stDstMag2, &stDstAng2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(IveHandle0, IveHandle2);
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDstMag0, stDstMag2));
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDstAng0, stDstAng2));
}

TEST_F(IvePkgTest, AX_IVE_Sobel) {
    /**
     * AX_S32 AX_IVE_Sobel(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pstDst,
    AX_IVE_SOBEL_CTRL_T *pstSobelCtrl, AX_BOOL bInstant);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_IVE_HANDLE IveHandle0, IveHandle1, IveHandle2;
    m_random.randomize(IveHandle0);

    AX_IVE_SRC_IMAGE_T stSrc0, stSrc1;
    m_random.randomize(stSrc0);

    AX_IVE_DST_IMAGE_T stDst0, stDst1, stDst2;
    m_random.randomize(stDst0);

    AX_IVE_SOBEL_CTRL_T stSobelCtrl0, stSobelCtrl1;
    m_random.randomize(stSobelCtrl0);

    AX_BOOL bInstant0, bInstant1;
    m_random.randomize(bInstant0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_Sobel, IveHandle0, &stSrc0, &stDst0, &stSobelCtrl0, bInstant0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_Sobel, m_req_decode.get_api());
    EXPECT_EQ(true,
              m_req_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_Sobel, &IveHandle1, &stSrc1, &stDst1, &stSobelCtrl1, &bInstant1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IveHandle0, IveHandle1);
    EXPECT_EQ(true, struct_equal<AX_IVE_SRC_IMAGE_T>(stSrc0, stSrc1));
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDst0, stDst1));
    EXPECT_EQ(true, struct_equal<AX_IVE_SOBEL_CTRL_T>(stSobelCtrl0, stSobelCtrl1));
    EXPECT_EQ(bInstant0, bInstant1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_Sobel, m_req_status, IveHandle1, &stDst1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_Sobel, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_Sobel, &m_ack_status, &IveHandle2, &stDst2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(IveHandle0, IveHandle2);
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDst0, stDst2));
}

TEST_F(IvePkgTest, AX_IVE_GMM) {
    /**
     * AX_S32 AX_IVE_GMM(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pstFg,
    AX_IVE_DST_IMAGE_T *pstBg, AX_IVE_MEM_INFO_T *pstModel, AX_IVE_GMM_CTRL_T *pstGmmCtrl, AX_BOOL bInstant);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_IVE_HANDLE IveHandle0, IveHandle1, IveHandle2;
    m_random.randomize(IveHandle0);

    AX_IVE_SRC_IMAGE_T stSrc0, stSrc1;
    m_random.randomize(stSrc0);

    AX_IVE_DST_IMAGE_T stFg0, stFg1, stFg2;
    m_random.randomize(stFg0);

    AX_IVE_DST_IMAGE_T stBg0, stBg1, stBg2;
    m_random.randomize(stBg0);

    AX_IVE_MEM_INFO_T stModel0, stModel1, stModel2;
    m_random.randomize(stModel0);

    AX_IVE_GMM_CTRL_T stGmmCtrl0, stGmmCtrl1;
    m_random.randomize(stGmmCtrl0);

    AX_BOOL bInstant0, bInstant1;
    m_random.randomize(bInstant0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_GMM, IveHandle0, &stSrc0, &stFg0, &stBg0, &stModel0, &stGmmCtrl0,
                                      bInstant0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_GMM, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_GMM, &IveHandle1, &stSrc1, &stFg1, &stBg1, &stModel1,
                                        &stGmmCtrl1, &bInstant1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IveHandle0, IveHandle1);
    EXPECT_EQ(true, struct_equal<AX_IVE_SRC_IMAGE_T>(stSrc0, stSrc1));
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stFg0, stFg1));
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stBg0, stBg1));
    EXPECT_EQ(true, struct_equal<AX_IVE_MEM_INFO_T>(stModel0, stModel1));
    EXPECT_EQ(true, struct_equal<AX_IVE_GMM_CTRL_T>(stGmmCtrl0, stGmmCtrl1));
    EXPECT_EQ(bInstant0, bInstant1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_GMM, m_req_status, IveHandle1, &stFg1, &stBg1, &stModel1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_GMM, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_GMM, &m_ack_status, &IveHandle2, &stFg2, &stBg2, &stModel2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(IveHandle0, IveHandle2);
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stFg0, stFg2));
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stBg0, stBg2));
    EXPECT_EQ(true, struct_equal<AX_IVE_MEM_INFO_T>(stModel0, stModel2));
}

TEST_F(IvePkgTest, AX_IVE_GMM2) {
    /**
     * AX_S32 AX_IVE_GMM2(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pstFg,
 AX_IVE_DST_IMAGE_T *pstBg, AX_IVE_MEM_INFO_T *pstModel, AX_IVE_GMM2_CTRL_T *pstGmm2Ctrl, AX_BOOL bInstant);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_IVE_HANDLE IveHandle0, IveHandle1, IveHandle2;
    m_random.randomize(IveHandle0);

    AX_IVE_SRC_IMAGE_T stSrc0, stSrc1;
    m_random.randomize(stSrc0);

    AX_IVE_DST_IMAGE_T stFg0, stFg1, stFg2;
    m_random.randomize(stFg0);

    AX_IVE_DST_IMAGE_T stBg0, stBg1, stBg2;
    m_random.randomize(stBg0);

    AX_IVE_MEM_INFO_T stModel0, stModel1, stModel2;
    m_random.randomize(stModel0);

    AX_IVE_GMM_CTRL_T stGmmCtrl0, stGmmCtrl1;
    m_random.randomize(stGmmCtrl0);

    AX_BOOL bInstant0, bInstant1;
    m_random.randomize(bInstant0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_GMM2, IveHandle0, &stSrc0, &stFg0, &stBg0, &stModel0, &stGmmCtrl0,
                                      bInstant0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_GMM2, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_GMM2, &IveHandle1, &stSrc1, &stFg1, &stBg1, &stModel1,
                                        &stGmmCtrl1, &bInstant1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IveHandle0, IveHandle1);
    EXPECT_EQ(true, struct_equal<AX_IVE_SRC_IMAGE_T>(stSrc0, stSrc1));
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stFg0, stFg1));
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stBg0, stBg1));
    EXPECT_EQ(true, struct_equal<AX_IVE_MEM_INFO_T>(stModel0, stModel1));
    EXPECT_EQ(true, struct_equal<AX_IVE_GMM_CTRL_T>(stGmmCtrl0, stGmmCtrl1));
    EXPECT_EQ(bInstant0, bInstant1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_GMM2, m_req_status, IveHandle1, &stFg1, &stBg1, &stModel1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_GMM2, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_GMM2, &m_ack_status, &IveHandle2, &stFg2, &stBg2, &stModel2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(IveHandle0, IveHandle2);
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stFg0, stFg2));
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stBg0, stBg2));
    EXPECT_EQ(true, struct_equal<AX_IVE_MEM_INFO_T>(stModel0, stModel2));
}

TEST_F(IvePkgTest, AX_IVE_Thresh) {
    /**
     * AX_S32 AX_IVE_Thresh(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pstDst,
    AX_IVE_THRESH_CTRL_T *pstThrCtrl, AX_BOOL bInstant);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_IVE_HANDLE IveHandle0, IveHandle1, IveHandle2;
    m_random.randomize(IveHandle0);

    AX_IVE_SRC_IMAGE_T stSrc0, stSrc1;
    m_random.randomize(stSrc0);

    AX_IVE_DST_IMAGE_T stDst0, stDst1, stDst2;
    m_random.randomize(stDst0);

    AX_IVE_THRESH_CTRL_T stThrCtrl0, stThrCtrl1;
    m_random.randomize(stThrCtrl0);

    AX_BOOL bInstant0, bInstant1;
    m_random.randomize(bInstant0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_Thresh, IveHandle0, &stSrc0, &stDst0, &stThrCtrl0, bInstant0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_Thresh, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_Thresh, &IveHandle1, &stSrc1, &stDst1, &stThrCtrl1, &bInstant1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IveHandle0, IveHandle1);
    EXPECT_EQ(true, struct_equal<AX_IVE_SRC_IMAGE_T>(stSrc0, stSrc1));
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDst0, stDst1));
    EXPECT_EQ(true, struct_equal<AX_IVE_THRESH_CTRL_T>(stThrCtrl0, stThrCtrl1));
    EXPECT_EQ(bInstant0, bInstant1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_Thresh, m_req_status, IveHandle1, &stDst1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_Thresh, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_Thresh, &m_ack_status, &IveHandle2, &stDst2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(IveHandle0, IveHandle2);
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDst0, stDst2));
}

TEST_F(IvePkgTest, AX_IVE_16BitTo8Bit) {
    /**
     * AX_S32 AX_IVE_16BitTo8Bit(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pstDst,
    AX_IVE_16BIT_TO_8BIT_CTRL_T *pst16BitTo8BitCtrl, AX_BOOL bInstant);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_IVE_HANDLE IveHandle0, IveHandle1, IveHandle2;
    m_random.randomize(IveHandle0);

    AX_IVE_SRC_IMAGE_T stSrc0, stSrc1;
    m_random.randomize(stSrc0);

    AX_IVE_DST_IMAGE_T stDst0, stDst1, stDst2;
    m_random.randomize(stDst0);

    AX_IVE_16BIT_TO_8BIT_CTRL_T st16BitTo8BitCtrl0, st16BitTo8BitCtrl1;
    m_random.randomize(st16BitTo8BitCtrl0);

    AX_BOOL bInstant0, bInstant1;
    m_random.randomize(bInstant0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_16BitTo8Bit, IveHandle0, &stSrc0, &stDst0, &st16BitTo8BitCtrl0,
                                      bInstant0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_16BitTo8Bit, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_16BitTo8Bit, &IveHandle1, &stSrc1, &stDst1, &st16BitTo8BitCtrl1,
                                        &bInstant1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IveHandle0, IveHandle1);
    EXPECT_EQ(true, struct_equal<AX_IVE_SRC_IMAGE_T>(stSrc0, stSrc1));
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDst0, stDst1));
    EXPECT_EQ(true, struct_equal<AX_IVE_16BIT_TO_8BIT_CTRL_T>(st16BitTo8BitCtrl0, st16BitTo8BitCtrl1));
    EXPECT_EQ(bInstant0, bInstant1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_16BitTo8Bit, m_req_status, IveHandle1, &stDst1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_16BitTo8Bit, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_16BitTo8Bit, &m_ack_status, &IveHandle2, &stDst2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(IveHandle0, IveHandle2);
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDst0, stDst2));
}

TEST_F(IvePkgTest, AX_IVE_CSC) {
    /**
     * AX_S32 AX_IVE_CSC(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pstDst,
    AX_IVE_ENGINE_E enEngine, AX_BOOL bInstant);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_IVE_HANDLE IveHandle0, IveHandle1, IveHandle2;
    m_random.randomize(IveHandle0);

    AX_IVE_SRC_IMAGE_T stSrc0, stSrc1;
    m_random.randomize(stSrc0);

    AX_IVE_DST_IMAGE_T stDst0, stDst1, stDst2;
    m_random.randomize(stDst0);

    AX_IVE_ENGINE_E enEngine0, enEngine1;
    m_random.randomize(enEngine0);

    AX_BOOL bInstant0, bInstant1;
    m_random.randomize(bInstant0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_CSC, IveHandle0, &stSrc0, &stDst0, enEngine0, bInstant0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_CSC, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_CSC, &IveHandle1, &stSrc1, &stDst1, &enEngine1, &bInstant1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IveHandle0, IveHandle1);
    EXPECT_EQ(true, struct_equal<AX_IVE_SRC_IMAGE_T>(stSrc0, stSrc1));
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDst0, stDst1));
    EXPECT_EQ(enEngine0, enEngine1);
    EXPECT_EQ(bInstant0, bInstant1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_CSC, m_req_status, IveHandle1, &stDst1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_CSC, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_CSC, &m_ack_status, &IveHandle2, &stDst2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(IveHandle0, IveHandle2);
    EXPECT_EQ(true, struct_equal<AX_IVE_DST_IMAGE_T>(stDst0, stDst2));
}

TEST_F(IvePkgTest, AX_IVE_MAU_MatMul) {
    /**
     * AX_S32 AX_IVE_MAU_MatMul(AX_IVE_HANDLE *pIveHandle, AX_IVE_MAU_MATMUL_INPUT_T *pstSrc, AX_IVE_MAU_MATMUL_OUTPUT_T *pstDst,
    AX_IVE_MAU_MATMUL_CTRL_T *pstMatMulCtrl, AX_IVE_ENGINE_E enEngine, AX_BOOL bInstant);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_IVE_HANDLE IveHandle0, IveHandle1, IveHandle2;
    m_random.randomize(IveHandle0);

    AX_IVE_MAU_MATMUL_INPUT_T stSrc0, stSrc1;
    m_random.randomize(stSrc0);

    AX_IVE_MAU_MATMUL_OUTPUT_T stDst0, stDst1, stDst2;
    m_random.randomize(stDst0);

    AX_IVE_MAU_MATMUL_CTRL_T stMatMulCtrl0, stMatMulCtrl1;
    m_random.randomize(stMatMulCtrl0);

    AX_IVE_ENGINE_E enEngine0, enEngine1;
    m_random.randomize(enEngine0);

    AX_BOOL bInstant0, bInstant1;
    m_random.randomize(bInstant0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_MAU_MatMul, IveHandle0, &stSrc0, &stDst0, &stMatMulCtrl0,
                                      enEngine0, bInstant0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_MAU_MatMul, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_MAU_MatMul, &IveHandle1, &stSrc1, &stDst1, &stMatMulCtrl1,
                                        &enEngine1, &bInstant1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IveHandle0, IveHandle1);
    EXPECT_EQ(true, struct_equal<AX_IVE_MAU_MATMUL_INPUT_T>(stSrc0, stSrc1));
    EXPECT_EQ(true, struct_equal<AX_IVE_MAU_MATMUL_OUTPUT_T>(stDst0, stDst1));
    EXPECT_EQ(true, struct_equal<AX_IVE_MAU_MATMUL_CTRL_T>(stMatMulCtrl0, stMatMulCtrl1));
    EXPECT_EQ(enEngine0, enEngine1);
    EXPECT_EQ(bInstant0, bInstant1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_MAU_MatMul, m_req_status, IveHandle1, &stDst1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_MAU_MatMul, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_MAU_MatMul, &m_ack_status, &IveHandle2, &stDst2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(IveHandle0, IveHandle2);
    EXPECT_EQ(true, struct_equal<AX_IVE_MAU_MATMUL_OUTPUT_T>(stDst0, stDst2));
}

TEST_F(IvePkgTest, AX_IVE_NPU_CreateMatMulHandle) {
    /**
     * AX_S32 AX_IVE_NPU_CreateMatMulHandle(AX_IVE_MATMUL_HANDLE *pHandle, AX_IVE_NPU_MATMUL_CTRL_T *pstMatMulCtrl);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_IVE_HANDLE IveHandle0, IveHandle1, IveHandle2;
    m_random.randomize(IveHandle0);

    AX_IVE_NPU_MATMUL_CTRL_T stMatMulCtrl0, stMatMulCtrl1;
    m_random.randomize(stMatMulCtrl0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_NPU_CreateMatMulHandle, IveHandle0, &stMatMulCtrl0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_NPU_CreateMatMulHandle, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_NPU_CreateMatMulHandle, &IveHandle1, &stMatMulCtrl1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IveHandle0, IveHandle1);
    EXPECT_EQ(true, struct_equal<AX_IVE_NPU_MATMUL_CTRL_T>(stMatMulCtrl0, stMatMulCtrl1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_NPU_CreateMatMulHandle, m_req_status, IveHandle1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_NPU_CreateMatMulHandle, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_NPU_CreateMatMulHandle, &m_ack_status, &IveHandle2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(IveHandle0, IveHandle2);
}

TEST_F(IvePkgTest, AX_IVE_NPU_DestroyMatMulHandle) {
    /**
     * AX_S32 AX_IVE_NPU_DestroyMatMulHandle(AX_IVE_MATMUL_HANDLE *pHandle);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_IVE_HANDLE IveHandle0, IveHandle1;
    m_random.randomize(IveHandle0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_NPU_DestroyMatMulHandle, IveHandle0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_NPU_DestroyMatMulHandle, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_NPU_DestroyMatMulHandle, &IveHandle1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IveHandle0, IveHandle1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_NPU_DestroyMatMulHandle, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_NPU_DestroyMatMulHandle, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_NPU_DestroyMatMulHandle, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(IvePkgTest, AX_IVE_NPU_MatMul) {
    /**
     * AX_S32 AX_IVE_NPU_MatMul(AX_IVE_MATMUL_HANDLE hHandle, AX_IVE_MAU_MATMUL_INPUT_T *pstSrc, AX_IVE_MAU_MATMUL_OUTPUT_T *pstDst,
    AX_IVE_ENGINE_E enEngine, AX_BOOL bInstant);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_IVE_HANDLE IveHandle0, IveHandle1;
    m_random.randomize(IveHandle0);

    AX_IVE_MAU_MATMUL_INPUT_T stSrc0, stSrc1;
    m_random.randomize(stSrc0);

    AX_IVE_MAU_MATMUL_OUTPUT_T stDst0, stDst1, stDst2;
    m_random.randomize(stDst0);

    AX_IVE_ENGINE_E enEngine0, enEngine1;
    m_random.randomize(enEngine0);

    AX_BOOL bInstant0, bInstant1;
    m_random.randomize(bInstant0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_CSC, IveHandle0, &stSrc0, &stDst0, enEngine0, bInstant0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_CSC, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_CSC, &IveHandle1, &stSrc1, &stDst1, &enEngine1, &bInstant1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IveHandle0, IveHandle1);
    EXPECT_EQ(true, struct_equal<AX_IVE_MAU_MATMUL_INPUT_T>(stSrc0, stSrc1));
    EXPECT_EQ(true, struct_equal<AX_IVE_MAU_MATMUL_OUTPUT_T>(stDst0, stDst1));
    EXPECT_EQ(enEngine0, enEngine1);
    EXPECT_EQ(bInstant0, bInstant1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVE_API::AX_IVE_CSC, m_req_status, &stDst1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVE_API::AX_IVE_CSC, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVE_API::AX_IVE_CSC, &m_ack_status, &stDst2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(true, struct_equal<AX_IVE_MAU_MATMUL_OUTPUT_T>(stDst0, stDst2));
}