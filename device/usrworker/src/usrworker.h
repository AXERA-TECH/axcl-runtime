#ifndef USRWORKER_H
#define USRWORKER_H

#include <memory>
#include <thread>
#include <atomic>
#include <string>
#include <vector>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <sys/types.h>
#include <unistd.h>
#include <cstdint>
#include <functional>
#include <cstddef>
#include "usrworker_def.h"
#include "usrworker_ipc.h"
#include "usrworker_channel.h"
#include "axcl_worker_runtime.h"

#define DEFAULT_HEARTBEAT_MISS_THRESHOLD 3
#define DEFAULT_INIT_TIMEOUT_MS 3000

class usrworker
{
public:
    usrworker();
    ~usrworker();
    int32_t init();
    void deinit();
    using StatusCallback = std::function<void(AXCL_WORKER_STATUS_E status)>;
    void set_status_callback(StatusCallback cb);

    int32_t send_data(const void *buf, uint32_t size, int32_t timeout);
    int32_t recv_data(void *buf, uint32_t buf_size, uint32_t* recvlen, int32_t timeout);

private:
    std::atomic<bool> m_running{false};
    std::thread m_monitor_thread;
    std::unique_ptr<ChildIPC> m_ipc;
    std::unique_ptr<usrworkchannel> m_channel;
    IPC_MSG_INIT_T m_init_config;

    StatusCallback m_status_cb;
    std::chrono::steady_clock::time_point m_last_ack_time;

    void monitor_thread_func(int heartbeat_miss_threshold = DEFAULT_HEARTBEAT_MISS_THRESHOLD);
    int32_t wait_for_init_message(int timeout_ms = DEFAULT_INIT_TIMEOUT_MS);
    int32_t setup();
    void destroy();
};

#endif // USRWORKER_H
