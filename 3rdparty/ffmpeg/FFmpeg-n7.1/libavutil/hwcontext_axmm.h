/*
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef AVUTIL_HWCONTEXT_AXMM_H
#define AVUTIL_HWCONTEXT_AXMM_H

#if defined(WIN32)
#include <windows.h>
#endif

#include "axcl.h"

/**
 * @file
 * An API-specific header for AV_HWDEVICE_TYPE_AXMM.
 */

/**
 * This struct is allocated as AVHWDeviceContext.hwctx
 */
typedef struct AVAXMMDeviceContext {
} AVAXMMDeviceContext;

/**
 * This struct is allocated as AVHWFramesContext.hwctx
 */
typedef struct AVAXMMFramesContext {
    unsigned devid;
    unsigned blk_id;
} AVAXMMFramesContext;


typedef AX_S32   (*pFnAXCL_SYS_Init)(AX_VOID);
typedef AX_S32   (*pFnAXCL_SYS_Deinit)(AX_VOID);
typedef AX_S32   (*pFnAXCL_POOL_Init)(AX_VOID);
typedef AX_S32   (*pFnAXCL_POOL_Exit)(AX_VOID);
typedef AX_POOL  (*pFnAXCL_POOL_CreatePool)(AX_POOL_CONFIG_T *);
typedef AX_S32   (*pFnAXCL_POOL_DestroyPool)(AX_POOL);
typedef AX_BLK   (*pFnAXCL_POOL_GetBlock)(AX_POOL, AX_U64, const AX_S8 *);
typedef AX_S32   (*pFnAXCL_POOL_ReleaseBlock)(AX_BLK);
typedef AX_BLK   (*pFnAXCL_POOL_PhysAddr2Handle)(AX_U64);
typedef AX_U64   (*pFnAXCL_POOL_Handle2PhysAddr)(AX_BLK);
typedef AX_VOID *(*pFnAXCL_POOL_GetBlockVirAddr)(AX_BLK);
typedef AX_S32   (*pFnAXCL_POOL_IncreaseRefCnt)(AX_BLK);
typedef AX_S32   (*pFnAXCL_POOL_DecreaseRefCnt)(AX_BLK);

typedef struct {
    pFnAXCL_SYS_Init sys_init;
    pFnAXCL_SYS_Deinit sys_deinit;
    pFnAXCL_POOL_Init pool_init;
    pFnAXCL_POOL_Exit pool_exit;
    pFnAXCL_POOL_CreatePool pool_create;
    pFnAXCL_POOL_DestroyPool pool_destroy;
    pFnAXCL_POOL_GetBlock pool_get_block;
    pFnAXCL_POOL_ReleaseBlock pool_release_block;
    pFnAXCL_POOL_PhysAddr2Handle pool_paddr2handle;
    pFnAXCL_POOL_Handle2PhysAddr pool_handle2paddr;
    pFnAXCL_POOL_GetBlockVirAddr pool_handle2vaddr;
    pFnAXCL_POOL_IncreaseRefCnt pool_ref_get;
    pFnAXCL_POOL_DecreaseRefCnt pool_ref_put;
} AXMMDeviceFuncs;

typedef axclError (*pFnaxclInit)(const char *);
typedef axclError (*pFnaxclFinalize)(void);
typedef axclError (*pFnaxclrtSetDevice)(int32_t);
typedef axclError (*pFnaxclrtResetDevice)(int32_t);
typedef axclError (*pFnaxclrtGetDeviceList)(axclrtDeviceList *);
typedef axclError (*pFnaxclrtCreateContext)(axclrtContext *, int32_t);
typedef axclError (*pFnaxclrtGetCurrentContext)(axclrtContext *);
typedef axclError (*pFnaxclrtDestroyContext)(axclrtContext);
typedef axclError (*pFnaxclrtMemcpy)(void *, const void *, size_t, axclrtMemcpyKind);

typedef struct {
    pFnaxclInit rt_init;
    pFnaxclFinalize rt_finalize;
    pFnaxclrtSetDevice rt_set_device;
    pFnaxclrtResetDevice rt_reset_device;
    pFnaxclrtGetDeviceList rt_get_device_list;
    pFnaxclrtCreateContext rt_create_context;
    pFnaxclrtGetCurrentContext rt_get_current_context;
    pFnaxclrtDestroyContext rt_destroy_context;
    pFnaxclrtMemcpy rt_memcpy;
} AXRTDeviceFuncs;

typedef struct {
    int devid;
    int dev_index;

    int alloc_blk;

#if defined(WIN32)
    HMODULE rt_lib;
    HMODULE mm_sys_lib;
#else
    void *rt_lib;
    void *mm_sys_lib;
#endif

    AXRTDeviceFuncs rtdev_funcs;
    AXMMDeviceFuncs mmdev_funcs;
} AXMMDeviceContext;

typedef struct {
    /* The public AVAXMMFramesContext. See hwcontext_axmm.h for it */
    AVAXMMFramesContext p;

    int devid;
    int dev_index;

    int alloc_blk;

    AX_POOL pool_id;
    AX_POOL_CONFIG_T pool_cfg;
} AXMMFramesContext;

#endif /* AVUTIL_HWCONTEXT_AXMM_H */
