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

#include "package/device/native/native_request.hpp"
#include "package/include/native/api.hpp"

namespace axcl::pkg::device {

class ive_request : public native_request {
public:
    explicit ive_request();
    virtual ~ive_request();

    ive_request(ive_request &&other) noexcept;
    ive_request &operator=(ive_request &&other) noexcept;
    ive_request(const ive_request &) = delete;
    ive_request &operator=(const ive_request &) = delete;

    template <typename... Args>
    [[nodiscard]] bool pack(axcl::pkg::native::IVE_API api, Args... args) {
        if (!set_api(api)) {
            return false;
        }

        return m_args.set_args(args...);
    }

protected:
    bool add_data(const void *data, size_t size) override;
    bool set_api(axcl::pkg::native::IVE_API api);

private:
    struct impl;
    impl *m_impl;
};

}  // namespace axcl::pkg::device