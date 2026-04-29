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

namespace axcl::worker {

class stream;
class stream_manager {
public:
    stream_manager();
    stream* create_stream(uint32_t stream_id, bool enable_timeout_monitor=true);
    stream* get_stream(uint32_t stream_id);
    stream* get_or_create_stream(uint32_t stream_id);
    bool destroy_stream(uint32_t stream_id);
    void destroy_all_streams();

private:
    std::mutex m_mtx;
    std::unordered_map<uint32_t /* stream id */, std::shared_ptr<stream>> m_streams;
};

}  // namespace axcl::worker
