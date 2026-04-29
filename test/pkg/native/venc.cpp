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
#include "package/device/native/module/venc_request.hpp"
#include "package/device/native/module/venc_response.hpp"
#include "package/host/native/module/venc_request.hpp"
#include "package/host/native/module/venc_response.hpp"

class VencPkgTest : public NativeTest<axcl::pkg::host::venc_request, axcl::pkg::device::venc_response, axcl::pkg::device::venc_request,
                                      axcl::pkg::host::venc_response> {
protected:
    VencPkgTest()
        : NativeTest<axcl::pkg::host::venc_request, axcl::pkg::device::venc_response, axcl::pkg::device::venc_request,
                     axcl::pkg::host::venc_response>(axcl::pkg::native::module::VENC) {};

    bool CompareGopSvcT(const AX_VENC_GOP_SVC_T_T &svc1, const AX_VENC_GOP_SVC_T_T &svc2) {
        if (svc1.u32GopSize == svc2.u32GopSize) {
            std::cout << "u32GopSize matches: " << svc1.u32GopSize << std::endl;
        } else {
            std::cout << "u32GopSize mismatch! svc1: " << svc1.u32GopSize << ", svc2: " << svc2.u32GopSize << std::endl;
            return false;
        }

        uint32_t i = 0;
        while (svc1.s8SvcTCfg[i] != nullptr && svc2.s8SvcTCfg[i] != nullptr) {
            if (::strcmp(svc1.s8SvcTCfg[i], svc2.s8SvcTCfg[i]) == 0) {
                std::cout << "String " << i + 1 << " matches: " << svc1.s8SvcTCfg[i] << std::endl;
            } else {
                std::cout << "String " << i + 1 << " mismatch! svc1: " << svc1.s8SvcTCfg[i] << ", svc2: " << svc2.s8SvcTCfg[i] << std::endl;
                return false;
            }
            ++i;
        }

        if (svc1.s8SvcTCfg[i] == nullptr && svc2.s8SvcTCfg[i] == nullptr) {
            std::cout << "String arrays match in length." << std::endl;
        } else {
            std::cout << "String array length mismatch!" << std::endl;
            return false;
        }

        return true;
    }
};

