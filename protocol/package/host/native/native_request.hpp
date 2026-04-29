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

#include "package/host/request.hpp"
#include "package/include/native/args_helper.hpp"
#include "package/include/native/type.hpp"

namespace axcl::pkg::host {

class native_request : public request {
public:
    explicit native_request();
    virtual ~native_request();

    native_request(native_request &&other) noexcept;
    native_request &operator=(native_request &&other) noexcept;
    native_request(const native_request &) = delete;
    native_request &operator=(const native_request &) = delete;

protected:
    [[nodiscard]] void *select(const axcl::pkg::native::module &module) const;
    virtual bool add_data(const void *data, size_t size);

protected:
    axcl::pkg::native::args_helper m_args;

private:
    struct impl;
    impl *m_impl;
};

}  // namespace axcl::pkg::host