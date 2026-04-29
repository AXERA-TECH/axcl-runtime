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
#include "package/device/native/module/ivps_request.hpp"
#include "package/device/native/module/ivps_response.hpp"
#include "package/host/native/module/ivps_request.hpp"
#include "package/host/native/module/ivps_response.hpp"

class IvpsPkgTest : public NativeTest<axcl::pkg::host::ivps_request, axcl::pkg::device::ivps_response, axcl::pkg::device::ivps_request,
                                      axcl::pkg::host::ivps_response> {
protected:
    IvpsPkgTest()
        : NativeTest<axcl::pkg::host::ivps_request, axcl::pkg::device::ivps_response, axcl::pkg::device::ivps_request,
                     axcl::pkg::host::ivps_response>(axcl::pkg::native::module::IVPS) {};
};

TEST_F(IvpsPkgTest, AX_IVPS_Init) {
    /**
     * AX_S32 AX_IVPS_Init();
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_Init));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_Init, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_Init));
    CheckRequestPackageHead(msg);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_Init, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_Init, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_Init, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(IvpsPkgTest, AX_IVPS_Deinit) {
    /**
     * AX_S32 AX_IVPS_Deinit();
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_Deinit));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_Deinit, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_Deinit));
    CheckRequestPackageHead(msg);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_Deinit, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_Deinit, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_Deinit, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(IvpsPkgTest, AX_IVPS_CreateGrp) {
    /**
     * AX_S32 AX_IVPS_CreateGrp(IVPS_GRP IvpsGrp, const AX_IVPS_GRP_ATTR_T *ptGrpAttr);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    IVPS_GRP IvpsGrp0, IvpsGrp1;
    m_random.randomize(IvpsGrp0);

    AX_IVPS_GRP_ATTR_T tGrpAttr0, tGrpAttr1;
    m_random.randomize(tGrpAttr0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_CreateGrp, IvpsGrp0, &tGrpAttr0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_CreateGrp, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_CreateGrp, &IvpsGrp1, &tGrpAttr1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IvpsGrp0, IvpsGrp1);
    EXPECT_EQ(true, struct_equal<AX_IVPS_GRP_ATTR_T>(tGrpAttr0, tGrpAttr1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_CreateGrp, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_CreateGrp, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_CreateGrp, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(IvpsPkgTest, AX_IVPS_CreateGrpEx) {
    /**
     * AX_S32 AX_IVPS_CreateGrpEx(IVPS_GRP *IvpsGrp, const AX_IVPS_GRP_ATTR_T *ptGrpAttr);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    IVPS_GRP IvpsGrp0, IvpsGrp1, IvpsGrp2;
    m_random.randomize(IvpsGrp0);

    AX_IVPS_GRP_ATTR_T tGrpAttr0, tGrpAttr1;
    m_random.randomize(tGrpAttr0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_CreateGrpEx, IvpsGrp0, &tGrpAttr0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_CreateGrpEx, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_CreateGrpEx, &IvpsGrp1, &tGrpAttr1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IvpsGrp0, IvpsGrp1);
    EXPECT_EQ(true, struct_equal<AX_IVPS_GRP_ATTR_T>(tGrpAttr0, tGrpAttr1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_CreateGrpEx, m_req_status, IvpsGrp1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_CreateGrpEx, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_CreateGrpEx, &m_ack_status, &IvpsGrp2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(IvpsGrp0, IvpsGrp2);
}

TEST_F(IvpsPkgTest, AX_IVPS_DestoryGrp) {
    /**
     * AX_S32 AX_IVPS_DestoryGrp(IVPS_GRP IvpsGrp);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    IVPS_GRP IvpsGrp0, IvpsGrp1;
    m_random.randomize(IvpsGrp0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_DestoryGrp, IvpsGrp0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_DestoryGrp, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_DestoryGrp, &IvpsGrp1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IvpsGrp0, IvpsGrp1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_DestoryGrp, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_DestoryGrp, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_DestoryGrp, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(IvpsPkgTest, AX_IVPS_SetPipelineAttr) {
    /**
     * AX_S32 AX_IVPS_SetPipelineAttr(IVPS_GRP IvpsGrp, AX_IVPS_PIPELINE_ATTR_T *ptPipelineAttr);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    IVPS_GRP IvpsGrp0, IvpsGrp1;
    m_random.randomize(IvpsGrp0);

    AX_IVPS_PIPELINE_ATTR_T tPipelineAttr0, tPipelineAttr1;
    m_random.randomize(tPipelineAttr0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_SetPipelineAttr, IvpsGrp0, &tPipelineAttr0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_SetPipelineAttr, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_SetPipelineAttr, &IvpsGrp1, &tPipelineAttr1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IvpsGrp0, IvpsGrp1);
    EXPECT_EQ(true, struct_equal<AX_IVPS_PIPELINE_ATTR_T>(tPipelineAttr0, tPipelineAttr1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_SetPipelineAttr, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_SetPipelineAttr, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_SetPipelineAttr, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(IvpsPkgTest, AX_IVPS_GetPipelineAttr) {
    /**
     * AX_S32 AX_IVPS_GetPipelineAttr(IVPS_GRP IvpsGrp, AX_IVPS_PIPELINE_ATTR_T *ptPipelineAttr);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    IVPS_GRP IvpsGrp0, IvpsGrp1;
    m_random.randomize(IvpsGrp0);

    AX_IVPS_PIPELINE_ATTR_T tPipelineAttr0, tPipelineAttr1, tPipelineAttr2;
    m_random.randomize(tPipelineAttr0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_GetPipelineAttr, IvpsGrp0, &tPipelineAttr0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_GetPipelineAttr, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_GetPipelineAttr, &IvpsGrp1, &tPipelineAttr1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IvpsGrp0, IvpsGrp1);
    EXPECT_EQ(true, struct_equal<AX_IVPS_PIPELINE_ATTR_T>(tPipelineAttr0, tPipelineAttr1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_GetPipelineAttr, m_req_status, &tPipelineAttr1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_GetPipelineAttr, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_GetPipelineAttr, &m_ack_status, &tPipelineAttr2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(true, struct_equal<AX_IVPS_PIPELINE_ATTR_T>(tPipelineAttr0, tPipelineAttr2));
}

TEST_F(IvpsPkgTest, AX_IVPS_StartGrp) {
    /**
     * AX_S32 AX_IVPS_StartGrp(IVPS_GRP IvpsGrp);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    IVPS_GRP IvpsGrp0, IvpsGrp1;
    m_random.randomize(IvpsGrp0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_StartGrp, IvpsGrp0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_StartGrp, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_StartGrp, &IvpsGrp1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IvpsGrp0, IvpsGrp1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_StartGrp, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_StartGrp, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_StartGrp, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(IvpsPkgTest, AX_IVPS_StopGrp) {
    /**
     * AX_S32 AX_IVPS_StopGrp(IVPS_GRP IvpsGrp);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    IVPS_GRP IvpsGrp0, IvpsGrp1;
    m_random.randomize(IvpsGrp0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_StopGrp, IvpsGrp0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_StopGrp, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_StopGrp, &IvpsGrp1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IvpsGrp0, IvpsGrp1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_StopGrp, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_StopGrp, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_StopGrp, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(IvpsPkgTest, AX_IVPS_EnableChn) {
    /**
     * AX_S32 AX_IVPS_EnableChn(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    IVPS_GRP IvpsGrp0, IvpsGrp1;
    m_random.randomize(IvpsGrp0);

    IVPS_CHN IvpsChn0, IvpsChn1;
    m_random.randomize(IvpsChn0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_EnableChn, IvpsGrp0, IvpsChn0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_EnableChn, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_EnableChn, &IvpsGrp1, &IvpsChn1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IvpsGrp0, IvpsGrp1);
    EXPECT_EQ(IvpsChn0, IvpsChn1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_EnableChn, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_EnableChn, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_EnableChn, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(IvpsPkgTest, AX_IVPS_SendFrame) {
    /**
     * AX_S32 AX_IVPS_SendFrame(IVPS_GRP IvpsGrp, const AX_VIDEO_FRAME_T *ptFrame, AX_S32 nMilliSec);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    IVPS_GRP IvpsGrp0, IvpsGrp1;
    m_random.randomize(IvpsGrp0);

    AX_VIDEO_FRAME_T tFrame0, tFrame1;
    m_random.randomize(tFrame0);

    AX_S32 nMilliSec0, nMilliSec1;
    m_random.randomize(nMilliSec0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_SendFrame, IvpsGrp0, &tFrame0, nMilliSec0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_SendFrame, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_SendFrame, &IvpsGrp1, &tFrame1, &nMilliSec1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IvpsGrp0, IvpsGrp1);
    EXPECT_EQ(true, struct_equal<AX_VIDEO_FRAME_T>(tFrame0, tFrame1));
    EXPECT_EQ(nMilliSec0, nMilliSec1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_SendFrame, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_SendFrame, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_SendFrame, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(IvpsPkgTest, AX_IVPS_GetChnFrame) {
    /**
     * AX_S32 AX_IVPS_GetChnFrame(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn, AX_VIDEO_FRAME_T *ptFrame, AX_S32 nMilliSec);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    IVPS_GRP IvpsGrp0, IvpsGrp1;
    m_random.randomize(IvpsGrp0);

    IVPS_CHN IvpsChn0, IvpsChn1;
    m_random.randomize(IvpsChn0);

    AX_VIDEO_FRAME_T tFrame0, tFrame1, tFrame2;
    m_random.randomize(tFrame0);

    AX_S32 nMilliSec0, nMilliSec1;
    m_random.randomize(nMilliSec0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_GetChnFrame, IvpsGrp0, IvpsChn0, &tFrame0, nMilliSec0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_GetChnFrame, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_GetChnFrame, &IvpsGrp1, &IvpsChn1, &tFrame1, &nMilliSec1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IvpsGrp0, IvpsGrp1);
    EXPECT_EQ(IvpsChn0, IvpsChn1);
    EXPECT_EQ(true, struct_equal<AX_VIDEO_FRAME_T>(tFrame0, tFrame1));
    EXPECT_EQ(nMilliSec0, nMilliSec1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_GetChnFrame, m_req_status, &tFrame1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_GetChnFrame, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_GetChnFrame, &m_ack_status, &tFrame2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(true, struct_equal<AX_VIDEO_FRAME_T>(tFrame0, tFrame2));
}

TEST_F(IvpsPkgTest, AX_IVPS_ReleaseChnFrame) {
    /**
     * AX_S32 AX_IVPS_ReleaseChnFrame(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn, AX_VIDEO_FRAME_T *ptFrame);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    IVPS_GRP IvpsGrp0, IvpsGrp1;
    m_random.randomize(IvpsGrp0);

    IVPS_CHN IvpsChn0, IvpsChn1;
    m_random.randomize(IvpsChn0);

    AX_VIDEO_FRAME_T tFrame0, tFrame1;
    m_random.randomize(tFrame0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_ReleaseChnFrame, IvpsGrp0, IvpsChn0, &tFrame0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_ReleaseChnFrame, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_ReleaseChnFrame, &IvpsGrp1, &IvpsChn1, &tFrame1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IvpsGrp0, IvpsGrp1);
    EXPECT_EQ(IvpsChn0, IvpsChn1);
    EXPECT_EQ(true, struct_equal<AX_VIDEO_FRAME_T>(tFrame0, tFrame1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_ReleaseChnFrame, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_ReleaseChnFrame, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_ReleaseChnFrame, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(IvpsPkgTest, AX_IVPS_GetGrpFrame) {
    /**
     * AX_S32 AX_IVPS_GetGrpFrame(IVPS_GRP IvpsGrp, AX_VIDEO_FRAME_T *ptFrame, AX_S32 nMilliSec);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    IVPS_GRP IvpsGrp0, IvpsGrp1;
    m_random.randomize(IvpsGrp0);

    AX_VIDEO_FRAME_T tFrame0, tFrame1, tFrame2;
    m_random.randomize(tFrame0);

    AX_S32 nMilliSec0, nMilliSec1;
    m_random.randomize(nMilliSec0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_GetGrpFrame, IvpsGrp0, &tFrame0, nMilliSec0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_GetGrpFrame, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_GetGrpFrame, &IvpsGrp1, &tFrame1, &nMilliSec1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IvpsGrp0, IvpsGrp1);
    EXPECT_EQ(true, struct_equal<AX_VIDEO_FRAME_T>(tFrame0, tFrame1));
    EXPECT_EQ(nMilliSec0, nMilliSec1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_GetGrpFrame, m_req_status, &tFrame1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_GetGrpFrame, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_GetGrpFrame, &m_ack_status, &tFrame2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(true, struct_equal<AX_VIDEO_FRAME_T>(tFrame0, tFrame2));
}

TEST_F(IvpsPkgTest, AX_IVPS_ReleaseGrpFrame) {
    /**
     * AX_S32 AX_IVPS_ReleaseGrpFrame(IVPS_GRP IvpsGrp, AX_VIDEO_FRAME_T *ptFrame);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    IVPS_GRP IvpsGrp0, IvpsGrp1;
    m_random.randomize(IvpsGrp0);

    AX_VIDEO_FRAME_T tFrame0, tFrame1;
    m_random.randomize(tFrame0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_ReleaseGrpFrame, IvpsGrp0, &tFrame0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_ReleaseGrpFrame, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_ReleaseGrpFrame, &IvpsGrp1, &tFrame1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IvpsGrp0, IvpsGrp1);
    EXPECT_EQ(true, struct_equal<AX_VIDEO_FRAME_T>(tFrame0, tFrame1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_ReleaseGrpFrame, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_ReleaseGrpFrame, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_ReleaseGrpFrame, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(IvpsPkgTest, AX_IVPS_GetChnFd) {
    /**
     * AX_S32 AX_IVPS_GetChnFd(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    IVPS_GRP IvpsGrp0, IvpsGrp1;
    m_random.randomize(IvpsGrp0);

    IVPS_CHN IvpsChn0, IvpsChn1;
    m_random.randomize(IvpsChn0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_GetChnFd, IvpsGrp0, IvpsChn0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_GetChnFd, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_GetChnFd, &IvpsGrp1, &IvpsChn1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IvpsGrp0, IvpsGrp1);
    EXPECT_EQ(IvpsChn0, IvpsChn1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_GetChnFd, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_GetChnFd, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_GetChnFd, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(IvpsPkgTest, AX_IVPS_GetDebugFifoFrame) {
    /**
     * AX_S32 AX_IVPS_GetDebugFifoFrame(IVPS_GRP IvpsGrp, AX_VIDEO_FRAME_T *ptFrame);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    IVPS_GRP IvpsGrp0, IvpsGrp1;
    m_random.randomize(IvpsGrp0);

    AX_VIDEO_FRAME_T tFrame0, tFrame1, tFrame2;
    m_random.randomize(tFrame0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_GetDebugFifoFrame, IvpsGrp0, &tFrame0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_GetDebugFifoFrame, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_GetDebugFifoFrame, &IvpsGrp1, &tFrame1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IvpsGrp0, IvpsGrp1);
    EXPECT_EQ(true, struct_equal<AX_VIDEO_FRAME_T>(tFrame0, tFrame1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_GetDebugFifoFrame, m_req_status, &tFrame1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_GetDebugFifoFrame, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_GetDebugFifoFrame, &m_ack_status, &tFrame2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(IvpsPkgTest, AX_IVPS_ReleaseDebugFifoFrame) {
    /**
     * AX_S32 AX_IVPS_ReleaseDebugFifoFrame(IVPS_GRP IvpsGrp, AX_VIDEO_FRAME_T *ptFrame);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    IVPS_GRP IvpsGrp0, IvpsGrp1;
    m_random.randomize(IvpsGrp0);

    AX_VIDEO_FRAME_T tFrame0, tFrame1;
    m_random.randomize(tFrame0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_ReleaseDebugFifoFrame, IvpsGrp0, &tFrame0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_ReleaseDebugFifoFrame, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_ReleaseDebugFifoFrame, &IvpsGrp1, &tFrame1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IvpsGrp0, IvpsGrp1);
    EXPECT_EQ(true, struct_equal<AX_VIDEO_FRAME_T>(tFrame0, tFrame1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_ReleaseDebugFifoFrame, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_ReleaseDebugFifoFrame, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_ReleaseDebugFifoFrame, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(IvpsPkgTest, AX_IVPS_CloseAllFd) {
    /**
     * AX_S32 AX_IVPS_CloseAllFd(AX_VOID);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_CloseAllFd));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_CloseAllFd, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_CloseAllFd));
    CheckRequestPackageHead(msg);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_CloseAllFd, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_CloseAllFd, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_CloseAllFd, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(IvpsPkgTest, AX_IVPS_SetGrpLDCAttr) {
    /**
     * AX_S32 AX_IVPS_SetGrpLDCAttr(IVPS_GRP IvpsGrp, IVPS_FILTER IvpsFilter, const AX_IVPS_LDC_ATTR_T *ptLDCAttr);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    IVPS_GRP IvpsGrp0, IvpsGrp1;
    m_random.randomize(IvpsGrp0);

    IVPS_FILTER IvpsFilter0, IvpsFilter1;
    m_random.randomize(IvpsFilter0);

    AX_IVPS_LDC_ATTR_T tLDCAttr0, tLDCAttr1;
    m_random.randomize(tLDCAttr0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_SetGrpLDCAttr, IvpsGrp0, IvpsFilter0, &tLDCAttr0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_SetGrpLDCAttr, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_SetGrpLDCAttr, &IvpsGrp1, &IvpsFilter1, &tLDCAttr1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IvpsGrp0, IvpsGrp1);
    EXPECT_EQ(IvpsFilter0, IvpsFilter1);
    EXPECT_EQ(true, struct_equal<AX_IVPS_LDC_ATTR_T>(tLDCAttr0, tLDCAttr1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_SetGrpLDCAttr, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_SetGrpLDCAttr, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_SetGrpLDCAttr, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(IvpsPkgTest, AX_IVPS_GetGrpLDCAttr) {
    /**
     * AX_S32 AX_IVPS_GetGrpLDCAttr(IVPS_GRP IvpsGrp, IVPS_FILTER IvpsFilter, AX_IVPS_LDC_ATTR_T *ptLDCAttr);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    IVPS_GRP IvpsGrp0, IvpsGrp1;
    m_random.randomize(IvpsGrp0);

    IVPS_FILTER IvpsFilter0, IvpsFilter1;
    m_random.randomize(IvpsFilter0);

    AX_IVPS_LDC_ATTR_T tLDCAttr0, tLDCAttr1, tLDCAttr2;
    m_random.randomize(tLDCAttr0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_GetGrpLDCAttr, IvpsGrp0, IvpsFilter0, &tLDCAttr0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_GetGrpLDCAttr, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_GetGrpLDCAttr, &IvpsGrp1, &IvpsFilter1, &tLDCAttr1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IvpsGrp0, IvpsGrp1);
    EXPECT_EQ(IvpsFilter0, IvpsFilter1);
    EXPECT_EQ(true, struct_equal<AX_IVPS_LDC_ATTR_T>(tLDCAttr0, tLDCAttr1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_GetGrpLDCAttr, m_req_status, &tLDCAttr1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_GetGrpLDCAttr, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_GetGrpLDCAttr, &m_ack_status, &tLDCAttr2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(true, struct_equal<AX_IVPS_LDC_ATTR_T>(tLDCAttr0, tLDCAttr2));
}

TEST_F(IvpsPkgTest, AX_IVPS_SetChnLDCAttr) {
    /**
     * AX_S32 AX_IVPS_SetChnLDCAttr(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn, IVPS_FILTER IvpsFilter, const AX_IVPS_LDC_ATTR_T *ptLDCAttr);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    IVPS_GRP IvpsGrp0, IvpsGrp1;
    m_random.randomize(IvpsGrp0);

    IVPS_CHN IvpsChn0, IvpsChn1;
    m_random.randomize(IvpsChn0);

    IVPS_FILTER IvpsFilter0, IvpsFilter1;
    m_random.randomize(IvpsFilter0);

    AX_IVPS_LDC_ATTR_T tLDCAttr0, tLDCAttr1;
    m_random.randomize(tLDCAttr0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_SetChnLDCAttr, IvpsGrp0, IvpsChn0, IvpsFilter0, &tLDCAttr0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_SetChnLDCAttr, m_req_decode.get_api());
    EXPECT_EQ(true,
              m_req_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_SetChnLDCAttr, &IvpsGrp1, &IvpsChn1, &IvpsFilter1, &tLDCAttr1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IvpsGrp0, IvpsGrp1);
    EXPECT_EQ(IvpsChn0, IvpsChn1);
    EXPECT_EQ(IvpsFilter0, IvpsFilter1);
    EXPECT_EQ(true, struct_equal<AX_IVPS_LDC_ATTR_T>(tLDCAttr0, tLDCAttr1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_SetChnLDCAttr, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_SetChnLDCAttr, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_SetChnLDCAttr, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(IvpsPkgTest, AX_IVPS_GetChnLDCAttr) {
    /**
     * AX_S32 AX_IVPS_GetChnLDCAttr(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn, IVPS_FILTER IvpsFilter, AX_IVPS_LDC_ATTR_T *ptLDCAttr);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    IVPS_GRP IvpsGrp0, IvpsGrp1;
    m_random.randomize(IvpsGrp0);

    IVPS_CHN IvpsChn0, IvpsChn1;
    m_random.randomize(IvpsChn0);

    IVPS_FILTER IvpsFilter0, IvpsFilter1;
    m_random.randomize(IvpsFilter0);

    AX_IVPS_LDC_ATTR_T tLDCAttr0, tLDCAttr1, tLDCAttr2;
    m_random.randomize(tLDCAttr0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_GetChnLDCAttr, IvpsGrp0, IvpsChn0, IvpsFilter0, &tLDCAttr0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_GetChnLDCAttr, m_req_decode.get_api());
    EXPECT_EQ(true,
              m_req_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_GetChnLDCAttr, &IvpsGrp1, &IvpsChn1, &IvpsFilter1, &tLDCAttr1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IvpsGrp0, IvpsGrp1);
    EXPECT_EQ(IvpsChn0, IvpsChn1);
    EXPECT_EQ(IvpsFilter0, IvpsFilter1);
    EXPECT_EQ(true, struct_equal<AX_IVPS_LDC_ATTR_T>(tLDCAttr0, tLDCAttr1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_GetChnLDCAttr, m_req_status, &tLDCAttr1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_GetChnLDCAttr, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_GetChnLDCAttr, &m_ack_status, &tLDCAttr2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(true, struct_equal<AX_IVPS_LDC_ATTR_T>(tLDCAttr0, tLDCAttr2));
}

TEST_F(IvpsPkgTest, AX_IVPS_SetGrpPoolAttr) {
    /**
     * AX_S32 AX_IVPS_SetGrpPoolAttr(IVPS_GRP IvpsGrp, const AX_IVPS_POOL_ATTR_T *ptPoolAttr);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    IVPS_GRP IvpsGrp0, IvpsGrp1;
    m_random.randomize(IvpsGrp0);

    AX_IVPS_POOL_ATTR_T tPoolAttr0, tPoolAttr1;
    m_random.randomize(tPoolAttr0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_SetGrpPoolAttr, IvpsGrp0, &tPoolAttr0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_SetGrpPoolAttr, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_SetGrpPoolAttr, &IvpsGrp1, &tPoolAttr1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IvpsGrp0, IvpsGrp1);
    EXPECT_EQ(true, struct_equal<AX_IVPS_POOL_ATTR_T>(tPoolAttr0, tPoolAttr1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_SetGrpPoolAttr, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_SetGrpPoolAttr, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_SetGrpPoolAttr, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(IvpsPkgTest, AX_IVPS_SetChnPoolAttr) {
    /**
     * AX_S32 AX_IVPS_SetChnPoolAttr(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn, const AX_IVPS_POOL_ATTR_T *ptPoolAttr);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    IVPS_GRP IvpsGrp0, IvpsGrp1;
    m_random.randomize(IvpsGrp0);

    IVPS_CHN IvpsChn0, IvpsChn1;
    m_random.randomize(IvpsChn0);

    AX_IVPS_POOL_ATTR_T tPoolAttr0, tPoolAttr1;
    m_random.randomize(tPoolAttr0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_SetChnPoolAttr, IvpsGrp0, IvpsChn0, &tPoolAttr0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_SetChnPoolAttr, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_SetChnPoolAttr, &IvpsGrp1, &IvpsChn1, &tPoolAttr1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IvpsGrp0, IvpsGrp1);
    EXPECT_EQ(IvpsChn0, IvpsChn1);
    EXPECT_EQ(true, struct_equal<AX_IVPS_POOL_ATTR_T>(tPoolAttr0, tPoolAttr1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_SetChnPoolAttr, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_SetChnPoolAttr, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_SetChnPoolAttr, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(IvpsPkgTest, AX_IVPS_SetGrpUserFRC) {
    /**
     * AX_S32 AX_IVPS_SetGrpUserFRC(IVPS_GRP IvpsGrp, const AX_IVPS_USER_FRAME_RATE_CTRL_T *ptFrameRateAttr);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    IVPS_GRP IvpsGrp0, IvpsGrp1;
    m_random.randomize(IvpsGrp0);

    AX_IVPS_USER_FRAME_RATE_CTRL_T tFrameRateAttr0, tFrameRateAttr1;
    m_random.randomize(tFrameRateAttr0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_SetGrpUserFRC, IvpsGrp0, &tFrameRateAttr0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_SetGrpUserFRC, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_SetGrpUserFRC, &IvpsGrp1, &tFrameRateAttr1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IvpsGrp0, IvpsGrp1);
    EXPECT_EQ(true, struct_equal<AX_IVPS_USER_FRAME_RATE_CTRL_T>(tFrameRateAttr0, tFrameRateAttr1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_SetGrpUserFRC, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_SetGrpUserFRC, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_SetGrpUserFRC, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(IvpsPkgTest, AX_IVPS_SetChnUserFRC) {
    /**
     * AX_S32 AX_IVPS_SetChnUserFRC(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn, const AX_IVPS_USER_FRAME_RATE_CTRL_T *ptFrameRateAttr);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    IVPS_GRP IvpsGrp0, IvpsGrp1;
    m_random.randomize(IvpsGrp0);

    IVPS_CHN IvpsChn0, IvpsChn1;
    m_random.randomize(IvpsChn0);

    AX_IVPS_USER_FRAME_RATE_CTRL_T tFrameRateAttr0, tFrameRateAttr1;
    m_random.randomize(tFrameRateAttr0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_SetChnUserFRC, IvpsGrp0, IvpsChn0, &tFrameRateAttr0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_SetChnUserFRC, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_SetChnUserFRC, &IvpsGrp1, &IvpsChn1, &tFrameRateAttr1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IvpsGrp0, IvpsGrp1);
    EXPECT_EQ(IvpsChn0, IvpsChn1);
    EXPECT_EQ(true, struct_equal<AX_IVPS_USER_FRAME_RATE_CTRL_T>(tFrameRateAttr0, tFrameRateAttr1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_SetChnUserFRC, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_SetChnUserFRC, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_SetChnUserFRC, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(IvpsPkgTest, AX_IVPS_SetGrpCrop) {
    /**
     * AX_S32 AX_IVPS_SetGrpCrop(IVPS_GRP IvpsGrp, const AX_IVPS_CROP_INFO_T *ptCropInfo);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    IVPS_GRP IvpsGrp0, IvpsGrp1;
    m_random.randomize(IvpsGrp0);

    AX_IVPS_CROP_INFO_T tCropInfo0, tCropInfo1;
    m_random.randomize(tCropInfo0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_SetGrpCrop, IvpsGrp0, &tCropInfo0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_SetGrpCrop, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_SetGrpCrop, &IvpsGrp1, &tCropInfo1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IvpsGrp0, IvpsGrp1);
    EXPECT_EQ(true, struct_equal<AX_IVPS_CROP_INFO_T>(tCropInfo0, tCropInfo1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_SetGrpCrop, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_SetGrpCrop, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_SetGrpCrop, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(IvpsPkgTest, AX_IVPS_GetGrpCrop) {
    /**
     * AX_S32 AX_IVPS_GetGrpCrop(IVPS_GRP IvpsGrp, AX_IVPS_CROP_INFO_T *ptCropInfo);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    IVPS_GRP IvpsGrp0, IvpsGrp1;
    m_random.randomize(IvpsGrp0);

    AX_IVPS_CROP_INFO_T tCropInfo0, tCropInfo1, tCropInfo2;
    m_random.randomize(tCropInfo0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_GetGrpCrop, IvpsGrp0, &tCropInfo0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_GetGrpCrop, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_GetGrpCrop, &IvpsGrp1, &tCropInfo1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IvpsGrp0, IvpsGrp1);
    EXPECT_EQ(true, struct_equal<AX_IVPS_CROP_INFO_T>(tCropInfo0, tCropInfo1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_GetGrpCrop, m_req_status, &tCropInfo1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_GetGrpCrop, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_GetGrpCrop, &m_ack_status, &tCropInfo2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(IvpsPkgTest, AX_IVPS_SetChnAttr) {
    /**
     * AX_S32 AX_IVPS_SetChnAttr(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn, IVPS_FILTER IvpsFilter, const AX_IVPS_CHN_ATTR_T *ptChnAttr);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    IVPS_GRP IvpsGrp0, IvpsGrp1;
    m_random.randomize(IvpsGrp0);

    IVPS_CHN IvpsChn0, IvpsChn1;
    m_random.randomize(IvpsChn0);

    IVPS_FILTER IvpsFilter0, IvpsFilter1;
    m_random.randomize(IvpsFilter0);

    AX_IVPS_CHN_ATTR_T tChnAttr0, tChnAttr1;
    m_random.randomize(tChnAttr0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_SetChnAttr, IvpsGrp0, IvpsChn0, IvpsFilter0, &tChnAttr0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_SetChnAttr, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_SetChnAttr, &IvpsGrp1, &IvpsChn1, &IvpsFilter1, &tChnAttr1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IvpsGrp0, IvpsGrp1);
    EXPECT_EQ(IvpsChn0, IvpsChn1);
    EXPECT_EQ(IvpsFilter0, IvpsFilter1);
    EXPECT_EQ(true, struct_equal<AX_IVPS_CHN_ATTR_T>(tChnAttr0, tChnAttr1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_SetChnAttr, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_SetChnAttr, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_SetChnAttr, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(IvpsPkgTest, AX_IVPS_GetChnAttr) {
    /**
     * AX_S32 AX_IVPS_GetChnAttr(IVPS_GRP IvpsGrp, IVPS_CHN IvpsChn, IVPS_FILTER IvpsFilter, AX_IVPS_CHN_ATTR_T *ptChnAttr);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    IVPS_GRP IvpsGrp0, IvpsGrp1;
    m_random.randomize(IvpsGrp0);

    IVPS_CHN IvpsChn0, IvpsChn1;
    m_random.randomize(IvpsChn0);

    IVPS_FILTER IvpsFilter0, IvpsFilter1;
    m_random.randomize(IvpsFilter0);

    AX_IVPS_CHN_ATTR_T tChnAttr0, tChnAttr1, tChnAttr2;
    m_random.randomize(tChnAttr0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_GetChnAttr, IvpsGrp0, IvpsChn0, IvpsFilter0, &tChnAttr0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_GetChnAttr, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_GetChnAttr, &IvpsGrp1, &IvpsChn1, &IvpsFilter1, &tChnAttr1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IvpsGrp0, IvpsGrp1);
    EXPECT_EQ(IvpsChn0, IvpsChn1);
    EXPECT_EQ(IvpsFilter0, IvpsFilter1);
    EXPECT_EQ(true, struct_equal<AX_IVPS_CHN_ATTR_T>(tChnAttr0, tChnAttr1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_GetChnAttr, m_req_status, &tChnAttr1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_GetChnAttr, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_GetChnAttr, &m_ack_status, &tChnAttr2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(IvpsPkgTest, AX_IVPS_EnableBackupFrame) {
    /**
     * AX_S32 AX_IVPS_EnableBackupFrame(IVPS_GRP IvpsGrp, AX_U8 nFifoDepth);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    IVPS_GRP IvpsGrp0, IvpsGrp1;
    m_random.randomize(IvpsGrp0);

    IVPS_CHN nFifoDepth0, nFifoDepth1;
    m_random.randomize(nFifoDepth0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_EnableBackupFrame, IvpsGrp0, nFifoDepth0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_EnableBackupFrame, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_EnableBackupFrame, &IvpsGrp1, &nFifoDepth1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IvpsGrp0, IvpsGrp1);
    EXPECT_EQ(nFifoDepth0, nFifoDepth1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_EnableBackupFrame, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_EnableBackupFrame, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_EnableBackupFrame, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(IvpsPkgTest, AX_IVPS_DisableBackupFrame) {
    /**
     * AX_S32 AX_IVPS_DisableBackupFrame(IVPS_GRP IvpsGrp);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    IVPS_GRP IvpsGrp0, IvpsGrp1;
    m_random.randomize(IvpsGrp0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_DisableBackupFrame, IvpsGrp0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_DisableBackupFrame, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_DisableBackupFrame, &IvpsGrp1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IvpsGrp0, IvpsGrp1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_DisableBackupFrame, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_DisableBackupFrame, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_DisableBackupFrame, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(IvpsPkgTest, AX_IVPS_ResetGrp) {
    /**
     * AX_S32 AX_IVPS_ResetGrp(IVPS_GRP IvpsGrp);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    IVPS_GRP IvpsGrp0, IvpsGrp1;
    m_random.randomize(IvpsGrp0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_ResetGrp, IvpsGrp0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_ResetGrp, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_ResetGrp, &IvpsGrp1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(IvpsGrp0, IvpsGrp1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_ResetGrp, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_ResetGrp, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_ResetGrp, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(IvpsPkgTest, AX_IVPS_GetEngineDutyCycle) {
    /**
     * AX_S32 AX_IVPS_GetEngineDutyCycle(AX_IVPS_DUTY_CYCLE_ATTR_T *ptDutyCycle);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_IVPS_DUTY_CYCLE_ATTR_T tDutyCycl0, tDutyCycl1, tDutyCycl2;
    m_random.randomize(tDutyCycl0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_GetEngineDutyCycle, &tDutyCycl0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_GetEngineDutyCycle, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_GetEngineDutyCycle, &tDutyCycl1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(true, struct_equal<AX_IVPS_DUTY_CYCLE_ATTR_T>(tDutyCycl0, tDutyCycl1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::IVPS_API::AX_IVPS_GetEngineDutyCycle, m_req_status, &tDutyCycl1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::IVPS_API::AX_IVPS_GetEngineDutyCycle, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::IVPS_API::AX_IVPS_GetEngineDutyCycle, &m_ack_status, &tDutyCycl2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}