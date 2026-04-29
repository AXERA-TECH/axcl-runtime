/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "cmd_info.hpp"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <chrono>
#include <iostream>
#include <map>
#include <mutex>
#include <regex>
#include <sstream>
#include <string>
#include <thread>
#include "axcl_rt_control_internal.h"
#include "axcl_rt_device_internal.h"
#include "cmd_set.hpp"
#include "device_manager.hpp"
#include "os.hpp"
#include "smi_log.hpp"
#include "smi_util.hpp"

#define TAG "smi"
#define TIMEOUT_1S (1000)
#define TIMEOUT_2S (2000)
#define TITLE_FORMAT "%-20s: "
#define PRINT_DEVICE(d) printf(TITLE_FORMAT "%d (0x%x)\n", "Device ID", (d), (d))
#define PRINT_NEXT() printf("\n")

static int32_t get_temperature(int32_t &temp) {
    constexpr const char *cmd = "cat /sys/class/thermal/thermal_zone0/temp";
    const char *output = nullptr;
    if (axclError ret = axclrtControlExecuteShellCmd(cmd, nullptr, 0, &output, TIMEOUT_1S); AXCL_SUCC != ret) {
        return ret;
    }

    if (output) {
        temp = atoi(output);
        return 0;
    }

    return 1;
}

static void parse_cpu_loading(std::string input, std::string &cpu, std::string &load, double &avg) {
    /**
     * CPU:  1.1% usr  1.1% sys  0.0% nic 97.6% idle  0.0% io  0.0% irq  0.0% sirq
     * Load average: 3.03 3.06 3.02 1/153 890
     */
    constexpr const char *PREFIX_CPUS = "CPU:";
    constexpr const char *PREFIX_LOAD = "Load average:";
    const size_t cpus_pos = input.find(PREFIX_CPUS);
    const size_t load_pos = input.find(PREFIX_LOAD);
    if (cpus_pos != std::string::npos) {
        std::string::size_type pos = cpus_pos + ::strlen(PREFIX_CPUS);
        while (input[pos] == ' ') {
            ++pos; /* remove blank */
        }

        if (load_pos != std::string::npos) {
            cpu = input.substr(pos, load_pos - pos - 1 /* \n */);
        } else {
            cpu = input.substr(pos, input.size() - pos);
        }

        size_t nic_pos = input.find("nic", pos);
        size_t idl_pos = input.find("idle", pos);
        if (nic_pos != std::string::npos && idl_pos != std::string::npos && idl_pos > nic_pos) {
            /* nic 97.6% idle => 97.6 */
            std::string::size_type beg = nic_pos + ::strlen("nic");
            std::string::size_type end = idl_pos;

            while (beg < end && input[beg] == ' ') {
                ++beg;
            }
            while (end > beg && input[end - 1] == ' ') {
                --end;
            }

            avg = std::atof(input.substr(beg, end - beg).c_str());
            avg = 100.0 - avg;
        }
    } else {
        avg = 0.0f;
        cpu = "";
    }

    if (load_pos != std::string::npos) {
        size_t pos = load_pos + ::strlen(PREFIX_LOAD);
        while (input[pos] == ' ') {
            ++pos;
        }

        load = input.substr(pos);
    } else {
        load = "";
    }
}

static int32_t get_top_avg_cpu(std::string &cpu, std::string &load, double &avg) {
    avg = 0.0f;

    // constexpr const char *cmd = R"(top -bn1 | grep "CPU:" | grep -v 'grep' | awk '{ $1=""; print substr($0,2) }')";
    constexpr const char *cmd = R"(top -bn1 | grep -E "CPU:|Load average:" | grep -v "grep")";
    const char *output = nullptr;
    if (axclError ret = axclrtControlExecuteShellCmd(cmd, nullptr, 0, &output, TIMEOUT_1S); AXCL_SUCC != ret) {
        return ret;
    }

    if (output) {
        parse_cpu_loading(output, cpu, load, avg);
    }

    return 0;
}

static int32_t get_top_avg_cpu(double &avg) {
    std::string _1;
    std::string _2;
    return get_top_avg_cpu(_1, _2, avg);
}

