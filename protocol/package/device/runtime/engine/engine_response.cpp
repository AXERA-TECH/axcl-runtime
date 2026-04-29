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

axcl::pkg::device::engine_response::engine_response()
    : m_impl(new impl) {
}

axcl::pkg::device::engine_response::engine_response(engine_response&& other) noexcept
    : m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::device::engine_response& axcl::pkg::device::engine_response::operator=(engine_response&& other) noexcept {
    if (this != &other) {
        delete this->m_impl;
        this->m_impl = std::exchange(other.m_impl, nullptr);
    }
    return *this;
}

axcl::pkg::device::engine_response::~engine_response() {
    delete this->m_impl;
}

bool axcl::pkg::device::engine_response::decode(const void* message, const uint32_t size) const {
    if (!runtime_response::decode(message, size)) {
        return false;
    }

    return m_impl->select(static_cast<axcl::runtime::request::Payload*>(runtime_response::get_payload()));
}

axcl::pkg::runtime::engine::API axcl::pkg::device::engine_response::get_api() const {
    return this->m_impl->get_api();
}

axcl::pkg::runtime::engine::NPU axcl::pkg::device::engine_response::get_param_npu_type() const {
    return this->m_impl->get_param_npu_type();
}

uint64_t axcl::pkg::device::engine_response::get_param_handle() const {
    return this->m_impl->get_param_handle();
}

uint64_t axcl::pkg::device::engine_response::get_param_context() const {
    return this->m_impl->get_param_context();
}

void* axcl::pkg::device::engine_response::get_param_address() const {
    return this->m_impl->get_param_address();
}

uint64_t axcl::pkg::device::engine_response::get_param_size() const {
    return this->m_impl->get_param_size();
}

uint32_t axcl::pkg::device::engine_response::get_param_group() const {
    return this->m_impl->get_param_group();
}

axcl::pkg::runtime::engine::desc axcl::pkg::device::engine_response::get_param_io() const {
    return this->m_impl->get_param_io();
}

uint32_t axcl::pkg::device::engine_response::get_param_affinity() const {
    return this->m_impl->get_param_affinity();
}

uint32_t axcl::pkg::device::engine_response::get_noresp_flag() const {
    return this->m_impl->get_noresp_flag();
}