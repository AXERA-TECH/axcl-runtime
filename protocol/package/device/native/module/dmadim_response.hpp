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

#include "package/device/native/native_response.hpp"

namespace axcl::pkg::device {

class dmadim_response : public native_response {
public:
    explicit dmadim_response();
    virtual ~dmadim_response();

    dmadim_response(dmadim_response&& other) noexcept;
    dmadim_response& operator=(dmadim_response&& other) noexcept;
    dmadim_response(const dmadim_response&) = delete;
    dmadim_response& operator=(const dmadim_response&) = delete;

    [[nodiscard]] bool decode(const void* message, uint32_t size) const override;

    axcl::pkg::native::DMADIM_API get_api() const;

    template <typename... Args>
    [[nodiscard]] bool unpack(axcl::pkg::native::DMADIM_API api, Args... args) {
        axcl::pkg::native::API native_api;
        native_api.dmadim = api;
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

}  // namespace axcl::pkg::device
