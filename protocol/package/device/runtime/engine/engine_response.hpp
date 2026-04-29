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

#include "package/device/runtime/runtime_response.hpp"
#include "package/include/runtime/engine/type.hpp"

namespace axcl::pkg::device {

class engine_response final : public runtime_response {
public:
    explicit engine_response();

    engine_response(engine_response&&) noexcept;
    engine_response& operator=(engine_response&&) noexcept;
    engine_response(const engine_response&) = delete;
    engine_response& operator=(const engine_response&) = delete;

    ~engine_response() override;

    [[nodiscard]] bool decode(const void* message, uint32_t size) const override;

    [[nodiscard]] runtime::engine::API get_api() const;

    [[nodiscard]] runtime::engine::NPU get_param_npu_type() const;
    [[nodiscard]] void* get_param_address() const;
    [[nodiscard]] uint64_t get_param_size() const;
    [[nodiscard]] uint64_t get_param_handle() const;
    [[nodiscard]] uint64_t get_param_context() const;
    [[nodiscard]] uint32_t get_param_group() const;
    [[nodiscard]] runtime::engine::desc get_param_io() const;
    [[nodiscard]] uint32_t get_param_affinity() const;
    [[nodiscard]] uint32_t get_noresp_flag() const;

private:
    struct impl;
    impl* m_impl;
};

} // namespace axcl::pkg::device
