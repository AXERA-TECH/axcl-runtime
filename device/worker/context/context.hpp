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

#include <condition_variable>
#include <list>
#include <memory>
#include <mutex>
#include <unordered_map>

namespace axcl::worker {

class context_manager;
class stream_manager;

class context  {

public:
    context(context_manager* mgr, uint32_t context_id);
    virtual ~context();

    uint32_t get_context_id() const {
        return m_context_id;
    }

    context_manager* get_context_manager() {
        return m_context_manager;
    }

    stream_manager* get_stream_manager() {
        return m_stream_manager.get();
    }

private:
    context_manager* m_context_manager;
    uint32_t m_context_id;
    std::shared_ptr<stream_manager> m_stream_manager;
};

}  // namespace axcl::worker