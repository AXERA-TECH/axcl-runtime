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
#include "axcl_logger.hpp"

AXCL_EXPORT axclError axclrtEngineCreateIO(axclrtEngineIOInfo ioInfo, axclrtEngineIO *io) {
    LOG_MM_D(TAG, "Start...");
    if (nullptr == ioInfo) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "info");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }
    if (nullptr == io) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "io");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }

    return axcl::rt::engine::create_io(ioInfo, io);
}

AXCL_EXPORT axclError axclrtEngineDestroyIO(axclrtEngineIO io) {
    LOG_MM_D(TAG, "Start...");
    if (nullptr == io) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "io");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }

    return axcl::rt::engine::destroy_io(io);
}

AXCL_EXPORT axclError axclrtEngineSetInputBufferByIndex(axclrtEngineIO io, const uint32_t index, const void *dataBuffer, const uint64_t size) {
    LOG_MM_D(TAG, "Start...");
    if (nullptr == io) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "io");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }
    if (nullptr == dataBuffer) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "dataBuffer");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }
    if (0 == size) {
        LOG_MM_E(TAG, ENGINE_ERR_BUFFER_SIZE, size);
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }

    return axcl::rt::engine::set_input(io, index, dataBuffer, size);
}

AXCL_EXPORT axclError axclrtEngineSetOutputBufferByIndex(axclrtEngineIO io, const uint32_t index, const void *dataBuffer, const uint64_t size) {
    LOG_MM_D(TAG, "Start...");
    if (nullptr == io) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "io");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }
    if (nullptr == dataBuffer) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "dataBuffer");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }
    if (0 == size) {
        LOG_MM_E(TAG, ENGINE_ERR_BUFFER_SIZE, size);
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }

    return axcl::rt::engine::set_output(io, index, dataBuffer, size);
}

AXCL_EXPORT axclError axclrtEngineSetInputBufferByName(axclrtEngineIO io, const char *name, const void *dataBuffer, const uint64_t size) {
    LOG_MM_D(TAG, "Start...");
    if (nullptr == io) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "io");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }
    if (nullptr == name) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "name");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }
    if (nullptr == dataBuffer) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "dataBuffer");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }
    if (0 == size) {
        LOG_MM_E(TAG, ENGINE_ERR_BUFFER_SIZE, size);
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }

    return axcl::rt::engine::set_input(io, name, dataBuffer, size);
}

AXCL_EXPORT axclError axclrtEngineSetOutputBufferByName(axclrtEngineIO io, const char *name, const void *dataBuffer, const uint64_t size) {
    LOG_MM_D(TAG, "Start...");
    if (nullptr == io) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "io");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }
    if (nullptr == name) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "name");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }
    if (nullptr == dataBuffer) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "dataBuffer");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }
    if (0 == size) {
        LOG_MM_E(TAG, ENGINE_ERR_BUFFER_SIZE, size);
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }

    return axcl::rt::engine::set_output(io, name, dataBuffer, size);
}

AXCL_EXPORT axclError axclrtEngineGetInputBufferByIndex(axclrtEngineIO io, const uint32_t index, void **dataBuffer, uint64_t *size) {
    LOG_MM_D(TAG, "Start...");
    if (nullptr == io) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "io");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }
    if (nullptr == dataBuffer) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "dataBuffer");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }
    if (nullptr == size) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "size");
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }

    return axcl::rt::engine::get_input(io, index, dataBuffer, size);
}

AXCL_EXPORT axclError axclrtEngineGetOutputBufferByIndex(axclrtEngineIO io, const uint32_t index, void **dataBuffer, uint64_t *size) {
    LOG_MM_D(TAG, "Start...");
    if (nullptr == io) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "io");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }
    if (nullptr == dataBuffer) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "dataBuffer");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }
    if (nullptr == size) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "size");
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }

    return axcl::rt::engine::get_output(io, index, dataBuffer, size);
}

