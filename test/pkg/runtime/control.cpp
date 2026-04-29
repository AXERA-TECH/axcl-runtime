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
#include "package/device/runtime/system/control/control_request.hpp"
#include "package/device/runtime/system/control/control_response.hpp"
#include "package/host/runtime/system/control/control_request.hpp"
#include "package/host/runtime/system/control/control_response.hpp"
#include "runtime.hpp"

class ControlPkgTest : public RuntimeTest<axcl::pkg::host::control_request, axcl::pkg::device::control_response,
                                          axcl::pkg::device::control_request, axcl::pkg::host::control_response> {
protected:
    ControlPkgTest()
        : RuntimeTest<axcl::pkg::host::control_request, axcl::pkg::device::control_response, axcl::pkg::device::control_request,
                      axcl::pkg::host::control_response>(axcl::pkg::runtime::module::SYSTEM) {};
};

TEST_F(ControlPkgTest, CreateContext) {
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    uint32_t pid, tid, context, stream;
    m_random.randomize(pid);
    m_random.randomize(tid);
    m_random.randomize(context);
    m_random.randomize(stream);

    m_req_encode.create_context(pid, tid, context, stream);

    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(axcl::pkg::runtime::system::command::CREATE_CONTEXT, m_req_decode.get_cmd());

    /* device -> host */
    m_ack_encode.create_context(m_req_decode.get_pid(), m_req_decode.get_tid(), m_req_decode.get_context(), m_req_decode.get_stream());
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(axcl::pkg::runtime::system::command::CREATE_CONTEXT, m_ack_decode.get_cmd());
    EXPECT_EQ(pid, m_ack_decode.get_pid());
    EXPECT_EQ(tid, m_ack_decode.get_tid());
    EXPECT_EQ(context, m_ack_decode.get_context());
    EXPECT_EQ(stream, m_ack_decode.get_stream());
}

TEST_F(ControlPkgTest, DestroyContext) {
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    uint32_t pid, tid, context, stream;
    m_random.randomize(pid);
    m_random.randomize(tid);
    m_random.randomize(context);
    m_random.randomize(stream);

    m_req_encode.destroy_context(pid, tid, context, stream);
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(axcl::pkg::runtime::system::command::DESTROY_CONTEXT, m_req_decode.get_cmd());

    /* device -> host */
    m_ack_encode.destroy_context(m_req_decode.get_pid(), m_req_decode.get_tid(), m_req_decode.get_context(), m_req_decode.get_stream());
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(axcl::pkg::runtime::system::command::DESTROY_CONTEXT, m_ack_decode.get_cmd());
    EXPECT_EQ(pid, m_ack_decode.get_pid());
    EXPECT_EQ(tid, m_ack_decode.get_tid());
    EXPECT_EQ(context, m_ack_decode.get_context());
    EXPECT_EQ(stream, m_ack_decode.get_stream());
}

TEST_F(ControlPkgTest, CreateStream) {
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    uint32_t context, stream;
    m_random.randomize(context);
    m_random.randomize(stream);

    m_req_encode.create_stream(context, stream);
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(axcl::pkg::runtime::system::command::CREATE_STREAM, m_req_decode.get_cmd());

    /* device -> host */
    m_ack_encode.create_stream(m_req_decode.get_context(), m_req_decode.get_stream());
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(axcl::pkg::runtime::system::command::CREATE_STREAM, m_ack_decode.get_cmd());
    EXPECT_EQ(context, m_ack_decode.get_context());
    EXPECT_EQ(stream, m_ack_decode.get_stream());
}

TEST_F(ControlPkgTest, DestroyStream) {
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    uint32_t context, stream;
    m_random.randomize(context);
    m_random.randomize(stream);

    m_req_encode.destroy_stream(context, stream);
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(axcl::pkg::runtime::system::command::DESTROY_STREAM, m_req_decode.get_cmd());

    /* device -> host */
    m_ack_encode.destroy_stream(m_req_decode.get_context(), m_req_decode.get_stream());
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(axcl::pkg::runtime::system::command::DESTROY_STREAM, m_ack_decode.get_cmd());
    EXPECT_EQ(context, m_ack_decode.get_context());
    EXPECT_EQ(stream, m_ack_decode.get_stream());
}

