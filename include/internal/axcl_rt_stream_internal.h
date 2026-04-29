/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#ifndef __AXCL_RT_STREAM_INTERNAL_H__
#define __AXCL_RT_STREAM_INTERNAL_H__

#include "axcl_rt_stream.h"
#include "axcl_rt_type_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

AXCL_EXPORT axclError axclrtGetDefaultStream(axclrtContext context, axclrtStream *stream);
AXCL_EXPORT axclError axclrtGetEnviromentByStream(axclrtStream stream, axclrtEnvironment *env);

#ifdef __cplusplus
}
#endif

#endif /* __AXCL_RT_STREAM_INTERNAL_H__ */