/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#ifndef __AXCL_RT_TASK_H__
#define __AXCL_RT_TASK_H__

#include "axcl_base.h"
#include "axcl_rt_type_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void* axclrtTask;

#define AXCL_DEF_TASK_ERR(e)            AXCL_DEF_RUNTIME_ERR(AXCL_RUNTIME_TASK, (e))

#define AXCL_ERR_TASK_NULL_POINTER      AXCL_DEF_TASK_ERR(AXCL_ERR_NULL_POINTER)
#define AXCL_ERR_TASK_ILLEGAL_PARAM     AXCL_DEF_TASK_ERR(AXCL_ERR_ILLEGAL_PARAM)
#define AXCL_ERR_TASK_NO_MEMORY         AXCL_DEF_TASK_ERR(AXCL_ERR_NO_MEMORY)
#define AXCL_ERR_TASK_TIMEOUT           AXCL_DEF_TASK_ERR(AXCL_ERR_TIMEOUT)
#define AXCL_ERR_TASK_EXECUTE_FAIL      AXCL_DEF_TASK_ERR(AXCL_ERR_EXECUTE_FAIL)
#define AXCL_ERR_TASK_UNKNOWN           AXCL_DEF_TASK_ERR(AXCL_ERR_UNKNOWN)

#define AXCL_ERR_TASK_SUBMIT            AXCL_DEF_TASK_ERR(0x81)

/**
 * task
 */
AXCL_EXPORT axclError axclrtCreateTask(axclrtTask* task, axclrtStream stream, uint64_t token, const void* data, size_t size, int32_t timeout, bool has_response, int64_t flag);
AXCL_EXPORT axclError axclrtWaitTaskFinish(axclrtTask task);
AXCL_EXPORT axclError axclrtGetTaskResponse(axclrtTask task, void** buf, uint32_t* size);
AXCL_EXPORT axclError axclrtDestoryTask(axclrtTask task);

#ifdef __cplusplus
}
#endif

#endif /* __AXCL_RT_TASK_H__ */