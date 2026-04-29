/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "engine_request_imp.hpp"

axcl::pkg::device::engine_request::engine_request() : m_impl(new impl(select(runtime::module::ENGINE))) {
}

axcl::pkg::device::engine_request::engine_request(engine_request&& other) noexcept
    : runtime_request(std::move(other)),
      m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::device::engine_request& axcl::pkg::device::engine_request::operator=(engine_request&& other) noexcept {
    if (this != &other) {
        delete this->m_impl;
        this->m_impl = std::exchange(other.m_impl, nullptr);
    }
    return *this;
}

axcl::pkg::device::engine_request::~engine_request() {
    delete this->m_impl;
}

void axcl::pkg::device::engine_request::set_api(const runtime::engine::API& type) const {
    this->m_impl->set_api(type);
}

void axcl::pkg::device::engine_request::set_status(const int32_t status) const {
    this->m_impl->set_status(status);
}

void axcl::pkg::device::engine_request::set_param_npu_type(const runtime::engine::NPU type) const {
    this->m_impl->set_param_npu_type(type);
}

void axcl::pkg::device::engine_request::set_param_engine_version(const std::string& version) const {
    this->m_impl->set_param_engine_version(version);
}

void axcl::pkg::device::engine_request::set_param_handle(const uint64_t handle) const {
    this->m_impl->set_param_handle(handle);
}

void axcl::pkg::device::engine_request::set_param_model_version(const std::string& version) const {
    this->m_impl->set_param_model_version(version);
}

void axcl::pkg::device::engine_request::set_param_model_type(const runtime::engine::MODEL type) const {
    this->m_impl->set_param_model_type(type);
}

void axcl::pkg::device::engine_request::set_param_model_usage(const uint64_t sys, const uint64_t cmm) const {
    this->m_impl->set_param_model_usage(sys, cmm);
}

void axcl::pkg::device::engine_request::set_param_context(const uint64_t context) const {
    this->m_impl->set_param_context(context);
}

void axcl::pkg::device::engine_request::set_param_meta(const runtime::engine::meta& io) const {
    this->m_impl->set_param_meta(io);
}

void axcl::pkg::device::engine_request::set_param_affinity(const uint32_t affinity) const {
    this->m_impl->set_param_affinity(affinity);
}

void axcl::pkg::device::engine_request::set_param_elapsed(const std::vector<float>& elapsed) const {
    this->m_impl->set_param_elapsed(elapsed);
}