static int32_t get_top_avg_npu(std::map<std::string, std::string> &npu) {
    const char *output = nullptr;
    axclError ret = axclrtControlExecuteShellCmd("cat /proc/ax_proc/npu/top", nullptr, 0, &output, TIMEOUT_1S);
    if (AXCL_SUCC == ret && output) {
        /**
         * core:vnpu-Non
         * time:1
         * period:1000000
         * utilization:81%
         */
        std::string top(output);
        std::regex core_pattern(R"(core:\s*(\S+))");
        std::regex util_pattern(R"(utilization:\s*(\S+))");
        auto core_beg = std::sregex_iterator(top.begin(), top.end(), core_pattern);
        auto core_end = std::sregex_iterator();
        auto util_beg = std::sregex_iterator(top.begin(), top.end(), util_pattern);
        auto util_end = std::sregex_iterator();
        auto core_it = core_beg;
        auto util_it = util_beg;
        while (core_it != core_end && util_it != util_end) {
            npu.emplace((*core_it)[1], (*util_it)[1]);
            ++core_it;
            ++util_it;
        }

        return 0;
    }

    return 1;
}

static int32_t get_top_avg_npu(double &avg) {
    avg = 0.0f;
    std::map<std::string, std::string> cores;
    if (int32_t ret = get_top_avg_npu(cores); 0 != ret) {
        return ret;
    }

    size_t n = 0;
    for (auto &&kv : cores) {
        if (!kv.first.empty()) {
            avg += std::atof(kv.second.c_str());
            ++n;
        }
    }

    avg /= n;
    return 0;
}

static const char *get_sys_mem_info() {
#if 0
    const char *cmd =
        R"(cat /proc/meminfo | grep -E "MemTotal|MemFree|MemAvailable|Shmem:|Buffers|Cached|SwapTotal|SwapFree|SwapCached|CmaTotal|CmaFree")";
#else
    const char *cmd = "cat /proc/meminfo";
#endif
    const char *output = nullptr;
    if (axclError ret = axclrtControlExecuteShellCmd(cmd, nullptr, 0, &output, TIMEOUT_2S); AXCL_SUCC == ret) {
        return output;
    }

    return nullptr;
}

static int32_t get_sys_mem_info(uint32_t &total /* kB */, uint32_t &used /* kB */) {
    total = used = 0;

    const char *output = get_sys_mem_info();
    if (!output) {
        return 1;
    }

    /**
     * MemTotal:         954084 kB
     * MemFree:          778992 kB
     * MemAvailable:     760380 kB
     * ...
     */
    const char *total_str = strstr(output, "MemTotal:");
    if (total_str) {
        sscanf(total_str, "MemTotal: %u", &total);
    }

    const char *free_str = strstr(output, "MemFree:");
    if (free_str) {
        uint32_t mem_free = 0;
        if (sscanf(free_str, "MemFree: %u", &mem_free) == 1) {
            used = total - mem_free;
        }
    }

    return 0;
}

static int32_t get_npu_cmm_info_by_pid(uint32_t pid, uint32_t &total /* KiB */) {
    total = 0;

    char cmd[128];
    sprintf(cmd, "cat /proc/ax_proc/mem_cmm_info | grep -E 'name=\"npu_.*_pid%u.*\"' || true", pid);
    const char *output = nullptr;
    axclError ret = axclrtControlExecuteShellCmd(cmd, nullptr, 0, &output, TIMEOUT_2S);
    if (AXCL_SUCC == ret && output) {
        /**
         * |-Block: phys(0x181AD1000, 0x181AE3FFF), cache =non-cacheable, length=76KB(0MB),    name="npu_m_pid592"
         */
        std::istringstream input(output);
        std::string line;
        constexpr const char *KEY = "length=";
        size_t KEY_LEN = ::strlen(KEY);
        while (std::getline(input, line)) {
            size_t pos = line.find(KEY);
            if (pos != std::string::npos) {
                int value = atoi(line.substr(pos + KEY_LEN).c_str());
                if (value > 0) {
                    total += value;
                }
            }
        }

        return 0;
    }

    return 1;
}

