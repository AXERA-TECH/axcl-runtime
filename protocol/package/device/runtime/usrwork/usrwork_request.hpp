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

#include "package/device/runtime/runtime_request.hpp"
#include "package/include/runtime/usrwork/type.hpp"

namespace axcl::pkg::device {

class usrwork_request : public runtime_request {
public:
    explicit usrwork_request();
    virtual ~usrwork_request();

    usrwork_request(usrwork_request&& other) noexcept;
    usrwork_request& operator=(usrwork_request&& other) noexcept;
    usrwork_request(const usrwork_request&) = delete;
    usrwork_request& operator=(const usrwork_request&) = delete;

    void set_api(axcl::pkg::runtime::usrwork::API api) const;
    void set_status(int32_t status) const;
    void set_pid(uint32_t pid) const;
    void set_data_size(uint32_t data_size) const;
    void set_data(const void* data, uint32_t data_size) const;

private:
    struct impl;
    impl* m_impl;
};

}  // namespace axcl::pkg::device