/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "usrworker_manager.h"
#include <iostream>
#include <cstring>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <algorithm>
#include <thread>
#include <chrono>
#include "axcl_logger.hpp"
#include "usrworker_ipc.h"

#define TAG "usrworker_manager"
#define WORKER_INIT_TIMEOUT_MS 5000
#define WORKER_EXIT_TIMEOUT_MS 5000

usrworker_manager::usrworker_manager() :
    m_monitor_thread(),
    m_workers() {
}

usrworker_manager::~usrworker_manager() {
    deinit();
}

int32_t usrworker_manager::init() {
    m_pid = getpid();
    m_ipc = std::make_unique<ParentIPC>(m_pid);
    if (0 != m_ipc->init()) {
        LOG_MM_E(TAG, "Failed to initialize IPC");
        return -1;
    }

    m_stop_monitor = false;
    m_monitor_thread = std::thread([this]() {
        monitor_workers_thread_func();
    });

    return 0;
}

void usrworker_manager::deinit() {
    if (m_pid == 0) {
        return;
    }

    m_stop_monitor = true;
    if (m_monitor_thread.joinable()) {
        m_monitor_thread.join();
    }

    if (m_ipc) {
        m_ipc->deinit();
    }

    std::vector<uint32_t> pids = get_worker_pids();
    for (auto pid : pids) {
        if (0 != stop_worker(pid)) {
            LOG_MM_W(TAG, "Failed to stop worker {}", pid);
        }
    }

    std::lock_guard<std::mutex> lock(m_mutex);
    m_workers.clear();

    m_pid = 0;
}

int32_t usrworker_manager::exec_worker(const std::string& bin_path,
            const std::vector<std::string>& args,
            const IPC_MSG_INIT_T& init,
            uint32_t& out_pid,
            int32_t timeout_ms) {

    if (bin_path.empty()) {
        LOG_MM_E(TAG, "Invalid binary path");
        return -1;
    }

    pid_t child_pid = fork();
    if (child_pid < 0) {
        LOG_MM_E(TAG, "Failed to fork process: {}", strerror(errno));
        return -1;
    }

    if (child_pid == 0) {
        std::vector<char*> argv;
        argv.reserve(args.size() + 2);
        argv.push_back(const_cast<char*>(bin_path.c_str()));
        for (const auto& arg : args) {
            argv.push_back(const_cast<char*>(arg.c_str()));
        }
        argv.push_back(nullptr);
        execv(bin_path.c_str(), argv.data());
        LOG_MM_E(TAG, "failed to exec {}: {}", bin_path.c_str(), strerror(errno));
        exit(EXIT_FAILURE);
    } else {
        WorkerStatus ws(bin_path, args, init);
        add_worker(child_pid, std::move(ws));

        if (0 == m_ipc->send_init_message(child_pid, init)) {
            if (0 != wait_for_ack(child_pid, ACK_FLAG_INIT, WORKER_INIT_TIMEOUT_MS)) {
                LOG_MM_E(TAG, "Timeout waiting for config ACK from worker {}", child_pid);
                del_worker(child_pid);
                return -1;
            }
        } else {
            LOG_MM_E(TAG, "Failed to send config message to worker {}", child_pid);
            del_worker(child_pid);
            return -1;
        }

        if (timeout_ms != 0) {
            int status;
            pid_t ret;
            if (timeout_ms == -1) {
                ret = waitpid(child_pid, &status, 0);
            } else {
                auto start_time = std::chrono::steady_clock::now();
                while (true) {
                    ret = waitpid(child_pid, &status, WNOHANG);
                    if (ret == child_pid || (ret == -1 && errno == ECHILD)) {
                        break;
                    } else if (ret == -1) {
                        LOG_MM_E(TAG, "waitpid failed for worker {}: {}", child_pid, strerror(errno));
                        del_worker(child_pid);
                        return -1;
                    }

                    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::steady_clock::now() - start_time);
                    if (elapsed.count() >= timeout_ms) {
                        LOG_MM_W(TAG, "Timeout waiting for worker {} to exit after {} ms", child_pid, timeout_ms);
                        out_pid = child_pid;
                        return -1;
                    }

                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            }

            if (ret == child_pid) {
                LOG_MM_I(TAG, "Worker {} exited with status=0x{:x}", child_pid, status);
                del_worker(child_pid);
                out_pid = child_pid;
                return 0;
            }
        }

        out_pid = child_pid;
        return 0;
    }

    return -1;
}

int32_t usrworker_manager::stop_worker(uint32_t child_pid) {
    WorkerStatus* ws = nullptr;
    if (!get_worker(child_pid, ws)) {
        LOG_MM_W(TAG, "Worker {} not found in active workers list", child_pid);
        return 0;
    }

    if (kill(child_pid, 0) != 0) {
        LOG_MM_I(TAG, "Worker {} process not found, cleaning up resources", child_pid);
        del_worker(child_pid);
        return 0;
    }

    LOG_MM_W(TAG, "Force killing worker {} with SIGKILL", child_pid);
    if (kill(child_pid, SIGKILL) == 0) {
        int status;
        pid_t ret = waitpid(child_pid, &status, 0);
        if (ret == static_cast<pid_t>(child_pid)) {
            LOG_MM_I(TAG, "Worker {} exited with status=0x{:x}", child_pid, status);
        }
    } else {
        LOG_MM_E(TAG, "Failed to send SIGKILL to worker {}: {}", child_pid, strerror(errno));
    }

    del_worker(child_pid);
    return 0;
}

