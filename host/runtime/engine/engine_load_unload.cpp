/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "engine.hpp"

#include "engine_strings.hpp"
#include "res_guard.hpp"
#include "axcl_logger.hpp"
#include "os.hpp"
#include <filesystem>

AXCL_EXPORT axclError axclrtEngineLoadFromFile(const char *modelPath, uint64_t *modelId) {
    LOG_MM_D(TAG, "Start...");
    if (nullptr == modelPath) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "modelPath");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }
    if (nullptr == modelId) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "modelId");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }

    std::filesystem::path file_path(modelPath);
    if (!std::filesystem::exists(file_path) || !std::filesystem::is_regular_file(file_path)) {
        LOG_MM_E(TAG, ENGINE_ERR_MODEL_PATH, modelPath);
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }

    const auto file_size = std::filesystem::file_size(file_path);

    auto fd_holder = res_guard<xp_file_t>(
        xp_fs_open_file(modelPath, XP_FILE_READ_ONLY),
        [](const xp_file_t& fd) { if (fd != XP_INVALID_FILE) xp_fs_close_file(fd); }
    );

    if (XP_INVALID_FILE == fd_holder.get()) {
        LOG_MM_E(TAG, ENGINE_ERR_LOAD_FILE_FAIL, modelPath);
        return false;
    }

    auto map_holder = res_guard<void*>(
        xp_mmap_file(fd_holder.get(), file_size, XP_MAP_READ_ONLY),
        [&file_size](void*& buffer) { if (buffer != nullptr) xp_munmap_file(buffer, file_size); }
    );

    if (nullptr == map_holder.get()) {
        LOG_MM_E(TAG, ENGINE_ERR_MAP_FILE_FAIL, modelPath);
        return false;
    }

    auto mem_holder = res_guard<void*>(
        [&file_size]() -> void* {
            void *mem = nullptr;
            axclrtMalloc(&mem, file_size, axclrtMemMallocPolicy{});
            return mem;
        },
        [](void*& dev) { if (dev != nullptr) axclrtFree(dev); }
    );

    if (nullptr == mem_holder.get()) {
        LOG_MM_E(TAG, ENGINE_ERR_MODEL_MALLOC, modelPath);
        return AXCL_ERR_MEMORY_EXECUTE_FAIL;
    }

    if (0 != axclrtMemcpy(mem_holder.get(), map_holder.get(), static_cast<size_t>(file_size), axclrtMemcpyKind::AXCL_MEMCPY_HOST_TO_DEVICE)) {
        LOG_MM_E(TAG, ENGINE_ERR_MODEL_COPY, modelPath);
        return AXCL_ERR_MEMORY_EXECUTE_FAIL;
    }

    return axclrtEngineLoadFromMem(mem_holder.get(), file_size, modelId);
}

AXCL_EXPORT axclError axclrtEngineLoadFromMem(const void *model, const uint64_t modelSize, uint64_t *modelId) {
    LOG_MM_D(TAG, "Start...");
    if (nullptr == model) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "model");
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }
    if (0 == modelSize) {
        LOG_MM_E(TAG, ENGINE_ERR_MODEL_SIZE, modelSize);
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }
    if (nullptr == modelId) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "modelId");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }

    const axcl::rt::engine api;
    return api.load(model, modelSize, modelId);
}

AXCL_EXPORT axclError axclrtEngineUnload(const uint64_t modelId) {
    LOG_MM_D(TAG, "Start...");
    if (0 == modelId) {
        LOG_MM_E(TAG, ENGINE_ERR_MODEL_ID, modelId);
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }

    const axcl::rt::engine api;
    return api.unload(modelId);
}

axclError axcl::rt::engine::load(const void *model, const uint64_t size, uint64_t *id) const {
    axclrtStream stream = nullptr;
    axclrtEnvironment env;
    axclError ret;
    if (ret = get_runtime_environment(&stream, env); AXCL_SUCC != ret) {
        return ret;
    }

    this->m_req.load(model, size);
    if (ret = invoke(stream, env, NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    *id = this->m_rsp.get_param_handle();
    return AXCL_SUCC;
}

axclError axcl::rt::engine::unload(uint64_t id) const {
    axclrtStream stream = nullptr;
    axclrtEnvironment env;
    axclError ret;
    if (ret = get_runtime_environment(&stream, env); AXCL_SUCC != ret) {
        return ret;
    }

    this->m_req.unload(id);
    if (ret = invoke(stream, env, NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    return AXCL_SUCC;
}