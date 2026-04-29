/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include <memory>
#include "package/device/runtime/p2p/p2p_request.hpp"
#include "package/device/runtime/p2p/p2p_response.hpp"
#include "package/host/runtime/p2p/p2p_request.hpp"
#include "package/host/runtime/p2p/p2p_response.hpp"
#include "runtime.hpp"
#

class P2pPkgTest : public RuntimeTest<axcl::pkg::host::p2p_request, axcl::pkg::device::p2p_response, axcl::pkg::device::p2p_request,
                                      axcl::pkg::host::p2p_response> {
protected:
    P2pPkgTest()
        : RuntimeTest<axcl::pkg::host::p2p_request, axcl::pkg::device::p2p_response, axcl::pkg::device::p2p_request,
                      axcl::pkg::host::p2p_response>(axcl::pkg::runtime::module::SYSTEM){};
};

TEST_F(P2pPkgTest, p2pSetupConfig) {
    void *msg;
    uint32_t msg_len;

    constexpr int num = 8;
    struct axcl::pkg::runtime::p2p::p2p_config config1[num];
    for (int i = 0; i < num; ++i) {
        m_random.randomize(config1[i]);
    }

    /* host -> device */
    m_req_encode.setup_p2p_config(config1, num);
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(axcl::pkg::runtime::p2p::API::P2P_SETUP_CONFIG, m_req_decode.get_api());
    auto config2 = m_req_decode.get_p2p_config();
    EXPECT_EQ(true, num == config2.size());
    for (int i = 0; i < num; ++i) {
        EXPECT_EQ(config1[i].target_id, config2[i].target_id);
        EXPECT_EQ(config1[i].dma_pci_addr, config2[i].dma_pci_addr);
        EXPECT_EQ(config1[i].mbox_pci_addr, config2[i].mbox_pci_addr);
        EXPECT_EQ(config1[i].cmm_phy_addr, config2[i].cmm_phy_addr);
        EXPECT_EQ(config1[i].cmm_size, config2[i].cmm_size);
    }

    /* device -> host */
    int32_t status;
    m_random.randomize(status);
    m_ack_encode.set_api(axcl::pkg::runtime::p2p::API::P2P_SETUP_CONFIG);
    m_ack_encode.set_status(status);
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(status, m_ack_decode.get_status());
}

TEST_F(P2pPkgTest, p2pStartBandwidthTest) {
    void *msg;
    uint32_t msg_len;

    constexpr int num = 7;
    struct axcl::pkg::runtime::p2p::bwt_config config1[num];
    for (int i = 0; i < num; ++i) {
        m_random.randomize(config1[i]);
    }

    uint32_t target_id;
    m_random.randomize(target_id);

    /* host -> device */
    m_req_encode.start_bandwidth_test(target_id, config1, num, 3000/* timeout */);
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(axcl::pkg::runtime::p2p::API::P2P_START_BANDWIDTH_TEST, m_req_decode.get_api());
    EXPECT_EQ(target_id, m_req_decode.get_target_id());
    auto config2 = m_req_decode.get_bandwidth_test_config();
    EXPECT_EQ(true, num == config2.size());
    for (int i = 0; i < num; ++i) {
        EXPECT_EQ(config1[i].target_id, config2[i].target_id);
        EXPECT_EQ(config1[i].chunk_size, config2[i].chunk_size);
        EXPECT_EQ(config1[i].loop_count, config2[i].loop_count);
        EXPECT_EQ(config1[i].loop_interval, config2[i].loop_interval);
    }

    /* device -> host */
    int32_t status;
    m_random.randomize(status);
    struct axcl::pkg::runtime::p2p::bwt_result result1, result2;
    m_random.randomize(result1);
    m_ack_encode.set_api(axcl::pkg::runtime::p2p::API::P2P_START_BANDWIDTH_TEST);
    m_ack_encode.set_status(status);
    m_ack_encode.set_bandwidth_test_result(result1);
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(status, m_ack_decode.get_status());
    result2 = m_ack_decode.get_bandwidth_test_result();
    EXPECT_EQ(result1.target_id, result2.target_id);
    EXPECT_EQ(result1.min, result2.min);
    EXPECT_EQ(result1.avg, result2.avg);
    EXPECT_EQ(result1.max, result2.max);
}