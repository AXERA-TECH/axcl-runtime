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
#include "package/device/runtime/system/latency/latency_request.hpp"
#include "package/device/runtime/system/latency/latency_response.hpp"
#include "package/host/runtime/system/latency/latency_request.hpp"
#include "package/host/runtime/system/latency/latency_response.hpp"
#include "runtime.hpp"

class LatencyPkgTest : public RuntimeTest<axcl::pkg::host::latency_request, axcl::pkg::device::latency_response,
                                          axcl::pkg::device::latency_request, axcl::pkg::host::latency_response> {
protected:
    LatencyPkgTest()
        : RuntimeTest<axcl::pkg::host::latency_request, axcl::pkg::device::latency_response, axcl::pkg::device::latency_request,
                      axcl::pkg::host::latency_response>(axcl::pkg::runtime::module::SYSTEM) {};
};

TEST_F(LatencyPkgTest, LatencyLoopTest) {
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    constexpr uint32_t SIZE = 8 * 1024 * 1024;
    std::unique_ptr<char[]> data = std::make_unique<char[]>(SIZE);
    m_random.randomize((uint8_t *)data.get(), SIZE);
    m_req_encode.set_latency_data((const void *)data.get(), SIZE);
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    CheckRequestPackageHead(msg);

    /* device -> host */
    size_t len = 0;
    const void *tmp = m_req_decode.get_latency_data(len);
    m_ack_encode.set_latency_data(tmp, len);
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(std::string(data.get(), SIZE), m_ack_decode.get_latency_data());
}
