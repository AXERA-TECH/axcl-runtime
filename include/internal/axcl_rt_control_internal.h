/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#ifndef __AXCL_RT_CONTROL_INTERNAL_H__
#define __AXCL_RT_CONTROL_INTERNAL_H__

#include "axcl_rt_context.h"
#include "axcl_rt_type_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

#define AXCL_DEF_SYSCTRL_ERR(e)              AXCL_DEF_RUNTIME_ERR(AXCL_RUNTIME_SYSCTRL, (e))
#define AXCL_ERR_SYSCTRL_NULL_POINTER        AXCL_DEF_SYSCTRL_ERR(AXCL_ERR_NULL_POINTER)
#define AXCL_ERR_SYSCTRL_ENCODE              AXCL_DEF_SYSCTRL_ERR(AXCL_ERR_ENCODE)
#define AXCL_ERR_SYSCTRL_DECODE              AXCL_DEF_SYSCTRL_ERR(AXCL_ERR_DECODE)
#define AXCL_ERR_SYSCTRL_UNEXPECT_RESPONSE   AXCL_DEF_SYSCTRL_ERR(AXCL_ERR_UNEXPECT_RESPONSE)
#define AXCL_ERR_SYSCTRL_EXECUTE_FAIL        AXCL_DEF_SYSCTRL_ERR(AXCL_ERR_EXECUTE_FAIL)
#define AXCL_ERR_SYSCTRL_CHECK_CONTEXT_ID    AXCL_DEF_SYSCTRL_ERR(0x81)

AXCL_EXPORT axclError axclrtControlCreateContext(int32_t context_id);
AXCL_EXPORT axclError axclrtControlDestroyContext(int32_t context_id);

AXCL_EXPORT axclError axclrtControlCreateStream(int32_t context_id, axclrtStream stream);
AXCL_EXPORT axclError axclrtControlDestroyStream(int32_t context_id, axclrtStream stream);
AXCL_EXPORT axclError axclrtControlSyncStream(int32_t context_id, axclrtStream stream, int32_t timeout_ms);

AXCL_EXPORT axclError axclrtControlCreateEvent(int32_t event_id);
AXCL_EXPORT axclError axclrtControlDestroyEvent(int32_t event_id);
AXCL_EXPORT axclError axclrtControlRecordEvent(axclrtStream stream, int32_t event_id);
AXCL_EXPORT axclError axclrtControlStreamWaitEvent(axclrtStream stream, int32_t event_id, int32_t timeout_ms);


/**
 * @param output: thread_local string, be careful output value is not available for multi-thread
*/
AXCL_EXPORT axclError axclrtControlExecuteShellCmd(const char *cmd, const char *const args[], size_t argc, const char **output, int32_t timeout);

#ifdef __cplusplus
}
#endif

#endif /* __AXCL_RT_CONTROL_INTERNAL_H__ */
