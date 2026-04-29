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

#include <cstdint>
#include <memory>
#include <mutex>
#include <unordered_map>

namespace axcl::rt {

class context;
class stream;
class stream_manager {
public:
    stream_manager(context* ctx);

    stream* create_stream();
    bool destory_stream(stream* s, bool force);
    int32_t sync_stream(stream* s, int32_t timeout_ms);

    void sync();

    stream* create_default_stream();

    context* get_context() {
        return m_context;
    }

    stream* get_default_stream() {
        return m_default_stream;
    }

private:
    context* m_context;
    uint32_t m_stream_id;
    stream* m_default_stream;
    std::mutex m_mtx;
    std::unordered_map<uint32_t /* stream id */, std::shared_ptr<stream>> m_streams;
};

}  // namespace axcl::rt
