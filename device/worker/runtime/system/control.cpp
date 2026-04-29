/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "control.hpp"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sstream>
#include <mutex>

#include "axcl_logger.hpp"
#include "axcl_rt_event.h"
#include "../../dispatch/dispatcher.hpp"
#include "../../context/context_manager.hpp"
#include "../../context/context.hpp"
#include "../../stream/stream_manager.hpp"
#include "../../stream/stream.hpp"
#include "../../event/event_manager.hpp"
#include "../../event/event.hpp"
#include "os.hpp"

#define TAG "control"

namespace axcl::worker {

template<>
bool runtime_api<axcl::pkg::runtime::system::command,
                axcl::pkg::device::control_response,
                axcl::pkg::device::control_request>::handle_ack_err(const axcl::pkg::device::control_response& decoder, int32_t ret) {

    auto noresp_flag = decoder.get_noresp_flag();
    if (noresp_flag == 0) {
        return false;
    }

    auto api = decoder.get_cmd();
    if (ret != 0) {
        throw RuntimeApiException(ret, m_module, static_cast<int32_t>(api));
    }

    return true;
}

control::control(dispatcher *dispatcher) : base(dispatcher), m_api(TAG) {
    m_api.append({
        SYSTEM_CONTROL_API_APPEND(CREATE_CONTEXT),
        SYSTEM_CONTROL_API_APPEND(DESTROY_CONTEXT),

        SYSTEM_CONTROL_API_APPEND(CREATE_STREAM),
        SYSTEM_CONTROL_API_APPEND(DESTROY_STREAM),
        SYSTEM_CONTROL_API_APPEND(SYNC_STREAM),

        SYSTEM_CONTROL_API_APPEND(CREATE_EVENT),
        SYSTEM_CONTROL_API_APPEND(DESTROY_EVENT),
        SYSTEM_CONTROL_API_APPEND(RECORD_EVENT),
        SYSTEM_CONTROL_API_APPEND(WAIT_EVENT),

        SYSTEM_CONTROL_API_APPEND(SHELL),
        SYSTEM_CONTROL_API_APPEND(SETUP_PCIE_SUB_ID),
    });
}

void control::run(const void *data, const size_t &size) {
    m_api.run(get_dispatcher(), data, size);
}

SYSTEM_CONTROL_API_IMPL(CREATE_CONTEXT) {
    const auto context_id = response->get_context();
    const auto stream_id = response->get_stream();

    const auto slave_pid = getpid();
    const auto slave_tid = gettid();

    on_context_create(context_id, stream_id);

    request->create_context(slave_pid, slave_tid, context_id, stream_id);

    return 0;
}

SYSTEM_CONTROL_API_IMPL(DESTROY_CONTEXT) {
    const auto context_id = response->get_context();
    const auto stream_id = response->get_stream();

    const auto slave_pid = getpid();
    const auto slave_tid = gettid();

    on_context_destroy(context_id, stream_id);

    request->destroy_context(slave_pid, slave_tid, context_id, stream_id);

    return 0;
}

SYSTEM_CONTROL_API_IMPL(CREATE_STREAM) {
    const auto context_id = response->get_context();
    const auto stream_id = response->get_stream();

    on_stream_create(context_id, stream_id);

    request->create_stream(context_id, stream_id);
    return 0;
}

SYSTEM_CONTROL_API_IMPL(DESTROY_STREAM) {
    const auto context_id = response->get_context();
    const auto stream_id = response->get_stream();

    on_stream_destroy(context_id, stream_id);

    request->destroy_stream(context_id, stream_id);

    return 0;
}

stream* control::get_stream(uint32_t context_id, uint32_t stream_id, int& error_code) {
    error_code = -1;

    auto context_mgr = get_dispatcher()->get_context_manager();
    if (!context_mgr) {
        LOG_MM_E(TAG, "Context manager not found");
        return nullptr;
    }

    auto ctx = context_mgr->get_context(context_id);
    if (!ctx) {
        LOG_MM_E(TAG, "Context not found: context_id={}", context_id);
        return nullptr;
    }

    auto stream_manager = ctx->get_stream_manager();
    if (!stream_manager) {
        LOG_MM_E(TAG, "Stream manager not found for context_id={}", context_id);
        return nullptr;
    }

    auto stream = stream_manager->get_stream(stream_id);
    if (!stream) {
        LOG_MM_E(TAG, "Stream not found: context_id={}, stream_id={}", context_id, stream_id);
        return nullptr;
    }

    error_code = 0;
    return stream;
}

SYSTEM_CONTROL_API_IMPL(SYNC_STREAM) {
    const auto context_id = response->get_context();
    const auto stream_id = response->get_stream();

    int error_code = -1;

    auto stream = get_stream(context_id, stream_id, error_code);
    if (stream) {
        auto last_error = stream->last_error();
        if (last_error.has_error()) {
            error_code = last_error.error_code();
        }

        stream->reset_timeout_monitor();
    }

    request->sync_stream(context_id, stream_id, error_code);
    return 0;
}

SYSTEM_CONTROL_API_IMPL(CREATE_EVENT) {
    const auto context_id = response->get_context();
    const auto stream_id = response->get_stream();
    const auto event_id = response->get_event();

    if (!on_event_create(context_id, stream_id, event_id)) {
        LOG_MM_E(TAG, "Failed to create event in manager: event_id={}", event_id);
        return AXCL_ERR_EVENT_CREATE;
    }
    request->create_event(context_id, stream_id, event_id);

    LOG_MM_I(TAG, "Event creation request sent: event_id={}", event_id);

    return 0;
}

SYSTEM_CONTROL_API_IMPL(DESTROY_EVENT) {

    const auto context_id = response->get_context();
    const auto stream_id = response->get_stream();
    const auto event_id = response->get_event();

    if (!on_event_destroy(context_id, stream_id, event_id)) {
        LOG_MM_E(TAG, "Failed to destroy event in manager: event_id={}", event_id);
        return AXCL_ERR_EVENT_DESTROY;
    }

    request->destroy_event(context_id, stream_id, event_id);
    LOG_MM_I(TAG, "Event destruction request sent: event_id={}", event_id);

    return 0;
}

SYSTEM_CONTROL_API_IMPL(RECORD_EVENT) {

    const auto stream_id = response->get_stream();
    const auto event_id = response->get_event();

    auto dispatcher = get_dispatcher();
    auto event_manager = dispatcher->get_event_manager();
    if (!event_manager->record_event(event_id, stream_id)) {
        LOG_MM_E(TAG, "Failed to record event in manager: event_id={}, stream_id={}", event_id, stream_id);
        return AXCL_ERR_EVENT_RECORD;
    }

    LOG_MM_I(TAG, "Event record request sent: event_id={}, stream_id={}", event_id, stream_id);

    return 0;
}

SYSTEM_CONTROL_API_IMPL(WAIT_EVENT) {

    const auto stream_id = response->get_stream();
    const auto event_id = response->get_event();
    const auto timeout = response->get_timeout();

    auto dispatcher = get_dispatcher();
    auto event_manager = dispatcher->get_event_manager();

    if (!event_manager->wait_event(event_id, stream_id, timeout)) {
        if (timeout > 0) {
            LOG_MM_E(TAG, "Wait event timeout: event_id={}, stream_id={}, timeout={}ms", event_id, stream_id, timeout);
        } else {
            LOG_MM_E(TAG, "Failed to wait for event: event_id={}, stream_id={}", event_id, stream_id);
        }
        return AXCL_ERR_EVENT_WAIT;
    }

    LOG_MM_I(TAG, "Event wait request sent: event_id={}, stream_id={}, timeout={}", event_id, stream_id, timeout);

    return 0;
}

SYSTEM_CONTROL_API_IMPL(SHELL) {
    std::stringstream cmd;
    cmd << response->get_shell_cmd();
    const auto args = response->get_shell_args();
    const size_t argc = args.size();
    for (size_t i = 0; i < argc; ++i) {
        cmd << " " << args[i];
    }

    std::string output;
    int32_t status = execute_shell(cmd.str().c_str(), output);
    request->set_shell_cmd_output(status, output.c_str());

    return 0;
}

SYSTEM_CONTROL_API_IMPL(SETUP_PCIE_SUB_ID) {
    const auto sub_vendor_id = response->get_pcie_sub_vendor_id();
    const auto sub_device_id = response->get_pcie_sub_device_id();

    int32_t status = write_pcie_sub_id_to_flash(sub_vendor_id, sub_device_id);
    request->set_pcie_sub_id_setup_status(status);

    return 0;
}

int32_t control::execute_shell(const char *cmd, std::string &output) {
    int32_t ret;
    int pipe_out[2];
    if (::pipe(pipe_out) < 0) {
        ret = errno;
        LOG_MM_E(TAG, "pipe() fail, {}", ::strerror(ret));
        return ret;
    }

    pid_t pid = vfork();
    if (pid < 0) {
        ret = errno;
        ::close(pipe_out[0]);
        ::close(pipe_out[1]);
        LOG_MM_E(TAG, "vfork() fail, {}", ::strerror(ret));
        return ret;
    }

    if (0 == pid) {
        /* child */
        ::close(pipe_out[0]);
        ::dup2(pipe_out[1], STDOUT_FILENO);
        ::close(pipe_out[1]);

        ::execlp("sh", "sh", "-c", cmd, NULL);

        /* execl failure */
        LOG_MM_E(TAG, "child execl {} fail, {}", cmd, ::strerror(errno));
        ::_exit(EXIT_FAILURE);
    }

    /* parent */
    ::close(pipe_out[1]);

    output.clear();
    char buffer[128];
    ssize_t bytes;
    while ((bytes = ::read(pipe_out[0], buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes] = '\0';
        output += buffer;
    }

    if (bytes == -1) {
        ret = errno;
        ::close(pipe_out[0]);
        LOG_MM_E(TAG, "read() fail, {}", ::strerror(ret));
        return ret;
    }

    ::close(pipe_out[0]);

    int status;
    if (::waitpid(pid, &status, 0) == -1) {
        ret = errno;
        LOG_MM_E(TAG, "waitpid() fail, {}", ::strerror(ret));
        return ret;
    }

    if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
        LOG_MM_E(TAG, "child process exited with error: {}", WEXITSTATUS(status));
        return WEXITSTATUS(status);
    }

    return 0;
}

int32_t control::get_storage_type(uint32_t &storage) {
    int fd = open("/dev/mem", O_RDWR | O_SYNC, S_IRUSR | S_IRUSR);
    if (fd < 0) {
        LOG_MM_E(TAG, "open /dev/mem failed");
        return -1;
    }

    /* mmap reg */
    volatile uint8_t *mem = (volatile uint8_t *)mmap((void *)0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x4200000 /* COMM_SYS_GLB */);
    if (MAP_FAILED == mem) {
        close(fd);
        LOG_MM_E(TAG, "mmap /dev/mem failed");
        return -1;
    }

    storage = *((volatile uint32_t *)(mem + 0x10));
    storage &= 0x03;

    munmap((void *)mem, 4096);
    close(fd);

    return 0;
}

int32_t control::write_pcie_sub_id_to_flash(uint32_t sub_vendor_id, uint32_t sub_device_id) {
    uint32_t storage = 0;
    if (get_storage_type(storage) < 0 || storage != 0x02 /*nor flash*/) {
        LOG_MM_E(TAG, "storage {} is not nor flash", storage);
        return -1;
    }

    constexpr const char *DEV_NAME = "/dev/mtdblock0";
    int fd = open(DEV_NAME, O_RDWR);
    if (fd < 0) {
        LOG_MM_E(TAG, "open {} fail, error: {}", DEV_NAME, errno);
        return -2;
    }

    constexpr off_t offset = 512 * 1024; /* fix to 512KB offset */
    if (lseek(fd, offset, SEEK_SET) < 0) {
        LOG_MM_E(TAG, "lseek {} fail, error {}", DEV_NAME, errno);
        close(fd);
        return -3;
    }

    uint32_t buf[3];
    buf[0] = 0x5c6d8e9f; /* fixed magic */
    buf[1] = sub_vendor_id;
    buf[2] = sub_device_id;
    ssize_t sz = write(fd, buf, sizeof(buf));
    if (sz < 0) {
        LOG_MM_E(TAG, "write sub vendor id {} and sub device id {} fail, error: {}", sub_vendor_id, sub_device_id, errno);
        close(fd);
        return -4;
    }

    fsync(fd);
    close(fd);
    return 0;
}

bool control::on_context_create(uint32_t context_id, uint32_t stream_id) {
    std::lock_guard<std::mutex> lck(m_mtx);

    auto dispatcher = get_dispatcher();
    auto context_manager = dispatcher->get_context_manager();

    context* ctx = context_manager->get_context(context_id);
    if (ctx) {
        return true;
    }

    ctx = context_manager->create_context(context_id);
    if (!ctx) {
        return false;
    }

    stream_manager* sm = ctx->get_stream_manager();
    if (!sm) {
        return false;
    }

    stream* strm = sm->create_stream(stream_id, false);
    if (!strm) {
        return false;
    }

    return true;
}

bool control::on_context_destroy(uint32_t context_id, uint32_t stream_id) {
    std::lock_guard<std::mutex> lck(m_mtx);

    auto dispatcher = get_dispatcher();
    auto context_manager = dispatcher->get_context_manager();

    context* ctx = context_manager->get_context(context_id);
    if (!ctx) {
        return true;
    }

    stream_manager* sm = ctx->get_stream_manager();
    if (sm) {
        sm->destroy_all_streams();
    }

    bool result = context_manager->destroy_context(context_id);
    if (!result) {
        return false;
    }

    return true;
}

bool control::on_stream_create(uint32_t context_id, uint32_t stream_id) {
    std::lock_guard<std::mutex> lck(m_mtx);

    auto dispatcher = get_dispatcher();
    auto context_manager = dispatcher->get_context_manager();

    context* ctx = context_manager->get_context(context_id);
    if (!ctx) {
        return false;
    }

    stream_manager* sm = ctx->get_stream_manager();
    if (!sm) {
        return false;
    }

    stream* strm = sm->get_stream(stream_id);
    if (strm) {
        return true;
    }

    strm = sm->create_stream(stream_id);
    if (!strm) {
        return false;
    }

    return true;
}

bool control::on_stream_destroy(uint32_t context_id, uint32_t stream_id) {
    std::lock_guard<std::mutex> lck(m_mtx);

    auto dispatcher = get_dispatcher();
    auto context_manager = dispatcher->get_context_manager();

    context* ctx = context_manager->get_context(context_id);
    if (!ctx) {
        return false;
    }
    stream_manager* sm = ctx->get_stream_manager();
    if (!sm) {
        return false;
    }
    bool result = sm->destroy_stream(stream_id);
    if (!result) {
        return false;
    }

    return true;
}

bool control::on_event_create(uint32_t context_id, uint32_t stream_id, uint32_t event_id) {
    std::lock_guard<std::mutex> lck(m_mtx);

    auto dispatcher = get_dispatcher();
    auto event_manager = dispatcher->get_event_manager();

    event* evt = event_manager->create_event(event_id);
    if (!evt) {
        return false;
    }

    return true;
}

bool control::on_event_destroy(uint32_t context_id, uint32_t stream_id, uint32_t event_id) {
    std::lock_guard<std::mutex> lck(m_mtx);

    auto dispatcher = get_dispatcher();
    auto event_manager = dispatcher->get_event_manager();
    bool result = event_manager->destory_event(event_id);
    if (!result) {
        return false;
    }

    return true;
}

} /* namespace axcl::worker */
