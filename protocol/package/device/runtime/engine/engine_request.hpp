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

#include "package/device/runtime/runtime_request.hpp"
#include "package/include/runtime/engine/type.hpp"

namespace axcl::pkg::device
{
    class engine_request final : public runtime_request
    {
    public:
        explicit engine_request();

        engine_request(engine_request&&) noexcept;
        engine_request& operator=(engine_request&&) noexcept;
        explicit engine_request(const engine_request&) = delete;
        engine_request& operator=(const engine_request&) = delete;

        ~engine_request() override;

        void set_api(const runtime::engine::API& type) const;

        void set_status(int32_t status) const;
        void set_param_npu_type(runtime::engine::NPU type) const;
        void set_param_engine_version(const std::string& version) const;
        void set_param_handle(uint64_t handle) const;
        void set_param_model_version(const std::string& version) const;
        void set_param_model_type(runtime::engine::MODEL type) const;
        void set_param_model_usage(uint64_t sys, uint64_t cmm) const;
        void set_param_context(uint64_t context) const;
        void set_param_meta(const runtime::engine::meta& io) const;
        void set_param_affinity(uint32_t affinity) const;
        void set_param_elapsed(const std::vector<float>& elapsed) const;

    private:
        struct impl;
        impl* m_impl;
    };
}
