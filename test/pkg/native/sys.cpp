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
#include "package/device/native/module/sys_request.hpp"
#include "package/device/native/module/sys_response.hpp"
#include "package/host/native/module/sys_request.hpp"
#include "package/host/native/module/sys_response.hpp"
#include "ax_sys_api.h"

class SysPkgTest : public NativeTest<axcl::pkg::host::sys_request, axcl::pkg::device::sys_response, axcl::pkg::device::sys_request,
                                     axcl::pkg::host::sys_response> {
protected:
    SysPkgTest()
        : NativeTest<axcl::pkg::host::sys_request, axcl::pkg::device::sys_response, axcl::pkg::device::sys_request,
                     axcl::pkg::host::sys_response>(axcl::pkg::native::module::SYS) {};
};

TEST_F(SysPkgTest, AX_SYS_Init) {
    /**
     * AX_S32 AX_SYS_Init();
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_Init));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_Init, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_Init));
    CheckRequestPackageHead(msg);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_Init, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_Init, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_Init, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(SysPkgTest, AX_SYS_Deinit) {
    /**
     * AX_S32 AX_SYS_Deinit();
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_Deinit));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_Deinit, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_Deinit));
    CheckRequestPackageHead(msg);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_Deinit, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_Deinit, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_Deinit, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(SysPkgTest, AX_SYS_MemGetPartitionInfo) {
    /**
     * AX_S32 AX_SYS_MemGetPartitionInfo(AX_CMM_PARTITION_INFO_T *pCmmPartitionInfo);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_CMM_PARTITION_INFO_T CmmPartitionInf0, CmmPartitionInf1, CmmPartitionInf2;
    m_random.randomize(CmmPartitionInf0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_MemGetPartitionInfo, &CmmPartitionInf0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_MemGetPartitionInfo, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_MemGetPartitionInfo, &CmmPartitionInf1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(true, struct_equal<AX_CMM_PARTITION_INFO_T>(CmmPartitionInf0, CmmPartitionInf1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_MemGetPartitionInfo, m_req_status, &CmmPartitionInf1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_MemGetPartitionInfo, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_MemGetPartitionInfo, &m_ack_status, &CmmPartitionInf2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(true, struct_equal<AX_CMM_PARTITION_INFO_T>(CmmPartitionInf0, CmmPartitionInf2));
}

TEST_F(SysPkgTest, AX_SYS_MemQueryStatus) {
    /**
     * AX_SYS_MemQueryStatus(AX_CMM_STATUS_T *pCmmStatus);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_CMM_STATUS_T CmmStatus0, CmmStatus1, CmmStatus2;
    m_random.randomize(CmmStatus0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_MemQueryStatus, &CmmStatus0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_MemQueryStatus, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_MemQueryStatus, &CmmStatus1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(true, struct_equal<AX_CMM_STATUS_T>(CmmStatus0, CmmStatus1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_MemQueryStatus, m_req_status, &CmmStatus1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_MemQueryStatus, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_MemQueryStatus, &m_ack_status, &CmmStatus2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(true, struct_equal<AX_CMM_STATUS_T>(CmmStatus0, CmmStatus2));
}

TEST_F(SysPkgTest, AX_SYS_Link) {
    /**
     * AX_S32 AX_SYS_Link(const AX_MOD_INFO_T *pSrc, const AX_MOD_INFO_T *pDest);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_MOD_INFO_T Src0, Src1;
    m_random.randomize(Src0);

    AX_MOD_INFO_T Dest0, Dest1;
    m_random.randomize(Dest0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_Link, &Src0, &Dest0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_Link, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_Link, &Src1, &Dest1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(true, struct_equal<AX_MOD_INFO_T>(Src0, Src1));
    EXPECT_EQ(true, struct_equal<AX_MOD_INFO_T>(Dest0, Dest1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_Link, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_Link, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_Link, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(SysPkgTest, AX_SYS_UnLink) {
    /**
     * AX_S32 AX_SYS_UnLink(const AX_MOD_INFO_T *pSrc, const AX_MOD_INFO_T *pDest);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_MOD_INFO_T Src0, Src1;
    m_random.randomize(Src0);

    AX_MOD_INFO_T Dest0, Dest1;
    m_random.randomize(Dest0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_UnLink, &Src0, &Dest0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_UnLink, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_UnLink, &Src1, &Dest1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(true, struct_equal<AX_MOD_INFO_T>(Src0, Src1));
    EXPECT_EQ(true, struct_equal<AX_MOD_INFO_T>(Dest0, Dest1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_UnLink, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_UnLink, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_UnLink, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(SysPkgTest, AX_SYS_GetLinkByDest) {
    /**
     * AX_S32 AX_SYS_GetLinkByDest(const AX_MOD_INFO_T *pDest, AX_MOD_INFO_T *pSrc);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_MOD_INFO_T Dest0, Dest1;
    m_random.randomize(Dest0);

    AX_MOD_INFO_T Src0, Src1, Src2;
    m_random.randomize(Src0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_GetLinkByDest, &Dest0, &Src0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_GetLinkByDest, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_GetLinkByDest, &Dest1, &Src1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(true, struct_equal<AX_MOD_INFO_T>(Src0, Src1));
    EXPECT_EQ(true, struct_equal<AX_MOD_INFO_T>(Dest0, Dest1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_GetLinkByDest, m_req_status, &Src1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_GetLinkByDest, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_GetLinkByDest, &m_ack_status, &Src2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(true, struct_equal<AX_MOD_INFO_T>(Src0, Src2));
}

TEST_F(SysPkgTest, AX_SYS_GetLinkBySrc) {
    /**
     * AX_S32 AX_SYS_GetLinkBySrc(const AX_MOD_INFO_T *pSrc, AX_LINK_DEST_T *pLinkDest);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_MOD_INFO_T Src0, Src1;
    m_random.randomize(Src0);

    AX_LINK_DEST_T LinkDest0, LinkDest1, LinkDest2;
    m_random.randomize(LinkDest0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_GetLinkBySrc, &Src0, &LinkDest0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_GetLinkBySrc, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_GetLinkBySrc, &Src1, &LinkDest1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(true, struct_equal<AX_MOD_INFO_T>(Src0, Src1));
    EXPECT_EQ(true, struct_equal<AX_LINK_DEST_T>(LinkDest0, LinkDest1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_GetLinkBySrc, m_req_status, &LinkDest1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_GetLinkBySrc, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_GetLinkBySrc, &m_ack_status, &LinkDest2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(true, struct_equal<AX_LINK_DEST_T>(LinkDest0, LinkDest2));
}

TEST_F(SysPkgTest, AX_POOL_SetConfig) {
    /**
     * AX_S32 AX_POOL_SetConfig(const AX_POOL_FLOORPLAN_T *pPoolFloorPlan);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_POOL_FLOORPLAN_T PoolFloorPlan0, PoolFloorPlan1;
    m_random.randomize(PoolFloorPlan0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_POOL_SetConfig, &PoolFloorPlan0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_POOL_SetConfig, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_POOL_SetConfig, &PoolFloorPlan1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(true, struct_equal<AX_POOL_FLOORPLAN_T>(PoolFloorPlan0, PoolFloorPlan1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_POOL_SetConfig, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_POOL_SetConfig, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_POOL_SetConfig, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(SysPkgTest, AX_POOL_GetConfig) {
    /**
     * AX_S32 AX_POOL_GetConfig (AX_POOL_FLOORPLAN_T *pPoolFloorPlan);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_POOL_FLOORPLAN_T PoolFloorPlan0, PoolFloorPlan1, PoolFloorPlan2;
    m_random.randomize(PoolFloorPlan0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_POOL_GetConfig, &PoolFloorPlan0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_POOL_GetConfig, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_POOL_GetConfig, &PoolFloorPlan1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(true, struct_equal<AX_POOL_FLOORPLAN_T>(PoolFloorPlan0, PoolFloorPlan1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_POOL_GetConfig, m_req_status, &PoolFloorPlan1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_POOL_GetConfig, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_POOL_GetConfig, &m_ack_status, &PoolFloorPlan2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(true, struct_equal<AX_POOL_FLOORPLAN_T>(PoolFloorPlan0, PoolFloorPlan2));
}

TEST_F(SysPkgTest, AX_POOL_Init) {
    /**
     * AX_S32 AX_POOL_Init(AX_VOID);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_POOL_Init));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_POOL_Init, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_POOL_Init));
    CheckRequestPackageHead(msg);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_POOL_Init, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_POOL_Init, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_POOL_Init, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(SysPkgTest, AX_POOL_Exit) {
    /**
     * AX_S32 AX_POOL_Exit(AX_VOID);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_POOL_Exit));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_POOL_Exit, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_POOL_Exit));
    CheckRequestPackageHead(msg);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_POOL_Exit, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_POOL_Exit, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_POOL_Exit, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(SysPkgTest, AX_POOL_CreatePool) {
    /**
     * AX_POOL AX_POOL_CreatePool(AX_POOL_CONFIG_T *pPoolConfig);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_POOL_CONFIG_T PoolConfig0, PoolConfig1;
    m_random.randomize(PoolConfig0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_POOL_CreatePool, &PoolConfig0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_POOL_CreatePool, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_POOL_CreatePool, &PoolConfig1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(true, struct_equal<AX_POOL_CONFIG_T>(PoolConfig0, PoolConfig1));

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_POOL_CreatePool, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_POOL_CreatePool, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_POOL_CreatePool, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(SysPkgTest, AX_POOL_DestroyPool) {
    /**
     * AX_S32 AX_POOL_DestroyPool(AX_POOL PoolId);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_POOL PoolId0, PoolId1;
    m_random.randomize(PoolId0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_POOL_DestroyPool, PoolId0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_POOL_DestroyPool, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_POOL_DestroyPool, &PoolId1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(PoolId0, PoolId1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_POOL_DestroyPool, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_POOL_DestroyPool, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_POOL_DestroyPool, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(SysPkgTest, AX_POOL_ReleaseBlock) {
    /**
     * AX_S32 AX_POOL_ReleaseBlock(AX_BLK BlockId);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_BLK BlockId0, BlockId1;
    m_random.randomize(BlockId0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_POOL_ReleaseBlock, BlockId0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_POOL_ReleaseBlock, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_POOL_ReleaseBlock, &BlockId1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(BlockId0, BlockId1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_POOL_ReleaseBlock, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_POOL_ReleaseBlock, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_POOL_ReleaseBlock, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(SysPkgTest, AX_POOL_PhysAddr2Handle) {
    /**
     * AX_BLK AX_POOL_PhysAddr2Handle(AX_U64 PhysAddr);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_U64 PhysAddr0, PhysAddr1;
    m_random.randomize(PhysAddr0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_POOL_PhysAddr2Handle, PhysAddr0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_POOL_PhysAddr2Handle, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_POOL_PhysAddr2Handle, &PhysAddr1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(PhysAddr0, PhysAddr1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_POOL_PhysAddr2Handle, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_POOL_PhysAddr2Handle, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_POOL_PhysAddr2Handle, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(SysPkgTest, AX_POOL_Handle2PhysAddr) {
    /**
     * AX_U64 AX_POOL_Handle2PhysAddr(AX_BLK BlockId);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_BLK BlockId0, BlockId1;
    m_random.randomize(BlockId0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_POOL_Handle2PhysAddr, BlockId0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_POOL_Handle2PhysAddr, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_POOL_Handle2PhysAddr, &BlockId1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(BlockId0, BlockId1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_POOL_Handle2PhysAddr, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_POOL_Handle2PhysAddr, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_POOL_Handle2PhysAddr, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(SysPkgTest, AX_POOL_Handle2MetaPhysAddr) {
    /**
     * AX_U64 AX_POOL_Handle2MetaPhysAddr(AX_BLK BlockId);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_BLK BlockId0, BlockId1;
    m_random.randomize(BlockId0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_POOL_Handle2MetaPhysAddr, BlockId0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_POOL_Handle2MetaPhysAddr, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_POOL_Handle2MetaPhysAddr, &BlockId1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(BlockId0, BlockId1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_POOL_Handle2MetaPhysAddr, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_POOL_Handle2MetaPhysAddr, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_POOL_Handle2MetaPhysAddr, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(SysPkgTest, AX_POOL_Handle2PoolId) {
    /**
     * AX_POOL AX_POOL_Handle2PoolId(AX_BLK BlockId);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_BLK BlockId0, BlockId1;
    m_random.randomize(BlockId0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_POOL_Handle2PoolId, BlockId0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_POOL_Handle2PoolId, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_POOL_Handle2PoolId, &BlockId1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(BlockId0, BlockId1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_POOL_Handle2PoolId, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_POOL_Handle2PoolId, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_POOL_Handle2PoolId, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(SysPkgTest, AX_POOL_Handle2BlkSize) {
    /**
     * AX_U64 AX_POOL_Handle2BlkSize(AX_BLK BlockId);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_BLK BlockId0, BlockId1;
    m_random.randomize(BlockId0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_POOL_Handle2BlkSize, BlockId0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_POOL_Handle2BlkSize, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_POOL_Handle2BlkSize, &BlockId1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(BlockId0, BlockId1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_POOL_Handle2BlkSize, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_POOL_Handle2BlkSize, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_POOL_Handle2BlkSize, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(SysPkgTest, AX_POOL_MmapPool) {
    /**
     * AX_S32 AX_POOL_MmapPool(AX_POOL PoolId);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_POOL PoolId0, PoolId1;
    m_random.randomize(PoolId0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_POOL_MmapPool, PoolId0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_POOL_MmapPool, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_POOL_MmapPool, &PoolId1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(PoolId0, PoolId1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_POOL_MmapPool, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_POOL_MmapPool, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_POOL_MmapPool, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(SysPkgTest, AX_POOL_MunmapPool) {
    /**
     * AX_S32 AX_POOL_MunmapPool(AX_POOL PoolId);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_POOL PoolId0, PoolId1;
    m_random.randomize(PoolId0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_POOL_MunmapPool, PoolId0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_POOL_MunmapPool, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_POOL_MunmapPool, &PoolId1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(PoolId0, PoolId1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_POOL_MunmapPool, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_POOL_MunmapPool, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_POOL_MunmapPool, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(SysPkgTest, AX_POOL_GetBlockVirAddr) {
    /**
     * AX_VOID *AX_POOL_GetBlockVirAddr(AX_BLK BlockId);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_BLK BlockId0, BlockId1;
    m_random.randomize(BlockId0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_POOL_GetBlockVirAddr, BlockId0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_POOL_GetBlockVirAddr, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_POOL_GetBlockVirAddr, &BlockId1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(BlockId0, BlockId1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_POOL_GetBlockVirAddr, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_POOL_GetBlockVirAddr, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_POOL_GetBlockVirAddr, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(SysPkgTest, AX_POOL_GetMetaVirAddr) {
    /**
     * AX_VOID *AX_POOL_GetMetaVirAddr(AX_BLK BlockId);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_BLK BlockId0, BlockId1;
    m_random.randomize(BlockId0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_POOL_GetMetaVirAddr, BlockId0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_POOL_GetMetaVirAddr, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_POOL_GetMetaVirAddr, &BlockId1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(BlockId0, BlockId1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_POOL_GetMetaVirAddr, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_POOL_GetMetaVirAddr, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_POOL_GetMetaVirAddr, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(SysPkgTest, AX_POOL_IncreaseRefCnt) {
    /**
     * AX_S32 AX_POOL_IncreaseRefCnt(AX_BLK BlockId);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_BLK BlockId0, BlockId1;
    m_random.randomize(BlockId0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_POOL_IncreaseRefCnt, BlockId0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_POOL_IncreaseRefCnt, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_POOL_IncreaseRefCnt, &BlockId1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(BlockId0, BlockId1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_POOL_IncreaseRefCnt, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_POOL_IncreaseRefCnt, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_POOL_IncreaseRefCnt, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(SysPkgTest, AX_POOL_DecreaseRefCnt) {
    /**
     * AX_S32 AX_POOL_DecreaseRefCnt(AX_BLK BlockId);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_BLK BlockId0, BlockId1;
    m_random.randomize(BlockId0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_POOL_DecreaseRefCnt, BlockId0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_POOL_DecreaseRefCnt, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_POOL_DecreaseRefCnt, &BlockId1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(BlockId0, BlockId1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_POOL_DecreaseRefCnt, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_POOL_DecreaseRefCnt, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_POOL_DecreaseRefCnt, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(SysPkgTest, AX_SYS_GetCurPTS) {
    /**
     * AX_S32 AX_SYS_GetCurPTS(AX_U64 *pu64CurPTS);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_U64 u64CurPTS0, u64CurPTS1, u64CurPTS2;
    m_random.randomize(u64CurPTS0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_GetCurPTS, u64CurPTS0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_GetCurPTS, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_GetCurPTS, &u64CurPTS1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(u64CurPTS0, u64CurPTS1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_GetCurPTS, m_req_status, u64CurPTS1));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_GetCurPTS, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_GetCurPTS, &m_ack_status, &u64CurPTS2));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
    EXPECT_EQ(u64CurPTS0, u64CurPTS2);
}

TEST_F(SysPkgTest, AX_SYS_InitPTSBase) {
    /**
     * AX_S32 AX_SYS_InitPTSBase(AX_U64 u64PTSBase);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_U64 u64PTSBase0, u64PTSBase1;
    m_random.randomize(u64PTSBase0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_InitPTSBase, u64PTSBase0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_InitPTSBase, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_InitPTSBase, &u64PTSBase1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(u64PTSBase0, u64PTSBase1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_InitPTSBase, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_InitPTSBase, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_InitPTSBase, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(SysPkgTest, AX_SYS_SyncPTS) {
    /**
     * AX_S32 AX_SYS_SyncPTS(AX_U64 u64PTSBase);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_U64 u64PTSBase0, u64PTSBase1;
    m_random.randomize(u64PTSBase0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_SyncPTS, u64PTSBase0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_SyncPTS, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_SyncPTS, &u64PTSBase1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(u64PTSBase0, u64PTSBase1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_SyncPTS, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_SyncPTS, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_SyncPTS, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(SysPkgTest, AX_SYS_GetChipType) {
    /**
     * AX_CHIP_TYPE_E AX_SYS_GetChipType(AX_VOID);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_GetChipType));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_GetChipType, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_GetChipType));
    CheckRequestPackageHead(msg);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_GetChipType, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_GetChipType, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_GetChipType, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(SysPkgTest, AX_SYS_SetLogLevel) {
    /**
     * AX_S32 AX_SYS_SetLogLevel(AX_LOG_LEVEL_E target);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_LOG_LEVEL_E target0, target1;
    m_random.randomize(target0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_SetLogLevel, target0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_SetLogLevel, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_SetLogLevel, &target1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(target0, target1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_SetLogLevel, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_SetLogLevel, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_SetLogLevel, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(SysPkgTest, AX_SYS_SetLogTarget) {
    /**
     * AX_S32 AX_SYS_SetLogTarget(AX_LOG_TARGET_E target);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_LOG_TARGET_E target0, target1;
    m_random.randomize(target0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_SetLogTarget, target0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_SetLogTarget, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_SetLogTarget, &target1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(target0, target1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_SetLogTarget, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_SetLogTarget, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_SetLogTarget, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(SysPkgTest, AX_SYS_EnableTimestamp) {
    /**
     * AX_S32 AX_SYS_EnableTimestamp(AX_BOOL enable);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_BOOL enable0, enable1;
    m_random.randomize(enable0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_EnableTimestamp, enable0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_EnableTimestamp, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_EnableTimestamp, &enable1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(enable0, enable1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_EnableTimestamp, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_EnableTimestamp, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_EnableTimestamp, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(SysPkgTest, AX_SYS_Sleep) {
    /**
     * AX_S32 AX_SYS_Sleep(AX_VOID);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_Sleep));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_Sleep, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_Sleep));
    CheckRequestPackageHead(msg);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_Sleep, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_Sleep, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_Sleep, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(SysPkgTest, AX_SYS_WakeLock) {
    /**
     * AX_S32 AX_SYS_WakeLock(const AX_MOD_ID_E ModId);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_MOD_ID_E ModId0, ModId1;
    m_random.randomize(ModId0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_WakeLock, ModId0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_WakeLock, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_WakeLock, &ModId1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(ModId0, ModId1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_WakeLock, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_WakeLock, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_WakeLock, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(SysPkgTest, AX_SYS_WakeUnlock) {
    /**
     * AX_S32 AX_SYS_WakeUnlock(const AX_MOD_ID_E ModId);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_MOD_ID_E ModId0, ModId1;
    m_random.randomize(ModId0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_WakeUnlock, ModId0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_WakeUnlock, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_WakeUnlock, &ModId1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(ModId0, ModId1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_WakeUnlock, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_WakeUnlock, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_WakeUnlock, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}

TEST_F(SysPkgTest, AX_SYS_UnregisterEventCb) {
    /**
     * AX_S32 AX_SYS_UnregisterEventCb(const AX_MOD_ID_E ModId);
     */
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    AX_MOD_ID_E ModId0, ModId1;
    m_random.randomize(ModId0);

    EXPECT_EQ(true, m_req_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_UnregisterEventCb, ModId0));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_UnregisterEventCb, m_req_decode.get_api());
    EXPECT_EQ(true, m_req_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_UnregisterEventCb, &ModId1));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(ModId0, ModId1);

    /* device -> host */
    EXPECT_EQ(true, m_ack_encode.pack(axcl::pkg::native::SYS_API::AX_SYS_UnregisterEventCb, m_req_status));
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    EXPECT_EQ(axcl::pkg::native::SYS_API::AX_SYS_UnregisterEventCb, m_ack_decode.get_api());
    EXPECT_EQ(true, m_ack_decode.unpack(axcl::pkg::native::SYS_API::AX_SYS_UnregisterEventCb, &m_ack_status));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(m_req_status, m_ack_status);
}