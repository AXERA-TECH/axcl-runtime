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

#include "axcl_ivps_type.h"
#include "package/device/native/native_request.hpp"
#include "package/include/native/api.hpp"

namespace axcl::pkg::device {

class ivps_request : public native_request {
public:
    explicit ivps_request();
    virtual ~ivps_request();

    ivps_request(ivps_request &&other) noexcept;
    ivps_request &operator=(ivps_request &&other) noexcept;
    ivps_request(const ivps_request &) = delete;
    ivps_request &operator=(const ivps_request &) = delete;

    template <typename... Args>
    [[nodiscard]] bool pack(axcl::pkg::native::IVPS_API api, Args... args) {
        if (!set_api(api)) {
            return false;
        }

        return m_args.set_args(args...);
    }

protected:
    bool add_data(const void *data, size_t size) override;
    bool set_api(axcl::pkg::native::IVPS_API api);

private:
    struct impl;
    impl *m_impl;
};

}  // namespace axcl::pkg::device