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

#include "engine_response.hpp"

#include "protocol.hpp"
#include "axcl_assert.hpp"
#include "enum_cast.hpp"
#include "axcl_logger.hpp"

#define TAG "package"

struct axcl::pkg::device::engine_response::impl
{
    [[nodiscard]] bool check_payload()
    {
        auto status = this->m_payload->has_head() && this->m_payload->has_body();
        if (!status)
        {
            LOG_MM_E(TAG, "engine runtime response check payload fail, has head {} has body {}", this->m_payload->has_head(),
                     this->m_payload->has_body());
            return status;
        }

        const auto head = this->m_payload->head();
        const auto body = this->m_payload->body();

        switch (head.api())
        {
        case axcl::runtime::engine::ApiKind::InitNpu:
            this->m_type = runtime::engine::API::INIT;
            status = (this->m_payload->body().has_npu_kind());
            break;
        case axcl::runtime::engine::ApiKind::FinalNpu:
            this->m_type = runtime::engine::API::FINAL;
            break;
        case axcl::runtime::engine::ApiKind::GetNpuType:
            this->m_type = runtime::engine::API::GET_NPU_TYPE;
            break;
        case axcl::runtime::engine::ApiKind::GetEngineVersion:
            this->m_type = runtime::engine::API::GET_ENGINE_VERSION;
            break;
        case axcl::runtime::engine::ApiKind::LoadModel:
            this->m_type = runtime::engine::API::LOAD;
            status = (this->m_payload->body().has_address()
                && 0 != this->m_payload->body().address()
                && this->m_payload->body().has_size()
                && 0 != this->m_payload->body().size());
            break;
        case axcl::runtime::engine::ApiKind::GetModelVersion:
            this->m_type = runtime::engine::API::GET_VERSION;
            status = (this->m_payload->body().has_handle() && 0 != this->m_payload->body().handle());
            break;
        case axcl::runtime::engine::ApiKind::GetModelType:
            this->m_type = runtime::engine::API::GET_TYPE;
            status = (this->m_payload->body().has_handle() && 0 != this->m_payload->body().handle());
            break;
        case axcl::runtime::engine::ApiKind::GetModelIoInfo:
            this->m_type = runtime::engine::API::GET_INFO;
            status = this->m_payload->body().has_handle()
                && 0 != this->m_payload->body().handle();
            break;
        case axcl::runtime::engine::ApiKind::GetMemoryUsage:
            this->m_type = runtime::engine::API::GET_USAGE;
            status = (this->m_payload->body().has_handle() && 0 != this->m_payload->body().handle());
            break;
        case axcl::runtime::engine::ApiKind::UnloadModel:
            this->m_type = runtime::engine::API::UNLOAD;
            status = (this->m_payload->body().has_handle() && 0 != this->m_payload->body().handle());
            break;
        case axcl::runtime::engine::ApiKind::CreateContext:
            this->m_type = runtime::engine::API::CREATE_CONTEXT;
            status = (this->m_payload->body().has_handle() && 0 != this->m_payload->body().handle());
            break;
        case axcl::runtime::engine::ApiKind::RunModel:
            this->m_type = runtime::engine::API::RUN;
            status = (this->m_payload->body().has_handle()
                && 0 != this->m_payload->body().handle()
                && this->m_payload->body().has_context()
                && 0 != this->m_payload->body().context()
                && this->m_payload->body().has_group()
                && this->m_payload->body().io().inputs_size() > 0
                && this->m_payload->body().io().outputs_size() > 0);
            break;
        case axcl::runtime::engine::ApiKind::SetAffinity:
            this->m_type = runtime::engine::API::SET_AFFINITY;
            status = (this->m_payload->body().has_handle()
                && 0 != this->m_payload->body().handle()
                && this->m_payload->body().has_affinity()
                && 0 != this->m_payload->body().affinity());
            break;
        case axcl::runtime::engine::ApiKind::GetAffinity:
            this->m_type = runtime::engine::API::GET_AFFINITY;
            status = (this->m_payload->body().has_handle()
                && 0 != this->m_payload->body().handle());
            break;
        case axcl::runtime::engine::ApiKind::P2P:
            this->m_type = runtime::engine::API::P2P;
            status = this->m_payload->body().has_group()
                && this->m_payload->body().has_size();
            break;
        default:
            LOG_MM_E(TAG, "unknown api {} of engine runtime response", static_cast<int32_t>(head.api()));
            break;
        }

        return status;
    }

    [[nodiscard]] bool select(axcl::runtime::request::Payload* payload)
    {
        if (axcl::runtime::ENGINE != payload->head().module()) {
            LOG_MM_E(TAG,  "current module {} is not memory module", static_cast<int32_t>(payload->head().module()));
            AXCL_ASSERT(0, "current module %d is not engine module", static_cast<int32_t>(payload->head().module()));
            return false;
        }

        this->m_payload = payload->mutable_body()->mutable_engine();
        return this->check_payload();
    }

    [[nodiscard]] runtime::engine::API get_api() const
    {
        return this->m_type;
    }

    [[nodiscard]] runtime::engine::NPU get_param_npu_type() const
    {
        return static_cast<runtime::engine::NPU>(this->m_payload->body().npu_kind());
    }

    [[nodiscard]] uint64_t get_param_handle() const
    {
        return this->m_payload->body().handle();
    }

    [[nodiscard]] uint64_t get_param_context() const
    {
        return this->m_payload->body().context();
    }

    [[nodiscard]] void* get_param_address() const
    {
        return reinterpret_cast<void*>(this->m_payload->body().address());
    }

    [[nodiscard]] uint64_t get_param_size() const
    {
        return this->m_payload->body().size();
    }

    [[nodiscard]] uint32_t get_param_group() const
    {
        return this->m_payload->body().group();
    }

    [[nodiscard]] runtime::engine::desc get_param_io() const
    {
        const auto& desc_src = this->m_payload->body().io();
        const auto& input_src = desc_src.inputs();
        const auto& output_src = desc_src.outputs();

        runtime::engine::desc io;
        io.batch = desc_src.batch();
        io.inputs.resize(input_src.size());
        io.outputs.resize(output_src.size());;
        auto& input_dst = io.inputs;
        auto& output_dst = io.outputs;

        for (int i = 0; i < input_src.size(); i++)
        {
            input_dst[i].address = input_src[i].address();
            input_dst[i].size = input_src[i].size();
        }

        for (int i = 0; i < output_src.size(); i++)
        {
            output_dst[i].address = output_src[i].address();
            output_dst[i].size = output_src[i].size();
        }

        return io;
    }

    [[nodiscard]] uint32_t get_param_affinity() const
    {
        return this->m_payload->body().affinity();
    }

    [[nodiscard]] int32_t get_noresp_flag() const {
        return this->m_payload->body().noresp();
    }

    runtime::engine::API m_type{};
    axcl::runtime::engine::request::Payload* m_payload{};
};
