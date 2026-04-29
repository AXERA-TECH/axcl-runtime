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

#include "package/host/runtime/runtime_response.hpp"
#include "package/include/runtime/usrwork/type.hpp"

namespace axcl::pkg::host {

class usrwork_response : public runtime_response {
public:
    explicit usrwork_response();
    virtual ~usrwork_response();

    usrwork_response(usrwork_response&& other) noexcept;
    usrwork_response& operator=(usrwork_response&& other) noexcept;
    usrwork_response(const usrwork_response&) = delete;
    usrwork_response& operator=(const usrwork_response&) = delete;

    bool decode(const void* message, uint32_t size) const override;
    axcl::pkg::runtime::usrwork::API get_api() const;

    int32_t get_status() const;
    uint32_t get_pid() const;
    uint32_t get_recvlen() const;
    const void* get_data(size_t& size) const;

private:
    struct impl;
    impl* m_impl;
};

}  // namespace axcl::pkg::host