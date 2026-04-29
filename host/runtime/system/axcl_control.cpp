/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "axcl_rt_control.h"
#include "axcl_rt_control_internal.h"
#include "axcl_logger.hpp"
#include "system/control.hpp"

#define TAG "control"

AXCL_EXPORT axclError axclrtControlCreateContext(int32_t context_id) {
    axcl::rt::system::control api;
    return api.create_context(context_id);
}

AXCL_EXPORT axclError axclrtControlDestroyContext(int32_t context_id) {
    axcl::rt::system::control api;
    return api.destroy_context(context_id);
}

AXCL_EXPORT axclError axclrtControlExecuteShellCmd(const char *cmd, const char *const args[], size_t argc, const char **output, int32_t timeout) {
    axcl::rt::system::control api;
    return api.execute_shell_cmd(cmd, args, argc, output, timeout);
}

AXCL_EXPORT axclError axclrtControlChangePCIeSubId(uint32_t sub_vendor_id, uint32_t sub_device_id) {
    axcl::rt::system::control api;
    return api.change_pcie_sub_id(sub_vendor_id, sub_device_id);
}

AXCL_EXPORT axclError axclrtControlCreateStream(int32_t context_id, axclrtStream stream) {
    axcl::rt::system::control api;
    return api.create_stream(context_id, stream);
}

AXCL_EXPORT axclError axclrtControlDestroyStream(int32_t context_id, axclrtStream stream) {
    axcl::rt::system::control api;
    return api.destroy_stream(context_id, stream);
}

AXCL_EXPORT axclError axclrtControlSyncStream(int32_t context_id, axclrtStream stream, int32_t timeout_ms) {
    axcl::rt::system::control api;
    return api.sync_stream(context_id, stream, timeout_ms);
}

AXCL_EXPORT axclError axclrtControlCreateEvent(int32_t event_id) {
    axcl::rt::system::control api;
    return api.create_event(event_id);
}

AXCL_EXPORT axclError axclrtControlDestroyEvent(int32_t event_id) {
    axcl::rt::system::control api;
    return api.destroy_event(event_id);
}

AXCL_EXPORT axclError axclrtControlRecordEvent(axclrtStream stream, int32_t event_id) {
    axcl::rt::system::control api;
    return api.record_event(stream, event_id);
}

AXCL_EXPORT axclError axclrtControlStreamWaitEvent(axclrtStream stream, int32_t event_id, int32_t timeout_ms) {
    axcl::rt::system::control api;
    return api.stream_wait_event(stream, event_id, timeout_ms);
}
