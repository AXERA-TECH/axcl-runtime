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

#ifndef AVCODEC_AXVDEC_H
#define AVCODEC_AXVDEC_H

#if defined(WIN32)
#include <windows.h>
#endif

#include "config.h"

#include "libavutil/fifo.h"
#include "libavutil/opt.h"
#include "hwconfig.h"

#include "avcodec.h"

#include "axcl.h"

#define VDEC_GRP_MAX         (164)
#define AX_SHIFT_LEFT_ALIGN(a) (1 << (a))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define AX_VDEC_WIDTH_ALIGN     AX_SHIFT_LEFT_ALIGN(8)
#define AX_JDEC_WIDTH_ALIGN     AX_SHIFT_LEFT_ALIGN(6)

#define AX_VDEC_HEIGHT_ALIGN     AX_SHIFT_LEFT_ALIGN(6)

#define STREAM_BUFFER_MIN_SIZE           (10 * 1024 * 1024)
#define AX_VDEC_MAX_WIDTH           8192
#define AX_VDEC_MAX_HEIGHT          8192

#define AX_VDEC_DPB_NUM          16

#define CHECK_WITH_RET(cond, ret) \
    do { \
        if ((cond) != 0) { \
            av_log(NULL, AV_LOG_ERROR, "check %s failed\n", #cond); \
            return (ret); \
        } \
    } while (0)

#define AX_VDEC_ALIGN(value, n) (((value) + (n) - 1) & ~((n) - 1))

typedef AX_S32 (*pFnAXCL_SYS_Init)(AX_VOID);
typedef AX_S32 (*pFnAXCL_SYS_Deinit)(AX_VOID);

typedef struct {
    pFnAXCL_SYS_Init sys_init;
    pFnAXCL_SYS_Deinit sys_deinit;
} AXSYSDevFuncs;

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
} AXRTDevFuncs;

typedef AX_S32 (*pFnAXCL_VDEC_Init)(const AX_VDEC_MOD_ATTR_T *pstModAttr);
typedef AX_S32 (*pFnAXCL_VDEC_Deinit)(AX_VOID);
typedef AX_S32 (*pFnAXCL_VDEC_ExtractStreamHeaderInfo)(const AX_VDEC_STREAM_T *pstStreamBuf, AX_PAYLOAD_TYPE_E enVideoType, AX_VDEC_BITSTREAM_INFO_T *pstBitStreamInfo);
typedef AX_S32 (*pFnAXCL_VDEC_CreateGrp)(AX_VDEC_GRP VdGrp, const AX_VDEC_GRP_ATTR_T *pstGrpAttr);
typedef AX_S32 (*pFnAXCL_VDEC_CreateGrpEx)(AX_VDEC_GRP *VdGrp, const AX_VDEC_GRP_ATTR_T *pstGrpAttr);
typedef AX_S32 (*pFnAXCL_VDEC_DestroyGrp)(AX_VDEC_GRP VdGrp);
typedef AX_S32 (*pFnAXCL_VDEC_GetGrpAttr)(AX_VDEC_GRP VdGrp, AX_VDEC_GRP_ATTR_T *pstGrpAttr);
typedef AX_S32 (*pFnAXCL_VDEC_SetGrpAttr)(AX_VDEC_GRP VdGrp, const AX_VDEC_GRP_ATTR_T *pstGrpAttr);
typedef AX_S32 (*pFnAXCL_VDEC_StartRecvStream)(AX_VDEC_GRP VdGrp, const AX_VDEC_RECV_PIC_PARAM_T *pstRecvParam);
typedef AX_S32 (*pFnAXCL_VDEC_StopRecvStream)(AX_VDEC_GRP VdGrp);
typedef AX_S32 (*pFnAXCL_VDEC_QueryStatus)(AX_VDEC_GRP VdGrp, AX_VDEC_GRP_STATUS_T *pstGrpStatus);
typedef AX_S32 (*pFnAXCL_VDEC_ResetGrp)(AX_VDEC_GRP VdGrp);
typedef AX_S32 (*pFnAXCL_VDEC_SetGrpParam)(AX_VDEC_GRP VdGrp, const AX_VDEC_GRP_PARAM_T *pstGrpParam);
typedef AX_S32 (*pFnAXCL_VDEC_GetGrpParam)(AX_VDEC_GRP VdGrp, AX_VDEC_GRP_PARAM_T *pstGrpParam);
typedef AX_S32 (*pFnAXCL_VDEC_SelectGrp)(AX_VDEC_GRP_SET_INFO_T *pstGrpSet, AX_S32 s32MilliSec);
typedef AX_S32 (*pFnAXCL_VDEC_SendStream)(AX_VDEC_GRP VdGrp, const AX_VDEC_STREAM_T *pstStream, AX_S32 s32MilliSec);
typedef AX_S32 (*pFnAXCL_VDEC_GetChnFrame)(AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn, AX_VIDEO_FRAME_INFO_T *pstFrameInfo, AX_S32 s32MilliSec);
typedef AX_S32 (*pFnAXCL_VDEC_ReleaseChnFrame)(AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn, const AX_VIDEO_FRAME_INFO_T *pstFrameInfo);
typedef AX_S32 (*pFnAXCL_VDEC_GetUserData)(AX_VDEC_GRP VdGrp, AX_VDEC_USERDATA_T *pstUserData);
typedef AX_S32 (*pFnAXCL_VDEC_ReleaseUserData)(AX_VDEC_GRP VdGrp, const AX_VDEC_USERDATA_T *pstUserData);
typedef AX_S32 (*pFnAXCL_VDEC_SetUserPic)(AX_VDEC_GRP VdGrp, const AX_VDEC_USRPIC_T *pstUsrPic);
typedef AX_S32 (*pFnAXCL_VDEC_EnableUserPic)(AX_VDEC_GRP VdGrp);
typedef AX_S32 (*pFnAXCL_VDEC_DisableUserPic)(AX_VDEC_GRP VdGrp);
typedef AX_S32 (*pFnAXCL_VDEC_SetDisplayMode)(AX_VDEC_GRP VdGrp, AX_VDEC_DISPLAY_MODE_E enDisplayMode);
typedef AX_S32 (*pFnAXCL_VDEC_GetDisplayMode)(AX_VDEC_GRP VdGrp, AX_VDEC_DISPLAY_MODE_E *penDisplayMode);
typedef AX_S32 (*pFnAXCL_VDEC_AttachPool)(AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn, AX_POOL PoolId);
typedef AX_S32 (*pFnAXCL_VDEC_DetachPool)(AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn);
typedef AX_S32 (*pFnAXCL_VDEC_EnableChn)(AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn);
typedef AX_S32 (*pFnAXCL_VDEC_DisableChn)(AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn);
typedef AX_S32 (*pFnAXCL_VDEC_SetChnAttr)(AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn, const AX_VDEC_CHN_ATTR_T *pstVdChnAttr);
typedef AX_S32 (*pFnAXCL_VDEC_GetChnAttr)(AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn, AX_VDEC_CHN_ATTR_T *pstVdChnAttr);
typedef AX_S32 (*pFnAXCL_VDEC_JpegDecodeOneFrame)(AX_VDEC_DEC_ONE_FRM_T *pstParam);
typedef AX_S32 (*pFnAXCL_VDEC_GetStreamBufInfo)(AX_VDEC_GRP VdGrp, AX_VDEC_STREAM_BUF_INFO_T *pstStreamBufInfo);
typedef AX_S32 (*pFnAXCL_VDEC_GetVuiParam)(AX_VDEC_GRP VdGrp, AX_VDEC_VUI_PARAM_T *pstVuiParam);

