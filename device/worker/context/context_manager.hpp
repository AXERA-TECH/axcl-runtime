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

class context;
class stream;
class context_manager {
public:
    context_manager();

    context* create_context(uint32_t context_id);
    bool destroy_context(uint32_t context_id);
    context* get_context(uint32_t context_id);
    stream* get_or_create_stream(uint32_t context_id, uint32_t stream_id);

private:
    std::mutex m_mtx;
    std::unordered_map<uint32_t /* context id */, std::shared_ptr<context>> m_contexts;
};

}  // namespace axcl::worker
