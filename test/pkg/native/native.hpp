/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#pragma once

#include <gtest/gtest.h>
#include <cstdint>
#include <iostream>
#include "axcl_package.hpp"
#include "compare.hpp"
#include "package/include/native/type.hpp"
#include "package/include/type.hpp"
#include "randomizer.hpp"
#include "version.hpp"

template <typename REQ_ENC, typename REQ_DEC, typename RSP_ENC, typename RSP_DEC>
class NativeTest : public testing::Test {
protected:
    NativeTest(axcl::pkg::native::module mod) {
        m_head_type = CREATE_PACKAGE_HEAD_TYPE(axcl::pkg::PROTO, axcl::pkg::module::NATIVE, mod, 0);
    }

protected:
    void SetUp() override {
        m_random.randomize(m_head);
        m_head.type = m_head_type;

        axcl::pkg::environment env;
        m_random.randomize(env);
        m_req_encode.set_env(env);
        m_ack_encode.set_env(env);

        m_req_encode.set_timestamp(0);
        m_ack_encode.set_timestamp(0);

        m_random.randomize(m_req_status);
        m_random.randomize(m_ack_status);
    }

    void TearDown() override {
    }

    void CheckRequestPackageHead(const void *pkg) {
        EXPECT_EQ(true, struct_equal<axcl::pkg::head>(m_head, *(axcl::pkg::head *)pkg));
        EXPECT_EQ(m_req_encode.get_seq_num(), m_head.seq_num);

        EXPECT_EQ(true, struct_equal<axcl::pkg::version>(m_req_encode.get_version(), PROTOCOL_VERSION));
        EXPECT_EQ(true, struct_equal<axcl::pkg::version>(m_req_encode.get_version(), m_req_decode.get_version()));
        EXPECT_EQ(true, struct_equal<axcl::pkg::environment>(m_req_encode.get_env(), m_req_decode.get_env()));
        EXPECT_EQ(m_req_encode.get_seq_num(), m_req_decode.get_seq_num());
        EXPECT_EQ(m_req_encode.get_timestamp(), m_req_decode.get_timestamp());
    }

    void CheckResponsePackageHead(const void *pkg) {
        EXPECT_EQ(true, struct_equal<axcl::pkg::head>(m_head, *(axcl::pkg::head *)pkg));
        EXPECT_EQ(m_ack_encode.get_seq_num(), m_head.seq_num);

        EXPECT_EQ(true, struct_equal<axcl::pkg::version>(m_ack_encode.get_version(), PROTOCOL_VERSION));
        EXPECT_EQ(true, struct_equal<axcl::pkg::version>(m_ack_encode.get_version(), m_ack_decode.get_version()));
        EXPECT_EQ(true, struct_equal<axcl::pkg::environment>(m_ack_encode.get_env(), m_ack_decode.get_env()));
        EXPECT_EQ(m_ack_encode.get_seq_num(), m_ack_decode.get_seq_num());
        EXPECT_EQ(m_ack_encode.get_timestamp(), m_ack_decode.get_timestamp());

        /* make sure seq num not changed between host and device */
        EXPECT_EQ(m_req_encode.get_seq_num(), m_ack_decode.get_seq_num());
    }

    static axcl::randomizer m_random;

    static REQ_ENC m_req_encode;
    static REQ_DEC m_req_decode;
    static RSP_ENC m_ack_encode;
    static RSP_DEC m_ack_decode;

    static axcl::pkg::version PROTOCOL_VERSION;

    int32_t m_req_status;
    int32_t m_ack_status;
    axcl::pkg::head m_head;
    uint32_t m_head_type;
};

template <typename REQ_ENC, typename REQ_DEC, typename RSP_ENC, typename RSP_DEC>
axcl::randomizer NativeTest<REQ_ENC, REQ_DEC, RSP_ENC, RSP_DEC>::m_random;

template <typename REQ_ENC, typename REQ_DEC, typename RSP_ENC, typename RSP_DEC>
REQ_ENC NativeTest<REQ_ENC, REQ_DEC, RSP_ENC, RSP_DEC>::m_req_encode;

template <typename REQ_ENC, typename REQ_DEC, typename RSP_ENC, typename RSP_DEC>
REQ_DEC NativeTest<REQ_ENC, REQ_DEC, RSP_ENC, RSP_DEC>::m_req_decode;

template <typename REQ_ENC, typename REQ_DEC, typename RSP_ENC, typename RSP_DEC>
RSP_ENC NativeTest<REQ_ENC, REQ_DEC, RSP_ENC, RSP_DEC>::m_ack_encode;

template <typename REQ_ENC, typename REQ_DEC, typename RSP_ENC, typename RSP_DEC>
RSP_DEC NativeTest<REQ_ENC, REQ_DEC, RSP_ENC, RSP_DEC>::m_ack_decode;

template <typename REQ_ENC, typename REQ_DEC, typename RSP_ENC, typename RSP_DEC>
axcl::pkg::version NativeTest<REQ_ENC, REQ_DEC, RSP_ENC, RSP_DEC>::PROTOCOL_VERSION = {
    AXCL_PROTOCOL_MAJOR_VERSION, AXCL_PROTOCOL_MINOR_VERSION, AXCL_PROTOCOL_PATCH_VERSION};
