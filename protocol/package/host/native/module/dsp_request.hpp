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

#include "package/host/native/native_request.hpp"
#include "package/include/native/api.hpp"

namespace axcl::pkg::host {

class dsp_request final : public native_request {
public:
    explicit dsp_request();
    ~dsp_request() override;

    dsp_request(dsp_request &&other) noexcept;
    dsp_request &operator=(dsp_request &&other) noexcept;
    dsp_request(const dsp_request &) = delete;
    dsp_request &operator=(const dsp_request &) = delete;

    template <typename... Args>
    [[nodiscard]] bool pack(const native::DSP_API api, Args... args) {
        if (!set_api(api)) {
            return false;
        }

        return m_args.set_args(args...);
    }

    [[nodiscard]] axcl::pkg::native::DSP_API get_api() const;

protected:
    [[nodiscard]] bool add_data(const void *data, size_t size) override;
    [[nodiscard]] bool set_api(axcl::pkg::native::DSP_API api) const;

private:
    struct impl;
    impl *m_impl;
};

}  // namespace axcl::pkg::host
