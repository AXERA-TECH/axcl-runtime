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

#include "package/device/runtime/system/system_response.hpp"

namespace axcl::pkg::device {

class log_response : public system_response {
public:
    explicit log_response();
    virtual ~log_response();

    log_response(log_response&& other) noexcept;
    log_response& operator=(log_response&& other) noexcept;
    log_response(const log_response&) = delete;
    log_response& operator=(const log_response&) = delete;

    uint32_t get_type() const;
    uint32_t get_flag() const;
    [[nodiscard]] bool decode(const void* message, uint32_t size) const override;

private:
    struct impl;
    impl* m_impl;
};

}  // namespace axcl::pkg::device