#ifndef USRWORKER_MANAGER_H
#define USRWORKER_MANAGER_H

#include <memory>
#include <thread>
#include <atomic>
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <sys/types.h>
#include <unistd.h>
#include <cstdint>
#include "usrworker_def.h"
#include "usrworker_ipc.h"

class ParentIPC;
class usrworker_manager
{
    struct WorkerStatus {
        std::string bin_path;
        std::vector<std::string> args;
        IPC_MSG_INIT_T init;
        std::chrono::steady_clock::time_point last_heartbeat;

        std::unique_ptr<std::condition_variable> ack_cv;
        uint32_t pending_ack_flags{0};
        WorkerStatus() = default;
        WorkerStatus(const std::string& path, const std::vector<std::string>& args_vec, const IPC_MSG_INIT_T& init_data)
            : bin_path(path), args(args_vec), init(init_data), last_heartbeat(std::chrono::steady_clock::now()) {
            ack_cv = std::make_unique<std::condition_variable>();
        }
    };

public:
    usrworker_manager();
    ~usrworker_manager();
    usrworker_manager(const usrworker_manager&) = delete;
    usrworker_manager& operator=(const usrworker_manager&) = delete;

    int32_t init();
    void deinit();
    int32_t exec_worker(const std::string& bin_path,
                       const std::vector<std::string>& args,
                       const IPC_MSG_INIT_T& init,
                       uint32_t& out_pid,
                       int32_t timeout_ms = 0);
    int32_t stop_worker(uint32_t pid);
    std::vector<uint32_t> get_worker_pids() const;
    bool is_worker_alive(uint32_t pid) const;

private:
    uint32_t m_pid{0};
    std::atomic<bool> m_stop_monitor{false};
    std::thread m_monitor_thread;
    std::mutex m_mutex;
    std::map<uint32_t, WorkerStatus> m_workers;
    std::unique_ptr<ParentIPC> m_ipc;

    void monitor_workers_thread_func();

    void add_worker(uint32_t pid, WorkerStatus&& ws);
    void del_worker(uint32_t pid);
    bool get_worker(uint32_t pid, WorkerStatus*& ws);

    void notify_ack_received(uint32_t pid, uint32_t ack_flag);
    int32_t wait_for_ack(uint32_t pid, uint32_t ack_flag, uint32_t timeout_ms);
};

#endif // USRWORKER_MANAGER_H
