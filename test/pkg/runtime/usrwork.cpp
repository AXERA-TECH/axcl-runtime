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
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include "package/device/runtime/usrwork/usrwork_request.hpp"
#include "package/device/runtime/usrwork/usrwork_response.hpp"
#include "package/host/runtime/usrwork/usrwork_request.hpp"
#include "package/host/runtime/usrwork/usrwork_response.hpp"
#include "runtime.hpp"

class UsrworkPkgTest : public RuntimeTest<axcl::pkg::host::usrwork_request, axcl::pkg::device::usrwork_response,
                                          axcl::pkg::device::usrwork_request, axcl::pkg::host::usrwork_response> {
protected:
    UsrworkPkgTest()
        : RuntimeTest<axcl::pkg::host::usrwork_request, axcl::pkg::device::usrwork_response, axcl::pkg::device::usrwork_request,
                      axcl::pkg::host::usrwork_response>(axcl::pkg::runtime::module::USRWORK){};
};

TEST_F(UsrworkPkgTest, TransferFile) {
    void *msg;
    uint32_t msg_len;

    {
        std::string src_path = "/test_program";
        std::string dst_path = "/opt/bin/test_program";
        uint32_t policy = static_cast<uint32_t>(axcl::pkg::runtime::usrwork::FILE_TRANSFER_POLICY::FILE_TRANSFER_FROM_HOST_TO_DEVICE);

        m_req_encode.transfer_file(src_path.c_str(), dst_path.c_str(), policy);

        uint32_t file_size = 1024;
        std::vector<uint8_t> file_data(file_size);
        for (uint32_t i = 0; i < file_size; ++i) {
            file_data[i] = static_cast<uint8_t>(i % 256);
        }
        m_req_encode.set_data(file_data.data(), file_size);

        EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));

        EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
        CheckRequestPackageHead(msg);
        EXPECT_EQ(axcl::pkg::runtime::usrwork::API::TRANSFER_FILE, m_req_decode.get_api());
        EXPECT_EQ(src_path, m_req_decode.get_src_path());
        EXPECT_EQ(dst_path, m_req_decode.get_dst_path());
        EXPECT_EQ(axcl::pkg::runtime::usrwork::FILE_TRANSFER_POLICY::FILE_TRANSFER_FROM_HOST_TO_DEVICE, m_req_decode.get_policy());

        size_t received_data_size;
        const void* received_data = m_req_decode.get_data(received_data_size);
        EXPECT_NE(nullptr, received_data);
        EXPECT_EQ(file_size, received_data_size);
        if (received_data && received_data_size > 0) {
            const uint8_t* data_ptr = static_cast<const uint8_t*>(received_data);
            for (size_t i = 0; i < received_data_size; ++i) {
                EXPECT_EQ(file_data[i], data_ptr[i]) << "Data mismatch at index: " << i;
            }
        }

        int32_t status;
        m_random.randomize(status);
        m_ack_encode.set_api(axcl::pkg::runtime::usrwork::API::TRANSFER_FILE);
        m_ack_encode.set_status(status);

        EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));

        EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
        CheckResponsePackageHead(msg);
        EXPECT_EQ(axcl::pkg::runtime::usrwork::API::TRANSFER_FILE, m_ack_decode.get_api());
        EXPECT_EQ(status, m_ack_decode.get_status());
    }

    {
        std::string src_path = "/opt/bin/test_program";
        std::string dst_path = "/test_program";
        uint32_t policy = static_cast<uint32_t>(axcl::pkg::runtime::usrwork::FILE_TRANSFER_POLICY::FILE_TRANSFER_FROM_DEVICE_TO_HOST);

        m_req_encode.transfer_file(src_path.c_str(), dst_path.c_str(), policy);

        EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));

        EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
        CheckRequestPackageHead(msg);
        EXPECT_EQ(axcl::pkg::runtime::usrwork::API::TRANSFER_FILE, m_req_decode.get_api());
        EXPECT_EQ(src_path, m_req_decode.get_src_path());
        EXPECT_EQ(dst_path, m_req_decode.get_dst_path());
        EXPECT_EQ(axcl::pkg::runtime::usrwork::FILE_TRANSFER_POLICY::FILE_TRANSFER_FROM_DEVICE_TO_HOST, m_req_decode.get_policy());

        int32_t status;
        m_random.randomize(status);
        m_ack_encode.set_api(axcl::pkg::runtime::usrwork::API::TRANSFER_FILE);
        m_ack_encode.set_status(status);

        uint32_t device_file_size = 2048;
        std::vector<uint8_t> device_file_data(device_file_size);
        for (uint32_t i = 0; i < device_file_size; ++i) {
            device_file_data[i] = static_cast<uint8_t>((i * 7 + 13) % 256);
        }
        m_ack_encode.set_data(device_file_data.data(), device_file_size);

        EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));

        EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
        CheckResponsePackageHead(msg);
        EXPECT_EQ(axcl::pkg::runtime::usrwork::API::TRANSFER_FILE, m_ack_decode.get_api());
        EXPECT_EQ(status, m_ack_decode.get_status());

        size_t response_data_size;
        const void* response_data = m_ack_decode.get_data(response_data_size);
        EXPECT_NE(nullptr, response_data);
        EXPECT_EQ(device_file_size, response_data_size);
        if (response_data && response_data_size > 0) {
            const uint8_t* data_ptr = static_cast<const uint8_t*>(response_data);
            for (size_t i = 0; i < response_data_size; ++i) {
                EXPECT_EQ(device_file_data[i], data_ptr[i]) << "Device returned data mismatch at index: " << i;
            }
        }
    }
}

