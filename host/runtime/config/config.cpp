/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "config.hpp"

#if defined(WINDOWS)
#include <windows.h>
#endif

#include <cctype>
#include <fstream>
#include "cJSON.h"

axcl::config::config() {
#if defined(WINDOWS)
    char buffer[MAX_PATH];
    GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    char* pos = strrchr(buffer, '\\');
    if (pos != nullptr) {
        *pos = '\0';
    }

    m_log_path = buffer;
    m_log_path += "\\log\\axcl_logs.txt";
#endif
}

bool axcl::config::load(const char* json) {
    if (!json) {
        /* default configuration */
        return true;
    }

    std::lock_guard<std::mutex> lck(m_mtx);
    if (m_loaded) {
        return true;
    }

    std::string content;
    const char* p = json;
    while (*p && std::isspace(*p)) {
        ++p;
    }
    if (*p == '{' || *p == '[') {
        /* json string */
        content = json;
    } else {
        printf("json file\n");
        /* json file */
        std::ifstream ifs(json);
        if (!ifs.is_open()) {
            return false;
        }

        std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
        ifs.close();

        if (content.empty()) {
            return false;
        }
    }

    cJSON* root = cJSON_Parse(content.c_str());
    if (!root) {
        return false;
    }

    parse_log_config(root);
    parse_dma_buf_size_config(root);

    cJSON_Delete(root);

    m_loaded = true;
    return true;
}

void axcl::config::unload() {
    std::lock_guard<std::mutex> lck(m_mtx);
    m_loaded = false;
}

void axcl::config::parse_log_config(cJSON* root) {
    cJSON* log = cJSON_GetObjectItem(root, "log");
    if (log) {
        cJSON* host = cJSON_GetObjectItem(log, "host");
        if (host) {
            cJSON* path = cJSON_GetObjectItem(host, "path");
            if (path && cJSON_IsString(path)) {
                m_log_path = path->valuestring;
            }

            cJSON* lv = cJSON_GetObjectItem(host, "level");
            if (lv && cJSON_IsNumber(lv)) {
                m_log_level[0] = lv->valueint;
            }
        }

        cJSON* device = cJSON_GetObjectItem(log, "device");
        if (device) {
            cJSON* lv = cJSON_GetObjectItem(device, "level");
            if (lv && cJSON_IsNumber(lv)) {
                m_log_level[1] = lv->valueint;
            }
        }
    }
}

void axcl::config::parse_dma_buf_size_config(cJSON* root) {
    cJSON* node = cJSON_GetObjectItem(root, "dma buf size");
    if (node) {
        uint32_t size = 0;
        if (cJSON_IsString(node)) {
            std::string str = node->valuestring;

            if (str.size() > 2 && str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
                size = static_cast<uint32_t>(std::stoul(str, nullptr, 16));
            } else {
                if (str.size() > 0) {
                    bool hex = false;
                    for (size_t i = 0; i < str.size(); ++i) {
                        if (std::isxdigit(str[i])) {
                            hex = true;
                            break;
                        }
                    }

                    size = static_cast<uint32_t>(std::stoul(str, nullptr, hex ? 16 : 10));
                }
            }

        } else if (cJSON_IsNumber(node)) {
            if (node->valueint > 0) {
                size = static_cast<uint32_t>(node->valueint);
            }
        }

        if (size >= 0x100000 /* min: 1MBytes */) {
            m_dma_buf_size = size;
        }
    }
}