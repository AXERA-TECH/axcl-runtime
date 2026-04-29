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
#include "axcl_logger.hpp"

#define TAG "package"

namespace axcl::pkg::host {
struct engine_response::impl {
    [[nodiscard]] bool check_payload() {
        if (!this->m_payload->has_head() || !this->m_payload->has_body()) {
            LOG_MM_E(TAG, "engine runtime response check payload fail, has head {} has body {}", this->m_payload->has_head(),
                     this->m_payload->has_body());
            return false;
        }

        auto status = true;
        const auto head = this->m_payload->head();
        const auto body = this->m_payload->body();

        switch (head.api()) {
            case axcl::runtime::engine::ApiKind::InitNpu:
                this->m_api = runtime::engine::API::INIT;
                break;
            case axcl::runtime::engine::ApiKind::FinalNpu:
                this->m_api = runtime::engine::API::FINAL;
                break;
            case axcl::runtime::engine::ApiKind::UnloadModel:
                this->m_api = runtime::engine::API::UNLOAD;
                break;
            case axcl::runtime::engine::ApiKind::RunModel:
                this->m_api = runtime::engine::API::RUN;
                break;
            case axcl::runtime::engine::ApiKind::SetAffinity:
                this->m_api = runtime::engine::API::SET_AFFINITY;
                break;
            case axcl::runtime::engine::ApiKind::GetAffinity:
                this->m_api = runtime::engine::API::GET_AFFINITY;
                status = (body.has_affinity() && 0 != body.affinity());
                break;
            case axcl::runtime::engine::ApiKind::GetNpuType:
                this->m_api = runtime::engine::API::GET_NPU_TYPE;
                status = body.has_npu_kind();
                break;
            case axcl::runtime::engine::ApiKind::GetEngineVersion:
                this->m_api = runtime::engine::API::GET_ENGINE_VERSION;
                status = body.has_engine_version() && !body.engine_version().empty();
                break;
            case axcl::runtime::engine::ApiKind::LoadModel:
                this->m_api = runtime::engine::API::LOAD;
                status = body.has_handle() && 0 != body.handle();
                break;
            case axcl::runtime::engine::ApiKind::GetModelVersion:
                this->m_api = runtime::engine::API::GET_VERSION;
                status = body.has_model_version() && !body.model_version().empty();
                break;
            case axcl::runtime::engine::ApiKind::GetModelType:
                this->m_api = runtime::engine::API::GET_TYPE;
                status = body.has_model_kind();
                break;
            case axcl::runtime::engine::ApiKind::GetModelIoInfo:
                this->m_api = runtime::engine::API::GET_INFO;
                status = 0 != body.meta().inputs_size() && 0 != body.meta().outputs_size();
                break;
            case axcl::runtime::engine::ApiKind::GetMemoryUsage:
                this->m_api = runtime::engine::API::GET_USAGE;
                status = body.has_sys() && body.has_cmm();
                break;
            case axcl::runtime::engine::ApiKind::CreateContext:
                this->m_api = runtime::engine::API::CREATE_CONTEXT;
                status = body.has_context() && 0 != body.context();
                break;
            case axcl::runtime::engine::ApiKind::P2P:
                this->m_api = runtime::engine::API::P2P;
                break;
            default:
                LOG_MM_E(TAG, "unknown api {} of engine runtime response", static_cast<int32_t>(head.api()));
                break;
        }
        return status;
    }

    [[nodiscard]] bool select(axcl::runtime::response::Payload *payload) {
        if (axcl::runtime::ENGINE != payload->head().module()) {
            LOG_MM_E(TAG,  "current module {} is not engine module", static_cast<int32_t>(payload->head().module()));
            AXCL_ASSERT(0, "current module %d is not engine module", static_cast<int32_t>(payload->head().module()));
            return false;
        }

        m_payload = payload->mutable_body()->mutable_engine();
        AXCL_ASSERT(m_payload, "nil pointer");

        return check_payload();
    }

    [[nodiscard]] runtime::engine::API get_type() const {
        return this->m_api;
    }

    [[nodiscard]] int32_t get_status() const {
        return this->m_payload->body().status();
    }

    [[nodiscard]] runtime::engine::NPU get_param_npu_type() const {
        return static_cast<runtime::engine::NPU>(this->m_payload->body().npu_kind());
    }

