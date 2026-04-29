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

#include "package/host/native/native_response.hpp"

namespace axcl::pkg::host {

class vdec_response : public native_response {
public:
    explicit vdec_response();
    virtual ~vdec_response();

    vdec_response(vdec_response&& other) noexcept;
    vdec_response& operator=(vdec_response&& other) noexcept;
    vdec_response(const vdec_response&) = delete;
    vdec_response& operator=(const vdec_response&) = delete;

    [[nodiscard]] bool decode(const void* message, uint32_t size) const override;

    axcl::pkg::native::VDEC_API get_api() const;

    template <typename... Args>
    [[nodiscard]] bool unpack(axcl::pkg::native::VDEC_API api, Args... args) {
        axcl::pkg::native::API native_api;
        native_api.vdec = api;
        return native_response::unpack(native_api, args...);
    }

protected:
    void* get_payload() const override;
    bool check_api(axcl::pkg::native::API api) override;
    size_t get_data_size(int index) override;
    bool get_data(int index, void* data, size_t size) override;
    const std::string& get_data(int index) override;

private:
    struct impl;
    impl* m_impl;
};

}  // namespace axcl::pkg::host