TEST_F(ControlPkgTest, SyncStream) {
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    uint32_t context, stream;
    int32_t timeout;
    m_random.randomize(context);
    m_random.randomize(stream);
    m_random.randomize(timeout);

    m_req_encode.sync_stream(context, stream, timeout);
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(axcl::pkg::runtime::system::command::SYNC_STREAM, m_req_decode.get_cmd());
    EXPECT_EQ(timeout, m_req_decode.get_timeout());

    /* device -> host */
    int32_t status;
    m_random.randomize(status);
    m_ack_encode.sync_stream(m_req_decode.get_context(), m_req_decode.get_stream(), status);
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(axcl::pkg::runtime::system::command::SYNC_STREAM, m_ack_decode.get_cmd());
    EXPECT_EQ(context, m_ack_decode.get_context());
    EXPECT_EQ(stream, m_ack_decode.get_stream());
    EXPECT_EQ(status, m_ack_decode.get_status());
}

TEST_F(ControlPkgTest, CreateEvent) {
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    uint32_t context, stream, event;
    m_random.randomize(context);
    m_random.randomize(stream);
    m_random.randomize(event);

    m_req_encode.create_event(context, stream, event);
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(axcl::pkg::runtime::system::command::CREATE_EVENT, m_req_decode.get_cmd());

    /* device -> host */
    m_ack_encode.create_event(m_req_decode.get_context(), m_req_decode.get_stream(), m_req_decode.get_event());
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(axcl::pkg::runtime::system::command::CREATE_EVENT, m_ack_decode.get_cmd());
    EXPECT_EQ(context, m_ack_decode.get_context());
    EXPECT_EQ(stream, m_ack_decode.get_stream());
    EXPECT_EQ(event, m_ack_decode.get_event());
}

TEST_F(ControlPkgTest, DestroyEvent) {
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    uint32_t context, stream, event;
    m_random.randomize(context);
    m_random.randomize(stream);
    m_random.randomize(event);

    m_req_encode.destroy_event(context, stream, event);
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(axcl::pkg::runtime::system::command::DESTROY_EVENT, m_req_decode.get_cmd());

    /* device -> host */
    m_ack_encode.destroy_event(m_req_decode.get_context(), m_req_decode.get_stream(), m_req_decode.get_event());
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(axcl::pkg::runtime::system::command::DESTROY_EVENT, m_ack_decode.get_cmd());
    EXPECT_EQ(context, m_ack_decode.get_context());
    EXPECT_EQ(stream, m_ack_decode.get_stream());
    EXPECT_EQ(event, m_ack_decode.get_event());
}

TEST_F(ControlPkgTest, RecordEvent) {
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    uint32_t context, stream, event;
    m_random.randomize(context);
    m_random.randomize(stream);
    m_random.randomize(event);

    m_req_encode.record_event(context, stream, event);
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(axcl::pkg::runtime::system::command::RECORD_EVENT, m_req_decode.get_cmd());

    /* device -> host */
    int32_t status;
    m_random.randomize(status);
    m_ack_encode.record_event(m_req_decode.get_context(), m_req_decode.get_stream(), m_req_decode.get_event(), status);
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(axcl::pkg::runtime::system::command::RECORD_EVENT, m_ack_decode.get_cmd());
    EXPECT_EQ(context, m_ack_decode.get_context());
    EXPECT_EQ(stream, m_ack_decode.get_stream());
    EXPECT_EQ(event, m_ack_decode.get_event());
    EXPECT_EQ(status, m_ack_decode.get_status());
}

