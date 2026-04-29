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

#include "axcl_rt_type.h"
#include "axcl_rt_type_internal.h"
#include "package/host/runtime/memory/memory_request.hpp"
#include "package/host/runtime/memory/memory_response.hpp"
#include <mutex>
#include <unordered_map>
#include <memory>

namespace axcl::rt {

class memory {
public:
    memory() = default;

    axclError malloc(void **devPtr, size_t size, axclrtMemMallocPolicy policy);
    axclError malloc_cached(void **devPtr, size_t size, axclrtMemMallocPolicy policy);
    axclError free(const void *devPtr);
    axclError flush(const void *devPtr, size_t size);
    axclError invalidate(const void *devPtr, size_t size);
    axclError memset(void *devPtr, uint8_t value, size_t count);
    axclError memcpy(void *dstPtr, const void *srcPtr, size_t count, axclrtMemcpyKind kind);
    axclError memcmp(const void *devPtr1, const void *devPtr2, size_t count);
    axclError memset_async(void *devPtr, uint8_t value, size_t count, axclrtStream stream);
    axclError memcpy_async(void *dstPtr, const void *srcPtr, size_t count, axclrtMemcpyKind kind, axclrtStream stream);
    axclError memcmp_async(const void *devPtr1, const void *devPtr2, size_t count, axclrtStream stream);

protected:
    axclError get_runtime_environment(axclrtStream &stream, axclrtEnvironment &env);
    axclError invoke(axclrtStream stream, const axclrtEnvironment &env, int32_t timeout);
    axclError invoke_async(axclrtStream stream, const axclrtEnvironment &env, int32_t timeout);

private:
    axcl::pkg::host::memory_request m_req;
    axcl::pkg::host::memory_response m_rsp;
};

}  // namespace axcl::rt