    [[nodiscard]] std::string get_param_engine_version() const {
        return this->m_payload->body().engine_version();
    }

    [[nodiscard]] uint64_t get_param_handle() const {
        return this->m_payload->body().handle();
    }

    [[nodiscard]] std::string get_param_model_version() const {
        return this->m_payload->body().model_version();
    }

    [[nodiscard]] runtime::engine::MODEL get_param_model_type() const {
        return static_cast<runtime::engine::MODEL>(this->m_payload->body().model_kind());
    }

    [[nodiscard]] uint64_t get_param_sys_usage() const {
        return this->m_payload->body().sys();
    }

    [[nodiscard]] uint64_t get_param_cmm_usage() const {
        return this->m_payload->body().cmm();
    }

    [[nodiscard]] uint64_t get_param_context() const {
        return this->m_payload->body().context();
    }

    [[nodiscard]] runtime::engine::meta get_param_meta() const {
        const auto &meta_fields = this->m_payload->body().meta();
        const auto &input_src = meta_fields.inputs();
        const auto &output_src = meta_fields.outputs();

        runtime::engine::meta meta;
        meta.inputs.resize(input_src.size());
        meta.outputs.resize(output_src.size());

        for (int i = 0; i < input_src.size(); ++i) {
            meta.inputs[i].name = input_src[i].name();
            meta.inputs[i].layout = static_cast<runtime::engine::LAYOUT>(input_src[i].layout());
            meta.inputs[i].format = static_cast<runtime::engine::FORMAT>(input_src[i].format());
            meta.inputs[i].groups.resize(input_src[i].groups_size());

            for (int j = 0; j < input_src[i].groups_size(); ++j) {
                meta.inputs[i].groups[j].size = input_src[i].groups(j).size();
                meta.inputs[i].groups[j].shape.resize(input_src[i].groups(j).shape_size());
                meta.inputs[i].groups[j].stride.resize(input_src[i].groups(j).stride_size());
                for (int k = 0; k < input_src[i].groups(j).shape_size(); ++k) {
                    meta.inputs[i].groups[j].shape[k] = input_src[i].groups(j).shape(k);
                }
                for (int k = 0; k < input_src[i].groups(j).stride_size(); ++k) {
                    meta.inputs[i].groups[j].stride[k] = input_src[i].groups(j).stride(k);
                }
            }
        }
        for (int i = 0; i < output_src.size(); ++i) {
            meta.outputs[i].name = output_src[i].name();
            meta.outputs[i].layout = static_cast<runtime::engine::LAYOUT>(output_src[i].layout());
            meta.outputs[i].format = static_cast<runtime::engine::FORMAT>(output_src[i].format());
            meta.outputs[i].groups.resize(output_src[i].groups_size());

            for (int j = 0; j < output_src[i].groups_size(); ++j) {
                meta.outputs[i].groups[j].size = output_src[i].groups(j).size();
                meta.outputs[i].groups[j].shape.resize(output_src[i].groups(j).shape_size());
                meta.outputs[i].groups[j].stride.resize(output_src[i].groups(j).stride_size());
                for (int k = 0; k < output_src[i].groups(j).shape_size(); ++k) {
                    meta.outputs[i].groups[j].shape[k] = output_src[i].groups(j).shape(k);
                }
                for (int k = 0; k < output_src[i].groups(j).stride_size(); ++k) {
                    meta.outputs[i].groups[j].stride[k] = output_src[i].groups(j).stride(k);
                }
            }
        }

        return meta;
    }

    [[nodiscard]] uint32_t get_param_affinity() const {
        const auto body = this->m_payload->body();
        return this->m_payload->body().affinity();
    }

    [[nodiscard]] std::vector<float> get_param_elapsed() const {
        const auto body = this->m_payload->body();
        std::vector<float> elapsed;
        elapsed.resize(body.elapsed_size());
        for (int i = 0; i < body.elapsed_size(); ++i) {
            elapsed[i] = body.elapsed(i);
        }
        return elapsed;
    }

    runtime::engine::API m_api{};
    axcl::runtime::engine::response::Payload *m_payload{};
};
}  // namespace axcl::pkg::host
