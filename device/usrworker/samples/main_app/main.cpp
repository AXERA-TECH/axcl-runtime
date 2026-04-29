#include <time.h>
#include <iostream>
#include <csignal>
#include <thread>
#include <chrono>
#include <atomic>
#include <string>
#include <vector>
#include <memory>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <cstdlib>
#include <algorithm>
#include <sstream>
#include "usrworker_manager.h"

#define TAG "main_app"

std::atomic<bool> g_should_exit(false);
std::atomic<int> g_sigint_count(0);
static volatile time_t g_last_sigint_time = 0;

void signal_handler(int sig) {
    if (sig == SIGINT || sig == SIGTERM) {
        if (sig == SIGINT) {
            time_t now = time(nullptr);
            time_t last = g_last_sigint_time;
            if (now - last <= 2) {
                g_sigint_count++;
            } else {
                g_sigint_count = 1;
            }
            g_last_sigint_time = now;

            int count = g_sigint_count.load();
            if (count >= 3) {
                printf("forcing exit...\n");
                g_should_exit = true;
                exit(0);
            } else if (count == 1) {
                printf("normal exit...\n");
                g_should_exit = true;

            } else {
                printf("press Ctrl+C %d more time(s) to force exit\n", 3 - count);
            }
        } else {
            printf("[%s] Received SIGTERM signal, shutting down...\n", TAG);
            g_should_exit = true;
        }
    }
}

void print_usage(const char* program_name) {
    printf("Usage: %s [options] -c \"<child_process_path> [child_process_arguments...]\"\n", program_name);
    printf("\n");
    printf("Options:\n");
    printf("  -i <milliseconds>  Heartbeat interval (default: 500)\n");
    printf("  -t <seconds>       Parent process timeout to actively terminate child process (default: -1, wait indefinitely)\n");
    printf("  -c <cmdline>       Child process command line (required, in quotes)\n");
    printf("  -h, --help         Show this help\n");
    printf("\n");
    printf("Examples:\n");
    printf("  %s -t 10 -c \"./child_app -t 0 -s 20\"\n", program_name);
    printf("  %s -i 1000 -c \"./child_app -t 1 -s 10\"\n", program_name);
}

bool parse_args(int argc, char* argv[], std::string& child_path, std::vector<std::string>& child_args,
                uint32_t& heartbeat_interval_ms, int32_t& timeout_seconds) {

    heartbeat_interval_ms = 500;
    timeout_seconds = -1;

    child_path.clear();
    child_args.clear();
    std::string child_cmdline;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            print_usage(argv[0]);
            return false;
        } else if (arg == "-i") {
            if (i + 1 < argc) {
                heartbeat_interval_ms = static_cast<uint32_t>(strtoul(argv[i + 1], nullptr, 10));
                i++;
            } else {
                printf("[%s] Missing heartbeat interval value\n", TAG);
                return false;
            }
        } else if (arg == "-t") {
            if (i + 1 < argc) {
                timeout_seconds = static_cast<int32_t>(atoi(argv[i + 1]));
                if (timeout_seconds == 0) {
                    printf("[%s] Invalid timeout: %s (cannot be 0)\n", TAG, argv[i + 1]);
                    return false;
                }
                if (timeout_seconds < -1) {
                    printf("[%s] Invalid timeout: %s (must be >= -1)\n", TAG, argv[i + 1]);
                    return false;
                }
                i++;
            } else {
                printf("[%s] Missing timeout value\n", TAG);
                return false;
            }
        } else if (arg == "-c") {
            if (i + 1 < argc) {
                child_cmdline = argv[++i];
            } else {
                printf("[%s] Missing child process command line after -c\n", TAG);
                return false;
            }
        } else {
            printf("[%s] Unknown option: %s\n", TAG, arg.c_str());
            return false;
        }
    }

    if (child_cmdline.empty()) {
        printf("[%s] No child process command line specified. Use -c \"./child_app ...\"\n", TAG);
        print_usage(argv[0]);
        return false;
    }

    char buf[512];
    strncpy(buf, child_cmdline.c_str(), sizeof(buf)-1);
    buf[sizeof(buf)-1] = '\0';
    char* p = strtok(buf, " ");
    bool first = true;
    while (p) {
        if (first) {
            child_path = p;
            first = false;
        } else {
            child_args.push_back(p);
        }
        p = strtok(NULL, " ");
    }
    if (child_path.empty()) {
        printf("[%s] Invalid child process command line: %s\n", TAG, child_cmdline.c_str());
        return false;
    }
    return true;
}

