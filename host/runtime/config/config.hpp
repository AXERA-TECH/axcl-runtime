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

#pragma once

#include <cstdint>
#include <mutex>
#include <string>

struct cJSON;
namespace axcl {

class config {
public:
    config();

    bool load(const char* json);
    void unload();

    const char* get_log_path() const {
        return m_log_path.c_str();
    }

    int32_t get_log_level(int32_t device) const {
        return m_log_level[(0 == device) ? 0 : 1];
    }

    uint32_t get_dma_buf_size() const {
        return m_dma_buf_size;
    }

protected:
    void parse_log_config(cJSON* root);
    void parse_dma_buf_size_config(cJSON* root);

private:
    std::mutex m_mtx;
    bool m_loaded = {false};

#if defined(WINDOWS)
    std::string m_log_path;
#else
    std::string m_log_path = "/tmp/axcl/axcl_logs.txt";
#endif
    /* 0: trace, 1: debug, 2: info, 3: warn, 4: error, 5: critical, 6: off */
    int32_t m_log_level[2] = {2, 2};
    uint32_t m_dma_buf_size = 0x400000;
};

}  // namespace axcl