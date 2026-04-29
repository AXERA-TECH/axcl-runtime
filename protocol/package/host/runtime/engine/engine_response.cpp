/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "engine_response_impl.hpp"

#include "protocol.hpp"
#include "axcl_logger.hpp"

namespace axcl::pkg::host {
    engine_response::engine_response() : m_impl(new impl) {
    }

    engine_response::engine_response(engine_response &&other) noexcept
        : m_impl(std::exchange(other.m_impl, nullptr)) {
    }

    engine_response &engine_response::operator=(engine_response &&other) noexcept {
        if (this != &other) {
            delete this->m_impl;
            this->m_impl = std::exchange(other.m_impl, nullptr);
        }
        return *this;
    }

    engine_response::~engine_response() {
        delete this->m_impl;
    }

    [[nodiscard]] bool engine_response::decode(const void *message, const uint32_t size) const {
        if (!runtime_response::decode(message, size)) {
            return false;
        }

        return m_impl->select(static_cast<axcl::runtime::response::Payload *>(runtime_response::get_payload()));
    }

    runtime::engine::API engine_response::get_api_type() const {
        return this->m_impl->get_type();
    }

    int32_t engine_response::get_status() const {
        return this->m_impl->get_status();
    }

    runtime::engine::NPU engine_response::get_param_npu_type() const {
        return this->m_impl->get_param_npu_type();
    }

    std::string engine_response::get_param_engine_version() const {
        return this->m_impl->get_param_engine_version();
    }

    uint64_t engine_response::get_param_handle() const {
        return this->m_impl->get_param_handle();
    }

    std::string engine_response::get_param_model_version() const {
        return this->m_impl->get_param_model_version();
    }

    runtime::engine::MODEL engine_response::get_param_model_type() const {
        return this->m_impl->get_param_model_type();
    }

    uint64_t engine_response::get_param_sys_usage() const {
        return this->m_impl->get_param_sys_usage();
    }

    uint64_t engine_response::get_param_cmm_usage() const {
        return this->m_impl->get_param_cmm_usage();
    }

    uint64_t engine_response::get_param_context() const {
        return this->m_impl->get_param_context();
    }

    runtime::engine::meta engine_response::get_param_meta() const {
        return this->m_impl->get_param_meta();
    }

    uint32_t engine_response::get_param_affinity() const {
        return this->m_impl->get_param_affinity();
    }

    std::vector<float> engine_response::get_param_elapsed() const {
        return this->m_impl->get_param_elapsed();
    }
}