void usrworker_manager::monitor_workers_thread_func() {
    const int SLEEP_INTERVAL_MS = 300;
    while (!m_stop_monitor) {
        IPC_MSG_T msg;
        int recv_result = m_ipc->receive_message(msg);
        if (recv_result == 0) {
            // process message
            switch (msg.body.type) {
                case IPC_MSG_TYPE_HEARTBEAT: {
                    WorkerStatus* ws = nullptr;
                    if (get_worker(msg.body.pid, ws)) {
                        ws->last_heartbeat = std::chrono::steady_clock::now();
                    }
                    m_ipc->send_ack(msg.body.pid, IPC_MSG_TYPE_HEARTBEAT, 0);
                    break;
                }

                case IPC_MSG_TYPE_ACK: {
                    uint32_t ack_flag = (msg.body.ack.type == IPC_MSG_TYPE_INIT) ? ACK_FLAG_INIT : 0;
                    if (ack_flag != 0) {
                        notify_ack_received(msg.body.pid, ack_flag);
                    } else {
                        LOG_MM_E(TAG, "Unknown ACK type: {}", static_cast<int>(msg.body.ack.type));
                    }
                    break;
                }

                default:
                    LOG_MM_E(TAG, "Unknown message type: {}", static_cast<int>(msg.body.type));
                    break;
            }
        } else if (recv_result == -2) {
            // Timeout, continue loop
        } else {
            if (m_stop_monitor) {
                break;
            }
        }

        // check worker status
        auto now = std::chrono::steady_clock::now();
        const int N = 3;

        auto pids = get_worker_pids();
        for (auto pid : pids) {
            WorkerStatus* ws = nullptr;
            if (get_worker(pid, ws)) {
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - ws->last_heartbeat).count();
                uint32_t interval = ws->init.heartbeat_interval;

                if (elapsed > N * interval) {
                    LOG_MM_E(TAG, "Worker {} heartbeat timeout, elapsed {} ms", pid, elapsed);
                    bool alive = (kill(pid, 0) == 0) || (errno != ESRCH);
                    if (!alive) {
                        LOG_MM_I(TAG, "Worker {} process not found, cleaning up resources", pid);
                        del_worker(pid);
                    } else {
                        LOG_MM_W(TAG, "Worker {} process exists but heartbeat timeout, force stopping", pid);
                        stop_worker(pid);
                    }
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_INTERVAL_MS));
    }
}

std::vector<uint32_t> usrworker_manager::get_worker_pids() const {
    std::lock_guard<std::mutex> lock(const_cast<std::mutex&>(m_mutex));
    std::vector<uint32_t> pids;
    pids.reserve(m_workers.size());
    for (const auto& worker : m_workers) {
        pids.push_back(worker.first);
    }
    return pids;
}

int32_t usrworker_manager::wait_for_ack(uint32_t pid, uint32_t ack_flag, uint32_t timeout_ms) {
    std::unique_lock<std::mutex> lock(m_mutex);
    auto it = m_workers.find(pid);
    if (it == m_workers.end()) {
        LOG_MM_E(TAG, "No worker found for PID {}", pid);
        return -1;
    }

    auto& worker = it->second;
    LOG_MM_I(TAG, "Waiting for ACK flag 0x{:x} from PID {}, current_flags=0x{:x}", ack_flag, pid, worker.pending_ack_flags);

    bool received = worker.ack_cv->wait_for(lock, std::chrono::milliseconds(timeout_ms), [&]() {
        return (worker.pending_ack_flags & ack_flag) != 0;
    });

    if (received) {
        worker.pending_ack_flags &= ~ack_flag;
        LOG_MM_I(TAG, "ACK flag 0x{:x} received from PID {}, cleared flag, remaining_flags=0x{:x}", ack_flag, pid, worker.pending_ack_flags);
    } else {
        LOG_MM_E(TAG, "Timeout waiting for ACK flag 0x{:x} from PID {}", ack_flag, pid);
    }

    return received ? 0 : -1;
}

void usrworker_manager::notify_ack_received(uint32_t pid, uint32_t ack_flag) {
    WorkerStatus* ws = nullptr;
    if (get_worker(pid, ws)) {
        ws->pending_ack_flags |= ack_flag;
        ws->ack_cv->notify_one();
    }
}

void usrworker_manager::add_worker(uint32_t pid, WorkerStatus&& ws) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_workers[pid] = std::move(ws);
}

void usrworker_manager::del_worker(uint32_t pid) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_workers.erase(pid);
}

bool usrworker_manager::get_worker(uint32_t pid, WorkerStatus*& ws) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_workers.find(pid);
    if (it != m_workers.end()) {
        ws = &(it->second);
        return true;
    }
    ws = nullptr;
    return false;
}

bool usrworker_manager::is_worker_alive(uint32_t pid) const {
    auto running_pids = get_worker_pids();
    return std::find(running_pids.begin(), running_pids.end(), pid) != running_pids.end();
}