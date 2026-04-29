/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include <signal.h>
#include <cstdint>
#include <iostream>
#include <string>
#include "args.hxx"
#include "cmd_info.hpp"
#include "cmd_log.hpp"
#include "cmd_proc.hpp"
#include "cmd_reboot.hpp"
#include "cmd_set.hpp"
#include "cmd_shell.hpp"
#include "device_manager.hpp"
#include "single_proc.hpp"
#include "smi_log.hpp"

#define LOG_TAG "smi"

AXCL_SMI_LOG_LEVEL_E g_axcl_smi_log_level = AXCL_SMI_LOG_ERROR;

static bool g_exit = false;
static void exit_handler(int s) {
    g_exit = true;

    static int32_t count = 0;
    if (++count >= 3) {
        _Exit(EXIT_FAILURE);
    }
}

#if !defined (WINDOWS)
static void ignore_sig_pipe() {
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    if (sigemptyset(&sa.sa_mask) == -1 || sigaction(SIGPIPE, &sa, 0) == -1) {
        perror("failed to ignore SIGPIPE, sigaction");
        exit(EXIT_FAILURE);
    }
}
#endif  // !defined (WINDOWS)

static void install_signal_handler() {
    signal(SIGINT, exit_handler);
#if !defined (WINDOWS)
    signal(SIGQUIT, exit_handler);
    ignore_sig_pipe();
#endif  // !defined (WINDOWS)
}

static bool check_if_specify_a_device(uint32_t device_index) {
    return DEVCIE_MANAGER()->set_devices({device_index});
}

