/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "memory.hpp"
#include <string.h>
#include "axcl_rt.h"
#include "axcl_rt_internal.h"
#include "device/device.hpp"
#include "device/device_manager.hpp"
#include "axcl_logger.hpp"
#include "os.hpp"

#define TAG "memory"
#define DEVICE_MEMORY(id) device_manager::get_instance()->get_device(id)->get_device_memory()

axclError axcl::rt::memory::malloc(void **devPtr, size_t size, axclrtMemMallocPolicy policy) {
    if (!devPtr) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_MEMORY_NULL_POINTER;
    }

    axclrtStream stream;
    axclrtEnvironment env;
    axclError ret;
    if (ret = get_runtime_environment(stream, env); AXCL_SUCC != ret) {
        return ret;
    }

    m_req.alloc(size, static_cast<uint32_t>(policy), false);

    if (ret = invoke(stream, env, NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    *devPtr = m_rsp.get_addr();
    DEVICE_MEMORY(env.device)->insert(reinterpret_cast<uint64_t>(*devPtr), size);

    // LOG_MM_I(TAG, "malloc(size {}, ploicy {}) from device succ, addr: {}", size, static_cast<uint32_t>(policy), *devPtr);
    return AXCL_SUCC;
}

axclError axcl::rt::memory::malloc_cached(void **devPtr, size_t size, axclrtMemMallocPolicy policy) {
    if (!devPtr) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_MEMORY_NULL_POINTER;
    }

    axclrtStream stream;
    axclrtEnvironment env;
    axclError ret;
    if (ret = get_runtime_environment(stream, env); AXCL_SUCC != ret) {
        return ret;
    }

    m_req.alloc(size, static_cast<uint32_t>(policy), true);

    if (ret = invoke(stream, env, NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    *devPtr = m_rsp.get_addr();
    DEVICE_MEMORY(env.device)->insert(reinterpret_cast<uint64_t>(*devPtr), size);

    // LOG_MM_I(TAG, "malloc cached(size {}, ploicy {}) from device succ, addr: {}", size, static_cast<uint32_t>(policy), *devPtr);
    return AXCL_SUCC;
}

axclError axcl::rt::memory::free(const void *devPtr) {
    if (!devPtr) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_MEMORY_NULL_POINTER;
    }

    axclrtStream stream;
    axclrtEnvironment env;
    axclError ret;
    if (ret = get_runtime_environment(stream, env); AXCL_SUCC != ret) {
        return ret;
    }

    if (!DEVICE_MEMORY(env.device)->find(reinterpret_cast<uint64_t>(devPtr))) {
        LOG_MM_E(TAG, "memory {} is not allocated by device {}", devPtr, env.device);
        return AXCL_ERR_MEMORY_NOT_DEVICE_MEMORY;
    }

    m_req.free(devPtr);

    if (ret = invoke(stream, env, NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    DEVICE_MEMORY(env.device)->remove(reinterpret_cast<uint64_t>(devPtr));

    // LOG_MM_I(TAG, "free device memory {} succ", devPtr);
    return AXCL_SUCC;
}

axclError axcl::rt::memory::flush(const void *devPtr, size_t size) {
    if (!devPtr) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_MEMORY_NULL_POINTER;
    }

    axclrtStream stream;
    axclrtEnvironment env;
    axclError ret;
    if (ret = get_runtime_environment(stream, env); AXCL_SUCC != ret) {
        return ret;
    }

    if (!DEVICE_MEMORY(env.device)->find(reinterpret_cast<uint64_t>(devPtr))) {
        LOG_MM_E(TAG, "memory {} is not allocated by device {}", devPtr, env.device);
        return AXCL_ERR_MEMORY_NOT_DEVICE_MEMORY;
    }

    m_req.flush(devPtr, size);

    if (ret = invoke(stream, env, NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    // LOG_MM_I(TAG, "free device memory {} succ", devPtr);
    return AXCL_SUCC;
}

axclError axcl::rt::memory::invalidate(const void *devPtr, size_t size) {
    if (!devPtr) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_MEMORY_NULL_POINTER;
    }

    axclrtStream stream;
    axclrtEnvironment env;
    axclError ret;
    if (ret = get_runtime_environment(stream, env); AXCL_SUCC != ret) {
        return ret;
    }

    if (!DEVICE_MEMORY(env.device)->find(reinterpret_cast<uint64_t>(devPtr))) {
        LOG_MM_E(TAG, "memory {} is not allocated by device {}", devPtr, env.device);
        return AXCL_ERR_MEMORY_NOT_DEVICE_MEMORY;
    }

    m_req.invalidate(devPtr, size);

    if (ret = invoke(stream, env, NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    // LOG_MM_I(TAG, "invalidate device memory {} succ", devPtr);
    return AXCL_SUCC;
}

axclError axcl::rt::memory::memset(void *devPtr, uint8_t value, size_t count) {
    if (!devPtr) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_MEMORY_NULL_POINTER;
    }

    axclrtStream stream;
    axclrtEnvironment env;
    axclError ret;
    if (ret = get_runtime_environment(stream, env); AXCL_SUCC != ret) {
        return ret;
    }

    // if (!DEVICE_MEMORY(env.device)->find(reinterpret_cast<uint64_t>(devPtr))) {
    //     ::memset(devPtr, value, count);
    //     LOG_MM_I(TAG, "memset host memory {} to {} succ", devPtr, value);
    //     return AXCL_SUCC;
    // }

    m_req.set(devPtr, value, count);

    if (ret = invoke(stream, env, NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    // LOG_MM_I(TAG, "memset device memory {} to {} succ", devPtr, value);
    return AXCL_SUCC;
}

axclError axcl::rt::memory::memcpy(void *dstPtr, const void *srcPtr, size_t count, axclrtMemcpyKind kind) {
    if (!dstPtr || !srcPtr) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_MEMORY_NULL_POINTER;
    }

    // For host-to-host copy, use standard memcpy function directly
    if (AXCL_MEMCPY_HOST_TO_HOST == kind) {
        ::memcpy(dstPtr, srcPtr, count);
        return AXCL_SUCC;
    }

    // Get environment configuration
    axclrtStream stream;
    axclrtEnvironment env;
    axclError ret;
    if (ret = get_runtime_environment(stream, env); AXCL_SUCC != ret) {
        return ret;
    }

    // Set request parameters based on copy type
    switch (kind) {
        case AXCL_MEMCPY_HOST_TO_DEVICE:
        case AXCL_MEMCPY_HOST_PHY_TO_DEVICE:
            {
                auto kind_type = (AXCL_MEMCPY_HOST_TO_DEVICE == kind) ?
                    axcl::pkg::memcpy_kind::HOST_VIR_TO_DEVICE :
                    axcl::pkg::memcpy_kind::HOST_PHY_TO_DEVICE;
                m_req.write(srcPtr, dstPtr, count, kind_type);
            }
            break;

        case AXCL_MEMCPY_DEVICE_TO_HOST:
        case AXCL_MEMCPY_DEVICE_TO_HOST_PHY:
            {
                auto kind_type = (AXCL_MEMCPY_DEVICE_TO_HOST == kind) ?
                    axcl::pkg::memcpy_kind::DEVICE_TO_HOST_VIR :
                    axcl::pkg::memcpy_kind::DEVICE_TO_HOST_PHY;
                m_req.read(srcPtr, dstPtr, count, kind_type);
            }
            break;

        default: // Device-to-device copy
            m_req.sync(srcPtr, dstPtr, count);
            break;
    }

    // Execute the call
    if (ret = invoke(stream, env, NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    return AXCL_SUCC;
}

axclError axcl::rt::memory::memcmp(const void *devPtr1, const void *devPtr2, size_t count) {
    if (!devPtr1 || !devPtr2) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_MEMORY_NULL_POINTER;
    }

    // Get environment configuration
    axclrtStream stream;
    axclrtEnvironment env;
    axclError ret;
    if (ret = get_runtime_environment(stream, env); AXCL_SUCC != ret) {
        return ret;
    }

    // if (!DEVICE_MEMORY(env.device)->find(reinterpret_cast<uint64_t>(devPtr1))) {
    //     LOG_MM_E(TAG, "memory {} is not allocated by device {}", devPtr1, env.device);
    //     return AXCL_ERR_MEMORY_NOT_DEVICE_MEMORY;
    // }

    // if (!DEVICE_MEMORY(env.device)->find(reinterpret_cast<uint64_t>(devPtr2))) {
    //     LOG_MM_E(TAG, "memory {} is not allocated by device {}", devPtr2, env.device);
    //     return AXCL_ERR_MEMORY_NOT_DEVICE_MEMORY;
    // }

    // Set up comparison request
    m_req.cmp(devPtr1, devPtr2, count);

    // Execute the call
    if (ret = invoke(stream, env, NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    return AXCL_SUCC;
}

axclError axcl::rt::memory::get_runtime_environment(axclrtStream &stream, axclrtEnvironment &env) {
    axclError ret;
    axclrtContext context;

    // Get current context
    if (ret = axclrtGetCurrentContext(&context); AXCL_SUCC != ret) {
        return ret;
    }

    // Get default stream for context
    if (ret = axclrtGetDefaultStream(context, &stream); AXCL_SUCC != ret) {
        return ret;
    }

    // Get environment for stream
    if (ret = axclrtGetEnviromentByStream(stream, &env); AXCL_SUCC != ret) {
        return ret;
    }

    return AXCL_SUCC;
}

axclError axcl::rt::memory::invoke(axclrtStream stream, const axclrtEnvironment &env, int32_t timeout) {
    axcl::pkg::head hd;
    hd.seq_num = axclrtGetToken();
    hd.device = env.device;
    hd.context = env.context;
    hd.stream = env.stream;
    hd.command = 0;

    const auto api = m_req.get_api();
    bool has_response;
    int64_t flag;

    if (axcl::pkg::runtime::memory::API::READ == api || axcl::pkg::runtime::memory::API::WRITE == api) {
        hd.type = RUNTIME_MEMCPY_HANDLER_TYPE;
        has_response = false;
        flag = axcl::comm::PCIE_DMA;
    } else {
        hd.type = CREATE_PACKAGE_HEAD_TYPE(axcl::pkg::PROTO, axcl::pkg::module::RUNTIME, axcl::pkg::runtime::module::MEMORY, api);
        has_response = true;
        flag = axcl::comm::PCIE_MSG;
    }

    static uint32_t pid = 0;
    if (0 == pid) {
        pid = static_cast<uint32_t>(getpid());
    }
    static thread_local uint32_t tid = 0;
    if (0 == tid) {
        tid = gettid();
    }
    m_req.set_env({pid, tid});

    uint32_t len;
    void *pkg = m_req.encode(hd, len);
    if (!pkg) {
        LOG_MM_E(TAG, "encode api {} fail", static_cast<int32_t>(api));
        return AXCL_ERR_MEMORY_ENCODE;
    }

    axclError ret;
    axclrtTask task;
    if (ret = axclrtCreateTask(&task, stream, hd.seq_num, pkg, len, timeout, has_response, flag); AXCL_SUCC != ret) {
        return ret;
    }

    auto deleter = [](axclrtTask task) { axclrtDestoryTask(task); };
    std::unique_ptr<void, decltype(deleter)> sp(task, deleter);

    if (ret = axclrtWaitTaskFinish(task); AXCL_SUCC != ret) {
        return ret;
    }

    if (has_response) {
        if (ret = axclrtGetTaskResponse(task, &pkg, &len); AXCL_SUCC != ret) {
            return ret;
        }

        if (!m_rsp.decode(pkg, len)) {
            LOG_MM_E(TAG, "decode api {} response fail", static_cast<int32_t>(api));
            return AXCL_ERR_MEMORY_DECODE;
        }

        if (m_rsp.get_api() != api) {
            LOG_MM_E(TAG, "response api {} is not equal to request api {}", static_cast<int32_t>(m_rsp.get_api()),
                     static_cast<int32_t>(api));
            return AXCL_ERR_MEMORY_UNEXPECT_RESPONSE;
        }

        if (int32_t status = m_rsp.get_status(); 0 != status) {
            if (axcl::pkg::runtime::memory::API::CMP == api) {
                /* status = ::memcmp */
                return static_cast<axclError>(status);
            }

            LOG_MM_E(TAG, "request api {} return fail, status: {}", static_cast<int32_t>(api), status);
            return AXCL_ERR_MEMORY_EXECUTE_FAIL;
        }
    }

    return AXCL_SUCC;
}

axclError axcl::rt::memory::memset_async(void *devPtr, uint8_t value, size_t count, axclrtStream stream) {
    if (!devPtr) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_MEMORY_NULL_POINTER;
    }

    axclrtEnvironment env;
    axclError ret;
    if (ret = axclrtGetEnviromentByStream(stream, &env); AXCL_SUCC != ret) {
        return ret;
    }

    m_req.set(devPtr, value, count, 1);

    if (ret = invoke_async(stream, env, NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    return AXCL_SUCC;
}

axclError axcl::rt::memory::memcpy_async(void *dstPtr, const void *srcPtr, size_t count, axclrtMemcpyKind kind, axclrtStream stream) {
    if (!dstPtr || !srcPtr) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_MEMORY_NULL_POINTER;
    }

    // Get environment configuration
    axclrtEnvironment env;
    axclError ret;
    if (ret = axclrtGetEnviromentByStream(stream, &env); AXCL_SUCC != ret) {
        return ret;
    }

    // Set request parameters based on copy type
    switch (kind) {
        case AXCL_MEMCPY_HOST_TO_DEVICE:
        case AXCL_MEMCPY_HOST_PHY_TO_DEVICE:
            {
                auto kind_type = (AXCL_MEMCPY_HOST_TO_DEVICE == kind) ?
                    axcl::pkg::memcpy_kind::HOST_VIR_TO_DEVICE :
                    axcl::pkg::memcpy_kind::HOST_PHY_TO_DEVICE;
                m_req.write(srcPtr, dstPtr, count, kind_type, 1);
            }
            break;

        case AXCL_MEMCPY_DEVICE_TO_HOST:
        case AXCL_MEMCPY_DEVICE_TO_HOST_PHY:
            {
                auto kind_type = (AXCL_MEMCPY_DEVICE_TO_HOST == kind) ?
                    axcl::pkg::memcpy_kind::DEVICE_TO_HOST_VIR :
                    axcl::pkg::memcpy_kind::DEVICE_TO_HOST_PHY;
                m_req.read(srcPtr, dstPtr, count, kind_type, 1);
            }
            break;

        default: // Device-to-device copy
            m_req.sync(srcPtr, dstPtr, count, 1);
            break;
    }

    // Execute asynchronous call
    return invoke_async(stream, env, NO_TIMEOUT);
}

axclError axcl::rt::memory::memcmp_async(const void *devPtr1, const void *devPtr2, size_t count, axclrtStream stream) {
    if (!devPtr1 || !devPtr2) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_MEMORY_NULL_POINTER;
    }

    axclrtEnvironment env;
    axclError ret;
    if (ret = axclrtGetEnviromentByStream(stream, &env); AXCL_SUCC != ret) {
        return ret;
    }

    m_req.cmp(devPtr1, devPtr2, count, 1);

    if (ret = invoke_async(stream, env, NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    return AXCL_SUCC;
}

axclError axcl::rt::memory::invoke_async(axclrtStream stream, const axclrtEnvironment &env, int32_t timeout) {
    axcl::pkg::head hd;
    hd.seq_num = axclrtGetToken();
    hd.device = env.device;
    hd.context = env.context;
    hd.stream = env.stream;
    hd.command = 0;

    const auto api = m_req.get_api();
    int64_t flag;
    hd.type = CREATE_PACKAGE_HEAD_TYPE(axcl::pkg::PROTO, axcl::pkg::module::RUNTIME, axcl::pkg::runtime::module::MEMORY, api);
    flag = axcl::comm::PCIE_MSG;

    static uint32_t pid = 0;
    if (0 == pid) {
        pid = static_cast<uint32_t>(getpid());
    }
    static thread_local uint32_t tid = 0;
    if (0 == tid) {
        tid = gettid();
    }
    m_req.set_env({pid, tid});

    uint32_t len;
    void *pkg = m_req.encode(hd, len);
    if (!pkg) {
        LOG_MM_E(TAG, "encode api {} fail", static_cast<int32_t>(api));
        return AXCL_ERR_MEMORY_ENCODE;
    }

    axclError ret;
    axclrtTask task;
    if (ret = axclrtCreateTask(&task, stream, hd.seq_num, pkg, len, timeout, false /* has_response */, flag); AXCL_SUCC != ret) {
        return ret;
    }

    auto deleter = [](axclrtTask task) { axclrtDestoryTask(task); };
    std::unique_ptr<void, decltype(deleter)> sp(task, deleter);

    // wait for excute
    if (ret = axclrtWaitTaskFinish(task); AXCL_SUCC != ret) {
        return ret;
    }

    return AXCL_SUCC;
}
