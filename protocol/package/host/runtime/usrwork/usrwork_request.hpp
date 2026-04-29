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

#include "axcl_package.hpp"
#include "package/host/runtime/runtime_request.hpp"
#include "package/include/runtime/usrwork/type.hpp"
#include <vector>

namespace axcl::pkg::host {

class usrwork_request : public runtime_request {
public:
    explicit usrwork_request();
    virtual ~usrwork_request();

    usrwork_request(usrwork_request&& other) noexcept;
    usrwork_request& operator=(usrwork_request&& other) noexcept;
    usrwork_request(const usrwork_request&) = delete;
    usrwork_request& operator=(const usrwork_request&) = delete;

    void transfer_file(const char* src_path, const char* dst_path, uint32_t policy) const;
    void set_data(const void* data, uint32_t data_size) const;
    void exec_worker(const char* path, const std::vector<std::string>& argv,
                     uint32_t heartbeat_interval, uint32_t port_num,
                     const std::vector<uint32_t>& ports) const;
    void kill_worker(uint32_t pid) const;
    void worker_send(uint32_t pid, const void* buf, uint32_t size, int32_t timeout) const;
    void worker_recv(uint32_t pid, uint32_t bufsize, int32_t timeout) const;

    axcl::pkg::runtime::usrwork::API get_api() const;

private:
    struct impl;
    impl* m_impl;
};

}  // namespace axcl::pkg::host