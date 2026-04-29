/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include <array>
#include <malloc.h>
#include <signal.h>
#include <sys/prctl.h>
#include <sys/prctl.h>
#include <unistd.h>
#include "device.hpp"
#include "axcl_logger.hpp"
#include "ax_sys_api.h"

#define TAG "main"

#define WORKER_WORK_DIR "/opt/bin/axclSlave"
#define WORKER_LOG_FILE_SIZE (1 * 1024 * 1024)
#define WORKER_LOG_FILE_NUM (2)
#define WORKER_LOG_PATH "/opt/data/axclLog/slave_worker/"
#define WORKER_LOG_FLUSH_INTERVAL (3) // 3s

static bool g_running = true;

class sys_glo {
public:
    sys_glo() {
        ::AX_SYS_Init();
    }

    ~sys_glo() {
        ::AX_SYS_Deinit();
    }
};

// construct for sys init
sys_glo sys;

static void SigInt(int signo) {
    g_running = false;
    _exit(1);
}

static void SigTerm(int signo) {
    g_running = false;
    _exit(1);
}

static void SigStop(int signo) {
    LOG_MM_I(TAG, "SigStop Catch signal {}", signo);
}

int main(int argc, char* argv[]) {

    std::array<uint32_t, 4> ports = {0};

    if (argc != 7) {
        printf("invalid input args num %d\n", argc);
        _exit(EXIT_FAILURE);
    }

    ports[0] = atoi(argv[1]);
    ports[1] = atoi(argv[2]);
    ports[2] = atoi(argv[3]);
    ports[3] = atoi(argv[4]);

    const uint32_t dma_buf_size = atoi(argv[5]);
    const uint32_t log_level = atoi(argv[6]);

    if (ports[0] <= 0
        || ports[1] <= 0
        || ports[2] <= 0
        || ports[3] <= 0) {
        printf("invalid ports %d %d %d %d\n", ports[0], ports[1], ports[2], ports[3]);
        _exit(EXIT_FAILURE);
    }

    prctl(PR_SET_NAME, "worker");

    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, SigInt);
    signal(SIGTSTP, SigStop);
    signal(SIGTERM, SigTerm);

    /* Change the working directory to the root directory */
    chdir(WORKER_WORK_DIR);

    // "/opt/data/AXCllog/slave_worker/slave_worker_port15_16_17_18.log"
    std::string log_file = WORKER_LOG_PATH;
    log_file = log_file + "slave_worker" + "_port"
                + std::to_string(ports[0])
                + "_"
                + std::to_string(ports[1])
                + "_"
                + std::to_string(ports[2])
                + "_"
                + std::to_string(ports[3])
                + ".log";

    auto logger = axcl::logger::get_instance(log_file, WORKER_LOG_FILE_SIZE, WORKER_LOG_FILE_NUM - 1);
    logger->info("worker started {} {} {}, port {} {} {} {}", APP_BUILD_VERSION, __DATE__, __TIME__, ports[0], ports[1], ports[2], ports[3]);
    logger->set_level(axcl::logger::get_level(log_level));

    axcl::worker::device device(axcl::comm::COMM_TYPE::COMM_TYPE_PCIE, ports, dma_buf_size);

    // open device
    if (!device.open()) {
        _exit(EXIT_FAILURE);
    }

    // start device
    if (!device.start()) {
        _exit(EXIT_FAILURE);
    }

    while (g_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    // close device
    device.close();

    // flush log
    AXCL_LOGGER->flush();

    return 0;
}
