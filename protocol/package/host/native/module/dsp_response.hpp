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

class dsp_response final : public native_response {
public:
    explicit dsp_response();
    ~dsp_response() override;

    dsp_response(dsp_response&& other) noexcept;
    dsp_response& operator=(dsp_response&& other) noexcept;
    dsp_response(const dsp_response&) = delete;
    dsp_response& operator=(const dsp_response&) = delete;

    [[nodiscard]] bool decode(const void* message, uint32_t size) const override;

    native::DSP_API get_api() const;

    template <typename... Args>
    [[nodiscard]] bool unpack(const axcl::pkg::native::DSP_API api, Args... args) {
        native::API native_api{};
        native_api.dsp = api;
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
