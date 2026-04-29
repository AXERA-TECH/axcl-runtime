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

#include "package/device/runtime/system/system_request.hpp"

namespace axcl::pkg::device {

class log_request : public system_request {
public:
    explicit log_request();
    virtual ~log_request();

    log_request(log_request&& other) noexcept;
    log_request& operator=(log_request&& other) noexcept;
    log_request(const log_request&) = delete;
    log_request& operator=(const log_request&) = delete;

    void set_name(const std::string& name);
    void set_content(const char* data, uint32_t size);
    void set_content(const std::string& data);

private:
    struct impl;
    impl* m_impl;
};

}  // namespace axcl::pkg::device