static int32_t get_cmm_mem_info(uint32_t &total /* KiB */, uint32_t &used /* KiB */) {
    total = used = 0;
    const char *cmd = R"(cat /proc/ax_proc/mem_cmm_info  | grep "total size")";
    const char *output = nullptr;
    axclError ret = axclrtControlExecuteShellCmd(cmd, nullptr, 0, &output, TIMEOUT_1S);
    if (AXCL_SUCC == ret && output) {
        /*  total size=3145728KB(3072MB),used=53200KB(51MB + 976KB),remain=3092528KB(3020MB + 48KB),partition_number=1,block_number=48 */
        constexpr const char *PREFIX_TOTAL = "total size=";
        const size_t end = ::strlen(output);
        const char *total_str = strstr(output, PREFIX_TOTAL);
        if (total_str) {
            size_t beg = static_cast<size_t>(total_str - output) + ::strlen(PREFIX_TOTAL);
            while (beg < end && output[beg] == ' ') {
                ++beg; /* remove blank */
            }

            total = static_cast<uint32_t>(strtol(&output[beg], nullptr, 10));
        }

        constexpr const char *PREFIX_USED = "used=";
        const char *used_str = strstr(output, PREFIX_USED);
        if (used_str) {
            size_t beg = static_cast<size_t>(used_str - output) + ::strlen(PREFIX_USED);
            while (beg < end && output[beg] == ' ') {
                ++beg;
            }

            used = static_cast<uint32_t>(strtol(&output[beg], nullptr, 10));
        }

        return 0;
    }

    return 1;
}

static std::string get_firmware_version() {
    const char *output = nullptr;
    axclError ret = axclrtControlExecuteShellCmd("cat /proc/ax_proc/version", nullptr, 0, &output, TIMEOUT_1S);
    if (AXCL_SUCC == ret && output) {
        /* Ax_Version V2.17.0_20241125214038 */
        std::regex pattern(R"(V\d+\.\d+\.\d+)");
        std::cmatch match;
        if (std::regex_search(output, match, pattern)) {
            return match[0];
        }
    }

    return "--";
}

void axcl_smi_show_temperature() {
    constexpr const char *TEMPERATURE = "temperature";
    const auto &lst = DEVCIE_MANAGER()->get_devices();
    for (const auto &d : lst) {
        active_device_guard active(d);
        if (!active) {
            continue;
        }

        PRINT_DEVICE(d);

        int32_t temp = 0;
        if (0 == get_temperature(temp)) {
            printf(TITLE_FORMAT "%d\n", TEMPERATURE, temp);
        } else {
            printf(TITLE_FORMAT "%s\n", TEMPERATURE, "--");
        }

        PRINT_NEXT();
    }
}

void axcl_smi_show_top_avg_cpu() {
    constexpr const char *CPU_USAGE = R"(CPU Usage (%))";
    const auto &lst = DEVCIE_MANAGER()->get_devices();
    for (const auto &d : lst) {
        active_device_guard active(d);
        if (!active) {
            continue;
        }

        PRINT_DEVICE(d);

        std::string cpus;
        std::string load;
        double avg;
        if (!get_top_avg_cpu(cpus, load, avg)) {
            printf(TITLE_FORMAT "%.2f%%\n", CPU_USAGE, avg);
            printf(TITLE_FORMAT "%s\n", "CPU", cpus.c_str());
            printf(TITLE_FORMAT "%s", "Load average", load.c_str());
        } else {
            printf(TITLE_FORMAT "%s\n", CPU_USAGE, "--");
        }

        PRINT_NEXT();
    }
}

