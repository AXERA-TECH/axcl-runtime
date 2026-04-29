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

#include <memory>
#include <mutex>
#include <unordered_map>

namespace axcl::rt {

class device;
class context;
class context_manager {
public:
    context_manager(device *dev);

    void create_default();

    context *create_context();
    bool destory_context(context *ctx);

    void sync();

    bool bind_thread(const context *ctx);

    device *get_device() {
        return m_device;
    }

    const context *get_default_context() const {
        return m_default_context;
    }

    static const context *get_thread_context(uint32_t tid);

protected:
    static void bind(uint32_t tid, const context *ctx);
    static void unbind(const context *ctx);

private:
    device *m_device;
    uint32_t m_context_id;
    context *m_default_context;
    std::mutex m_mtx;
    std::unordered_map<uint32_t /* context id */, std::shared_ptr<context>> m_contexts;

    static std::mutex m_thread_context_map_mtx;
    static std::unordered_map<uint32_t /* thread id */, const context *> m_thread_context_map;
};

}  // namespace axcl::rt
