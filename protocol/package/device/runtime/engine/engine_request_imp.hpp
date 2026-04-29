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

struct axcl::pkg::device::engine_request::impl {
    explicit impl() = delete;

    explicit impl(void* payload)
        : m_payload(static_cast<axcl::runtime::engine::response::Payload*>(payload)) {
        this->m_payload->mutable_head();
        this->m_payload->mutable_body();
    }

    void set_api(const runtime::engine::API& type) const {
        const auto head = this->m_payload->mutable_head();
        head->set_api(static_cast<axcl::runtime::engine::ApiKind>(type));
    }

    void set_status(const int32_t status) const {
        const auto body = this->m_payload->mutable_body();
        body->set_status(status);
    }

    void set_param_npu_type(const runtime::engine::NPU type) const {
        const auto body = this->m_payload->mutable_body();
        body->set_npu_kind(static_cast<axcl::runtime::engine::NpuKind>(type));
    }

    void set_param_engine_version(const std::string& version) const {
        const auto body = this->m_payload->mutable_body();
        body->set_engine_version(version);
    }

    void set_param_handle(const uint64_t handle) const {
        const auto body = this->m_payload->mutable_body();
        body->set_handle(handle);
    }

    void set_param_model_version(const std::string& version) const {
        const auto body = this->m_payload->mutable_body();
        body->set_model_version(version);
    }

    void set_param_model_type(const runtime::engine::MODEL type) const {
        const auto body = this->m_payload->mutable_body();
        body->set_model_kind(static_cast<axcl::runtime::engine::ModelKind>(type));
    }

    void set_param_model_usage(const uint64_t sys, const uint64_t cmm) const {
        const auto body = this->m_payload->mutable_body();
        body->set_sys(sys);
        body->set_cmm(cmm);
    }

    void set_param_context(const uint64_t context) const {
        const auto body = this->m_payload->mutable_body();
        body->set_context(context);
    }

    void set_param_meta(const runtime::engine::meta& io) const {
        const auto body = this->m_payload->mutable_body();
        const auto dst_meta = body->mutable_meta();

        const auto& inputs_src = io.inputs;
        const auto& outputs_src = io.outputs;

        for (const auto& [name, layout, format, groups] : inputs_src) {
            const auto input_dst = dst_meta->add_inputs();
            input_dst->set_name(name);
            input_dst->set_layout(static_cast<axcl::runtime::engine::LayoutKind>(layout));
            input_dst->set_format(static_cast<axcl::runtime::engine::FormatKind>(format));
            for (const auto& [size, shape, stride] : groups) {
                const auto group = input_dst->add_groups();
                group->set_size(size);
                for (const auto val : shape) {
                    group->add_shape(val);
                }
                for (const auto val : stride) {
                    group->add_stride(val);
                }
            }
        }

        for (const auto& [name, layout, format, groups] : outputs_src)
        {
            const auto output_dst = dst_meta->add_outputs();
            output_dst->set_name(name);
            output_dst->set_layout(static_cast<axcl::runtime::engine::LayoutKind>(layout));
            output_dst->set_format(static_cast<axcl::runtime::engine::FormatKind>(format));
            for (const auto& [size, shape, stride] : groups) {
                const auto group = output_dst->add_groups();
                group->set_size(size);
                for (const auto val : shape) {
                    group->add_shape(val);
                }
                for (const auto val : stride) {
                    group->add_stride(val);
                }
            }
        }
    }

    void set_param_affinity(const uint32_t affinity) const {
        const auto body = this->m_payload->mutable_body();
        body->set_affinity(affinity);
    }

    void set_param_elapsed(const std::vector<float>& elapsed) const {
        const auto body = this->m_payload->mutable_body();
        for (const auto val : elapsed) {
            body->add_elapsed(val);
        }
    }

    axcl::runtime::engine::response::Payload* m_payload;
};
