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

#include <string>
#include "package/host/response.hpp"
#include "package/include/native/api.hpp"
#include "package/include/native/type.hpp"

namespace axcl::pkg::host {

class native_response : public response {
public:
    explicit native_response();
    virtual ~native_response();

    native_response(native_response&& other) noexcept;
    native_response& operator=(native_response&& other) noexcept;
    native_response(const native_response&) = delete;
    native_response& operator=(const native_response&) = delete;

    [[nodiscard]] bool decode(const void* message, uint32_t size) const override;

    axcl::pkg::native::module get_module() const;

    template <typename... Args>
    [[nodiscard]] bool unpack(axcl::pkg::native::API api, Args... args) {
        if (!check_api(api)) {
            return false;
        }

        return unpack(args...);
    }

protected:
    void* get_payload() const override;

    template <typename T, typename... Args>
    bool unpack(T* first, Args... args) {
        if (!get_arg(m_arg_index++, first)) {
            return false;
        }

        return unpack(args...);
    }

    bool unpack() {
        m_arg_index = 0;
        return true;
    }

    template <typename T>
    bool get_arg(int index, T* arg) {
        return get_data(index, arg, sizeof(T));
    }

    virtual bool check_api(axcl::pkg::native::API api);
    virtual size_t get_data_size(int index);
    virtual bool get_data(int index, void* data, size_t size);
    virtual const std::string& get_data(int index);

private:
    struct impl;
    impl* m_impl;

    mutable int m_arg_index = 0;
};

}  // namespace axcl::pkg::host

template <>
bool axcl::pkg::host::native_response::get_arg<axcl::pkg::uint8_array>(int index, axcl::pkg::uint8_array* arg);