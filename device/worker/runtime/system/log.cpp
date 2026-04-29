/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include <ctime>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <iomanip>
#include "log.hpp"
#include "dispatcher.hpp"
#include "axcl_logger.hpp"

#define TAG "log"

namespace axcl::worker {
#define AXCL_LOG_PATH ("/opt/data/")
#define AXCL_DAEMON_LOG_PATH ("axclLog/slave_daemon")
#define AXCL_WORKER_LOG_PATH ("axclLog/slave_worker")
#define AXCL_SYSLOG_LOG_PATH ("AXSyslog/syslog")
#define AXCL_KERNEL_LOG_PATH ("AXSyslog/kernel")

typedef enum log_type_e {
    log_daemon = 0x01,
    log_worker = 0x02,
    log_syslog = 0x10,
    log_kernel = 0x20
} log_type_e;

log::log(dispatcher *dispatcher) : base(dispatcher) {
}

void log::run(const void *data, const size_t& size) {
    DUMP(data, size);
}

void log::DUMP(const void *data, const size_t& size) {
    void *msg = nullptr;
    uint32_t msg_len = 0;
    uint32_t type = 0;
    uint32_t flag = 0;
    bool succ = false;

    axcl::pkg::device::log_response response;
    axcl::pkg::device::log_request request;

    // decode
    if (response.decode(data, size)) {
        type = response.get_type();
        flag = response.get_flag();

        auto now = std::time(nullptr);
        auto local_time = std::localtime(&now);

        std::ostringstream log_stream;

        // get log out file
        log_stream << "log_"
                   << std::setfill('0') << std::setw(4) << (local_time->tm_year + 1900)
                   << std::setfill('0') << std::setw(2) << (local_time->tm_mon + 1)
                   << std::setfill('0') << std::setw(2) << local_time->tm_mday
                   << std::setfill('0') << std::setw(2) << local_time->tm_hour
                   << std::setfill('0') << std::setw(2) << local_time->tm_min
                   << std::setfill('0') << std::setw(2) << local_time->tm_sec
                   << ".tar.gz";

       std::string log_out_file = log_stream.str();
       std::string log_out_path = "/tmp/" + log_stream.str();

       // get log dirs
       std::string log_dirs = "";

        if (type & log_daemon) {
            std::string path = AXCL_LOG_PATH;
            path += AXCL_DAEMON_LOG_PATH;
            if (std::filesystem::exists(path)) {
                log_dirs += " ";
                log_dirs += AXCL_DAEMON_LOG_PATH;
            }
        }
        if (type & log_worker) {
            std::string path = AXCL_LOG_PATH;
            path += AXCL_WORKER_LOG_PATH;
            if (std::filesystem::exists(path)) {
                log_dirs += " ";
                log_dirs += AXCL_WORKER_LOG_PATH;
            }
        }
        if (type & log_syslog) {
            std::string path = AXCL_LOG_PATH;
            path += AXCL_SYSLOG_LOG_PATH;
            if (std::filesystem::exists(path)) {
                log_dirs += " ";
                log_dirs += AXCL_SYSLOG_LOG_PATH;
            }
        }
        if (type & log_kernel) {
            std::string path = AXCL_LOG_PATH;
            path += AXCL_KERNEL_LOG_PATH;
            if (std::filesystem::exists(path)) {
                log_dirs += " ";
                log_dirs += AXCL_KERNEL_LOG_PATH;
            }
        }

        if (log_dirs.size() > 0) {
            std::string cmd = "tar -zcf "
                                + log_out_path
                                + " -C "
                                + AXCL_LOG_PATH
                                + log_dirs;

            LOG_MM_I(TAG, "log dump cmd: {}", cmd.c_str());

            std::system(cmd.c_str());

            if (std::filesystem::exists(log_out_path)) {
                const std::size_t max_size = 50 * 1024 * 1024;
                std::ifstream file(log_out_path, std::ios::binary);

                if (file) {
                    file.seekg(0, std::ios::end);
                    std::size_t file_size = file.tellg();
                    file.seekg(0, std::ios::beg);

                    LOG_MM_I(TAG, "log dump {} file size: {}", log_out_path.c_str(), file_size);

                    if (file_size < max_size) {
                        std::vector<char> buffer(file_size);
                        file.read(buffer.data(), file_size);

                        request.set_name(log_out_file);
                        request.set_content(buffer.data(), buffer.size());
                        succ = true;
                    } else {
                        succ = false;
                        request.set_content("tar.gz log file exceed size");
                    }

                    file.close();
                } else {
                    succ = false;
                    request.set_content("open log file failed");
                }

                std::filesystem::remove(log_out_file);
            } else {
                succ = false;
                request.set_content("tar command failed");
            }
        } else {
            succ = false;
            request.set_content("invalid log type");
        }
    } else {
        succ = false;
        request.set_content("decode msg failed");
    }

    if (!succ) {
        LOG_MM_E(TAG, "log dump failed type({}), flag({})", static_cast<int32_t>(type), static_cast<int32_t>(flag));
        request.set_name("failed.txt");
    }

    // encode
    {
        const auto hd = static_cast<const axcl::pkg::head *>(data);
        msg = request.encode(*hd, msg_len);
    }

    if (msg && msg_len) {
        get_dispatcher()->ack(msg, msg_len, -1, -1);
    }
}
}  // namespace axcl::worker
