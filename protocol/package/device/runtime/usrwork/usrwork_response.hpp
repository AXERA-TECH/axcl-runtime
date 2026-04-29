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

#include <vector>
#include <string>
#include "package/device/runtime/runtime_response.hpp"
#include "package/include/runtime/usrwork/type.hpp"

namespace axcl::pkg::device {

class usrwork_response : public runtime_response {
public:
    explicit usrwork_response();
    virtual ~usrwork_response();

    usrwork_response(usrwork_response&& other) noexcept;
    usrwork_response& operator=(usrwork_response&& other) noexcept;
    usrwork_response(const usrwork_response&) = delete;
    usrwork_response& operator=(const usrwork_response&) = delete;

    [[nodiscard]] bool decode(const void* message, uint32_t size) const override;

    axcl::pkg::runtime::usrwork::API get_api() const;
    uint32_t get_pid() const;
    int32_t get_timeout() const;
    uint32_t get_data_size() const;
    const void* get_data(size_t& size) const;
    const std::string& get_src_path() const;
    const std::string& get_dst_path() const;
    const std::string& get_path() const;
    std::vector<std::string> get_argv() const;
    axcl::pkg::runtime::usrwork::FILE_TRANSFER_POLICY get_policy() const;

    uint32_t get_heartbeat_interval() const;
    uint32_t get_port_num() const;
    std::vector<uint32_t> get_ports() const;

private:
    struct impl;
    impl* m_impl;
};

}  // namespace axcl::pkg::device