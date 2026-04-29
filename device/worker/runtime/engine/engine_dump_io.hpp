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

#include <sstream>

#define AXCL_ENGINE_HOST_DEBUG

#define TAG "engine"

namespace axcl::worker {

static std::string engine_meta_dump(const pkg::runtime::engine::io_meta *meta) {
    std::string group_string[meta->groups.size()];

    for (size_t i = 0; i < meta->groups.size(); i++) {
        const auto &[size, shape, stride] = meta->groups[i];

        std::stringstream shape_string, stride_string;
        shape_string << "(" << shape.size() << ": [";
        for (size_t j = 0; j < shape.size() - 1; j++) {
            shape_string << shape[j] << ",";
        }
        shape_string << shape[shape.size() - 1] << "])";
        if (!stride.empty()) {
            stride_string << "(" << stride.size() << ": [";
            for (size_t j = 0; j < stride.size() - 1; j++) {
                stride_string << stride[j] << ",";
            }
            stride_string << stride[stride.size() - 1] << "])";
        }
        group_string[i].append("{shape: " + shape_string.str() + ", size: {" + std::to_string(size) + "}");
        if (!stride.empty()) {
            group_string[i].append(", stride: " + stride_string.str() + "}");
        }
        group_string[i].append("}");
    }

    std::string dump_string;
    dump_string.append("name: {"
        + meta->name + "}, layout: {"
        + std::to_string(static_cast<uint8_t>(meta->layout))
        + "}, format: {"
        + std::to_string(static_cast<uint8_t>(meta->format))
        + "}, groups: {");

    for (size_t i = 0; i < meta->groups.size() - 1; i++) {
        dump_string.append(group_string[i] + ",");
    }
    dump_string.append(group_string[meta->groups.size() - 1] + "}");
    return {dump_string};
}


static void engine_info_dump(const pkg::runtime::engine::meta *io) {
#ifdef AXCL_ENGINE_HOST_DEBUG
    LOG_MM_D(TAG, "Total input size: {}.", io->inputs.size());
    for (size_t i = 0; i < io->inputs.size(); i++) {
        LOG_MM_D(TAG, "I->[{}]: {}.", i, engine_meta_dump(&io->inputs[i]));
    }
    LOG_MM_D(TAG, "Total output size: {}.", io->outputs.size());
    for (size_t i = 0; i < io->outputs.size(); i++) {
        LOG_MM_D(TAG, "O->[{}]: {}.", i, engine_meta_dump(&io->outputs[i]));
    }
#endif
}

} // namespace axcl::worker
