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

#include "package/host/runtime/runtime_response.hpp"
#include "package/include/runtime/engine/type.hpp"

namespace axcl::pkg::host {
    class engine_response final : public runtime_response {
    public:
        explicit engine_response();
        engine_response(engine_response&&) noexcept;
        engine_response& operator=(engine_response&&) noexcept;
        explicit engine_response(const engine_response&) = delete;
        engine_response& operator=(const engine_response&) = delete;

        ~engine_response() override;

        [[nodiscard]] bool decode(const void* message, uint32_t size) const override;

        [[nodiscard]] runtime::engine::API get_api_type() const;

        [[nodiscard]] int32_t get_status() const;

        [[nodiscard]] runtime::engine::NPU get_param_npu_type() const;
        [[nodiscard]] std::string get_param_engine_version() const;
        [[nodiscard]] uint64_t get_param_handle() const;
        [[nodiscard]] std::string get_param_model_version() const;
        [[nodiscard]] runtime::engine::MODEL get_param_model_type() const;
        [[nodiscard]] uint64_t get_param_sys_usage() const;
        [[nodiscard]] uint64_t get_param_cmm_usage() const;
        [[nodiscard]] uint64_t get_param_context() const;
        [[nodiscard]] runtime::engine::meta get_param_meta() const;
        [[nodiscard]] uint32_t get_param_affinity() const;
        [[nodiscard]] std::vector<float> get_param_elapsed() const;

    private:
        struct  impl;
        impl* m_impl;
    };
}