int main(int argc, char* argv[]) {
    axcl::single_proc sp = axcl::single_proc(argv[0], 0x61736D69);  // asmi
    install_signal_handler();

    args::ArgumentParser p(std::string("axcl-smi System Management Interface ") + AXCL_SMI_VERSION);
    args::CompletionFlag c(p, {"complete"});

    args::Group commands(p, "Commands", args::Group::Validators::AtMostOne);

    /* global options */
    args::ValueFlag<uint32_t> device(p, "", "Card index [0, connected cards number - 1]", {'d', "device"}, 0, args::Options::Global);

    /* version */
    args::ActionFlag version(p, "version", "Show axcl-smi version", {'v', "version"}, [&]() {
        std::cout << "AXCL-SMI " << AXCL_SMI_VERSION << " BUILD: " << __DATE__ << " " << __TIME__ << std::endl;
        g_exit = true;
    });

    /* info command */
    args::Command info(commands, "info", "Show device information", [&](args::Subparser& parser) {
        args::Group sub(parser, "", args::Group::Validators::Xor);
        args::Flag temp(sub, "", "Show SoC temperature", {"temp"});
        args::Flag mem(sub, "", "Show memory usage", {"mem"});
        args::Flag cmm(sub, "", "Show CMM usage", {"cmm"});
        args::Flag cpu(sub, "", "Show CPU usage", {"cpu"});
        args::Flag npu(sub, "", "Show NPU usage", {"npu"});
        parser.Parse();

        if (check_if_specify_a_device(*device)) {
            if (temp) {
                axcl_smi_show_temperature();
            }

            if (cpu) {
                axcl_smi_show_top_avg_cpu();
            }

            if (npu) {
                axcl_smi_show_top_avg_npu();
            }

            if (mem) {
                axcl_smi_show_mem_info();
            }

            if (cmm) {
                axcl_smi_show_cmm_info();
            }
        }

        g_exit = true;
    });

    args::Command proc(commands, "proc", "cat device proc", [&](args::Subparser& parser) {
        args::Group sub(parser, "", args::Group::Validators::Xor);
        args::Flag vdec(sub, "", "cat /proc/ax_proc/vdec", {"vdec"});
        args::Flag venc(sub, "", "cat /proc/ax_proc/venc", {"venc"});
        args::Flag jenc(sub, "", "cat /proc/ax_proc/jenc", {"jenc"});
        args::Flag ivps(sub, "", "cat /proc/ax_proc/ivps", {"ivps"});
        args::Flag rgn(sub, "", "cat /proc/ax_proc/rgn", {"rgn"});
        args::Flag ive(sub, "", "cat /proc/ax_proc/ive", {"ive"});
        args::Flag pool(sub, "", "cat /proc/ax_proc/pool", {"pool"});
        args::Flag link(sub, "", "cat /proc/ax_proc/link_table", {"link"});
        args::Flag cmm(sub, "", "cat /proc/ax_proc/mem_cmm_info", {"cmm"});
        parser.Parse();

        if (check_if_specify_a_device(*device)) {
            if (vdec) {
                axcl_smi_show_vdec_proc();
            }

            if (venc) {
                axcl_smi_show_venc_proc();
            }

            if (jenc) {
                axcl_smi_show_jenc_proc();
            }

            if (ivps) {
                axcl_smi_show_ivps_proc();
            }

            if (rgn) {
                axcl_smi_show_rgn_proc();
            }

            if (ive) {
                axcl_smi_show_ive_proc();
            }

            if (pool) {
                axcl_smi_show_pool_proc();
            }

            if (link) {
                axcl_smi_show_link_proc();
            }

            if (cmm) {
                axcl_smi_show_cmm_proc();
            }
        }

        g_exit = true;
    });

    /* set command */
    args::Command set(commands, "set", "Set", [&](args::Subparser& parser) {
        args::Group sub(parser, "", args::Group::Validators::Xor);
        args::MapFlag<uint32_t, uint32_t> freq(sub, "MHz", "Set CPU frequency in MHz", {'f', "freq"},
                                               {{1200000, 1200000}, {1400000, 1400000}, {1700000, 1700000}});
        // args::ValueFlag<std::string> ip(sub, "ip", "Configure network and set IP", {"ip"});
        parser.Parse();

        if (check_if_specify_a_device(*device)) {
            if (freq) {
                axcl_smi_set_cpu_freq(*freq);
            }
        }

        g_exit = true;
    });

    /* log command */
    args::Command log(commands, "log", "Dump logs from device", [&](args::Subparser& parser) {
        args::ValueFlag<int32_t> type(parser, "mask",
                                      "Specifies which logs to dump by a combination (bitwise OR) value of blow:\n"
                                      "  -1: all (default)  0x01: daemon  0x02: worker  0x10: syslog  0x20: kernel",
                                      {'t', "type"}, -1);
        args::ValueFlag<std::string> output(parser, "path", "Specifies the path to save dump logs (default: ./)", {'o', "output"}, "./");
        parser.Parse();

        if (check_if_specify_a_device(*device)) {
            axcl_smi_dump_device_log(*type, (*output).c_str());
        }

        g_exit = true;
    });

    /* shell command */
    args::Command shell(commands, "sh", "Execute a shell command", [&](args::Subparser& parser) {
        args::Positional<std::string> _cmd(parser, "cmd", "Shell command", args::Options::Required);
        args::PositionalList<std::string> _args(parser, "args", "Shell command arguments");
        parser.Parse();

        if (check_if_specify_a_device(*device)) {
            const size_t num = (*_args).size();
            if (num > 0) {
                std::vector<char*> cmd_args(num);
                size_t i = 0;
                for (const auto& m : _args) {
                    cmd_args[i++] = const_cast<char*>(m.c_str());
                }

                axcl_smi_execute_shell_cmd((*_cmd).c_str(), cmd_args.data(), num);
            } else {
                axcl_smi_execute_shell_cmd((*_cmd).c_str(), nullptr, 0);
            }
        }

        g_exit = true;
    });

    /* reboot command */
    args::Command reboot(commands, "reboot", "reboot device", [&](args::Subparser& parser) {
        parser.Parse();
        printf("Do you want to reboot device %d ? (y/n): ", *device);
        char c;
        std::cin >> c;
        if (c == 'y' || c == 'Y') {
            axcl_smi_reboot_device(*device);
        }

        g_exit = true;
    });

    args::HelpFlag help(p, "help", "Show this help menu", {'h', "help"});

    p.helpParams.proglineCommand = "<command> [<args>]";
    p.helpParams.proglineOptions = "";
    p.helpParams.optionsString = "";
    p.helpParams.width = 160;
    p.helpParams.showCommandChildren = true;
    p.helpParams.addChoices = true;
    p.helpParams.showProglineOptions = true;
    p.helpParams.showTerminator = true;
    p.helpParams.showProglinePositionals = false;
    p.helpParams.proglineShowFlags = true;
    p.helpParams.progindent = 0;
    p.helpParams.descriptionindent = 0;
    p.helpParams.flagindent = 0;
    p.helpParams.eachgroupindent = 4;
    p.helpParams.usageString = "usage:";
    p.helpParams.choiceString = ". One of: ";

    p.RequireCommand(false);

    try {
        p.ParseCLI(argc, argv);
    } catch (args::Help&) {
        std::cout << p;
        return 0;
    } catch (args::Completion& e) {
        std::cout << e.what();
        return 0;
    } catch (args::Error& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    if (g_exit) {
        return 0;
    }

    axcl_smi_show_info();

    return 0;
}