void axcl_smi_show_top_avg_npu() {
    constexpr const char *NPU_USAGE = R"(NPU Usage (%))";
    const auto &lst = DEVCIE_MANAGER()->get_devices();
    for (const auto &d : lst) {
        active_device_guard active(d);
        if (!active) {
            continue;
        }

        PRINT_DEVICE(d);

        const char *engine_version = AXCL_ENGINE_GetVersion();
        if (engine_version && ::strlen(engine_version) > 0) {
            printf(TITLE_FORMAT "%s\n", "engine version", engine_version);
        } else {
            printf(TITLE_FORMAT "%s\n", "engine version", "--");
        }

        std::map<std::string, std::string> cores;
        if (!get_top_avg_npu(cores)) {
            if (cores.empty()) {
                printf(TITLE_FORMAT "%s\n", NPU_USAGE, "--");
            } else {
                for (auto &&kv : cores) {
                    /*                               core              rate %     */
                    printf(TITLE_FORMAT "%s%%\n", kv.first.c_str(), kv.second.c_str());
                }
            }
        } else {
            printf(TITLE_FORMAT "%s\n", NPU_USAGE, "--");
        }

        PRINT_NEXT();
    }
}

void axcl_smi_show_mem_info() {
    const auto &lst = DEVCIE_MANAGER()->get_devices();
    for (const auto &d : lst) {
        active_device_guard active(d);
        if (!active) {
            continue;
        }

        PRINT_DEVICE(d);

        const char *output = get_sys_mem_info();
        if (output) {
            /**
             * MemTotal:         954084 kB
             * MemFree:          778992 kB
             * MemAvailable:     760380 kB
             * ...
             */
            std::istringstream input(output);
            std::string line;
            while (std::getline(input, line)) {
                char label[64];
                char value[64];
                if (sscanf(line.c_str(), "%63[^:]:%63[^\n]", label, value) == 2) {
                    char *s = value;
                    while (*s == ' ') {
                        s++;
                    }

                    printf(TITLE_FORMAT "%11s\n", label, s);
                }
            }
        } else {
            printf(TITLE_FORMAT "%s\n", "Mem Usage", "--");
        }

        PRINT_NEXT();
    }
}

void axcl_smi_show_cmm_info() {
    const auto &lst = DEVCIE_MANAGER()->get_devices();
    for (const auto &d : lst) {
        active_device_guard active(d);
        if (!active) {
            continue;
        }

        PRINT_DEVICE(d);

        uint32_t total, used;
        if (!get_cmm_mem_info(total, used)) {
            printf(TITLE_FORMAT "%8d KiB\n", "CMM Total", total);
            printf(TITLE_FORMAT "%8d KiB\n", "CMM Used", used);
            printf(TITLE_FORMAT "%8d kiB\n", "CMM Remain", total - used);
        } else {
            printf(TITLE_FORMAT "%s\n", "CMM Usage", "--");
        }

        PRINT_NEXT();
    }
}

