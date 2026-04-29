/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <atomic>
#include <chrono>
#include <thread>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include <cstdint>
#include "axcl_worker_runtime.h"

#define TAG "child_app"

typedef enum {
    PROCESS_MODE_NORMAL = 0,
    PROCESS_MODE_CRASH = 1,
    PROCESS_MODE_INTERACTIVE = 2
} PROCESS_MODE_E;

static volatile int g_running = 1;
static PROCESS_MODE_E g_process_mode = PROCESS_MODE_NORMAL;
static int g_execution_seconds = -1;

static void signal_handler(int sig) {
    printf("[%s] Received signal %d, preparing to exit\n", TAG, sig);
    g_running = 0;
}

void print_usage(const char* program_name) {
    printf("Usage: %s [options]\n", program_name);
    printf("Options:\n");
    printf("  -h, --help     Show this help message\n");
    printf("  -t <type>      Process type: 0=normal, 1=crash, 2=interactive (default: 0)\n");
    printf("  -s <seconds>   Execution time in seconds, -1 for infinite (default: -1)\n");
    printf("\n");
    printf("Examples:\n");
    printf("  %s              # Normal mode, run indefinitely\n", program_name);
    printf("  %s -t 0         # Normal mode (explicit)\n", program_name);
    printf("  %s -t 1         # Crash mode\n", program_name);
    printf("  %s -t 2         # Interactive mode\n", program_name);
    printf("  %s -s 15        # Normal mode, run for 15 seconds\n", program_name);
}

bool parse_args(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        const char* arg = argv[i];

        if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
            print_usage(argv[0]);
            return false;
        } else if (strcmp(arg, "-t") == 0) {
            if (i + 1 < argc) {
                int type = atoi(argv[i + 1]);
                if (type == 0) {
                    g_process_mode = PROCESS_MODE_NORMAL;
                } else if (type == 1) {
                    g_process_mode = PROCESS_MODE_CRASH;
                } else if (type == 2) {
                    g_process_mode = PROCESS_MODE_INTERACTIVE;
                } else {
                    printf("[%s] Invalid type: %s (must be 0, 1, or 2)\n", TAG, argv[i + 1]);
                    print_usage(argv[0]);
                    return false;
                }
                i++;
            } else {
                printf("[%s] Missing type value for -t option\n", TAG);
                print_usage(argv[0]);
                return false;
            }
        } else if (strcmp(arg, "-s") == 0) {
            if (i + 1 < argc) {
                g_execution_seconds = atoi(argv[i + 1]);
                if (g_execution_seconds < -1) {
                    printf("[%s] Invalid execution time: %s (must be >= -1)\n", TAG, argv[i + 1]);
                    print_usage(argv[0]);
                    return false;
                }
                i++;
            } else {
                printf("[%s] Missing execution time value for -s option\n", TAG);
                print_usage(argv[0]);
                return false;
            }
        } else if (arg[0] == '-') {
            printf("[%s] Unknown option: %s\n", TAG, arg);
            print_usage(argv[0]);
            return false;
        } else {
            printf("[%s] Unknown argument: %s\n", TAG, arg);
            print_usage(argv[0]);
            return false;
        }
    }
    return true;
}

static void parent_status_callback(AXCL_WORKER_STATUS_E status) {
    if (status == AXCL_WORKER_STATUS_PARENT_DEAD) {
        printf("[%s] [CALLBACK] Parent process is dead or unresponsive!\n", TAG);
    } else if (status == AXCL_WORKER_STATUS_OK) {
        printf("[%s] [CALLBACK] Status: OK\n", TAG);
    } else {
        printf("[%s] [CALLBACK] Status: %d\n", TAG, status);
    }
}

int main(int argc, char* argv[]) {

    if (!parse_args(argc, argv)) {
        return 1;
    }
    printf("[%s] Child process started, PID: %d. ", TAG, getpid());

    switch (g_process_mode) {
        case PROCESS_MODE_INTERACTIVE:
            printf("Running in INTERACTIVE mode");
            break;
        case PROCESS_MODE_CRASH:
            printf("Running in CRASH mode");
            break;
        case PROCESS_MODE_NORMAL:
        default:
            printf("Running in NORMAL mode");
            break;
    }

    if (g_execution_seconds >= 0) {
        printf(", will run for %d seconds.", g_execution_seconds);
    } else {
        printf(", will run indefinitely");
    }
    printf("\n");

    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);

    if (axclrtWorkerInit() != 0) {
        printf("[%s] Failed to initialize runtime\n", TAG);
        return -1;
    }
    axclrtWorkerSetStatusCallback(parent_status_callback);

    int counter = 0;
    auto start_time = std::chrono::steady_clock::now();

    if (g_process_mode == PROCESS_MODE_INTERACTIVE) {
        printf("[%s] Interactive mode: waiting for message...\n", TAG);
        char recv_buffer[1024];
        uint32_t recv_len = 0;
        int32_t ret = axclrtWorkerRecv(recv_buffer, sizeof(recv_buffer), &recv_len, 5000);
        if (ret == 0) {
            printf("[%s] Received message: %.*s (length: %u)\n", TAG, recv_len, recv_buffer, recv_len);
            ret = axclrtWorkerSend(recv_buffer, recv_len, 5000);
            if (ret == 0) {
                printf("[%s] Successfully sent message back\n", TAG);
            } else {
                printf("[%s] Failed to send message back, error: %d\n", TAG, ret);
            }
        } else {
            printf("[%s] Failed to receive message, error: %d\n", TAG, ret);
        }

        printf("[%s] Interactive mode completed, exiting\n", TAG);
    } else {
        while (g_running) {
            if (g_execution_seconds >= 0) {
                auto current_time = std::chrono::steady_clock::now();
                auto duration = current_time - start_time;
                auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
                if (elapsed >= g_execution_seconds) {
                    if (g_process_mode == PROCESS_MODE_CRASH) {
                        printf("[%s] Execution time limit reached (%d seconds), triggering crash...\n", TAG, g_execution_seconds);
                        int* ptr = nullptr;
                        *ptr = 100;
                    } else {
                        printf("[%s] Execution time limit reached (%d seconds), exiting\n", TAG, g_execution_seconds);
                        break;
                    }
                }
            }
            sleep(1);
            counter++;

            if (g_process_mode == PROCESS_MODE_CRASH && g_execution_seconds < 0 && counter > 10) {
                printf("[%s] Simulating crash...\n", TAG);
                int* ptr = nullptr;
                *ptr = 100;
            }
        }
    }

    axclrtWorkerDeInit();

    printf("[%s] Child process exiting normally\n", TAG);
    return 0;
}