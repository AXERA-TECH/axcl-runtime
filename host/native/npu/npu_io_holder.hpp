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

#include "npu.hpp"

#include <map>

inline void free_one_meta(const AX_ENGINE_IOMETA_T* meta) {
    delete[] meta->pName;
    delete[] meta->pShape;
    delete[] meta->pStride;
    delete meta->pExtraMeta;
};

inline void free_one_info(const AX_ENGINE_IO_INFO_T* info) {
    for (AX_U32 i = 0; i < info->nInputSize; i++) {
        const auto meta = &info->pInputs[i];
        free_one_meta(meta);
    }
    for (AX_U32 i = 0; i < info->nOutputSize; i++) {
        const auto meta = &info->pOutputs[i];
        free_one_meta(meta);
    }
    delete[] info->pInputs;
    delete[] info->pOutputs;
    delete info;
};

struct info_map_holder {
    using info_map = std::map<AX_U64, std::map<AX_U32, AX_ENGINE_IO_INFO_T*>>;

    static void free_io_info_map(info_map* g_map) {
        while (!g_map->empty()) {
            auto package_it = g_map->begin();
            auto package = g_map->extract(package_it);
            while (!package.mapped().empty()) {
                auto info_it = package.mapped().begin();
                auto info_package = package.mapped().extract(info_it);
                const auto info = info_package.mapped();
                free_one_info(info);
            }
        }
    }

    ~info_map_holder() {
        free_io_info_map(&this->map);
    }

    info_map map;
};
