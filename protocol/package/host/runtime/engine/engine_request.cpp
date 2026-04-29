/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
**************************************************************************************************/

#include "engine_request_impl.hpp"

#include "protocol.hpp"
#include "axcl_logger.hpp"

axcl::pkg::host::engine_request::engine_request()
    : m_impl(new impl(select(runtime::module::ENGINE))) {
}

axcl::pkg::host::engine_request::engine_request(engine_request &&other) noexcept
    : runtime_request(std::move(other)), m_impl(std::exchange(other.m_impl, nullptr)) {
}

axcl::pkg::host::engine_request &axcl::pkg::host::engine_request::operator=(engine_request &&other) noexcept {
    if (this != &other) {
        delete this->m_impl;
        this->m_impl = std::exchange(other.m_impl, nullptr);
    }
    return *this;
}

axcl::pkg::host::engine_request::~engine_request() {
    delete this->m_impl;
}

axcl::pkg::runtime::engine::API axcl::pkg::host::engine_request::get_api() const {
    return this->m_impl->get_api();
}

void axcl::pkg::host::engine_request::init_npu(const runtime::engine::NPU type) const {
    this->m_impl->init_npu(type);
}

void axcl::pkg::host::engine_request::final_npu() const {
    this->m_impl->final_npu();
}

void axcl::pkg::host::engine_request::get_npu_type() const {
    this->m_impl->get_npu_type();
}

void axcl::pkg::host::engine_request::get_engine_version() const {
    this->m_impl->get_engine_version();
}

void axcl::pkg::host::engine_request::load(const void *address, const uint64_t size) const {
    this->m_impl->load_model(address, size);
}

void axcl::pkg::host::engine_request::get_version(const uint64_t handle) const {
    this->m_impl->get_model_version(handle);
}

void axcl::pkg::host::engine_request::get_type(const uint64_t handle) const {
    this->m_impl->get_model_type(handle);
}

void axcl::pkg::host::engine_request::get_usage(const uint64_t handle) const {
    this->m_impl->get_model_usage(handle);
}

void axcl::pkg::host::engine_request::unload(const uint64_t handle) const {
    this->m_impl->unload_model(handle);
}

auto axcl::pkg::host::engine_request::create_context(const uint64_t handle) const -> void {
    this->m_impl->create_context(handle);
}

void axcl::pkg::host::engine_request::get_info(const uint64_t handle) const {
    this->m_impl->get_model_io_info(handle);
}

void axcl::pkg::host::engine_request::run(const uint64_t handle, const uint64_t context, const uint32_t group, const runtime::engine::desc &io, uint32_t noresp) const {
    this->m_impl->run(handle, context, group, io, noresp);
}

void axcl::pkg::host::engine_request::set_affinity(const uint64_t handle, const uint32_t affinity) const {
    this->m_impl->set_affinity(handle, affinity);
}

void axcl::pkg::host::engine_request::get_affinity(const uint64_t handle) const {
    this->m_impl->get_affinity(handle);
}

void axcl::pkg::host::engine_request::start_p2p(const uint32_t group, const uint64_t size) const {
    this->m_impl->start_p2p(group, size);
}

void *axcl::pkg::host::engine_request::encode(const head &head, uint32_t &size) const {
    return runtime_request::encode(head, size);
}