TEST_F(VencPkgTest, AX_VENC_Init) {
    /**
     * AX_S32 AX_VENC_Init(const AX_VENC_MOD_ATTR_T *pstModAttr);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_VENC_MOD_ATTR_T mod0, mod1;
    m_random.randomize(mod0);
    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_Init, &mod0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_Init, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_Init, &mod1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(true, struct_equal<AX_VENC_MOD_ATTR_T>(mod0, mod1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_Init, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_Init, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_Init, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VencPkgTest, AX_VENC_Deinit) {
    /**
     * AX_S32 AX_VENC_Deinit();
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_Deinit));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_Deinit, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_Deinit));
    CheckRequestPackageHead(msg);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_Deinit, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_Deinit, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_Deinit, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VencPkgTest, AX_VENC_CreateChn) {
    /**
     * AX_S32 AX_VENC_CreateChn(VENC_CHN VeChn, const AX_VENC_CHN_ATTR_T *pstAttr);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_CHN VeChn0, VeChn1;
    m_random.randomize(VeChn0);

    AX_VENC_CHN_ATTR_T stAttr0, stAttr1;
    m_random.randomize(stAttr0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_CreateChn, VeChn0, &stAttr0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_CreateChn, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_CreateChn, &VeChn1, &stAttr1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VeChn0, VeChn1);
    EXPECT_EQ(true, struct_equal<AX_VENC_CHN_ATTR_T>(stAttr0, stAttr1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_CreateChn, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_CreateChn, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_CreateChn, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VencPkgTest, AX_VENC_CreateChnEx) {
    /**
     * AX_S32 AX_VENC_CreateChnEx(VENC_CHN *pVeChn, const AX_VENC_CHN_ATTR_T *pstAttr);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_CHN VeChn0, VeChn1, VeChn2;
    m_random.randomize(VeChn0);

    AX_VENC_CHN_ATTR_T stAttr0, stAttr1;
    m_random.randomize(stAttr0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_CreateChnEx, VeChn0, &stAttr0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_CreateChnEx, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_CreateChnEx, &VeChn1, &stAttr1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VeChn0, VeChn1);
    EXPECT_EQ(true, struct_equal<AX_VENC_CHN_ATTR_T>(stAttr0, stAttr1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_CreateChnEx, m_req_status, VeChn1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_CreateChnEx, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_CreateChnEx, &m_ack_status, &VeChn2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(VeChn0, VeChn2);
}

TEST_F(VencPkgTest, AX_VENC_DestroyChn) {
    /**
     * AX_S32 AX_VENC_DestroyChn(VENC_CHN VeChn);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_CHN VeChn0, VeChn1;
    m_random.randomize(VeChn0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_DestroyChn, VeChn0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_DestroyChn, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_DestroyChn, &VeChn1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VeChn0, VeChn1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_DestroyChn, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_DestroyChn, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_DestroyChn, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VencPkgTest, AX_VENC_SendFrame) {
    /**
     * AX_S32 AX_VENC_SendFrame(VENC_CHN VeChn, const AX_VIDEO_FRAME_INFO_T *pstFrame, AX_S32 s32MilliSec);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_CHN VeChn0, VeChn1;
    m_random.randomize(VeChn0);

    AX_VIDEO_FRAME_INFO_T stFrame0, stFrame1;
    m_random.randomize(stFrame0);

    AX_S32 s32MilliSec0, s32MilliSec1;
    m_random.randomize(s32MilliSec0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_SendFrame, VeChn0, &stFrame0, s32MilliSec0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_SendFrame, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_SendFrame, &VeChn1, &stFrame1, &s32MilliSec1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VeChn0, VeChn1);
    EXPECT_EQ(true, struct_equal<AX_VIDEO_FRAME_INFO_T>(stFrame0, stFrame1));
    EXPECT_EQ(s32MilliSec0, s32MilliSec1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_SendFrame, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_SendFrame, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_SendFrame, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VencPkgTest, AX_VENC_SendFrameEx) {
    /**
     * AX_S32 AX_VENC_SendFrameEx(VENC_CHN VeChn, const AX_USER_FRAME_INFO_T *pstFrame, AX_S32 s32MilliSec);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_CHN VeChn0, VeChn1;
    m_random.randomize(VeChn0);

    AX_VIDEO_FRAME_INFO_T stFrame0, stFrame1;
    m_random.randomize(stFrame0);

    AX_S32 s32MilliSec0, s32MilliSec1;
    m_random.randomize(s32MilliSec0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_SendFrameEx, VeChn0, &stFrame0, s32MilliSec0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_SendFrameEx, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_SendFrameEx, &VeChn1, &stFrame1, &s32MilliSec1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VeChn0, VeChn1);
    EXPECT_EQ(true, struct_equal<AX_VIDEO_FRAME_INFO_T>(stFrame0, stFrame1));
    EXPECT_EQ(s32MilliSec0, s32MilliSec1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_SendFrameEx, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_SendFrameEx, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_SendFrameEx, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VencPkgTest, AX_VENC_SelectGrp) {
    /**
     * AX_S32 AX_VENC_SelectGrp(VENC_GRP grpId, AX_CHN_STREAM_STATUS_T *pstChnStrmState, AX_S32 s32MilliSec);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_GRP grpId0, grpId1;
    m_random.randomize(grpId0);

    AX_CHN_STREAM_STATUS_T stChnStrmState0, stChnStrmState1, stChnStrmState2;
    m_random.randomize(stChnStrmState0);

    AX_S32 s32MilliSec0, s32MilliSec1;
    m_random.randomize(s32MilliSec0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_SelectGrp, grpId0, &stChnStrmState0, s32MilliSec0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_SelectGrp, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_SelectGrp, &grpId1, &stChnStrmState1, &s32MilliSec1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(grpId0, grpId1);
    EXPECT_EQ(true, struct_equal<AX_CHN_STREAM_STATUS_T>(stChnStrmState0, stChnStrmState1));
    EXPECT_EQ(s32MilliSec0, s32MilliSec1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_SelectGrp, m_req_status, &stChnStrmState1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_SelectGrp, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_SelectGrp, &m_ack_status, &stChnStrmState2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(true, struct_equal<AX_CHN_STREAM_STATUS_T>(stChnStrmState0, stChnStrmState2));
}

TEST_F(VencPkgTest, AX_VENC_SelectClearGrp) {
    /**
     * AX_S32 AX_VENC_SelectClearGrp(VENC_GRP grpId);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_GRP grpId0, grpId1;
    m_random.randomize(grpId0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_SelectClearGrp, grpId0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_SelectClearGrp, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_SelectClearGrp, &grpId1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(grpId0, grpId1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_SelectClearGrp, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_SelectClearGrp, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_SelectClearGrp, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VencPkgTest, AX_VENC_SelectGrpAddChn) {
    /**
     * AX_S32 AX_VENC_SelectGrpAddChn(VENC_GRP grpId, VENC_CHN VeChn);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_GRP grpId0, grpId1;
    m_random.randomize(grpId0);

    VENC_CHN VeChn0, VeChn1;
    m_random.randomize(VeChn0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_SelectGrpAddChn, grpId0, VeChn0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_SelectGrpAddChn, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_SelectGrpAddChn, &grpId1, &VeChn1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(grpId0, grpId1);
    EXPECT_EQ(VeChn0, VeChn1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_SelectGrpAddChn, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_SelectGrpAddChn, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_SelectGrpAddChn, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VencPkgTest, AX_VENC_SelectGrpDeleteChn) {
    /**
     * AX_S32 AX_VENC_SelectGrpDeleteChn(VENC_GRP grpId, VENC_CHN VeChn);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_GRP grpId0, grpId1;
    m_random.randomize(grpId0);

    VENC_CHN VeChn0, VeChn1;
    m_random.randomize(VeChn0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_SelectGrpDeleteChn, grpId0, VeChn0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_SelectGrpDeleteChn, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_SelectGrpDeleteChn, &grpId1, &VeChn1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(grpId0, grpId1);
    EXPECT_EQ(VeChn0, VeChn1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_SelectGrpDeleteChn, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_SelectGrpDeleteChn, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_SelectGrpDeleteChn, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VencPkgTest, AX_VENC_SelectGrpQuery) {
    /**
     * AX_S32 AX_VENC_SelectGrpQuery(VENC_GRP grpId, AX_VENC_SELECT_GRP_PARAM_T *pstGrpInfo);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_GRP grpId0, grpId1;
    m_random.randomize(grpId0);

    AX_VENC_SELECT_GRP_PARAM_T stGrpInfo0, stGrpInfo1, stGrpInfo2;
    m_random.randomize(stGrpInfo0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_SelectGrpQuery, grpId0, stGrpInfo0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_SelectGrpQuery, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_SelectGrpQuery, &grpId1, &stGrpInfo1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(grpId0, grpId1);
    EXPECT_EQ(true, struct_equal<AX_VENC_SELECT_GRP_PARAM_T>(stGrpInfo0, stGrpInfo1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_SelectGrpQuery, m_req_status, &stGrpInfo1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_SelectGrpQuery, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_SelectGrpQuery, &m_ack_status, &stGrpInfo2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(true, struct_equal<AX_VENC_SELECT_GRP_PARAM_T>(stGrpInfo0, stGrpInfo2));
}

TEST_F(VencPkgTest, AX_VENC_GetFd) {
    /**
     * AX_S32 AX_VENC_GetFd(VENC_CHN VeChn);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_CHN VeChn0, VeChn1;
    m_random.randomize(VeChn0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_GetFd, VeChn0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_GetFd, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_GetFd, &VeChn1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VeChn0, VeChn1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_GetFd, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_GetFd, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_GetFd, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VencPkgTest, AX_VENC_GetStream) {
    /**
     * AX_S32 AX_VENC_GetStream(VENC_CHN VeChn, AX_VENC_STREAM_T *pstStream, AX_S32 s32MilliSec);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_CHN VeChn0, VeChn1;
    m_random.randomize(VeChn0);

    AX_VENC_STREAM_T stStream0, stStream1, stStream2;
    m_random.randomize(stStream0);

    AX_S32 s32MilliSec0, s32MilliSec1;
    m_random.randomize(s32MilliSec0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_GetStream, VeChn0, &stStream0, s32MilliSec0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_GetStream, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_GetStream, &VeChn1, &stStream1, &s32MilliSec1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VeChn0, VeChn1);
    EXPECT_EQ(true, struct_equal<AX_VENC_STREAM_T>(stStream0, stStream1));
    EXPECT_EQ(s32MilliSec0, s32MilliSec1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_GetStream, m_req_status, &stStream1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_GetStream, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_GetStream, &m_ack_status, &stStream2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(true, struct_equal<AX_VENC_STREAM_T>(stStream0, stStream2));
}

TEST_F(VencPkgTest, AX_VENC_ReleaseStream) {
    /**
     * AX_S32 AX_VENC_ReleaseStream(VENC_CHN VeChn, const AX_VENC_STREAM_T *pstStream);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_CHN VeChn0, VeChn1;
    m_random.randomize(VeChn0);

    AX_VENC_STREAM_T stStream0, stStream1;
    m_random.randomize(stStream0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_ReleaseStream, VeChn0, &stStream0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_ReleaseStream, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_ReleaseStream, &VeChn1, &stStream1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VeChn0, VeChn1);
    EXPECT_EQ(true, struct_equal<AX_VENC_STREAM_T>(stStream0, stStream1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_ReleaseStream, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_ReleaseStream, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_ReleaseStream, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VencPkgTest, AX_VENC_GetStreamBufInfo) {
    /**
     * AX_S32 AX_VENC_GetStreamBufInfo(VENC_CHN VeChn, AX_VENC_STREAM_BUF_INFO_T *pstStreamBufInfo);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_CHN VeChn0, VeChn1;
    m_random.randomize(VeChn0);

    AX_VENC_STREAM_BUF_INFO_T stStreamBufInfo0, stStreamBufInfo1, stStreamBufInfo2;
    m_random.randomize(stStreamBufInfo0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_GetStreamBufInfo, VeChn0, &stStreamBufInfo0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_GetStreamBufInfo, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_GetStreamBufInfo, &VeChn1, &stStreamBufInfo1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VeChn0, VeChn1);
    EXPECT_EQ(true, struct_equal<AX_VENC_STREAM_BUF_INFO_T>(stStreamBufInfo0, stStreamBufInfo1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_GetStreamBufInfo, m_req_status, &stStreamBufInfo1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_GetStreamBufInfo, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_GetStreamBufInfo, &m_ack_status, &stStreamBufInfo2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(true, struct_equal<AX_VENC_STREAM_BUF_INFO_T>(stStreamBufInfo0, stStreamBufInfo2));
}

TEST_F(VencPkgTest, AX_VENC_StartRecvFrame) {
    /**
     * AX_S32 AX_VENC_StartRecvFrame(VENC_CHN VeChn, const AX_VENC_RECV_PIC_PARAM_T *pstRecvParam);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_CHN VeChn0, VeChn1;
    m_random.randomize(VeChn0);

    AX_VENC_RECV_PIC_PARAM_T stRecvParam0, stRecvParam1;
    m_random.randomize(stRecvParam0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_StartRecvFrame, VeChn0, &stRecvParam0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_StartRecvFrame, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_StartRecvFrame, &VeChn1, &stRecvParam1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VeChn0, VeChn1);
    EXPECT_EQ(true, struct_equal<AX_VENC_RECV_PIC_PARAM_T>(stRecvParam0, stRecvParam1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_StartRecvFrame, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_StartRecvFrame, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_StartRecvFrame, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VencPkgTest, AX_VENC_StopRecvFrame) {
    /**
     * AX_S32 AX_VENC_StopRecvFrame(VENC_CHN VeChn);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_CHN VeChn0, VeChn1;
    m_random.randomize(VeChn0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_StopRecvFrame, VeChn0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_StopRecvFrame, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_StopRecvFrame, &VeChn1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VeChn0, VeChn1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_StopRecvFrame, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_StopRecvFrame, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_StopRecvFrame, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VencPkgTest, AX_VENC_ResetChn) {
    /**
     * AX_S32 AX_VENC_ResetChn(VENC_CHN VeChn);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_CHN VeChn0, VeChn1;
    m_random.randomize(VeChn0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_ResetChn, VeChn0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_ResetChn, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_ResetChn, &VeChn1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VeChn0, VeChn1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_ResetChn, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_ResetChn, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_ResetChn, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VencPkgTest, AX_VENC_SetRoiAttr) {
    /**
     * AX_S32 AX_VENC_SetRoiAttr(VENC_CHN VeChn, const AX_VENC_ROI_ATTR_T *pstRoiAttr);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_CHN VeChn0, VeChn1;
    m_random.randomize(VeChn0);

    AX_VENC_ROI_ATTR_T stRoiAttr0, stRoiAttr1;
    m_random.randomize(stRoiAttr0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_SetRoiAttr, VeChn0, &stRoiAttr0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_SetRoiAttr, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_SetRoiAttr, &VeChn1, &stRoiAttr1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VeChn0, VeChn1);
    EXPECT_EQ(true, struct_equal<AX_VENC_ROI_ATTR_T>(stRoiAttr0, stRoiAttr1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_SetRoiAttr, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_SetRoiAttr, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_SetRoiAttr, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VencPkgTest, AX_VENC_GetRoiAttr) {
    /**
     * AX_S32 AX_VENC_GetRoiAttr(VENC_CHN VeChn, AX_U32 u32Index, AX_VENC_ROI_ATTR_T *pstRoiAttr);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_CHN VeChn0, VeChn1;
    m_random.randomize(VeChn0);

    AX_U32 u32Index0, u32Index1;
    m_random.randomize(u32Index0);

    AX_VENC_ROI_ATTR_T stRoiAttr0, stRoiAttr1, stRoiAttr2;
    m_random.randomize(stRoiAttr0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_GetRoiAttr, VeChn0, u32Index0, &stRoiAttr0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_GetRoiAttr, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_GetRoiAttr, &VeChn1, &u32Index1, &stRoiAttr1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VeChn0, VeChn1);
    EXPECT_EQ(u32Index0, u32Index1);
    EXPECT_EQ(true, struct_equal<AX_VENC_ROI_ATTR_T>(stRoiAttr0, stRoiAttr1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_GetRoiAttr, m_req_status, &stRoiAttr1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_GetRoiAttr, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_GetRoiAttr, &m_ack_status, &stRoiAttr2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(true, struct_equal<AX_VENC_ROI_ATTR_T>(stRoiAttr0, stRoiAttr2));
}

TEST_F(VencPkgTest, AX_VENC_SetRcParam) {
    /**
     * AX_S32 AX_VENC_SetRcParam(VENC_CHN VeChn, const AX_VENC_RC_PARAM_T *pstRcParam);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_CHN VeChn0, VeChn1;
    m_random.randomize(VeChn0);

    AX_VENC_RC_PARAM_T stRcParam0, stRcParam1;
    m_random.randomize(stRcParam0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_SetRcParam, VeChn0, &stRcParam0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_SetRcParam, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_SetRcParam, &VeChn1, &stRcParam1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VeChn0, VeChn1);
    EXPECT_EQ(true, struct_equal<AX_VENC_RC_PARAM_T>(stRcParam0, stRcParam1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_SetRcParam, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_SetRcParam, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_SetRcParam, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VencPkgTest, AX_VENC_GetRcParam) {
    /**
     * AX_S32 AX_VENC_GetRcParam(VENC_CHN VeChn, AX_VENC_RC_PARAM_T *pstRcParam);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_CHN VeChn0, VeChn1;
    m_random.randomize(VeChn0);

    AX_VENC_RC_PARAM_T stRcParam0, stRcParam1, stRcParam2;
    m_random.randomize(stRcParam0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_GetRcParam, VeChn0, &stRcParam0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_GetRcParam, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_GetRcParam, &VeChn1, &stRcParam1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VeChn0, VeChn1);
    EXPECT_EQ(true, struct_equal<AX_VENC_RC_PARAM_T>(stRcParam0, stRcParam1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_GetRcParam, m_req_status, &stRcParam1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_GetRcParam, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_GetRcParam, &m_ack_status, &stRcParam2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(true, struct_equal<AX_VENC_RC_PARAM_T>(stRcParam0, stRcParam2));
}

TEST_F(VencPkgTest, AX_VENC_SetModParam) {
    /**
     * AX_S32 AX_VENC_SetModParam(AX_VENC_ENCODER_TYPE_E enVencType, const AX_VENC_MOD_PARAM_T *pstModParam);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_VENC_ENCODER_TYPE_E enVencType0, enVencType1;
    m_random.randomize(enVencType0);

    AX_VENC_MOD_PARAM_T stModParam0, stModParam1;
    m_random.randomize(stModParam0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_SetModParam, enVencType0, &stModParam0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_SetModParam, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_SetModParam, &enVencType1, &stModParam1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(enVencType0, enVencType1);
    EXPECT_EQ(true, struct_equal<AX_VENC_MOD_PARAM_T>(stModParam0, stModParam1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_SetModParam, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_SetModParam, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_SetModParam, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VencPkgTest, AX_VENC_GetModParam) {
    /**
     * AX_S32 AX_VENC_GetModParam(AX_VENC_ENCODER_TYPE_E enVencType, AX_VENC_MOD_PARAM_T *pstModParam);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_VENC_ENCODER_TYPE_E enVencType0, enVencType1;
    m_random.randomize(enVencType0);

    AX_VENC_MOD_PARAM_T stModParam0, stModParam1, stModParam2;
    m_random.randomize(stModParam0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_GetModParam, enVencType0, &stModParam0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_GetModParam, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_GetModParam, &enVencType1, &stModParam1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(enVencType0, enVencType1);
    EXPECT_EQ(true, struct_equal<AX_VENC_MOD_PARAM_T>(stModParam0, stModParam1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_GetModParam, m_req_status, stModParam1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_GetModParam, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_GetModParam, &m_ack_status, &stModParam2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(true, struct_equal<AX_VENC_MOD_PARAM_T>(stModParam0, stModParam2));
}

TEST_F(VencPkgTest, AX_VENC_SetVuiParam) {
    /**
     * AX_S32 AX_VENC_SetVuiParam(VENC_CHN VeChn, const AX_VENC_VUI_PARAM_T *pstVuiParam);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_CHN VeChn0, VeChn1;
    m_random.randomize(VeChn0);

    AX_VENC_VUI_PARAM_T stVuiParam0, stVuiParam1;
    m_random.randomize(stVuiParam0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_SetVuiParam, VeChn0, &stVuiParam0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_SetVuiParam, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_SetVuiParam, &VeChn1, &stVuiParam1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VeChn0, VeChn1);
    EXPECT_EQ(true, struct_equal<AX_VENC_VUI_PARAM_T>(stVuiParam0, stVuiParam1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_SetVuiParam, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_SetVuiParam, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_SetVuiParam, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VencPkgTest, AX_VENC_GetVuiParam) {
    /**
     * AX_S32 AX_VENC_GetVuiParam(VENC_CHN VeChn, AX_VENC_VUI_PARAM_T *pstVuiParam);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_CHN VeChn0, VeChn1;
    m_random.randomize(VeChn0);

    AX_VENC_VUI_PARAM_T stVuiParam0, stVuiParam1, stVuiParam2;
    m_random.randomize(stVuiParam0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_GetVuiParam, VeChn0, &stVuiParam0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_GetVuiParam, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_GetVuiParam, &VeChn1, &stVuiParam1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VeChn0, VeChn1);
    EXPECT_EQ(true, struct_equal<AX_VENC_VUI_PARAM_T>(stVuiParam0, stVuiParam1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_GetVuiParam, m_req_status, &stVuiParam1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_GetVuiParam, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_GetVuiParam, &m_ack_status, &stVuiParam2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(true, struct_equal<AX_VENC_VUI_PARAM_T>(stVuiParam0, stVuiParam2));
}

TEST_F(VencPkgTest, AX_VENC_SetChnAttr) {
    /**
     * AX_S32 AX_VENC_SetChnAttr(VENC_CHN VeChn, const AX_VENC_CHN_ATTR_T *pstChnAttr);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_CHN VeChn0, VeChn1;
    m_random.randomize(VeChn0);

    AX_VENC_CHN_ATTR_T stChnAttr0, stChnAttr1;
    m_random.randomize(stChnAttr0);
    stChnAttr0.stGopAttr.enGopMode = AX_VENC_GOPMODE_SVC_T;

    AX_U32 sSvcTGopSize = 4;
    /*SVC-T GOP4*/
    const AX_CHAR *stSvcTCfg[] = {
        "Frame1:  P      1      0       0.4624        2        1           -1          1",
        "Frame2:  P      2      0       0.4624        1        1           -2          1",
        "Frame3:  P      3      0       0.4624        2        2           -1 -3       1 0",
        "Frame4:  P      4      0       0.4624        0        1           -4          1",
        NULL,
    };

    stChnAttr0.stGopAttr.stSvcT.u32GopSize = sSvcTGopSize;
    stChnAttr0.stGopAttr.stSvcT.s8SvcTCfg = const_cast<AX_CHAR **>(stSvcTCfg);
    axcl::pkg::uint8_array arr0;
    EXPECT_EQ(true, m_req_encode.serialize_gop_svc_t(stChnAttr0.stGopAttr.stSvcT, arr0.data, arr0.size));
    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_SetChnAttr, VeChn0, &stChnAttr0, arr0));
    ::free(arr0.data);
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_SetChnAttr, m_req_decode.get_api());
    axcl::pkg::uint8_array arr1;
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_SetChnAttr, &VeChn1, &stChnAttr1, &arr1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VeChn0, VeChn1);
    EXPECT_EQ(true, struct_equal<AX_VENC_CHN_ATTR_T>(stChnAttr0, stChnAttr1));
    std::vector<uint8_t> svc_mem;
    EXPECT_EQ(true, m_req_decode.deserialize_gop_svc_t(arr1.data, arr1.size, stChnAttr1.stGopAttr.stSvcT, svc_mem));
    EXPECT_EQ(true, CompareGopSvcT(stChnAttr0.stGopAttr.stSvcT, stChnAttr1.stGopAttr.stSvcT));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_SetChnAttr, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_SetChnAttr, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_SetChnAttr, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VencPkgTest, AX_VENC_GetChnAttr) {
    /**
     * AX_S32 AX_VENC_GetChnAttr(VENC_CHN VeChn, AX_VENC_CHN_ATTR_T *pstChnAttr);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_CHN VeChn0, VeChn1;
    m_random.randomize(VeChn0);

    AX_VENC_CHN_ATTR_T stChnAttr0, stChnAttr1, stChnAttr2;
    m_random.randomize(stChnAttr0);
    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_GetChnAttr, VeChn0, &stChnAttr0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_GetChnAttr, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_GetChnAttr, &VeChn1, &stChnAttr1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VeChn0, VeChn1);
    EXPECT_EQ(true, struct_equal<AX_VENC_CHN_ATTR_T>(stChnAttr0, stChnAttr1));

    /* device -> host */
    axcl::pkg::uint8_array arr1, arr2;
    stChnAttr1.stGopAttr.enGopMode = AX_VENC_GOPMODE_SVC_T;
    AX_U32 sSvcTGopSize = 4;
    /* SVC-T GOP4 */
    const AX_CHAR *stSvcTCfg[] = {
        "Frame1:  P      1      0       0.4624        2        1           -1          1",
        "Frame2:  P      2      0       0.4624        1        1           -2          1",
        "Frame3:  P      3      0       0.4624        2        2           -1 -3       1 0",
        "Frame4:  P      4      0       0.4624        0        1           -4          1",
        NULL,
    };
    stChnAttr1.stGopAttr.stSvcT.u32GopSize = sSvcTGopSize;
    stChnAttr1.stGopAttr.stSvcT.s8SvcTCfg = const_cast<AX_CHAR **>(stSvcTCfg);
    EXPECT_EQ(true, m_ack_encode.serialize_gop_svc_t(stChnAttr1.stGopAttr.stSvcT, arr1.data, arr1.size));
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_GetChnAttr, m_req_status, &stChnAttr1, arr1));
    ::free(arr1.data);
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_GetChnAttr, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_GetChnAttr, &m_ack_status, &stChnAttr2, &arr2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(true, struct_equal<AX_VENC_CHN_ATTR_T>(stChnAttr1, stChnAttr2));
    std::vector<uint8_t> svc_mem;
    EXPECT_EQ(true, m_ack_decode.deserialize_gop_svc_t(arr2.data, arr2.size, stChnAttr2.stGopAttr.stSvcT, svc_mem));
    EXPECT_EQ(true, CompareGopSvcT(stChnAttr1.stGopAttr.stSvcT, stChnAttr2.stGopAttr.stSvcT));
}

