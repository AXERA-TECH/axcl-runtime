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

#include <ax_engine_api.h>

#define TAG "engine"

namespace axcl::worker {

struct io_meta
{
    uint32_t layout = 0;
    uint32_t memory = 0;
    uint32_t format = 0;
    uint32_t color = 0;
    uint32_t size = 0;
    uint32_t dim = 0;
    uint32_t shape[8] = {};
    uint32_t stride[8] = {};
};

static auto gen_meta_package(const AX_ENGINE_IOMETA_T* meta, const AX_U32 size) {
    using array = pkg::uint8_array;

    LOG_MM_D(TAG, "Size of struct _meta: {}.", sizeof(io_meta));

    size_t max_name_length = 0;
    for (AX_U32 i = 0; i < size; i++) {
        max_name_length = std::max(max_name_length, strlen(meta[i].pName) + 1);
    }

    const auto one_buffer_size = sizeof(io_meta) + max_name_length;
    const auto buffer_size = one_buffer_size * size;
    const std::shared_ptr<uint8_t[]> buffer(new uint8_t[buffer_size]());
    memset(buffer.get(), 0, buffer_size);

    const array package = {buffer.get(), static_cast<uint32_t>(buffer_size)};

    for (AX_U32 i = 0; i < size; i++) {
        const auto cur_ptr = buffer.get() + i * one_buffer_size;
        const auto meta_ptr = reinterpret_cast<io_meta*>(cur_ptr);
        const auto name_ptr = cur_ptr + sizeof(io_meta);

        meta_ptr->layout = meta[i].eLayout;
        meta_ptr->memory = meta[i].eMemoryType;
        meta_ptr->format = meta[i].eDataType;
        meta_ptr->color = (nullptr != meta[i].pExtraMeta ? meta[i].pExtraMeta->eColorSpace : std::numeric_limits<uint32_t>::max());
        meta_ptr->size = meta[i].nSize;
        meta_ptr->dim = meta[i].nShapeSize;
        for (AX_U8 j = 0; j < meta[i].nShapeSize; j++) {
            meta_ptr->shape[j] = meta[i].pShape[j];
        }
        if (nullptr != meta[i].pStride) {
            for (AX_U8 j = 0; j < meta[i].nShapeSize; j++) {
                meta_ptr->stride[j] = meta[i].pStride[j];
            }
        }

        memcpy(name_ptr, meta[i].pName, strlen(meta[i].pName));
        LOG_MM_D(TAG, "Copy meta[{}] name: {}.", i, std::string(meta[i].pName));
    }

    return std::make_tuple(package, buffer);
}

}  // namespace axcl::worker
