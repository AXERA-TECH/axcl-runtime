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

#include "engine_request.hpp"

#include "protocol.hpp"
#include "axcl_logger.hpp"

namespace axcl::pkg::host
{
    struct engine_request::impl
    {
        explicit impl(void* payload)
            : m_payload(static_cast<axcl::runtime::engine::request::Payload*>(payload))
        {
            this->m_payload->mutable_head();
            this->m_payload->mutable_body();
        }

        [[nodiscard]] runtime::engine::API get_api() const
        {
            return static_cast<runtime::engine::API>(this->m_payload->head().api());
        }

        void init_npu(runtime::engine::NPU type) const
        {
            const auto head = this->m_payload->mutable_head();
            head->set_api(axcl::runtime::engine::ApiKind::InitNpu);

            const auto body = this->m_payload->mutable_body();
            body->set_npu_kind(static_cast<axcl::runtime::engine::NpuKind>(type));
        }

        void final_npu() const
        {
            const auto head = this->m_payload->mutable_head();
            head->set_api(axcl::runtime::engine::ApiKind::FinalNpu);
        }

        void get_npu_type() const
        {
            const auto head = this->m_payload->mutable_head();
            head->set_api(axcl::runtime::engine::ApiKind::GetNpuType);
        }

        void get_engine_version() const
        {
            const auto head = this->m_payload->mutable_head();
            head->set_api(axcl::runtime::engine::ApiKind::GetEngineVersion);
        }

        void load_model(const void *address, const uint64_t size) const
        {
            const auto head = this->m_payload->mutable_head();
            head->set_api(axcl::runtime::engine::ApiKind::LoadModel);

            const auto body = this->m_payload->mutable_body();
            body->set_address(reinterpret_cast<uint64_t>(address));
            body->set_size(size);
        }

        void get_model_version(const uint64_t handle) const
        {
            const auto head = this->m_payload->mutable_head();
            head->set_api(axcl::runtime::engine::ApiKind::GetModelVersion);

            const auto body = this->m_payload->mutable_body();
            body->set_handle(handle);
        }

        void get_model_type(const uint64_t handle) const
        {
            const auto head = this->m_payload->mutable_head();
            head->set_api(axcl::runtime::engine::ApiKind::GetModelType);

            const auto body = this->m_payload->mutable_body();
            body->set_handle(handle);
        }

        void get_model_usage(const uint64_t handle) const
        {
            const auto head = this->m_payload->mutable_head();
            head->set_api(axcl::runtime::engine::ApiKind::GetMemoryUsage);

            const auto body = this->m_payload->mutable_body();
            body->set_handle(handle);
        }

        void unload_model(const uint64_t handle) const
        {
            const auto head = this->m_payload->mutable_head();
            head->set_api(axcl::runtime::engine::ApiKind::UnloadModel);

            const auto body = this->m_payload->mutable_body();
            body->set_handle(handle);
        }

        auto create_context(const uint64_t handle) const -> void
        {
            const auto head = this->m_payload->mutable_head();
            head->set_api(axcl::runtime::engine::ApiKind::CreateContext);

            const auto body = this->m_payload->mutable_body();
            body->set_handle(handle);
        }

        void get_model_io_info(const uint64_t handle) const
        {
            const auto head = this->m_payload->mutable_head();
            head->set_api(axcl::runtime::engine::ApiKind::GetModelIoInfo);

            const auto body = this->m_payload->mutable_body();
            body->set_handle(handle);
        }

        void run(const uint64_t handle, const uint64_t context, const uint32_t group, const runtime::engine::desc& io, uint32_t noresp) const
        {
            const auto head = this->m_payload->mutable_head();
            head->set_api(axcl::runtime::engine::ApiKind::RunModel);

            const auto body = this->m_payload->mutable_body();
            body->set_handle(handle);
            body->set_context(context);
            body->set_group(group);
            body->set_noresp(noresp);

            const auto desc_dst = body->mutable_io();
            desc_dst->set_batch(io.batch);

            for (const auto& [_, address, size] : io.inputs)
            {
                const auto input_dst = desc_dst->add_inputs();
                input_dst->set_address(address);
                input_dst->set_size(size);
            }

            for (const auto& [_, address, size] : io.outputs)
            {
                const auto output_dst = desc_dst->add_outputs();
                output_dst->set_address(address);
                output_dst->set_size(size);
            }
        }

        void set_affinity(const uint64_t handle, const uint32_t affinity) const
        {
            const auto head = this->m_payload->mutable_head();
            head->set_api(axcl::runtime::engine::ApiKind::SetAffinity);

            const auto body = this->m_payload->mutable_body();
            body->set_handle(handle);
            body->set_affinity(affinity);
        }

        void get_affinity(const uint64_t handle) const
        {
            const auto head = this->m_payload->mutable_head();
            head->set_api(axcl::runtime::engine::ApiKind::GetAffinity);

            const auto body = this->m_payload->mutable_body();
            body->set_handle(handle);
        }

        void start_p2p(const uint32_t group, const uint64_t size) const
        {
            const auto head = this->m_payload->mutable_head();
            head->set_api(axcl::runtime::engine::ApiKind::P2P);

            const auto body = this->m_payload->mutable_body();
            body->set_group(group);
            body->set_size(size);
        }

        axcl::runtime::engine::request::Payload* m_payload;
    };
}