typedef struct {
    pFnAXCL_VDEC_Init vdec_init;
    pFnAXCL_VDEC_Deinit vdec_deinit;
    pFnAXCL_VDEC_CreateGrpEx vdec_creategrp;
    pFnAXCL_VDEC_DestroyGrp vdec_destroygrp;
    pFnAXCL_VDEC_SelectGrp vdec_selectgrp;
    pFnAXCL_VDEC_SendStream vdec_sendstream;
    pFnAXCL_VDEC_GetChnFrame vdec_getframe;
    pFnAXCL_VDEC_ReleaseChnFrame vdec_releaseframe;
    pFnAXCL_VDEC_StartRecvStream vdec_startrecvsStream;
    pFnAXCL_VDEC_StopRecvStream vdec_stoprecvstream;
    pFnAXCL_VDEC_QueryStatus vdec_querystatus;
    pFnAXCL_VDEC_ExtractStreamHeaderInfo vdec_getstrminfo;
    pFnAXCL_VDEC_SetGrpParam vdec_setgrpparam;
    pFnAXCL_VDEC_SetDisplayMode vdec_setdispmode;
    pFnAXCL_VDEC_AttachPool vdec_attachpool;
    pFnAXCL_VDEC_DetachPool vdec_detachpool;
    pFnAXCL_VDEC_EnableChn vdec_enablechn;
    pFnAXCL_VDEC_DisableChn vdec_disablechn;
    pFnAXCL_VDEC_SetChnAttr vdec_setchnattr;
} AXVdecFuncs;

typedef struct AXVdecContext {
    AVClass *class;
    AX_PAYLOAD_TYPE_E enCodecType;
    AX_VDEC_GRP VdGrp;
    AX_VDEC_CHN VdChn;
    int devid;
    char *resize_expr;

    struct {
        int width;
        int height;
    } resize;

    char *device_index;
    int index;

    char *out_fmt_string;
    enum AVPixelFormat format;

    int bit_depth_luma;
    int bit_depth_chroma;

    AX_U32 u32StreamBufSize;
    AX_VDEC_INPUT_MODE_E enInputMode;
    AX_IMG_FORMAT_E enImgFormat;        /* Pixel format of target image */
    AX_U32 u32PicWidth;                 /* Width of scaler or crop target image */
    AX_U32 u32PicHeight;                /* Height of scaler or crop target image */
    AX_U32 u32FrameStride;
    AX_U32 u32FramePadding;
    AX_U32 u32CropX;
    AX_U32 u32CropY;
    AX_U32 u32FrameBufCnt;      /* frame buffer number, valid if bSdkAutoFramePool is TRUE */
    AX_U32 u32OutputFifoDepth;
    AX_BOOL bEof;
    AX_BOOL bInited;

    AVBufferRef *hwdevice;
    AVBufferRef *hwframe;

    AVPacket buffer_pkt;

#if defined(WIN32)
    HMODULE sys_lib;
#else
    void *sys_lib;
#endif
    AXSYSDevFuncs sysdev_funcs;

#if defined(WIN32)
    HMODULE rt_lib;
#else
    void *rt_lib;
#endif
    AXRTDevFuncs rtdev_funcs;
    axclrtContext context;

#if defined(WIN32)
    HMODULE mm_vdec_lib;
#else
    void *mm_vdec_lib;
#endif
    AXVdecFuncs vdec_funcs;
} AXVdecContext;


#endif /* AVCODEC_AXVDEC_H */
