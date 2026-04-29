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

class npu_request final : public native_request {
public:
    explicit npu_request();
    ~npu_request() override;

    npu_request(npu_request &&other) noexcept;
    npu_request &operator=(npu_request &&other) noexcept;
    npu_request(const npu_request &) = delete;
    npu_request &operator=(const npu_request &) = delete;

    template <typename... Args>
    [[nodiscard]] bool pack(const native::ENGINE_API api, Args... args) {
        if (!set_api(api)) {
            return false;
        }

        return m_args.set_args(args...);
    }

    axcl::pkg::native::ENGINE_API get_api() const;

protected:
    bool add_data(const void *data, size_t size) override;
    bool set_api(axcl::pkg::native::ENGINE_API api);

private:
    struct impl;
    impl *m_impl;
};

}  // namespace axcl::pkg::host
