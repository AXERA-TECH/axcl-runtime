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
#include "axcl_rt_context.h"
#include "axcl_rt_control_internal.h"
#include "context/context.hpp"
#include "device/device.hpp"
#include "axcl_logger.hpp"
#include "os.hpp"
#include <atomic>
#include <chrono>
#if defined(__linux__) && defined(__GLIBC__)
#include <malloc.h>
#endif

#define TAG "context manager"

namespace axcl::rt {

std::mutex context_manager::m_thread_context_map_mtx;
std::unordered_map<uint32_t /* thread id */, const context *> context_manager::m_thread_context_map;

context_manager::context_manager(device *dev) : m_device(dev), m_context_id(1) {
    m_default_context = nullptr;
}

void context_manager::create_default() {
    if (!m_default_context) {
        m_default_context = create_context();
    }
}

context *context_manager::create_context() {
    std::lock_guard<std::mutex> lck(m_mtx);

    auto tid = gettid();
    auto sp = std::make_shared<context>(this, m_context_id);
    if (!sp) {
        LOG_MM_E(TAG, "create context instance fail");
        return nullptr;
    }

    LOG_MM_I(TAG, "context {} is created, bind to thread {}", m_context_id, tid);

    auto *ctx = sp.get();
    m_contexts[m_context_id] = sp;

    context_manager::bind(tid, ctx);

    /* control: create context */
    if (axclError ret = axclrtControlCreateContext(static_cast<int32_t>(m_context_id)); ret != AXCL_SUCC) {
        LOG_MM_E(TAG, "control: create context({}) failed, ret {:#x}", m_context_id, static_cast<uint32_t>(ret));
    }

    ++m_context_id;

    return ctx;
}

bool context_manager::destory_context(context *ctx) {
    std::lock_guard<std::mutex> lck(m_mtx);

    auto context_id = ctx->get_context_id();
    auto it = m_contexts.find(context_id);
    if (m_contexts.end() == it) {
        LOG_MM_E(TAG, "unknown context {}", context_id);
        return false;
    }

    if (ctx == m_default_context) {
        LOG_MM_E(TAG, "default context cannot be destoried");
        return false;
    }

    /* wait all tasks are finished */
    ctx->sync();

    /* control: destroy context */
    if (axclError ret = axclrtControlDestroyContext(static_cast<int32_t>(context_id)); ret != AXCL_SUCC) {
        LOG_MM_E(TAG, "control: destroy context({}) failed, ret {:#x}", context_id, static_cast<uint32_t>(ret));
    }

    /* unbind all threads */
    context_manager::unbind(ctx);

    m_contexts.erase(it);

    malloc_trim(0);

    LOG_MM_I(TAG, "context {} is destoried", context_id);

    return true;
}

void context_manager::sync() {
    std::lock_guard<std::mutex> lck(m_mtx);
    for (auto &&m : m_contexts) {
        m.second->sync();
    }
}

bool context_manager::bind_thread(const context *ctx) {
    std::lock_guard<std::mutex> lck(m_mtx);

    auto context_id = ctx->get_context_id();
    auto it = m_contexts.find(context_id);
    if (m_contexts.end() == it) {
        LOG_MM_E(TAG, "unknown context {}", context_id);
        return false;
    }

    auto tid = gettid();
    context_manager::bind(tid, ctx);

    LOG_MM_I(TAG, "bind context {} to thread {}", context_id, tid);
    return true;
}

const context *context_manager::get_thread_context(uint32_t tid) {
    std::lock_guard<std::mutex> lck(m_thread_context_map_mtx);
    auto it = m_thread_context_map.find(tid);
    if (m_thread_context_map.end() == it) {
        return nullptr;
    }

    return it->second;
}

void context_manager::bind(uint32_t tid, const context *ctx) {
    std::lock_guard<std::mutex> lck(m_thread_context_map_mtx);
    m_thread_context_map[tid] = ctx;
}

void context_manager::unbind(const context *ctx) {
    std::lock_guard<std::mutex> lck(m_thread_context_map_mtx);
    for (auto m = m_thread_context_map.begin(); m != m_thread_context_map.end();) {
        if (m->second == ctx) {
            m = m_thread_context_map.erase(m);
        } else {
            ++m;
        }
    }
}

}  // namespace axcl::rt