TEST_F(UsrworkPkgTest, ExecWorker) {
    void *msg;
    uint32_t msg_len;

    std::string path = "/usr/bin/test_program";
    std::vector<std::string> argv = {"test_program", "--arg1", "value1", "--arg2", "value2"};
    uint32_t heartbeat_interval = 1000;
    uint32_t port_num = 2;
    std::vector<uint32_t> ports = {8080, 8081};

    m_req_encode.exec_worker(path.c_str(), argv, heartbeat_interval, port_num, ports);
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(axcl::pkg::runtime::usrwork::API::EXEC_WORKER, m_req_decode.get_api());
    EXPECT_EQ(path, m_req_decode.get_path());

    std::vector<std::string> received_argv = m_req_decode.get_argv();
    EXPECT_EQ(argv.size(), received_argv.size());
    for (std::size_t i = 0; i < argv.size(); ++i) {
        EXPECT_EQ(argv[i], received_argv[i]);
    }

    int32_t status;
    uint32_t pid;
    m_random.randomize(status);
    m_random.randomize(pid);
    m_ack_encode.set_api(axcl::pkg::runtime::usrwork::API::EXEC_WORKER);
    m_ack_encode.set_status(status);
    m_ack_encode.set_pid(pid);
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(axcl::pkg::runtime::usrwork::API::EXEC_WORKER, m_ack_decode.get_api());
    EXPECT_EQ(status, m_ack_decode.get_status());
    EXPECT_EQ(pid, m_ack_decode.get_pid());
}

TEST_F(UsrworkPkgTest, KillWorker) {
    void *msg;
    uint32_t msg_len;

    uint32_t pid;
    m_random.randomize(pid);

    m_req_encode.kill_worker(pid);
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(axcl::pkg::runtime::usrwork::API::KILL_WORKER, m_req_decode.get_api());

    int32_t status;
    m_random.randomize(status);
    m_ack_encode.set_api(axcl::pkg::runtime::usrwork::API::KILL_WORKER);
    m_ack_encode.set_status(status);
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(axcl::pkg::runtime::usrwork::API::KILL_WORKER, m_ack_decode.get_api());
    EXPECT_EQ(status, m_ack_decode.get_status());
}

