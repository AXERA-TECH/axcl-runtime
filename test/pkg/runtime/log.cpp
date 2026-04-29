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
#include "package/device/runtime/system/log/log_request.hpp"
#include "package/device/runtime/system/log/log_response.hpp"
#include "package/host/runtime/system/log/log_request.hpp"
#include "package/host/runtime/system/log/log_response.hpp"
#include "runtime.hpp"

class LogPkgTest : public RuntimeTest<axcl::pkg::host::log_request, axcl::pkg::device::log_response, axcl::pkg::device::log_request,
                                      axcl::pkg::host::log_response> {
protected:
    LogPkgTest()
        : RuntimeTest<axcl::pkg::host::log_request, axcl::pkg::device::log_response, axcl::pkg::device::log_request,
                      axcl::pkg::host::log_response>(axcl::pkg::runtime::module::SYSTEM) {};
};

TEST_F(LogPkgTest, TransferLogTest) {
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    uint32_t type, flag;
    m_random.randomize(type);
    m_random.randomize(flag);
    m_req_encode.set_type(type);
    m_req_encode.set_flag(flag);
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(type, m_req_decode.get_type());
    EXPECT_EQ(flag, m_req_decode.get_flag());

    /* device -> host */
    const std::string name = "axcl.log.tgz";
    constexpr uint32_t SIZE = 8 * 1024 * 1024;
    std::unique_ptr<char[]> data = std::make_unique<char[]>(SIZE);
    m_random.randomize((uint8_t *)data.get(), SIZE);
    m_ack_encode.set_name(name);
    m_ack_encode.set_content(data.get(), SIZE);
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(name, m_ack_decode.get_name());
    EXPECT_EQ(std::string(data.get(), SIZE), m_ack_decode.get_content());
}
