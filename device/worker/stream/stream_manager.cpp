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
#include "stream.hpp"
#include "axcl_logger.hpp"

#define TAG "stream manager"

namespace axcl::worker {

stream_manager::stream_manager() {
}

stream *stream_manager::create_stream(uint32_t stream_id, bool enable_timeout_monitor) {
    std::lock_guard<std::mutex> lck(m_mtx);
    auto it = m_streams.find(stream_id);
    if (m_streams.end() != it) {
        LOG_MM_E(TAG, "stream({}) already exists, cannot create duplicate", stream_id);
        return nullptr;
    }

    auto sp = std::make_shared<stream>(this, stream_id, enable_timeout_monitor);
    if (!sp) {
        LOG_MM_E(TAG, "failed to create stream({}) instance", stream_id);
        return nullptr;
    }

    auto *s = sp.get();
    m_streams[stream_id] = sp;

    return s;
}

bool stream_manager::destroy_stream(uint32_t stream_id) {

    std::shared_ptr<stream> stream_to_destroy;
    {
        std::lock_guard<std::mutex> lck(m_mtx);
        auto it = m_streams.find(stream_id);
        if (m_streams.end() == it) {
            LOG_MM_E(TAG, "Stream({}) not found, cannot destroy", stream_id);
            return false;
        }

        stream_to_destroy = it->second;
        m_streams.erase(it);
        LOG_M_I(TAG, "stream({}) removed from manager", stream_id);
    }

    return true;
}

stream* stream_manager::get_stream(uint32_t stream_id) {
    std::lock_guard<std::mutex> lck(m_mtx);

    auto it = m_streams.find(stream_id);
    if (m_streams.end() == it) {
        // LOG_MM_W(TAG, "Stream({}) not found", stream_id);
        return nullptr;
    }

    if (!it->second->is_active()) {
        LOG_MM_E(TAG, "Stream({}) is not active", stream_id);
        return nullptr;
    }

    return it->second.get();
}

stream* stream_manager::get_or_create_stream(uint32_t stream_id) {
    std::lock_guard<std::mutex> lck(m_mtx);

    auto it = m_streams.find(stream_id);
    if (m_streams.end() != it) {
        if (!it->second->is_active()) {
            LOG_MM_E(TAG, "stream({}) is not active, will create new instance", stream_id);
            m_streams.erase(it);
        } else {
            LOG_MM_D(TAG, "Using existing stream({})", stream_id);
            return it->second.get();
        }
    }

    auto sp = std::make_shared<stream>(this, stream_id);
    if (!sp) {
        LOG_MM_E(TAG, "Failed to create stream({}) instance", stream_id);
        return nullptr;
    }

    auto *s = sp.get();
    m_streams[stream_id] = sp;

    return s;
}

void stream_manager::destroy_all_streams() {

    std::vector<std::shared_ptr<stream>> streams_to_destroy;
    {
        std::lock_guard<std::mutex> lck(m_mtx);
        if (m_streams.empty()) {
            LOG_MM_I(TAG, "No streams to destroy");
            return;
        }

        for (const auto& pair : m_streams) {
            streams_to_destroy.push_back(pair.second);
        }

        m_streams.clear();
        LOG_M_I(TAG, "all streams removed from manager");
    }

    LOG_M_I(TAG, "all streams will be destroyed when no longer referenced");
}

}  // namespace axcl::worker