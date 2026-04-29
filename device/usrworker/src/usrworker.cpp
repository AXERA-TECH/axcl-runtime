/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "usrworker.h"
#include "usrworker_def.h"
#include "axcl_rt_usrwork_type.h"
#include <unistd.h>
#include <sys/types.h>
#include <cstring>
#include <chrono>
#include <thread>
#include <errno.h>
#include <ctime>
#include "axcl_logger.hpp"
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

#define TAG "usrworker"

usrworker::usrworker() : m_running(false) {
    memset(&m_init_config, 0, sizeof(IPC_MSG_INIT_T));
}

usrworker::~usrworker() {
    deinit();
}

int32_t usrworker::init() {
    LOG_MM_D(TAG, "+++");

    m_ipc = std::make_unique<ChildIPC>();
    if (m_ipc->init() != 0) {
        LOG_MM_E(TAG, "Failed to initialize IPC");
        return -1;
    }

    if (wait_for_init_message() != 0) {
        return -1;
    }

    if (setup() != 0) {
        return -1;
    }

    LOG_MM_D(TAG, "---");
    return 0;
}

void usrworker::deinit() {
    LOG_MM_D(TAG, "+++");
    destroy();
    if (m_ipc) {
        m_ipc->deinit();
        m_ipc.reset();
    }
    LOG_MM_D(TAG, "---");
}

void usrworker::set_status_callback(StatusCallback cb) {
    m_status_cb = cb;
}

void usrworker::monitor_thread_func(int heartbeat_miss_threshold) {

    auto last_heartbeat = std::chrono::steady_clock::now();
    m_last_ack_time = last_heartbeat;

    while (m_running) {
        IPC_MSG_T msg;
        if (m_ipc->receive_ack(msg) == 0) {
            switch (msg.body.type) {
                case IPC_MSG_TYPE_ACK:
                    if (msg.body.ack.type == IPC_MSG_TYPE_HEARTBEAT) {
                        m_last_ack_time = std::chrono::steady_clock::now();
                    }
                    break;
                default:
                    LOG_MM_W(TAG, "Unknown message type: {}", static_cast<int>(msg.body.type));
                    break;
            }
        } else {
            if (!m_running) {
                break;
            }
        }

        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_heartbeat);
        if (elapsed.count() >= m_init_config.heartbeat_interval) {
            int32_t timestamp = static_cast<int32_t>(std::time(nullptr));
            if (m_ipc->send_heartbeat(timestamp, 0) == 0) {
                last_heartbeat = now;
            } else {
                LOG_MM_E(TAG, "Failed to send heartbeat to parent");
                break;
            }
        }

        auto ack_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_last_ack_time);
        if (ack_elapsed.count() >= m_init_config.heartbeat_interval * heartbeat_miss_threshold) {
            LOG_MM_E(TAG, "Heartbeat ack timeout, parent may be dead. interval={}ms, threshold={}",
                     m_init_config.heartbeat_interval, heartbeat_miss_threshold);
            if (m_status_cb) {
                m_status_cb(AXCL_WORKER_STATUS_PARENT_DEAD);
            }
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(m_init_config.heartbeat_interval));
    }
}

int32_t usrworker::setup() {

    m_channel = std::make_unique<usrworkchannel>();
    axcl::comm::comm_args_t args;
    args.pcie.handshake = false;
    args.pcie.target = 0;
    args.pcie.port[0] = m_init_config.ports[0];
    args.pcie.port[1] = m_init_config.ports[1];
    args.pcie.port[2] = m_init_config.ports[2];
    args.pcie.port[3] = m_init_config.ports[3];
    if (m_channel->open(args) != 0) {
        LOG_MM_E(TAG, "Failed to open communication channel");
        return -1;
    }

    m_running = true;
    m_monitor_thread = std::thread([this]() {
        monitor_thread_func();
    });

    return 0;
}

void usrworker::destroy() {
    if (m_channel) {
        m_channel->close();
        m_channel.reset();
    }

    m_running = false;
    if (m_monitor_thread.joinable()) {
        m_monitor_thread.join();
    }
}

int32_t usrworker::wait_for_init_message(int timeout_ms) {

    IPC_MSG_T init_msg;
    auto start_time = std::chrono::steady_clock::now();
    const auto timeout = std::chrono::milliseconds(timeout_ms);

    while (true) {
        auto now = std::chrono::steady_clock::now();
        if (now - start_time > timeout) {
            LOG_MM_E(TAG, "Timeout waiting for init message ({}ms)", timeout_ms);
            return -1;
        }

        if (m_ipc->receive_init_message(init_msg) == 0) {
            if (init_msg.body.type == IPC_MSG_TYPE_INIT) {
                m_init_config = init_msg.body.init;
                if (m_ipc->send_ack(IPC_MSG_TYPE_INIT, 0) != 0) {
                    LOG_MM_E(TAG, "Failed to send init ACK to parent");
                    return -1;
                }
                return 0;
            } else {
                LOG_MM_W(TAG, "Received non-init message while waiting for init, type: {}", static_cast<int>(init_msg.body.type));
            }
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }
}

int32_t usrworker::send_data(const void *buf, uint32_t size, int32_t timeout) {
    if (!m_channel) {
        LOG_MM_E(TAG, "Channel not initialized");
        return -1;
    }

    return m_channel->send(buf, size, timeout);
}

int32_t usrworker::recv_data(void *buf, uint32_t buf_size, uint32_t* recvlen, int32_t timeout) {
    if (!m_channel) {
        LOG_MM_E(TAG, "Channel not initialized");
        return -1;
    }

    return m_channel->recv(buf, buf_size, recvlen, timeout);
}
