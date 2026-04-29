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

class vdec_request : public native_request {
public:
    explicit vdec_request();
    virtual ~vdec_request();

    vdec_request(vdec_request &&other) noexcept;
    vdec_request &operator=(vdec_request &&other) noexcept;
    vdec_request(const vdec_request &) = delete;
    vdec_request &operator=(const vdec_request &) = delete;

    template <typename... Args>
    [[nodiscard]] bool pack(axcl::pkg::native::VDEC_API api, Args... args) {
        if (!set_api(api)) {
            return false;
        }

        return m_args.set_args(args...);
    }

protected:
    bool add_data(const void *data, size_t size) override;
    bool set_api(axcl::pkg::native::VDEC_API api);

private:
    struct impl;
    impl *m_impl;
};

}  // namespace axcl::pkg::device