void axcl_smi_show_info() {
    struct card_process_info {
        uint32_t pid;
        std::string exe;
        uint32_t npu_used_cmm;
    };

    struct card_info {
        uint32_t card;
        char name[32];
        char firmware[16];
        char bus_id[16];
        uint32_t mem_used;
        uint32_t mem_total;
        uint32_t cmm_used;
        uint32_t cmm_total;
        char fan[8];
        char temperature[8];
        char pwr[32];
        char cpu_rate[8];
        char npu_rate[8];
        std::vector<struct card_process_info> processes;
    };

    const auto &lst = DEVCIE_MANAGER()->get_devices();
    std::vector<struct card_info> cards;
    uint32_t card_no = 0;
    std::vector<std::thread> threads;
    threads.reserve(lst.size());
    std::mutex mtx;

    for (const auto &d : lst) {
        threads.emplace_back([&cards, card_no, d, &mtx]() {
            context_guard ctx(d);
            if (!ctx) {
                return;
            }

            struct card_info card = {};
            card.card = card_no;

            if (const char *name = axclrtGetSocName(); name != nullptr) {
                strcpy(card.name, name);
            } else {
                strcpy(card.name, "--");
            }

            strcpy(card.firmware, get_firmware_version().substr(0, 15).c_str());

            axclrtDeviceBusInfo bus_info;
            if (axclError ret = axclrtGetDeviceBusInfo(d, &bus_info); AXCL_SUCC != ret) {
                strcpy(card.bus_id, "--");
            } else {
                sprintf(card.bus_id, "%04X:%02X:%02X.%X", bus_info.domain, bus_info.bus, bus_info.slot, bus_info.func);
            }

            get_sys_mem_info(card.mem_total, card.mem_used);
            card.mem_total /= 1024; /* kB */
            card.mem_used /= 1024;  /* kB */
            get_cmm_mem_info(card.cmm_total, card.cmm_used);
            card.cmm_total /= 1024; /* KiB */
            card.cmm_used /= 1024;  /* KiB */

            strcpy(card.fan, "--");

            int32_t temperature;
            if (0 == get_temperature(temperature)) {
                temperature = static_cast<int32_t>(round(temperature / 1000.0));
                sprintf(card.temperature, "%dC", temperature);
            } else {
                strcpy(card.temperature, "--");
            }

            strcpy(card.pwr, "-- / --");

            double cpu;
            if (0 == get_top_avg_cpu(cpu)) {
                sprintf(card.cpu_rate, "%d%%", static_cast<uint32_t>(cpu));
            } else {
                strcpy(card.cpu_rate, "--");
            }

            double npu;
            if (0 == get_top_avg_npu(npu)) {
                sprintf(card.npu_rate, "%d%%", static_cast<uint32_t>(npu));
            } else {
                strcpy(card.npu_rate, "--");
            }

            /* processes info. */
            axclrtDevicePidInfo pid_info;
            if (axclError ret = axclrtGetDevicePidInfo(d, &pid_info); AXCL_SUCC == ret) {
                std::vector<struct card_process_info> v;
                for (unsigned int i = 0; i < pid_info.num; ++i) {
                    if (pid_info.pid[i] == static_cast<uint32_t>(getpid())) {
                        /* skip axcl_smi process */
                        continue;
                    }

                    card_process_info p;
                    p.pid = pid_info.pid[i];
                    p.exe = get_exe_full_path(p.pid);
                    get_npu_cmm_info_by_pid(p.pid, p.npu_used_cmm);
                    v.push_back(std::move(p));
                }

                ::free(pid_info.pid);

                card.processes = std::move(v);
            }

            std::lock_guard<std::mutex> lck(mtx);
            cards.push_back(std::move(card));
        });

        ++card_no;
    }

    for (auto &&t : threads) {
        t.join();
    }

    std::sort(cards.begin(), cards.end(), [](const struct card_info &a, const struct card_info &b) {
        return a.card < b.card;
    });

    const std::string smi_version = std::string("AXCL-SMI  ") + AXCL_SMI_VERSION;
    const std::string sdk_version = std::string("Driver  ") + AXCL_SDK_VERSION;
    printf("+------------------------------------------------------------------------------------------------+\n");
    printf("| %-32s                              %32s |\n", smi_version.c_str(), sdk_version.c_str());
    printf("+-----------------------------------------+--------------+---------------------------------------+\n");
    printf("| Card  Name                     Firmware | Bus-Id       |                          Memory-Usage |\n");
    printf("| Fan   Temp                Pwr:Usage/Cap | CPU      NPU |                             CMM-Usage |\n");
    printf("|=========================================+==============+=======================================|\n");
    for (const auto &card : cards) {
        printf("| %4d  %-16s %16s | %12s |           %8d MiB / %8d MiB |\n", card.card, card.name, card.firmware, card.bus_id,
               card.mem_used, card.mem_total);
        printf("| %4s  %4s %28s | %-6s%6s |           %8d MiB / %8d MiB |\n", card.fan, card.temperature, card.pwr, card.cpu_rate,
               card.npu_rate, card.cmm_used, card.cmm_total);
        printf("+-----------------------------------------+--------------+---------------------------------------+\n");
    }
    printf("\n");
    printf("+------------------------------------------------------------------------------------------------+\n");
    printf("| Processes:                                                                                     |\n");
    printf("| Card      PID  Process Name                                                   NPU Memory Usage |\n");
    printf("|================================================================================================|\n");

    size_t num = 0;
    for (const auto &card : cards) {
        for (const auto &process : card.processes) {
            ++num;
            printf("| %4d %8d  %-66s %8d KiB |\n", card.card, process.pid, process.exe.substr(0, 66).c_str(), process.npu_used_cmm);
        }
    }

    if (num > 0) {
        printf("+------------------------------------------------------------------------------------------------+\n");
    }
}