TEST_F(ControlPkgTest, WaitEvent) {
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    uint32_t context, stream, event;
    int32_t timeout;
    m_random.randomize(context);
    m_random.randomize(stream);
    m_random.randomize(event);
    m_random.randomize(timeout);

    m_req_encode.wait_event(context, stream, event, timeout);
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(axcl::pkg::runtime::system::command::WAIT_EVENT, m_req_decode.get_cmd());
    EXPECT_EQ(timeout, m_req_decode.get_timeout());

    /* device -> host */
    int32_t status;
    m_random.randomize(status);
    m_ack_encode.wait_event(m_req_decode.get_context(), m_req_decode.get_stream(), m_req_decode.get_event(), status);
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(axcl::pkg::runtime::system::command::WAIT_EVENT, m_ack_decode.get_cmd());
    EXPECT_EQ(context, m_ack_decode.get_context());
    EXPECT_EQ(stream, m_ack_decode.get_stream());
    EXPECT_EQ(event, m_ack_decode.get_event());
    EXPECT_EQ(status, m_ack_decode.get_status());
}

TEST_F(ControlPkgTest, ShellCmd) {
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    const char *cmd1 = "cat /sys/class/thermal/thermal_zone0/temp";
    m_req_encode.set_shell_cmd(cmd1, nullptr, 0);
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(axcl::pkg::runtime::system::command::SHELL, m_req_decode.get_cmd());
    EXPECT_EQ(m_req_decode.get_shell_cmd(), std::string(cmd1));
    EXPECT_EQ(true, m_req_decode.get_shell_args().empty());

    /* device -> host */
    m_ack_encode.set_shell_cmd_output(1, "50340");
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(axcl::pkg::runtime::system::command::SHELL, m_ack_decode.get_cmd());
    EXPECT_EQ(1, m_ack_decode.get_shell_cmd_status());
    EXPECT_EQ(std::string("50340"), m_ack_decode.get_shell_cmd_output());

    /* host -> device */
    const char *cmd2 = "ls";
    const char *const args[3] = {"-l", "/opt/bin", "./"};
    m_req_encode.set_shell_cmd(cmd2, args, 3);
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(axcl::pkg::runtime::system::command::SHELL, m_req_decode.get_cmd());
    EXPECT_EQ(m_req_decode.get_shell_cmd(), std::string(cmd2));
    const auto argv = m_req_decode.get_shell_args();
    EXPECT_EQ(argv.size(), 3lu);
    for (size_t i = 0; i < 3; ++i) {
        EXPECT_EQ(argv[i], args[i]);
    }

    /* device -> host */
    m_ack_encode.set_shell_cmd_output(0, nullptr);
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(axcl::pkg::runtime::system::command::SHELL, m_ack_decode.get_cmd());
    EXPECT_EQ(0, m_ack_decode.get_shell_cmd_status());
    EXPECT_EQ(true, m_ack_decode.get_shell_cmd_output().empty());
}

TEST_F(ControlPkgTest, SetupPCIeSubId) {
    void *msg;
    uint32_t msg_len;

    /* host -> device */
    uint32_t sub_vendor_id, sub_device_id;
    m_random.randomize(sub_vendor_id);
    m_random.randomize(sub_device_id);

    m_req_encode.set_pcie_sub_id(sub_vendor_id, sub_device_id);
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(axcl::pkg::runtime::system::command::SETUP_PCIE_SUB_ID, m_req_decode.get_cmd());
    EXPECT_EQ(sub_vendor_id, m_req_decode.get_pcie_sub_vendor_id());
    EXPECT_EQ(sub_device_id, m_req_decode.get_pcie_sub_device_id());

    /* device -> host */
    int32_t status;
    m_random.randomize(status);
    m_ack_encode.set_pcie_sub_id_setup_status(status);
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(axcl::pkg::runtime::system::command::SETUP_PCIE_SUB_ID, m_ack_decode.get_cmd());
    EXPECT_EQ(status, m_ack_decode.get_pcie_sub_id_setup_status());
}
