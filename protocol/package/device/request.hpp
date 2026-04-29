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
#include "axcl_package.hpp"
#include "package/include/type.hpp"

namespace axcl::pkg::device {

class request {
public:
    explicit request();
    virtual ~request();

    request(request &&other) noexcept;
    request &operator=(request &&other) noexcept;
    request(const request &) = delete;
    request &operator=(const request &) = delete;

    void set_timestamp(uint64_t timestamp = 0 /* 0: now */) const;
    void set_env(const axcl::pkg::environment &env) const;
    void set_version(const axcl::pkg::version &v) const;

    uint64_t get_timestamp() const;
    uint64_t get_seq_num() const;
    axcl::pkg::version get_version() const;
    axcl::pkg::environment get_env() const;

    [[nodiscard]] virtual void *encode(const axcl::pkg::head &hd, uint32_t &size) const;

protected:
    [[nodiscard]] void *select(const axcl::pkg::module &module) const;

private:
    struct impl;
    impl *m_impl;
};

}  // namespace axcl::pkg::device