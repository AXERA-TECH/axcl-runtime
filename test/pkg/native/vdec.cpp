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
#include "package/device/native/module/vdec_request.hpp"
#include "package/device/native/module/vdec_response.hpp"
#include "package/host/native/module/vdec_request.hpp"
#include "package/host/native/module/vdec_response.hpp"
#include "ax_vdec_type.h"

class VdecPkgTest : public NativeTest<axcl::pkg::host::vdec_request, axcl::pkg::device::vdec_response, axcl::pkg::device::vdec_request,
                                      axcl::pkg::host::vdec_response> {
protected:
    VdecPkgTest()
        : NativeTest<axcl::pkg::host::vdec_request, axcl::pkg::device::vdec_response, axcl::pkg::device::vdec_request,
                     axcl::pkg::host::vdec_response>(axcl::pkg::native::module::VDEC) {};
};

TEST_F(VdecPkgTest, AX_VDEC_Init) {
    /**
     * AX_S32 AX_VDEC_Init (const AX_VDEC_MOD_ATTR_T *pstModAttr);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_VDEC_MOD_ATTR_T mod0, mod1;
    m_random.randomize(mod0);
    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_Init, &mod0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_Init, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_Init, &mod1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(true, struct_equal<AX_VDEC_MOD_ATTR_T>(mod0, mod1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_Init, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_Init, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_Init, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VdecPkgTest, AX_VDEC_Deinit) {
    /**
     * AX_S32 AX_VDEC_Deinit (AX_VOID);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_Deinit));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_Deinit, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_Deinit));
    CheckRequestPackageHead(msg);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_Deinit, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_Deinit, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_Deinit, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VdecPkgTest, AX_VDEC_CreateGrp) {
    /**
     * AX_S32 AX_VDEC_CreateGrp (AX_VDEC_GRP VdGrp, const AX_VDEC_GRP_ATTR_T *pstGrpAttr);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_VDEC_GRP VdGrp0, VdGrp1;
    m_random.randomize(VdGrp0);

    AX_VDEC_GRP_ATTR_T stGrpAttr0, stGrpAttr1;
    m_random.randomize(stGrpAttr0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_CreateGrp, VdGrp0, &stGrpAttr0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_CreateGrp, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_CreateGrp, &VdGrp1, &stGrpAttr1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VdGrp0, VdGrp1);
    EXPECT_EQ(true, struct_equal<AX_VDEC_GRP_ATTR_T>(stGrpAttr0, stGrpAttr1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_CreateGrp, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_CreateGrp, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_CreateGrp, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VdecPkgTest, AX_VDEC_CreateGrpEx) {
    /**
     * AX_S32 AX_VDEC_CreateGrpEx(AX_VDEC_GRP *VdGrp, const AX_VDEC_GRP_ATTR_T *pstGrpAttr);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_VDEC_GRP VdGrp0, VdGrp1, VdGrp2;
    m_random.randomize(VdGrp0);

    AX_VDEC_GRP_ATTR_T stGrpAttr0, stGrpAttr1;
    m_random.randomize(stGrpAttr0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_CreateGrpEx, VdGrp0, &stGrpAttr0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_CreateGrpEx, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_CreateGrpEx, &VdGrp1, &stGrpAttr1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VdGrp0, VdGrp1);
    EXPECT_EQ(true, struct_equal<AX_VDEC_GRP_ATTR_T>(stGrpAttr0, stGrpAttr1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_CreateGrpEx, m_req_status, VdGrp1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_CreateGrpEx, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_CreateGrpEx, &m_ack_status, &VdGrp2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(VdGrp0, VdGrp2);
}

TEST_F(VdecPkgTest, AX_VDEC_DestroyGrp) {
    /**
     * AX_S32 AX_VDEC_DestroyGrp (AX_VDEC_GRP VdGrp);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_VDEC_GRP VdGrp0, VdGrp1;
    m_random.randomize(VdGrp0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_DestroyGrp, VdGrp0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_DestroyGrp, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_DestroyGrp, &VdGrp1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VdGrp0, VdGrp1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_DestroyGrp, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_DestroyGrp, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_DestroyGrp, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VdecPkgTest, AX_VDEC_GetGrpAttr) {
    /**
     * AX_S32 AX_VDEC_GetGrpAttr (AX_VDEC_GRP VdGrp, AX_VDEC_GRP_ATTR_T *pstGrpAttr);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_VDEC_GRP VdGrp0, VdGrp1;
    m_random.randomize(VdGrp0);

    AX_VDEC_GRP_ATTR_T stGrpAttr0, stGrpAttr1, stGrpAttr2;
    m_random.randomize(stGrpAttr0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_GetGrpAttr, VdGrp0, &stGrpAttr0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_GetGrpAttr, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_GetGrpAttr, &VdGrp1, &stGrpAttr1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VdGrp0, VdGrp1);
    EXPECT_EQ(true, struct_equal<AX_VDEC_GRP_ATTR_T>(stGrpAttr0, stGrpAttr1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_GetGrpAttr, m_req_status, &stGrpAttr1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_GetGrpAttr, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_GetGrpAttr, &m_ack_status, &stGrpAttr2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(true, struct_equal<AX_VDEC_GRP_ATTR_T>(stGrpAttr0, stGrpAttr2));
}

TEST_F(VdecPkgTest, AX_VDEC_SetGrpAttr) {
    /**
     * AX_S32 AX_VDEC_SetGrpAttr (AX_VDEC_GRP VdGrp, const AX_VDEC_GRP_ATTR_T *pstGrpAttr);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_VDEC_GRP VdGrp0, VdGrp1;
    m_random.randomize(VdGrp0);

    AX_VDEC_GRP_ATTR_T stGrpAttr0, stGrpAttr1;
    m_random.randomize(stGrpAttr0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_SetGrpAttr, VdGrp0, &stGrpAttr0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_SetGrpAttr, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_SetGrpAttr, &VdGrp1, &stGrpAttr1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VdGrp0, VdGrp1);
    EXPECT_EQ(true, struct_equal<AX_VDEC_GRP_ATTR_T>(stGrpAttr0, stGrpAttr1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_SetGrpAttr, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_SetGrpAttr, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_SetGrpAttr, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VdecPkgTest, AX_VDEC_StartRecvStream) {
    /**
     * AX_S32 AX_VDEC_StartRecvStream (AX_VDEC_GRP VdGrp, const AX_VDEC_RECV_PIC_PARAM_T *pstRecvParam);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_VDEC_GRP VdGrp0, VdGrp1;
    m_random.randomize(VdGrp0);

    AX_VDEC_RECV_PIC_PARAM_T stRecvParam0, stRecvParam1;
    m_random.randomize(stRecvParam0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_StartRecvStream, VdGrp0, &stRecvParam0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_StartRecvStream, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_StartRecvStream, &VdGrp1, &stRecvParam1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VdGrp0, VdGrp1);
    EXPECT_EQ(true, struct_equal<AX_VDEC_RECV_PIC_PARAM_T>(stRecvParam0, stRecvParam1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_StartRecvStream, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_StartRecvStream, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_StartRecvStream, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VdecPkgTest, AX_VDEC_StopRecvStream) {
    /**
     * AX_S32 AX_VDEC_StopRecvStream (AX_VDEC_GRP VdGrp);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_VDEC_GRP VdGrp0, VdGrp1;
    m_random.randomize(VdGrp0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_StopRecvStream, VdGrp0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_StopRecvStream, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_StopRecvStream, &VdGrp1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VdGrp0, VdGrp1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_StopRecvStream, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_StopRecvStream, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_StopRecvStream, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VdecPkgTest, AX_VDEC_QueryStatus) {
    /**
     * AX_S32 AX_VDEC_QueryStatus (AX_VDEC_GRP VdGrp, AX_VDEC_GRP_STATUS_T *pstGrpStatus);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_VDEC_GRP VdGrp0, VdGrp1;
    m_random.randomize(VdGrp0);

    AX_VDEC_GRP_STATUS_T stGrpStatus0, stGrpStatus1, stGrpStatus2;
    m_random.randomize(stGrpStatus0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_QueryStatus, VdGrp0, &stGrpStatus0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_QueryStatus, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_QueryStatus, &VdGrp1, &stGrpStatus1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VdGrp0, VdGrp1);
    EXPECT_EQ(true, struct_equal<AX_VDEC_GRP_STATUS_T>(stGrpStatus0, stGrpStatus1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_QueryStatus, m_req_status, &stGrpStatus1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_QueryStatus, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_QueryStatus, &m_ack_status, &stGrpStatus2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(true, struct_equal<AX_VDEC_GRP_STATUS_T>(stGrpStatus0, stGrpStatus2));
}

TEST_F(VdecPkgTest, AX_VDEC_ResetGrp) {
    /**
     * AX_S32 AX_VDEC_ResetGrp (AX_VDEC_GRP VdGrp);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_VDEC_GRP VdGrp0, VdGrp1;
    m_random.randomize(VdGrp0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_ResetGrp, VdGrp0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_ResetGrp, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_ResetGrp, &VdGrp1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VdGrp0, VdGrp1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_ResetGrp, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_ResetGrp, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_ResetGrp, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VdecPkgTest, AX_VDEC_GetGrpParam) {
    /**
     * AX_S32 AX_VDEC_GetGrpParam (AX_VDEC_GRP VdGrp, AX_VDEC_GRP_PARAM_T* pstGrpParam);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_VDEC_GRP VdGrp0, VdGrp1;
    m_random.randomize(VdGrp0);

    AX_VDEC_GRP_PARAM_T stGrpParam0, stGrpParam1, stGrpParam2;
    m_random.randomize(stGrpParam0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_GetGrpParam, VdGrp0, &stGrpParam0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_GetGrpParam, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_GetGrpParam, &VdGrp1, &stGrpParam1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VdGrp0, VdGrp1);
    EXPECT_EQ(true, struct_equal<AX_VDEC_GRP_PARAM_T>(stGrpParam0, stGrpParam1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_GetGrpParam, m_req_status, &stGrpParam1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_GetGrpParam, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_GetGrpParam, &m_ack_status, &stGrpParam2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(true, struct_equal<AX_VDEC_GRP_PARAM_T>(stGrpParam0, stGrpParam2));
}

TEST_F(VdecPkgTest, AX_VDEC_SetGrpParam) {
    /**
     * AX_S32 AX_VDEC_SetGrpParam (AX_VDEC_GRP VdGrp, const AX_VDEC_GRP_PARAM_T* pstGrpParam);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_VDEC_GRP VdGrp0, VdGrp1;
    m_random.randomize(VdGrp0);

    AX_VDEC_GRP_PARAM_T stGrpParam0, stGrpParam1;
    m_random.randomize(stGrpParam0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_SetGrpParam, VdGrp0, &stGrpParam0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_SetGrpParam, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_SetGrpParam, &VdGrp1, &stGrpParam1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VdGrp0, VdGrp1);
    EXPECT_EQ(true, struct_equal<AX_VDEC_GRP_PARAM_T>(stGrpParam0, stGrpParam1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_SetGrpParam, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_SetGrpParam, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_SetGrpParam, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VdecPkgTest, AX_VDEC_SelectGrp) {
    /**
     * AX_S32 AX_VDEC_SelectGrp(AX_VDEC_GRP_SET_INFO_T *pstGrpSet, AX_S32 s32MilliSec);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_VDEC_GRP_SET_INFO_T stGrpSet0, stGrpSet1, stGrpSet2;
    m_random.randomize(stGrpSet0);

    AX_S32 s32MilliSec0, s32MilliSec1;
    m_random.randomize(s32MilliSec0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_SelectGrp, &stGrpSet0, s32MilliSec0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_SelectGrp, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_SelectGrp, &stGrpSet1, &s32MilliSec1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(s32MilliSec0, s32MilliSec1);
    EXPECT_EQ(true, struct_equal<AX_VDEC_GRP_SET_INFO_T>(stGrpSet0, stGrpSet1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_SelectGrp, m_req_status, &stGrpSet1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_SelectGrp, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_SelectGrp, &m_ack_status, &stGrpSet2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(true, struct_equal<AX_VDEC_GRP_SET_INFO_T>(stGrpSet0, stGrpSet2));
}

TEST_F(VdecPkgTest, AX_VDEC_SendStream) {
    /**
     * AX_S32 AX_VDEC_SendStream (AX_VDEC_GRP VdGrp, const AX_VDEC_STREAM_T *pstStream, AX_S32 s32MilliSec);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_VDEC_GRP VdGrp0, VdGrp1;
    m_random.randomize(VdGrp0);

    AX_VDEC_STREAM_T stStream0, stStream1;
    m_random.randomize(stStream0);

    constexpr size_t NALU_SIZE = 1024 * 1024;
    uint8_t *nalu = (uint8_t *)malloc(NALU_SIZE);
    m_random.randomize(nalu, NALU_SIZE);
    axcl::pkg::uint8_array array1, array2;
    array1.data = nalu;
    array1.size = NALU_SIZE;

    AX_S32 s32MilliSec0, s32MilliSec1;
    m_random.randomize(s32MilliSec0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_SendStream, VdGrp0, &stStream0, s32MilliSec0, array1));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_SendStream, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_SendStream, &VdGrp1, &stStream1, &s32MilliSec1, &array2));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VdGrp0, VdGrp1);
    EXPECT_EQ(true, struct_equal<AX_VDEC_STREAM_T>(stStream0, stStream1));
    EXPECT_EQ(s32MilliSec0, s32MilliSec1);

    EXPECT_EQ(array1.size, NALU_SIZE);
    EXPECT_EQ(array2.size, NALU_SIZE);
    EXPECT_EQ(0, memcmp(array1.data, nalu, NALU_SIZE));
    EXPECT_EQ(0, memcmp(array2.data, nalu, NALU_SIZE));

    // print_array(array1.data, array1.size);
    // print_array(array2.data, array2.size);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_SendStream, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_SendStream, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_SendStream, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VdecPkgTest, AX_VDEC_GetChnFrame) {
    /**
     * AX_S32 AX_VDEC_GetChnFrame (AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn, AX_VIDEO_FRAME_INFO_T *pstFrameInfo, AX_S32 s32MilliSec);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_VDEC_GRP VdGrp0, VdGrp1;
    m_random.randomize(VdGrp0);

    AX_VDEC_CHN VdChn0, VdChn1;
    m_random.randomize(VdChn0);

    AX_VIDEO_FRAME_INFO_T stFrameInfo0, stFrameInfo1, stFrameInfo2;
    m_random.randomize(stFrameInfo0);

    AX_S32 s32MilliSec0, s32MilliSec1;
    m_random.randomize(s32MilliSec0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_GetChnFrame, VdGrp0, VdChn0, &stFrameInfo0, s32MilliSec0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_GetChnFrame, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_GetChnFrame, &VdGrp1, &VdChn1, &stFrameInfo1, &s32MilliSec1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VdGrp0, VdGrp1);
    EXPECT_EQ(VdChn0, VdChn1);
    EXPECT_EQ(true, struct_equal<AX_VIDEO_FRAME_INFO_T>(stFrameInfo0, stFrameInfo1));
    EXPECT_EQ(s32MilliSec0, s32MilliSec1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_GetChnFrame, m_req_status, &stFrameInfo1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_GetChnFrame, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_GetChnFrame, &m_ack_status, &stFrameInfo2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(true, struct_equal<AX_VIDEO_FRAME_INFO_T>(stFrameInfo0, stFrameInfo2));

    // std::cout << "req frame u64PhyAddr: " << stFrameInfo0.stVFrame.u64PhyAddr[0] <<  ", u64VirAddr: " <<
    // stFrameInfo0.stVFrame.u64VirAddr[0] << std::endl; std::cout << "ack frame u64PhyAddr: " << stFrameInfo2.stVFrame.u64PhyAddr[0] <<  ",
    // u64VirAddr: " << stFrameInfo2.stVFrame.u64VirAddr[0] << std::endl;
}

TEST_F(VdecPkgTest, AX_VDEC_ReleaseChnFrame) {
    /**
     * AX_S32 AX_VDEC_ReleaseChnFrame (AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn, const AX_VIDEO_FRAME_INFO_T *pstFrameInfo);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_VDEC_GRP VdGrp0, VdGrp1;
    m_random.randomize(VdGrp0);

    AX_VDEC_CHN VdChn0, VdChn1;
    m_random.randomize(VdChn0);

    AX_VIDEO_FRAME_INFO_T stFrameInfo0, stFrameInfo1;
    m_random.randomize(stFrameInfo0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_ReleaseChnFrame, VdGrp0, VdChn0, &stFrameInfo0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_ReleaseChnFrame, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_ReleaseChnFrame, &VdGrp1, &VdChn1, &stFrameInfo1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VdGrp0, VdGrp1);
    EXPECT_EQ(VdChn0, VdChn1);
    EXPECT_EQ(true, struct_equal<AX_VIDEO_FRAME_INFO_T>(stFrameInfo0, stFrameInfo1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_ReleaseChnFrame, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_ReleaseChnFrame, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_ReleaseChnFrame, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VdecPkgTest, AX_VDEC_GetUserData) {
    /**
     * AX_S32 AX_VDEC_GetUserData (AX_VDEC_GRP VdGrp, AX_VDEC_USERDATA_T *pstUserData);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_VDEC_GRP VdGrp0, VdGrp1;
    m_random.randomize(VdGrp0);

    AX_VDEC_USERDATA_T stUserData0, stUserData1, stUserData2;
    m_random.randomize(stUserData0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_GetUserData, VdGrp0, &stUserData0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_GetUserData, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_GetUserData, &VdGrp1, &stUserData1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VdGrp0, VdGrp1);
    EXPECT_EQ(true, struct_equal<AX_VDEC_USERDATA_T>(stUserData0, stUserData1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_GetUserData, m_req_status, &stUserData1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_GetUserData, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_GetUserData, &m_ack_status, &stUserData2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(true, struct_equal<AX_VDEC_USERDATA_T>(stUserData0, stUserData2));
}

TEST_F(VdecPkgTest, AX_VDEC_ReleaseUserData) {
    /**
     * AX_S32 AX_VDEC_ReleaseUserData (AX_VDEC_GRP VdGrp, const AX_VDEC_USERDATA_T *pstUserData);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_VDEC_GRP VdGrp0, VdGrp1;
    m_random.randomize(VdGrp0);

    AX_VDEC_USERDATA_T stUserData0, stUserData1;
    m_random.randomize(stUserData0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_ReleaseUserData, VdGrp0, &stUserData0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_ReleaseUserData, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_ReleaseUserData, &VdGrp1, &stUserData1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VdGrp0, VdGrp1);
    EXPECT_EQ(true, struct_equal<AX_VDEC_USERDATA_T>(stUserData0, stUserData1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_ReleaseUserData, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_ReleaseUserData, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_ReleaseUserData, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VdecPkgTest, AX_VDEC_SetUserPic) {
    /**
     * AX_S32 AX_VDEC_SetUserPic (AX_VDEC_GRP VdGrp, const AX_VDEC_USRPIC_T *pstUsrPic);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_VDEC_GRP VdGrp0, VdGrp1;
    m_random.randomize(VdGrp0);

    AX_VDEC_USRPIC_T stUsrPic0, stUsrPic1;
    m_random.randomize(stUsrPic0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_SetUserPic, VdGrp0, &stUsrPic0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_SetUserPic, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_SetUserPic, &VdGrp1, &stUsrPic1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VdGrp0, VdGrp1);
    EXPECT_EQ(true, struct_equal<AX_VDEC_USRPIC_T>(stUsrPic0, stUsrPic1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_SetUserPic, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_SetUserPic, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_SetUserPic, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VdecPkgTest, AX_VDEC_EnableUserPic) {
    /**
     * AX_S32 AX_VDEC_EnableUserPic (AX_VDEC_GRP VdGrp);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_VDEC_GRP VdGrp0, VdGrp1;
    m_random.randomize(VdGrp0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_EnableUserPic, VdGrp0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_EnableUserPic, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_EnableUserPic, &VdGrp1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VdGrp0, VdGrp1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_EnableUserPic, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_EnableUserPic, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_EnableUserPic, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VdecPkgTest, AX_VDEC_DisableUserPic) {
    /**
     * AX_S32 AX_VDEC_DisableUserPic (AX_VDEC_GRP VdGrp);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_VDEC_GRP VdGrp0, VdGrp1;
    m_random.randomize(VdGrp0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_DisableUserPic, VdGrp0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_DisableUserPic, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_DisableUserPic, &VdGrp1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VdGrp0, VdGrp1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_DisableUserPic, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_DisableUserPic, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_DisableUserPic, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VdecPkgTest, AX_VDEC_SetDisplayMode) {
    /**
     * AX_S32 AX_VDEC_SetDisplayMode(AX_VDEC_GRP VdGrp, AX_VDEC_DISPLAY_MODE_E enDisplayMode);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_VDEC_GRP VdGrp0, VdGrp1;
    m_random.randomize(VdGrp0);

    AX_VDEC_DISPLAY_MODE_E enDisplayMode0, enDisplayMode1;
    enDisplayMode0 = AX_VDEC_DISPLAY_MODE_PLAYBACK;
    enDisplayMode1 = AX_VDEC_DISPLAY_MODE_BUTT;

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_SetDisplayMode, VdGrp0, enDisplayMode0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_SetDisplayMode, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_SetDisplayMode, &VdGrp1, &enDisplayMode1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VdGrp0, VdGrp1);
    EXPECT_EQ(enDisplayMode0, enDisplayMode1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_SetDisplayMode, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_SetDisplayMode, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_SetDisplayMode, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VdecPkgTest, AX_VDEC_GetDisplayMode) {
    /**
     * AX_S32 AX_VDEC_GetDisplayMode(AX_VDEC_GRP VdGrp, AX_VDEC_DISPLAY_MODE_E *penDisplayMode);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_VDEC_GRP VdGrp0, VdGrp1;
    m_random.randomize(VdGrp0);

    AX_VDEC_DISPLAY_MODE_E enDisplayMode0, enDisplayMode1, enDisplayMode2;
    enDisplayMode0 = AX_VDEC_DISPLAY_MODE_PLAYBACK;
    enDisplayMode1 = AX_VDEC_DISPLAY_MODE_BUTT;
    enDisplayMode2 = AX_VDEC_DISPLAY_MODE_PREVIEW;

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_GetDisplayMode, VdGrp0, enDisplayMode0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_GetDisplayMode, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_GetDisplayMode, &VdGrp1, &enDisplayMode1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VdGrp0, VdGrp1);
    EXPECT_EQ(enDisplayMode0, enDisplayMode1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_GetDisplayMode, m_req_status, enDisplayMode1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_GetDisplayMode, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_GetDisplayMode, &m_ack_status, &enDisplayMode2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(enDisplayMode0, enDisplayMode2);
}

TEST_F(VdecPkgTest, AX_VDEC_AttachPool) {
    /**
     * AX_S32 AX_VDEC_AttachPool (AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn, AX_POOL PoolId);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_VDEC_GRP VdGrp0, VdGrp1;
    m_random.randomize(VdGrp0);

    AX_VDEC_CHN VdChn0, VdChn1;
    m_random.randomize(VdChn0);

    AX_POOL PoolId0, PoolId1;
    m_random.randomize(PoolId0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_AttachPool, VdGrp0, VdChn0, PoolId0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_AttachPool, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_AttachPool, &VdGrp1, &VdChn1, &PoolId1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VdGrp0, VdGrp1);
    EXPECT_EQ(VdChn0, VdChn1);
    EXPECT_EQ(PoolId0, PoolId1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_AttachPool, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_AttachPool, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_AttachPool, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VdecPkgTest, AX_VDEC_DetachPool) {
    /**
     * AX_S32 AX_VDEC_DetachPool (AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_VDEC_GRP VdGrp0, VdGrp1;
    m_random.randomize(VdGrp0);

    AX_VDEC_CHN VdChn0, VdChn1;
    m_random.randomize(VdChn0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_DetachPool, VdGrp0, VdChn0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_DetachPool, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_DetachPool, &VdGrp1, &VdChn1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VdGrp0, VdGrp1);
    EXPECT_EQ(VdChn0, VdChn1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_DetachPool, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_DetachPool, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_DetachPool, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VdecPkgTest, AX_VDEC_EnableChn) {
    /**
     * AX_S32 AX_VDEC_EnableChn (AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_VDEC_GRP VdGrp0, VdGrp1;
    m_random.randomize(VdGrp0);

    AX_VDEC_CHN VdChn0, VdChn1;
    m_random.randomize(VdChn0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_EnableChn, VdGrp0, VdChn0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_EnableChn, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_EnableChn, &VdGrp1, &VdChn1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VdGrp0, VdGrp1);
    EXPECT_EQ(VdChn0, VdChn1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_EnableChn, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_EnableChn, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_EnableChn, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VdecPkgTest, AX_VDEC_DisableChn) {
    /**
     * AX_S32 AX_VDEC_DisableChn (AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_VDEC_GRP VdGrp0, VdGrp1;
    m_random.randomize(VdGrp0);

    AX_VDEC_CHN VdChn0, VdChn1;
    m_random.randomize(VdChn0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_DisableChn, VdGrp0, VdChn0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_DisableChn, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_DisableChn, &VdGrp1, &VdChn1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VdGrp0, VdGrp1);
    EXPECT_EQ(VdChn0, VdChn1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_DisableChn, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_DisableChn, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_DisableChn, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VdecPkgTest, AX_VDEC_SetChnAttr) {
    /**
     * AX_S32 AX_VDEC_SetChnAttr (AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn, const AX_VDEC_CHN_ATTR_T *pstVdChnAttr);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_VDEC_GRP VdGrp0, VdGrp1;
    m_random.randomize(VdGrp0);

    AX_VDEC_CHN VdChn0, VdChn1;
    m_random.randomize(VdChn0);

    AX_VDEC_CHN_ATTR_T stVdChnAttr0, stVdChnAttr1;
    m_random.randomize(stVdChnAttr0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_SetChnAttr, VdGrp0, VdChn0, &stVdChnAttr0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_SetChnAttr, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_SetChnAttr, &VdGrp1, &VdChn1, &stVdChnAttr1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VdGrp0, VdGrp1);
    EXPECT_EQ(VdChn0, VdChn1);
    EXPECT_EQ(true, struct_equal<AX_VDEC_CHN_ATTR_T>(stVdChnAttr0, stVdChnAttr1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_SetChnAttr, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_SetChnAttr, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_SetChnAttr, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(VdecPkgTest, AX_VDEC_GetChnAttr) {
    /**
     * AX_S32 AX_VDEC_GetChnAttr (AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn, AX_VDEC_CHN_ATTR_T *pstVdChnAttr);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_VDEC_GRP VdGrp0, VdGrp1;
    m_random.randomize(VdGrp0);

    AX_VDEC_CHN VdChn0, VdChn1;
    m_random.randomize(VdChn0);

    AX_VDEC_CHN_ATTR_T stVdChnAttr0, stVdChnAttr1, stVdChnAttr2;
    m_random.randomize(stVdChnAttr0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_GetChnAttr, VdGrp0, VdChn0, &stVdChnAttr0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_GetChnAttr, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_GetChnAttr, &VdGrp1, &VdChn1, &stVdChnAttr1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VdGrp0, VdGrp1);
    EXPECT_EQ(VdChn0, VdChn1);
    EXPECT_EQ(true, struct_equal<AX_VDEC_CHN_ATTR_T>(stVdChnAttr0, stVdChnAttr1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_GetChnAttr, m_req_status, &stVdChnAttr1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_GetChnAttr, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_GetChnAttr, &m_ack_status, &stVdChnAttr2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(true, struct_equal<AX_VDEC_CHN_ATTR_T>(stVdChnAttr0, stVdChnAttr2));
}

TEST_F(VdecPkgTest, AX_VDEC_JpegDecodeOneFrame) {
    /**
     * AX_S32 AX_VDEC_JpegDecodeOneFrame(AX_VDEC_DEC_ONE_FRM_T *pstParam);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_VDEC_DEC_ONE_FRM_T stParam0, stParam1, stParam2;
    m_random.randomize(stParam0);

    constexpr size_t JPEG_SIZE = 512 * 1024;
    uint8_t *jpg = (uint8_t *)malloc(JPEG_SIZE);
    m_random.randomize(jpg, JPEG_SIZE);
    axcl::pkg::uint8_array array0, array1;
    array0.data = jpg;
    array0.size = JPEG_SIZE;

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_JpegDecodeOneFrame, &stParam0, array0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_JpegDecodeOneFrame, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_JpegDecodeOneFrame, &stParam1, &array1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(true, struct_equal<AX_VDEC_DEC_ONE_FRM_T>(stParam0, stParam1));

    EXPECT_EQ(array0.size, JPEG_SIZE);
    EXPECT_EQ(array1.size, JPEG_SIZE);
    EXPECT_EQ(0, memcmp(array0.data, jpg, JPEG_SIZE));
    EXPECT_EQ(0, memcmp(array1.data, jpg, JPEG_SIZE));

    // print_array(array0.data, array0.size);
    // print_array(array1.data, array1.size);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_JpegDecodeOneFrame, m_req_status, &stParam1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_JpegDecodeOneFrame, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_JpegDecodeOneFrame, &m_ack_status, &stParam2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(true, struct_equal<AX_VDEC_DEC_ONE_FRM_T>(stParam0, stParam2));
}

TEST_F(VdecPkgTest, AX_VDEC_GetStreamBufInfo) {
    /**
     * AX_S32 AX_VDEC_GetStreamBufInfo(AX_VDEC_GRP VdGrp, AX_VDEC_STREAM_BUF_INFO_T *pstStreamBufInfo);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_VDEC_GRP VdGrp0, VdGrp1;
    m_random.randomize(VdGrp0);

    AX_VDEC_STREAM_BUF_INFO_T stStreamBufInfo0, stStreamBufInfo1, stStreamBufInfo2;
    m_random.randomize(stStreamBufInfo0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_GetStreamBufInfo, VdGrp0, &stStreamBufInfo0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_GetStreamBufInfo, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_GetStreamBufInfo, &VdGrp1, &stStreamBufInfo1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VdGrp0, VdGrp1);
    EXPECT_EQ(true, struct_equal<AX_VDEC_STREAM_BUF_INFO_T>(stStreamBufInfo0, stStreamBufInfo1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_GetStreamBufInfo, m_req_status, &stStreamBufInfo1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_GetStreamBufInfo, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_GetStreamBufInfo, &m_ack_status, &stStreamBufInfo2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(true, struct_equal<AX_VDEC_STREAM_BUF_INFO_T>(stStreamBufInfo0, stStreamBufInfo2));
}

TEST_F(VdecPkgTest, AX_VDEC_GetVuiParam) {
    /**
     * AX_S32 AX_VDEC_GetVuiParam(AX_VDEC_GRP VdGrp, AX_VDEC_VUI_PARAM_T *pstVuiParam);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_VDEC_GRP VdGrp0, VdGrp1;
    m_random.randomize(VdGrp0);

    AX_VDEC_VUI_PARAM_T stVuiParam0, stVuiParam1, stVuiParam2;
    m_random.randomize(stVuiParam0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_GetVuiParam, VdGrp0, &stVuiParam0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_GetVuiParam, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_GetVuiParam, &VdGrp1, &stVuiParam1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(VdGrp0, VdGrp1);
    EXPECT_EQ(true, struct_equal<AX_VDEC_VUI_PARAM_T>(stVuiParam0, stVuiParam1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::VDEC_API::AX_VDEC_GetVuiParam, m_req_status, &stVuiParam1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::VDEC_API::AX_VDEC_GetVuiParam, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::VDEC_API::AX_VDEC_GetVuiParam, &m_ack_status, &stVuiParam2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(true, struct_equal<AX_VDEC_VUI_PARAM_T>(stVuiParam0, stVuiParam2));
}