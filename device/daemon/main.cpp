/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include <malloc.h>
#include <signal.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <unistd.h>
#include "device.hpp"
#include "port.hpp"
#include "axcl_logger.hpp"
#include "single_proc.hpp"

#define TAG "main"

#define DAEMON_WORK_DIR "/opt/bin/axclSlave"
#define DAEMON_LOG_FILE_SIZE (1 * 1024 * 1024)
#define DAEMON_LOG_FILE_NUM (2)
#define DAEMON_LOG_PATH "/opt/data/axclLog/slave_daemon/"
#define DAEMON_LOG_FLUSH_INTERVAL (3) // 3s

static bool g_running = true;

static void SigInt(int signo) {
    LOG_MM_I(TAG, "SigInt Catch signal {}", signo);

    g_running = false;

    AXCL_LOGGER->flush();

    // flush log
    _exit(1);
}

static void SigStop(int signo) {
    LOG_MM_I(TAG, "SigStop Catch signal {}", signo);
}

static void SigChld(int signo) {
    LOG_MM_I(TAG, "SigChld Catch signal {}", signo);

    while (waitpid(-1, nullptr, WNOHANG) > 0) {
        // clean all child process
    }
}

static void signal_child(void) {
    struct sigaction sa;

    // Set up the SIGCHLD signal handler
    sa.sa_handler = SigChld;
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP; // Restart interrupted syscalls, don't report stopped children
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        _exit(EXIT_FAILURE);
    }
}

int main(int argc, char* argv[]) {
    axcl::single_proc sp = axcl::single_proc(argv[0], 0x6178636C); // key = axcl

    prctl(PR_SET_NAME, "daemon");

    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, SigInt);
    signal(SIGTSTP, SigStop);
    signal_child();

    /* Change the working directory to the root directory */
    chdir(DAEMON_WORK_DIR);

    // "/opt/data/AXCllog/slave_daemon/slave_daemon.log"
    std::string log_file = DAEMON_LOG_PATH;
    log_file += "slave_daemon.log";

    auto logger = axcl::logger::get_instance(log_file, DAEMON_LOG_FILE_SIZE, DAEMON_LOG_FILE_NUM - 1);
    logger->info("daemon started {} {} {}", APP_BUILD_VERSION, __DATE__, __TIME__);
    logger->set_level(spdlog::level::info);
    axcl::logger::flush_every(DAEMON_LOG_FLUSH_INTERVAL);

    axcl::daemon::device device(axcl::comm::COMM_TYPE::COMM_TYPE_PCIE);
    axcl::daemon::port port;

    device.register_sink(&port);

RETRY:
    while (g_running) {
        // open device
        if (device.open()) {
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    if (!g_running) {
        goto EXIT;
    }

    // start device
    if (!device.start()) {
        device.close();
        goto RETRY;
    }

    // open port
    if (!port.open()) {
        device.close();
        goto RETRY;
    }

    // start port
    if (!port.start()) {
        port.close();
        device.close();
        goto RETRY;
    }

    while (g_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

EXIT:
    // close port
    port.close();

    // close device
    device.close();

    // flush log
    AXCL_LOGGER->flush();

    return 0;
}
