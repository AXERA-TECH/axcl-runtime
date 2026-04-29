/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 * Author: wanglusheng@axera-tech.com
 *
 **************************************************************************************************/

#pragma once

#include <string>
#include "package/include/type.hpp"

namespace axcl::pkg::device {

class response {
public:
    explicit response();
    virtual ~response();

    response(response&& other) noexcept;
    response& operator=(response&& other) noexcept;
    explicit response(const response&) = delete;
    response& operator=(const response&) = delete;

    [[nodiscard]] virtual bool decode(const void* message, uint32_t size) const;

    uint64_t get_seq_num() const;
    axcl::pkg::version get_version() const;
    axcl::pkg::environment get_env() const;
    uint64_t get_timestamp() const;
    axcl::pkg::module get_module() const;

protected:
    virtual void* get_payload() const;

private:
    struct impl;
    impl* m_impl;
};

}  // namespace axcl::pkg::device
