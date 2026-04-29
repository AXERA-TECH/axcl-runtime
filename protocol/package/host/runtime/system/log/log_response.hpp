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

#include "package/host/runtime/system/system_response.hpp"

namespace axcl::pkg::host {

class log_response : public system_response {
public:
    explicit log_response();
    virtual ~log_response();

    log_response(log_response&& other) noexcept;
    log_response& operator=(log_response&& other) noexcept;
    log_response(const log_response&) = delete;
    log_response& operator=(const log_response&) = delete;

    const std::string& get_name() const;
    const std::string& get_content() const;
    const char* get_content(size_t& size) const;

    [[nodiscard]] bool decode(const void* message, uint32_t size) const override;

private:
    struct impl;
    impl* m_impl;
};

}  // namespace axcl::pkg::host