AXCL_EXPORT axclError axclrtEngineGetInputBufferByName(axclrtEngineIO io, const char *name, void **dataBuffer, uint64_t *size) {
    LOG_MM_D(TAG, "Start...");
    if (nullptr == io) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "io");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }
    if (nullptr == name) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "name");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }
    if (nullptr == dataBuffer) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "dataBuffer");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }
    if (nullptr == size) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "size");
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }

    return axcl::rt::engine::get_input(io, name, dataBuffer, size);
}

AXCL_EXPORT axclError axclrtEngineGetOutputBufferByName(axclrtEngineIO io, const char *name, void **dataBuffer, uint64_t *size) {
    LOG_MM_D(TAG, "Start...");
    if (nullptr == io) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "io");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }
    if (nullptr == name) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "name");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }
    if (nullptr == dataBuffer) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "dataBuffer");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }
    if (nullptr == size) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "size");
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }

    return axcl::rt::engine::get_output(io, name, dataBuffer, size);
}

AXCL_EXPORT axclError axclrtEngineSetDynamicBatchSize(axclrtEngineIO io, const uint32_t batchSize) {
    LOG_MM_D(TAG, "Start...");
    if (nullptr == io) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "io");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }

    return axcl::rt::engine::set_batch(io, batchSize);
}

axclError axcl::rt::engine::create_io(axclrtEngineIOInfo info, axclrtEngineIO *io) {
    // FIXME: using axclrtMalloc to allocate memory for io
    *io = new pkg::runtime::engine::desc;

    const auto io_ = static_cast<pkg::runtime::engine::desc*>(*io);
    const auto info_ = static_cast<pkg::runtime::engine::meta*>(info);

    io_->batch = 0;
    io_->inputs.resize(info_->inputs.size());
    io_->outputs.resize(info_->outputs.size());
    for (size_t i = 0; i < info_->inputs.size(); i++)
    {
        io_->inputs[i].name = info_->inputs[i].name;
        io_->inputs[i].address = 0;
        io_->inputs[i].size = 0;
    }
    for (size_t i = 0; i < info_->outputs.size(); i++)
    {
        io_->outputs[i].name = info_->outputs[i].name;
        io_->outputs[i].address = 0;
        io_->outputs[i].size = 0;
    }

    return AXCL_SUCC;
}

axclError axcl::rt::engine::destroy_io(axclrtEngineIO io) {
    const auto io_ = static_cast<pkg::runtime::engine::desc*>(io);

    // FIXME: using axclrtFree to free memory for io
    delete io_;

    return AXCL_SUCC;
}

axclError axcl::rt::engine::set_input(axclrtEngineIO io, const uint32_t index, const void *buffer, const uint64_t buffer_size) {
    const auto io_ = static_cast<pkg::runtime::engine::desc*>(io);
    if (index >= io_->inputs.size()) {
        LOG_MM_E(TAG, ENGINE_ERR_PARAM, "index", index);
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }

    io_->inputs[index].address = reinterpret_cast<uint64_t>(buffer);
    io_->inputs[index].size = buffer_size;

    return AXCL_SUCC;
}

axclError axcl::rt::engine::set_input(axclrtEngineIO io, const char *name, const void *buffer, const uint64_t buffer_size) {
    const auto io_ = static_cast<pkg::runtime::engine::desc*>(io);
    for (auto &[input_name, input_address, input_size] : io_->inputs) {
        if (name == std::string(name)) {
            input_address = reinterpret_cast<uint64_t>(buffer);
            input_size = buffer_size;
            return AXCL_SUCC;
        }
    }

    LOG_MM_E(TAG, ENGINE_ERR_PARAM, "name", name);
    return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
}

axclError axcl::rt::engine::get_input(axclrtEngineIO io, const uint32_t index, void **buffer, uint64_t *size) {
    if (nullptr == io || nullptr == buffer || nullptr == size) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "io or(and) buffer or(and) size");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }

    const auto io_ = static_cast<pkg::runtime::engine::desc*>(io);
    if (index >= io_->inputs.size()) {
        LOG_MM_E(TAG, ENGINE_ERR_PARAM, "index", index);
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }

    *buffer = reinterpret_cast<void*>(io_->inputs[index].address);
    *size = io_->inputs[index].size;

    return AXCL_SUCC;
}

