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
#include "axcl_rt_stream.h"
#include "axcl_rt_stream_internal.h"
#include "context/context.hpp"
#include "context/context_manager.hpp"
#include "device/device.hpp"
#include "axcl_logger.hpp"
#include "stream/stream.hpp"
#include "stream/stream_manager.hpp"

#define TAG "stream"

AXCL_EXPORT axclError axclrtGetDefaultStream(axclrtContext context, axclrtStream *stream) {
    if (!context || !stream) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_STREAM_NULL_POINTER;
    }

    axcl::rt::context *ctx = reinterpret_cast<axcl::rt::context *>(context);
    *stream = reinterpret_cast<axclrtStream>(ctx->get_stream_manager()->get_default_stream());
    if (!*stream) {
        return AXCL_ERR_STREAM_NULL_POINTER;
    }

    return AXCL_SUCC;
}

AXCL_EXPORT axclError axclrtCreateStream(axclrtStream *stream) {
    if (!stream) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_STREAM_NULL_POINTER;
    }

    axclrtContext ctx;
    if (axclError ret = axclrtGetCurrentContext(&ctx); AXCL_SUCC != ret) {
        return ret;
    }

    *stream = reinterpret_cast<axcl::rt::context *>(ctx)->get_stream_manager()->create_stream();
    if (!*stream) {
        return AXCL_ERR_STREAM_CREATE;
    }

    return AXCL_SUCC;
}

AXCL_EXPORT axclError axclrtDestroyStream(axclrtStream stream) {
    if (!stream) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_STREAM_NULL_POINTER;
    }

    auto *s = reinterpret_cast<axcl::rt::stream *>(stream);
    if (!s->get_stream_manager()->destory_stream(s, false)) {
        return AXCL_ERR_STREAM_DESTROY;
    }

    return AXCL_SUCC;
}

AXCL_EXPORT axclError axclrtDestroyStreamForce(axclrtStream stream) {
    if (!stream) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_STREAM_NULL_POINTER;
    }

    auto *s = reinterpret_cast<axcl::rt::stream *>(stream);
    if (!s->get_stream_manager()->destory_stream(s, true)) {
        return AXCL_ERR_STREAM_DESTROY;
    }

    return AXCL_SUCC;
}

AXCL_EXPORT axclError axclrtSynchronizeStream(axclrtStream stream) {
    if (!stream) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_STREAM_NULL_POINTER;
    }

    axclError ret;
    auto *s = reinterpret_cast<axcl::rt::stream *>(stream);
    if (ret = s->get_stream_manager()->sync_stream(s, -1); AXCL_SUCC != ret) {
        return ret;
    }

    return AXCL_SUCC;
}

AXCL_EXPORT axclError axclrtSynchronizeStreamWithTimeout(axclrtStream stream, int32_t timeout) {
    if (!stream) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_STREAM_NULL_POINTER;
    }

    axclError ret;
    auto *s = reinterpret_cast<axcl::rt::stream *>(stream);
    if (ret = s->get_stream_manager()->sync_stream(s, timeout); AXCL_SUCC != ret) {
        return ret;
    }

    return AXCL_SUCC;
}

AXCL_EXPORT axclError axclrtGetEnviromentByStream(axclrtStream stream, axclrtEnvironment *env) {
    if (!stream) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_STREAM_NULL_POINTER;
    }

    axcl::rt::stream *s = reinterpret_cast<axcl::rt::stream *>(stream);
    auto *ctx = s->get_stream_manager()->get_context();
    auto *dev = ctx->get_context_manager()->get_device();

    env->device = dev->get_device_id();
    env->context = ctx->get_context_id();
    env->stream = s->get_stream_id();

    return AXCL_SUCC;
}