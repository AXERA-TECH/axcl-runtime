/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include <stdlib.h>
#include "axcl_rt_memory.h"
#include "axcl_logger.hpp"
#include "memory/memory.hpp"

#define TAG "memory"

AXCL_EXPORT axclError axclrtMalloc(void **devPtr, size_t size, axclrtMemMallocPolicy policy) {
    axcl::rt::memory api;
    return api.malloc(devPtr, size, policy);
}

AXCL_EXPORT axclError axclrtMallocCached(void **devPtr, size_t size, axclrtMemMallocPolicy policy) {
    axcl::rt::memory api;
    return api.malloc_cached(devPtr, size, policy);
}

AXCL_EXPORT axclError axclrtFree(void *devPtr) {
    axcl::rt::memory api;
    return api.free(devPtr);
}

AXCL_EXPORT axclError axclrtMemFlush(void *devPtr, size_t size) {
    axcl::rt::memory api;
    return api.flush(devPtr, size);
}

AXCL_EXPORT axclError axclrtMemInvalidate(void *devPtr, size_t size) {
    axcl::rt::memory api;
    return api.invalidate(devPtr, size);
}

AXCL_EXPORT axclError axclrtMallocHost(void **hostPtr, size_t size) {
    if (!hostPtr) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_MEMORY_NULL_POINTER;
    }

    *hostPtr = ::malloc(size);
    return AXCL_SUCC;
}

AXCL_EXPORT axclError axclrtFreeHost(void *hostPtr) {
    ::free(hostPtr);
    return AXCL_SUCC;
}

AXCL_EXPORT axclError axclrtMemset(void *devPtr, uint8_t value, size_t count) {
    axcl::rt::memory api;
    return api.memset(devPtr, value, count);
}

AXCL_EXPORT axclError axclrtMemcpy(void *dstPtr, const void *srcPtr, size_t count, axclrtMemcpyKind kind) {
    axcl::rt::memory api;
    return api.memcpy(dstPtr, srcPtr, count, kind);
}

AXCL_EXPORT axclError axclrtMemcmp(const void *devPtr1, const void *devPtr2, size_t count) {
    axcl::rt::memory api;
    return api.memcmp(devPtr1, devPtr2, count);
}

AXCL_EXPORT axclError axclrtMemsetAsync(void *devPtr, uint8_t value, size_t count, axclrtStream stream) {
    axcl::rt::memory api;
    return api.memset_async(devPtr, value, count, stream);
}

AXCL_EXPORT axclError axclrtMemcpyAsync(void *dstPtr, const void *srcPtr, size_t count, axclrtMemcpyKind kind, axclrtStream stream) {
    axcl::rt::memory api;
    if (kind == AXCL_MEMCPY_HOST_TO_HOST) {
        LOG_MM_E(TAG, "Async memcpy does not support HOST_TO_HOST");
        return AXCL_ERR_UNSUPPORT;
    }
    return api.memcpy_async(dstPtr, srcPtr, count, kind, stream);
}

AXCL_EXPORT axclError axclrtMemcmpAsync(const void *devPtr1, const void *devPtr2, size_t count, axclrtStream stream) {
    axcl::rt::memory api;
    return api.memcmp_async(devPtr1, devPtr2, count, stream);
}