TEST_F(VencPkgTest, AX_VENC_SetRateJamStrategy) {
    /**
     * AX_S32 AX_VENC_SetRateJamStrategy(VENC_CHN VeChn, const AX_VENC_RATE_JAM_CFG_T *pstRateJamParam);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_CHN VeChn0, VeChn1;
    m_random.randomize(VeChn0);

    AX_VENC_RATE_JAM_CFG_T stRateJamParam0, stRateJamParam1;
    m_random.randomize(stRateJamParam0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_SetRateJamStrategy, VeChn0, &stRateJamParam0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_SetRateJamStrategy, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_SetRateJamStrategy, &VeChn1, &stRateJamParam1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VeChn0, VeChn1);
    EXPECT_EQ(true, struct_equal<AX_VENC_RATE_JAM_CFG_T>(stRateJamParam0, stRateJamParam1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_SetRateJamStrategy, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_SetRateJamStrategy, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_SetRateJamStrategy, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VencPkgTest, AX_VENC_GetRateJamStrategy) {
    /**
     * AX_S32 AX_VENC_GetRateJamStrategy(VENC_CHN VeChn, AX_VENC_RATE_JAM_CFG_T *pstRateJamParam);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_CHN VeChn0, VeChn1;
    m_random.randomize(VeChn0);

    AX_VENC_RATE_JAM_CFG_T stRateJamParam0, stRateJamParam1, stRateJamParam2;
    m_random.randomize(stRateJamParam0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_GetRateJamStrategy, VeChn0, &stRateJamParam0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_GetRateJamStrategy, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_GetRateJamStrategy, &VeChn1, &stRateJamParam1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VeChn0, VeChn1);
    EXPECT_EQ(true, struct_equal<AX_VENC_RATE_JAM_CFG_T>(stRateJamParam0, stRateJamParam1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_GetRateJamStrategy, m_req_status, &stRateJamParam1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_GetRateJamStrategy, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_GetRateJamStrategy, &m_ack_status, &stRateJamParam2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(true, struct_equal<AX_VENC_RATE_JAM_CFG_T>(stRateJamParam0, stRateJamParam2));
}

TEST_F(VencPkgTest, AX_VENC_SetSuperFrameStrategy) {
    /**
     * AX_S32 AX_VENC_SetSuperFrameStrategy(VENC_CHN VeChn, const AX_VENC_SUPERFRAME_CFG_T *pstSuperFrameCfg);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_CHN VeChn0, VeChn1;
    m_random.randomize(VeChn0);

    AX_VENC_SUPERFRAME_CFG_T stSuperFrameCfg0, stSuperFrameCfg1;
    m_random.randomize(stSuperFrameCfg0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_SetSuperFrameStrategy, VeChn0, &stSuperFrameCfg0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_SetSuperFrameStrategy, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_SetSuperFrameStrategy, &VeChn1, &stSuperFrameCfg1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VeChn0, VeChn1);
    EXPECT_EQ(true, struct_equal<AX_VENC_SUPERFRAME_CFG_T>(stSuperFrameCfg0, stSuperFrameCfg1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_SetSuperFrameStrategy, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_SetSuperFrameStrategy, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_SetSuperFrameStrategy, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VencPkgTest, AX_VENC_GetSuperFrameStrategy) {
    /**
     * AX_S32 AX_VENC_GetSuperFrameStrategy(VENC_CHN VeChn, AX_VENC_SUPERFRAME_CFG_T *pstSuperFrameCfg);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_CHN VeChn0, VeChn1;
    m_random.randomize(VeChn0);

    AX_VENC_SUPERFRAME_CFG_T stSuperFrameCfg0, stSuperFrameCfg1, stSuperFrameCfg2;
    m_random.randomize(stSuperFrameCfg0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_GetSuperFrameStrategy, VeChn0, &stSuperFrameCfg0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_GetSuperFrameStrategy, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_GetSuperFrameStrategy, &VeChn1, &stSuperFrameCfg1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VeChn0, VeChn1);
    EXPECT_EQ(true, struct_equal<AX_VENC_SUPERFRAME_CFG_T>(stSuperFrameCfg0, stSuperFrameCfg1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_GetSuperFrameStrategy, m_req_status, &stSuperFrameCfg1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_GetSuperFrameStrategy, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_GetSuperFrameStrategy, &m_ack_status, &stSuperFrameCfg2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(true, struct_equal<AX_VENC_SUPERFRAME_CFG_T>(stSuperFrameCfg0, stSuperFrameCfg2));
}

TEST_F(VencPkgTest, AX_VENC_SetIntraRefresh) {
    /**
     * AX_S32 AX_VENC_SetIntraRefresh(VENC_CHN VeChn, const AX_VENC_INTRA_REFRESH_T *pstIntraRefresh);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_CHN VeChn0, VeChn1;
    m_random.randomize(VeChn0);

    AX_VENC_INTRA_REFRESH_T stIntraRefresh0, stIntraRefresh1;
    m_random.randomize(stIntraRefresh0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_SetIntraRefresh, VeChn0, &stIntraRefresh0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_SetIntraRefresh, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_SetIntraRefresh, &VeChn1, &stIntraRefresh1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VeChn0, VeChn1);
    EXPECT_EQ(true, struct_equal<AX_VENC_INTRA_REFRESH_T>(stIntraRefresh0, stIntraRefresh1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_SetIntraRefresh, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_SetIntraRefresh, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_SetIntraRefresh, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VencPkgTest, AX_VENC_GetIntraRefresh) {
    /**
     * AX_S32 AX_VENC_GetIntraRefresh(VENC_CHN VeChn, AX_VENC_INTRA_REFRESH_T *pstIntraRefresh);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_CHN VeChn0, VeChn1;
    m_random.randomize(VeChn0);

    AX_VENC_INTRA_REFRESH_T stIntraRefresh0, stIntraRefresh1, stIntraRefresh2;
    m_random.randomize(stIntraRefresh0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_GetIntraRefresh, VeChn0, &stIntraRefresh0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_GetIntraRefresh, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_GetIntraRefresh, &VeChn1, &stIntraRefresh1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VeChn0, VeChn1);
    EXPECT_EQ(true, struct_equal<AX_VENC_INTRA_REFRESH_T>(stIntraRefresh0, stIntraRefresh1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_GetIntraRefresh, m_req_status, &stIntraRefresh1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_GetIntraRefresh, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_GetIntraRefresh, &m_ack_status, &stIntraRefresh2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(true, struct_equal<AX_VENC_INTRA_REFRESH_T>(stIntraRefresh0, stIntraRefresh2));
}

TEST_F(VencPkgTest, AX_VENC_SetUsrData) {
    /**
     * AX_S32 AX_VENC_SetUsrData(VENC_CHN VeChn, const AX_VENC_USR_DATA_T *pstUsrData);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_CHN VeChn0, VeChn1;
    m_random.randomize(VeChn0);

    AX_VENC_USR_DATA_T stUsrData0, stUsrData1;
    m_random.randomize(stUsrData0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_SetUsrData, VeChn0, &stUsrData0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_SetUsrData, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_SetUsrData, &VeChn1, &stUsrData1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VeChn0, VeChn1);
    EXPECT_EQ(true, struct_equal<AX_VENC_USR_DATA_T>(stUsrData0, stUsrData1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_SetUsrData, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_SetUsrData, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_SetUsrData, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VencPkgTest, AX_VENC_GetUsrData) {
    /**
     * AX_S32 AX_VENC_GetUsrData(VENC_CHN VeChn, AX_VENC_USR_DATA_T *pstUsrData);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_CHN VeChn0, VeChn1;
    m_random.randomize(VeChn0);

    AX_VENC_USR_DATA_T stUsrData0, stUsrData1, stUsrData2;
    m_random.randomize(stUsrData0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_GetUsrData, VeChn0, &stUsrData0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_GetUsrData, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_GetUsrData, &VeChn1, &stUsrData1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VeChn0, VeChn1);
    EXPECT_EQ(true, struct_equal<AX_VENC_USR_DATA_T>(stUsrData0, stUsrData1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_GetUsrData, m_req_status, &stUsrData1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_GetUsrData, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_GetUsrData, &m_ack_status, &stUsrData2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(true, struct_equal<AX_VENC_USR_DATA_T>(stUsrData0, stUsrData2));
}

TEST_F(VencPkgTest, AX_VENC_SetSliceSplit) {
    /**
     * AX_S32 AX_VENC_SetSliceSplit(VENC_CHN VeChn, const AX_VENC_SLICE_SPLIT_T *pstSliceSplit);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_CHN VeChn0, VeChn1;
    m_random.randomize(VeChn0);

    AX_VENC_SLICE_SPLIT_T stSliceSplit0, stSliceSplit1;
    m_random.randomize(stSliceSplit0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_SetSliceSplit, VeChn0, &stSliceSplit0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_SetSliceSplit, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_SetSliceSplit, &VeChn1, &stSliceSplit1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VeChn0, VeChn1);
    EXPECT_EQ(true, struct_equal<AX_VENC_SLICE_SPLIT_T>(stSliceSplit0, stSliceSplit1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_SetSliceSplit, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_SetSliceSplit, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_SetSliceSplit, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VencPkgTest, AX_VENC_GetSliceSplit) {
    /**
     * AX_S32 AX_VENC_GetSliceSplit(VENC_CHN VeChn, AX_VENC_SLICE_SPLIT_T *pstSliceSplit);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_CHN VeChn0, VeChn1;
    m_random.randomize(VeChn0);

    AX_VENC_SLICE_SPLIT_T stSliceSplit0, stSliceSplit1, stSliceSplit2;
    m_random.randomize(stSliceSplit0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_GetSliceSplit, VeChn0, &stSliceSplit0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_GetSliceSplit, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_GetSliceSplit, &VeChn1, &stSliceSplit1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VeChn0, VeChn1);
    EXPECT_EQ(true, struct_equal<AX_VENC_SLICE_SPLIT_T>(stSliceSplit0, stSliceSplit1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_GetSliceSplit, m_req_status, &stSliceSplit1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_GetSliceSplit, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_GetSliceSplit, &m_ack_status, &stSliceSplit2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(true, struct_equal<AX_VENC_SLICE_SPLIT_T>(stSliceSplit0, stSliceSplit2));
}

TEST_F(VencPkgTest, AX_VENC_RequestIDR) {
    /**
     * AX_S32 AX_VENC_RequestIDR(VENC_CHN VeChn, AX_BOOL bInstant);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_CHN VeChn0, VeChn1;
    m_random.randomize(VeChn0);

    AX_BOOL bInstant0, bInstant1;
    m_random.randomize(bInstant0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_RequestIDR, VeChn0, bInstant0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_RequestIDR, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_RequestIDR, &VeChn1, &bInstant1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VeChn0, VeChn1);
    EXPECT_EQ(bInstant0, bInstant1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_RequestIDR, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_RequestIDR, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_RequestIDR, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VencPkgTest, AX_VENC_QueryStatus) {
    /**
     * AX_S32 AX_VENC_QueryStatus(VENC_CHN VeChn, AX_VENC_CHN_STATUS_T *pstStatus);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_CHN VeChn0, VeChn1;
    m_random.randomize(VeChn0);

    AX_VENC_CHN_STATUS_T stStatus0, stStatus1;
    m_random.randomize(stStatus0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_QueryStatus, VeChn0, &stStatus0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_QueryStatus, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_QueryStatus, &VeChn1, &stStatus1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VeChn0, VeChn1);
    EXPECT_EQ(true, struct_equal<AX_VENC_CHN_STATUS_T>(stStatus0, stStatus1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_QueryStatus, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_QueryStatus, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_QueryStatus, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VencPkgTest, AX_VENC_SetJpegParam) {
    /**
     * AX_S32 AX_VENC_SetJpegParam(VENC_CHN VeChn, const AX_VENC_JPEG_PARAM_T *pstJpegParam);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_CHN VeChn0, VeChn1;
    m_random.randomize(VeChn0);

    AX_VENC_JPEG_PARAM_T stJpegParam0, stJpegParam1;
    m_random.randomize(stJpegParam0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_SetJpegParam, VeChn0, &stJpegParam0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_SetJpegParam, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_SetJpegParam, &VeChn1, &stJpegParam1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VeChn0, VeChn1);
    EXPECT_EQ(true, struct_equal<AX_VENC_JPEG_PARAM_T>(stJpegParam0, stJpegParam1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_SetJpegParam, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_SetJpegParam, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_SetJpegParam, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VencPkgTest, AX_VENC_GetJpegParam) {
    /**
     * AX_S32 AX_VENC_GetJpegParam(VENC_CHN VeChn, AX_VENC_JPEG_PARAM_T *pstJpegParam);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    VENC_CHN VeChn0, VeChn1;
    m_random.randomize(VeChn0);

    AX_VENC_JPEG_PARAM_T stJpegParam0, stJpegParam1, stJpegParam2;
    m_random.randomize(stJpegParam0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_GetJpegParam, VeChn0, &stJpegParam0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_GetJpegParam, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_GetJpegParam, &VeChn1, &stJpegParam1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VeChn0, VeChn1);
    EXPECT_EQ(true, struct_equal<AX_VENC_JPEG_PARAM_T>(stJpegParam0, stJpegParam1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_GetJpegParam, m_req_status, &stJpegParam1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_GetJpegParam, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_GetJpegParam, &m_ack_status, &stJpegParam2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(true, struct_equal<AX_VENC_JPEG_PARAM_T>(stJpegParam0, stJpegParam2));
}

TEST_F(VencPkgTest, AX_VENC_JpegEncodeOneFrame) {
    /**
     * AX_S32 AX_VENC_JpegEncodeOneFrame(AX_JPEG_ENCODE_ONCE_PARAMS_T *pstJpegParam);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_JPEG_ENCODE_ONCE_PARAMS_T stJpegParam0, stJpegParam1;
    m_random.randomize(stJpegParam0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_JpegEncodeOneFrame, &stJpegParam0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_JpegEncodeOneFrame, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_JpegEncodeOneFrame, &stJpegParam1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(true, struct_equal<AX_JPEG_ENCODE_ONCE_PARAMS_T>(stJpegParam0, stJpegParam1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VENC_API::AX_VENC_JpegEncodeOneFrame, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VENC_API::AX_VENC_JpegEncodeOneFrame, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VENC_API::AX_VENC_JpegEncodeOneFrame, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}
