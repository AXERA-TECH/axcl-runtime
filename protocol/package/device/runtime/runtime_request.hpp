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

#include "package/device/request.hpp"
#include "package/include/runtime/type.hpp"

namespace axcl::pkg::device {

class runtime_request : public request {
public:
    explicit runtime_request();
    virtual ~runtime_request();

    runtime_request(runtime_request &&other) noexcept;
    runtime_request &operator=(runtime_request &&other) noexcept;
    runtime_request(const runtime_request &) = delete;
    runtime_request &operator=(const runtime_request &) = delete;

protected:
    [[nodiscard]] void *select(const axcl::pkg::runtime::module &module) const;

private:
    struct impl;
    impl *m_impl;
};

}  // namespace axcl::pkg::device