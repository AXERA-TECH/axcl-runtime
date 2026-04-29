/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "axcl_rt_event.h"

#include "axcl_rt_context.h"
#include "axcl_rt_device.h"
#include "device/device.hpp"
#include "device/device_manager.hpp"
#include "event/event.hpp"
#include "event/event_manager.hpp"
#include "context/context.hpp"
#include "context/context_manager.hpp"

#include "axcl_logger.hpp"
#include "os.hpp"

#define TAG "context"

AXCL_EXPORT axclError axclrtCreateEvent(axclrtEvent *event) {
    if (!event) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_EVENT_NULL_POINTER;
    }

    axclError ret;
    int32_t deviceId = -1;
    if (ret = axclrtGetDevice(&deviceId); AXCL_SUCC != ret) {
        return ret;
    }

    *event = reinterpret_cast<axclrtEvent>(DEVICE_MANAGER()->get_device(deviceId)->get_event_manager()->create_event());
    if (!*event) {
        return AXCL_ERR_EVENT_CREATE;
    }

    return AXCL_SUCC;
}

AXCL_EXPORT axclError axclrtDestroyEvent(axclrtEvent event) {
    if (!event) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_EVENT_NULL_POINTER;
    }

    axclError ret;
    int32_t deviceId = -1;
    if (ret = axclrtGetDevice(&deviceId); AXCL_SUCC != ret) {
        return ret;
    }

    auto *e = reinterpret_cast<axcl::rt::event *>(event);
    if (!DEVICE_MANAGER()->get_device(deviceId)->get_event_manager()->destory_event(e)) {
        return AXCL_ERR_EVENT_DESTROY;

    }

    return AXCL_SUCC;
}

AXCL_EXPORT axclError axclrtRecordEvent(axclrtEvent event, axclrtStream stream) {

    if (!event || !stream) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_EVENT_NULL_POINTER;
    }

    axclError ret;
    int32_t deviceId = -1;
    if (ret = axclrtGetDevice(&deviceId); AXCL_SUCC != ret) {
        return ret;
    }

    auto *s = reinterpret_cast<axcl::rt::stream *>(stream);
    auto *e = reinterpret_cast<axcl::rt::event *>(event);
    if (!DEVICE_MANAGER()->get_device(deviceId)->get_event_manager()->record_event(e, s)) {
        return AXCL_ERR_EVENT_RECORD;
    }

    return AXCL_SUCC;
}

AXCL_EXPORT axclError axclrtStreamWaitEvent(axclrtStream stream, axclrtEvent event) {
    if (!event || !stream) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_EVENT_NULL_POINTER;
    }

    axclError ret;
    int32_t deviceId = -1;
    if (ret = axclrtGetDevice(&deviceId); AXCL_SUCC != ret) {
        return ret;
    }

    auto *s = reinterpret_cast<axcl::rt::stream *>(stream);
    auto *e = reinterpret_cast<axcl::rt::event *>(event);
    if (!DEVICE_MANAGER()->get_device(deviceId)->get_event_manager()->wait_event(e, s, -1)) {
        return AXCL_ERR_EVENT_WAIT;
    }

    return AXCL_SUCC;
}

AXCL_EXPORT axclError axclrtStreamWaitEventWithTimeout(axclrtStream stream, axclrtEvent event, int32_t timeout) {
    if (!event || !stream) {
        LOG_MM_E(TAG, "nil pointer");
        return AXCL_ERR_EVENT_NULL_POINTER;
    }

    axclError ret;
    int32_t deviceId = -1;
    if (ret = axclrtGetDevice(&deviceId); AXCL_SUCC != ret) {
        return ret;
    }

    auto *s = reinterpret_cast<axcl::rt::stream *>(stream);
    auto *e = reinterpret_cast<axcl::rt::event *>(event);
    if (!DEVICE_MANAGER()->get_device(deviceId)->get_event_manager()->wait_event(e, s, timeout)) {
        return AXCL_ERR_EVENT_WAIT;
    }

    return AXCL_SUCC;
}