IPC_MSG_INIT_T create_worker_config(uint32_t heartbeat_interval_ms) {
    IPC_MSG_INIT_T config;
    config.heartbeat_interval = heartbeat_interval_ms;
    config.port_num = 4;
    config.ports[0] = 30;
    config.ports[1] = 31;
    config.ports[2] = 32;
    config.ports[3] = 33;
    return config;
}

int main(int argc, char* argv[]) {
    // Set up signal handlers
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    printf("[%s] Starting AXCL Worker Manager...\n", TAG);

    std::string child_path;
    std::vector<std::string> child_args;
    uint32_t heartbeat_interval_ms;
    int32_t timeout_seconds;
    if (!parse_args(argc, argv, child_path, child_args, heartbeat_interval_ms, timeout_seconds)) {
        return 1;
    }

    printf("[%s] Child process path: %s", TAG, child_path.c_str());
    if (!child_args.empty()) {
        printf(", arguments: ");
        for (const auto& arg : child_args) {
            printf("%s ", arg.c_str());
        }
    }
    printf("\n");

    printf("[%s] Heartbeat interval: %u milliseconds\n", TAG, heartbeat_interval_ms);
    printf("[%s] Parent process will close child process after: %s\n", TAG,
           timeout_seconds >= 0 ? (std::to_string(timeout_seconds) + " seconds").c_str() : "wait indefinitely");

    std::unique_ptr<usrworker_manager> manager = std::make_unique<usrworker_manager>();
    if (0 != manager->init()) {
        printf("[%s] Failed to initialize worker manager\n", TAG);
        return -1;
    }
    IPC_MSG_INIT_T worker_config = create_worker_config(heartbeat_interval_ms);

    uint32_t pid;
    int32_t ret = manager->exec_worker(child_path, child_args, worker_config, pid);
    if (ret != 0) {
        printf("[%s] Failed to start child process\n", TAG);
        return -1;
    }
    printf("[%s] Started child process (PID: %u)\n", TAG, pid);

    auto start_time = std::chrono::steady_clock::now();
    uint32_t counter = 0;
    while (!g_should_exit) {
        auto current_time = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();

        auto pids = manager->get_worker_pids();
        bool process_running = std::find(pids.begin(), pids.end(), pid) != pids.end();
        if (process_running) {
            printf("[%s] Child process (PID: %u) is still running after %ld seconds\n", TAG, pid, elapsed);
        }

        if (timeout_seconds >= 0 && elapsed >= timeout_seconds && process_running) {
            printf("[%s] Timeout reached (%d seconds), actively closing child process (PID: %u)\n", TAG, timeout_seconds, pid);
            if (0 != manager->stop_worker(pid)) {
                printf("[%s] Failed to stop worker %u\n", TAG, pid);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        if (elapsed % 5 == 0 && elapsed > 0) {
            auto pids = manager->get_worker_pids();
            printf("[%s] Current child processes: ", TAG);
            for (const auto& child_pid : pids) {
                printf("%u ", child_pid);
            }
            printf("\n");
        }

        if (counter >= 10) {
            printf("[%s] Simulating crash...\n", TAG);
            int* ptr = nullptr;
            *ptr = 100;
        }
        counter++;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    manager->deinit();

    printf("[%s] WorkerManager shutdown complete\n", TAG);
    return 0;
}