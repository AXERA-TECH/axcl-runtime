/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "stream_manager.hpp"
#include "axcl_rt_stream.h"
#include "axcl_rt_control_internal.h"
#include "context/context.hpp"
#include "axcl_logger.hpp"
#include "stream/stream.hpp"

#define TAG "stream manager"

namespace axcl::rt {

stream_manager::stream_manager(context *ctx) : m_context(ctx), m_stream_id(1) {
    m_default_stream = create_default_stream();
}

stream *stream_manager::create_default_stream() {
    std::lock_guard<std::mutex> lck(m_mtx);
    auto sp = std::make_shared<stream>(this, m_stream_id);
    if (!sp) {
        LOG_MM_E(TAG, "create stream instance fail");
        return nullptr;
    }

    auto *s = sp.get();
    m_streams[m_stream_id] = sp;

    LOG_MM_I(TAG, "stream {} is created", m_stream_id);
    ++m_stream_id;
    return s;
}

stream *stream_manager::create_stream() {
    std::lock_guard<std::mutex> lck(m_mtx);
    auto sp = std::make_shared<stream>(this, m_stream_id);
    if (!sp) {
        LOG_MM_E(TAG, "create stream instance fail");
        return nullptr;
    }

    auto *s = sp.get();
    m_streams[m_stream_id] = sp;

    /* control: create stream */
    auto context_id = this->get_context()->get_context_id();
    if (axclError ret = axclrtControlCreateStream(context_id, static_cast<void*>(s)); ret != AXCL_SUCC) {
        LOG_MM_E(TAG, "control: create stream({}:{}) failed, ret {:#x}", context_id, m_stream_id, static_cast<uint32_t>(ret));
    }

    LOG_MM_I(TAG, "stream {} is created", m_stream_id);
    ++m_stream_id;
    return s;
}

bool stream_manager::destory_stream(stream *s, bool force) {
    if (s == m_default_stream) {
        LOG_MM_E(TAG, " default stream cannot be destoried");
        return false;
    }

    std::lock_guard<std::mutex> lck(m_mtx);
    auto stream_id = s->get_stream_id();
    auto it = m_streams.find(stream_id);
    if (m_streams.end() == it) {
        LOG_MM_E(TAG, "unknown stream {}", stream_id);
        return false;
    }

    /* control: destroy stream */
    auto context_id = this->get_context()->get_context_id();
    if (axclError ret = axclrtControlDestroyStream(context_id, static_cast<void*>(s)); ret != AXCL_SUCC) {
        LOG_MM_E(TAG, "control: create stream({}:{}) failed, ret {:#x}", context_id, stream_id, static_cast<uint32_t>(ret));
    }

    if (!force) {
        if (!s->sync()) {
            return false;
        }
    }

    m_streams.erase(it);

    LOG_MM_I(TAG, "stream {} is destoried", stream_id);
    return true;
}

int32_t stream_manager::sync_stream(stream* s, int32_t timeout_ms) {

    if (s == m_default_stream) {
        LOG_MM_E(TAG, "default stream cannot be synced");
        return -1;
    }

    std::lock_guard<std::mutex> lck(m_mtx);
    auto stream_id = s->get_stream_id();
    auto it = m_streams.find(stream_id);
    if (m_streams.end() == it) {
        LOG_MM_E(TAG, "unknown stream {}", stream_id);
        return -1;
    }

    /* control: sync stream */
    auto context_id = this->get_context()->get_context_id();
    axclError ret = axclrtControlSyncStream(context_id, static_cast<void*>(s), timeout_ms);
    if (ret != AXCL_SUCC) {
        LOG_MM_E(TAG, "control: sync stream({}:{}) failed, ret {:#x}", context_id, stream_id, static_cast<uint32_t>(ret));
    }

    // FIXME: need to sync?
    // if (!s->sync(timeout_ms /* + ?ms*/)) {
    //     return -1;
    // }
    //LOG_MM_I(TAG, "stream {} is synced", stream_id);

    return ret;
}

void stream_manager::sync() {
    std::lock_guard<std::mutex> lck(m_mtx);
    for (auto &&m : m_streams) {
        m.second->sync(-1);
    }
}

}  // namespace axcl::rt