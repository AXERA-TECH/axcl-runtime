/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include <exception>
#include <memory>
#include "axcl_rt_task.h"
#include "axcl_logger.hpp"
#include "stream/stream.hpp"
#include "task/task.hpp"

#define TAG "task"

typedef struct {
    std::shared_ptr<axcl::rt::task> sp;
} task_context;

AXCL_EXPORT axclError axclrtCreateTask(axclrtTask* task, axclrtStream stream, uint64_t token, const void* data, size_t size,
                                       int32_t timeout, bool has_response, int64_t flag) {
    if (!task) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_TASK_NULL_POINTER;
    }

    task_context* ctx = new (std::nothrow) task_context();
    if (!ctx) {
        LOG_MM_E(TAG, "allocate task context fail");
        return AXCL_ERR_TASK_NO_MEMORY;
    }

    axcl::rt::stream* s = reinterpret_cast<axcl::rt::stream*>(stream);
    ctx->sp = std::make_shared<axcl::rt::task>(s, token, data, size, timeout, has_response, flag);
    if (!ctx->sp) {
        delete ctx;
        LOG_MM_E(TAG, "allocate task instance fail");
        return AXCL_ERR_TASK_NO_MEMORY;
    }

    if (!s->submit_task(ctx->sp)) {
        delete ctx;
        return AXCL_ERR_TASK_SUBMIT;
    }

    *task = reinterpret_cast<axclrtTask>(ctx);
    return AXCL_SUCC;
}

AXCL_EXPORT axclError axclrtWaitTaskFinish(axclrtTask task) {
    if (!task) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_TASK_NULL_POINTER;
    }

    task_context* ctx = reinterpret_cast<task_context*>(task);
    int32_t ret = ctx->sp->wait();
    if (ret < 0) {
        if (-1 == ret) {
            return AXCL_ERR_TASK_TIMEOUT;
        } else if (-2 == ret) {
            return AXCL_ERR_TASK_EXECUTE_FAIL;
        } else {
            return AXCL_ERR_TASK_UNKNOWN;
        }
    }

    return AXCL_SUCC;
}

AXCL_EXPORT axclError axclrtGetTaskResponse(axclrtTask task, void** buf, uint32_t* size) {
    if (!task || !buf || !size) {
        LOG_MM_E(TAG, "illegal parameters");
        return AXCL_ERR_TASK_ILLEGAL_PARAM;
    }

    task_context* ctx = reinterpret_cast<task_context*>(task);
    *buf = ctx->sp->get_response(*size);

    return AXCL_SUCC;
}

AXCL_EXPORT axclError axclrtDestoryTask(axclrtTask task) {
    if (!task) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_TASK_NULL_POINTER;
    }

    task_context* ctx = reinterpret_cast<task_context*>(task);
    reinterpret_cast<axcl::rt::stream*>(ctx->sp->get_stream())->remove_task(ctx->sp);
    delete ctx;

    return AXCL_SUCC;
}
