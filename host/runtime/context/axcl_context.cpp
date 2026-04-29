/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "axcl_rt_context.h"
#include "axcl_rt_context_internal.h"
#include "axcl_rt_device.h"
#include "context/context.hpp"
#include "context/context_manager.hpp"
#include "device/device.hpp"
#include "device/device_manager.hpp"
#include "axcl_logger.hpp"
#include "os.hpp"

#define TAG "context"

AXCL_EXPORT axclError axclrtCreateContext(axclrtContext *context, int32_t deviceId) {
    if (!context) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_CONTEXT_NULL_POINTER;
    }

    /* active device if not actived */
    if (!DEVICE_MANAGER()->is_actived(deviceId)) {
        if (int32_t ret = DEVICE_MANAGER()->active(deviceId); AXCL_SUCC != ret) {
            return ret;
        }
    } else {
        DEVICE_MANAGER()->bind_thread(gettid(), deviceId);
    }

    *context = reinterpret_cast<axclrtContext>(DEVICE_MANAGER()->get_device(deviceId)->get_context_manager()->create_context());
    if (!*context) {
        return AXCL_ERR_CONTEXT_CREATE;
    }

    return AXCL_SUCC;
}

AXCL_EXPORT axclError axclrtDestroyContext(axclrtContext context) {
    if (!context) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_CONTEXT_NULL_POINTER;
    }

    auto ctx = reinterpret_cast<axcl::rt::context *>(context);
    if (!ctx->get_context_manager()->destory_context(ctx)) {
        return AXCL_ERR_CONTEXT_DESTROY;
    }

    return AXCL_SUCC;
}

AXCL_EXPORT axclError axclrtSetCurrentContext(axclrtContext context) {
    if (!context) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_CONTEXT_NULL_POINTER;
    }

    auto ctx = reinterpret_cast<axcl::rt::context *>(context);
    if (!ctx->get_context_manager()->bind_thread(ctx)) {
        return AXCL_ERR_CONTEXT_BIND_THREAD;
    }

    return AXCL_SUCC;
}

AXCL_EXPORT axclError axclrtGetCurrentContext(axclrtContext *context) {
    if (!context) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_CONTEXT_NULL_POINTER;
    }

    const auto tid = gettid();
    *context = reinterpret_cast<axclrtContext>(const_cast<axcl::rt::context *>(axcl::rt::context_manager::get_thread_context(tid)));
    if (!*context) {
        LOG_MM_E(TAG, "thread {} hasn't binded any context yet", tid);
        return AXCL_ERR_CONTEXT_NO_BIND_CONTEXT;
    }

    return AXCL_SUCC;
}

AXCL_EXPORT axclError axclrtGetContextId(axclrtContext context, int32_t *contextId) {
    if (!context || !contextId) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_CONTEXT_NULL_POINTER;
    }

    *contextId = reinterpret_cast<axcl::rt::context *>(context)->get_context_id();
    return AXCL_SUCC;
}

AXCL_EXPORT axclError axclrtGetDefaultContext(axclrtContext *context, int32_t deviceId) {
    if (!context) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_CONTEXT_NULL_POINTER;
    }

    if (!DEVICE_MANAGER()->is_actived(deviceId)) {
        LOG_MM_E(TAG, "device {} is not actived", deviceId);
        return AXCL_ERR_DEVICE_NOT_ACTIVE;
    }

    *context = reinterpret_cast<axclrtContext>(
        const_cast<axcl::rt::context *>(DEVICE_MANAGER()->get_device(deviceId)->get_context_manager()->get_default_context()));
    return AXCL_SUCC;
}