TEST_F(UsrworkPkgTest, WorkerSend) {
    void *msg;
    uint32_t msg_len;

    uint32_t pid;
    uint32_t size = 1024;
    int32_t timeout;
    m_random.randomize(pid);
    m_random.randomize(timeout);

    std::vector<uint8_t> test_data(size);
    for (uint32_t i = 0; i < size; ++i) {
        test_data[i] = static_cast<uint8_t>(i % 256);
    }

    m_req_encode.worker_send(pid, test_data.data(), size, static_cast<int32_t>(timeout));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(axcl::pkg::runtime::usrwork::API::WORKER_SEND, m_req_decode.get_api());
    EXPECT_EQ(pid, m_req_decode.get_pid());
    EXPECT_EQ(size, m_req_decode.get_data_size());
    EXPECT_EQ(timeout, m_req_decode.get_timeout());

    size_t data_size;
    const void* received_data = m_req_decode.get_data(data_size);
    EXPECT_NE(nullptr, received_data);
    if (received_data && data_size > 0) {
        const uint8_t* data_ptr = static_cast<const uint8_t*>(received_data);
        for (size_t i = 0; i < data_size; ++i) {
            EXPECT_EQ(test_data[i], data_ptr[i]);
        }
    }

    int32_t status;
    m_random.randomize(status);
    m_ack_encode.set_api(axcl::pkg::runtime::usrwork::API::WORKER_SEND);
    m_ack_encode.set_status(status);
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(axcl::pkg::runtime::usrwork::API::WORKER_SEND, m_ack_decode.get_api());
    EXPECT_EQ(status, m_ack_decode.get_status());
}

TEST_F(UsrworkPkgTest, WorkerRecv) {
    void *msg;
    uint32_t msg_len;

    uint32_t pid;
    uint32_t bufsize = 2048;
    int32_t timeout;
    m_random.randomize(pid);
    m_random.randomize(timeout);

    m_req_encode.worker_recv(pid, bufsize, static_cast<int32_t>(timeout));
    EXPECT_NE(nullptr, msg = m_req_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_req_decode.decode(msg, msg_len));
    CheckRequestPackageHead(msg);
    EXPECT_EQ(axcl::pkg::runtime::usrwork::API::WORKER_RECV, m_req_decode.get_api());
    EXPECT_EQ(pid, m_req_decode.get_pid());
    EXPECT_EQ(bufsize, m_req_decode.get_data_size());
    EXPECT_EQ(timeout, m_req_decode.get_timeout());

    int32_t status;
    uint32_t recvlen = 1024;
    m_random.randomize(status);

    std::vector<uint8_t> recv_data(recvlen);
    for (uint32_t i = 0; i < recvlen; ++i) {
        recv_data[i] = static_cast<uint8_t>((i * 13) % 256);
    }

    m_ack_encode.set_api(axcl::pkg::runtime::usrwork::API::WORKER_RECV);
    m_ack_encode.set_status(status);
    m_ack_encode.set_data_size(recvlen);
    m_ack_encode.set_data(recv_data.data(), recvlen);
    EXPECT_NE(nullptr, msg = m_ack_encode.encode(m_head, msg_len));
    EXPECT_EQ(true, m_ack_decode.decode(msg, msg_len));
    CheckResponsePackageHead(msg);
    EXPECT_EQ(axcl::pkg::runtime::usrwork::API::WORKER_RECV, m_ack_decode.get_api());
    EXPECT_EQ(status, m_ack_decode.get_status());
    EXPECT_EQ(recvlen, m_ack_decode.get_recvlen());

    size_t data_size;
    const void* received_data = m_ack_decode.get_data(data_size);
    EXPECT_NE(nullptr, received_data);
    if (received_data && data_size > 0) {
        const uint8_t* data_ptr = static_cast<const uint8_t*>(received_data);
        for (size_t i = 0; i < data_size; ++i) {
            EXPECT_EQ(recv_data[i], data_ptr[i]);
        }
    }
}