axclError axcl::rt::engine::get_input(axclrtEngineIO io, const char *name, void **buffer, uint64_t *buffer_size) {
    const auto io_ = static_cast<pkg::runtime::engine::desc*>(io);
    for (const auto &[input_name, input_address, input_size] : io_->inputs) {
        if (name == std::string(name)) {
            *buffer = reinterpret_cast<void*>(input_address);
            *buffer_size = input_size;
            return AXCL_SUCC;
        }
    }

    LOG_MM_E(TAG, ENGINE_ERR_PARAM, "name", name);
    return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
}

axclError axcl::rt::engine::set_output(axclrtEngineIO io, uint32_t index, const void *buffer, const uint64_t buffer_size) {
    if (nullptr == io || nullptr == buffer) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "io or(and) buffer");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }

    const auto io_ = static_cast<pkg::runtime::engine::desc*>(io);
    if (index >= io_->outputs.size()) {
        LOG_MM_E(TAG, ENGINE_ERR_PARAM, "index", index);
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }

    io_->outputs[index].address = reinterpret_cast<uint64_t>(buffer);
    io_->outputs[index].size = buffer_size;

    return AXCL_SUCC;
}

axclError axcl::rt::engine::set_output(axclrtEngineIO io, const char *name, const void *buffer, const uint64_t buffer_size) {
    const auto io_ = static_cast<pkg::runtime::engine::desc*>(io);
    for (auto &[output_name, output_address, output_size] : io_->outputs) {
        if (name == std::string(name)) {
            output_address = reinterpret_cast<uint64_t>(buffer);
            output_size = buffer_size;
            return AXCL_SUCC;
        }
    }

    LOG_MM_E(TAG, ENGINE_ERR_PARAM, "name", name);
    return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
}

axclError axcl::rt::engine::get_output(axclrtEngineIO io, const uint32_t index, void **buffer, uint64_t *buffer_size) {
    const auto io_ = static_cast<pkg::runtime::engine::desc*>(io);
    if (index >= io_->outputs.size()) {
        LOG_MM_E(TAG, ENGINE_ERR_PARAM, "index", index);
        return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
    }

    *buffer = reinterpret_cast<void*>(io_->outputs[index].address);
    *buffer_size = io_->outputs[index].size;

    return AXCL_SUCC;
}

axclError axcl::rt::engine::get_output(axclrtEngineIO io, const char *name, void **buffer, uint64_t *buffer_size) {
    const auto io_ = static_cast<pkg::runtime::engine::desc*>(io);
    for (const auto &[output_name, output_address, output_size] : io_->outputs) {
        if (name == std::string(name)) {
            *buffer = reinterpret_cast<void*>(output_address);
            *buffer_size = output_size;
            return AXCL_SUCC;
        }
    }

    LOG_MM_E(TAG, ENGINE_ERR_PARAM, "name", name);
    return AXCL_ERR_ENGINE_ILLEGAL_PARAM;
}

axclError axcl::rt::engine::set_batch(axclrtEngineIO io, const uint32_t batch_size) {
    if (nullptr == io) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "io");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }

    const auto io_ = static_cast<pkg::runtime::engine::desc*>(io);
    io_->batch = batch_size;

    return AXCL_SUCC;
}

axclError axcl::rt::engine::get_batch(axclrtEngineIO io, uint32_t *batch_size) {
    if (nullptr == io || nullptr == batch_size) {
        LOG_MM_E(TAG, ENGINE_ERR_NULL_PTR, "io or(and) batch_size");
        return AXCL_ERR_ENGINE_NULL_POINTER;
    }

    const auto io_ = static_cast<pkg::runtime::engine::desc*>(io);
    *batch_size = io_->batch;

    return AXCL_SUCC;
}
