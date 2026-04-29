/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "context_manager.hpp"
#include "context.hpp"
#include "axcl_logger.hpp"
#include "../stream/stream.hpp"
#include "../stream/stream_manager.hpp"

#define TAG "context manager"

namespace axcl::worker {

context_manager::context_manager() {
}

context *context_manager::create_context(uint32_t context_id) {

    std::lock_guard<std::mutex> lck(m_mtx);

    auto it = m_contexts.find(context_id);
    if (m_contexts.end() != it) {
        LOG_MM_E(TAG, "Context({}) already exists, cannot create duplicate", context_id);
        return nullptr;
    }

    auto sp = std::make_shared<context>(this, context_id);
    if (!sp) {
        LOG_MM_E(TAG, "Failed to create context({}) instance", context_id);
        return nullptr;
    }

    auto *ctx = sp.get();
    m_contexts[context_id] = sp;

    return ctx;
}

bool context_manager::destroy_context(uint32_t context_id) {
    std::lock_guard<std::mutex> lck(m_mtx);

    auto it = m_contexts.find(context_id);
    if (m_contexts.end() == it) {
        LOG_MM_E(TAG, "Context({}) not found, cannot destroy", context_id);
        return false;
    }

    m_contexts.erase(it);

    return true;
}

context* context_manager::get_context(uint32_t context_id) {
    std::lock_guard<std::mutex> lck(m_mtx);

    auto it = m_contexts.find(context_id);
    if (m_contexts.end() == it) {
        // LOG_MM_E(TAG, "Context({}) not found", context_id);
        return nullptr;
    }

    return it->second.get();
}

stream* context_manager::get_or_create_stream(uint32_t context_id, uint32_t stream_id) {
    std::lock_guard<std::mutex> lck(m_mtx);

    auto it = m_contexts.find(context_id);
    context* ctx = nullptr;
    if (m_contexts.end() != it) {
        ctx = it->second.get();
    } else {
        auto sp = std::make_shared<context>(this, context_id);
        if (!sp) {
            LOG_MM_E(TAG, "Failed to create context({}) instance", context_id);
            return nullptr;
        }

        ctx = sp.get();
        m_contexts[context_id] = sp;
    }

    stream_manager* sm = ctx->get_stream_manager();
    if (!sm) {
        LOG_MM_E(TAG, "Invalid stream manager for context({})", context_id);
        return nullptr;
    }

    return sm->get_or_create_stream(stream_id);
}

}  // namespace axcl::worker