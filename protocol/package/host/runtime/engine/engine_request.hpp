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

#include "package/host/runtime/runtime_request.hpp"
#include "package/include/runtime/engine/type.hpp"


namespace axcl::pkg::host {
    class engine_request final : public runtime_request {
    public:
        explicit engine_request();

        engine_request(engine_request&&) noexcept;
        engine_request& operator=(engine_request&&) noexcept;
        explicit engine_request(const engine_request&) = delete;
        engine_request& operator=(const engine_request&) = delete;

        ~engine_request() override;

        [[nodiscard]] runtime::engine::API get_api() const;

        void init_npu(runtime::engine::NPU type) const;
        void final_npu() const;
        void get_npu_type() const;
        void get_engine_version() const;
        void load(const void* address, uint64_t size) const;
        void get_version(uint64_t handle) const;
        void get_type(uint64_t handle) const;
        void get_usage(uint64_t handle) const;
        void unload(uint64_t handle) const;
        void create_context(uint64_t handle) const;
        void get_info(uint64_t handle) const;
        void run(uint64_t handle, uint64_t context, uint32_t group, const runtime::engine::desc& io, uint32_t noresp=0) const;
        void set_affinity(uint64_t handle, uint32_t affinity) const;
        void get_affinity(uint64_t handle) const;
        void start_p2p(uint32_t group, uint64_t size) const;

        [[nodiscard]] void* encode(const head& head, uint32_t& size) const override;

    private:
        struct impl;
        impl* m_impl;
    };
}
