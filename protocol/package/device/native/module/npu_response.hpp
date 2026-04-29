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

class npu_response final : public native_response {
public:
    explicit npu_response();
    ~npu_response() override;

    npu_response(npu_response&& other) noexcept;
    npu_response& operator=(npu_response&& other) noexcept;
    npu_response(const npu_response&) = delete;
    npu_response& operator=(const npu_response&) = delete;

    [[nodiscard]] bool decode(const void* message, uint32_t size) const override;

    native::ENGINE_API get_api() const;

    template <typename... Args>
    [[nodiscard]] bool unpack(const native::ENGINE_API api, Args... args) {
        native::API native_api{};
        native_api.engine = api;
        return native_response::unpack(native_api, args...);
    }

protected:
    void* get_payload() const override;
    bool check_api(native::API api) override;
    size_t get_data_size(int index) override;
    bool get_data(int index, void* data, size_t size) override;
    const std::string& get_data(int index) override;

private:
    struct impl;
    impl* m_impl;
};

}  // namespace axcl::pkg::device
