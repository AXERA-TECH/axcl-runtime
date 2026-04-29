/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#pragma once
#include <exception>
#include <vector>
#include <map>
#include <unordered_map>
#include "axcl_skel.h"
#include "err.hpp"
#include "log.hpp"

//#define CSkelMap std::map
#define CSkelMap std::unordered_map

#define CHECK_PTR(p)                     \
    do {                                 \
        if (!p) {                        \
            ALOGE("nil pointer");      \
            return AXCL_ERR_SKEL_NULL_PTR; \
        }                                \
    } while (0)

#define CHECK_SKEL_HANDLE(_handle)                      \
    do {                                               \
        if (!_handle) {                                 \
            ALOGE("handle is invalid"); \
            return AXCL_ERR_SKEL_INVALID_HANDLE;         \
        }                                              \
    } while (0)

#define CHECK_RETCODE(_statement)    \
    do {                             \
        AX_S32 __ret = (_statement); \
        if (AXCL_SKEL_SUCC != __ret) { \
            return __ret;            \
        }                            \
    } while (0)

#define CHECK_SKEL_INITED()                \
    do {                                 \
        if (!m_bInited) {                \
            ALOGE("SKEL is not inited"); \
            return AXCL_ERR_SKEL_NOT_INIT; \
        }                                \
    } while(0)

#define SAFE_DELETE_PTR(p)                \
    do {                                 \
        if (p) {                \
            delete p; \
            p = nullptr; \
        }                                \
    } while(0)

#ifndef AX_MAX
#define AX_MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif
        
#ifndef AX_MIN
#define AX_MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef ALIGN_UP
#define ALIGN_UP(x, align) (((x) + ((align)-1)) & ~((align)-1))
#endif

#ifndef ALIGN_DOWN
#define ALIGN_DOWN(x, align) ((x) & ~((align)-1))
#endif

//#define __AXCL_SKEL_DEBUG__

#ifdef __AXCL_SKEL_DEBUG__
#define __ELAPSED_SKEL_TIME__ AX_TRUE
#define __PRINT_SKEL_TIME__ AX_FALSE
#else
#define __ELAPSED_SKEL_TIME__ AX_FALSE
#define __PRINT_SKEL_TIME__ AX_FALSE
#endif
