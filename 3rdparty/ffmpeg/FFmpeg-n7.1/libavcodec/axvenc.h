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

#ifndef AVCODEC_AXVENC_H
#define AVCODEC_AXVENC_H

#if defined(WIN32)
#include <windows.h>
#endif

#include "config.h"

#include "libavutil/opt.h"
#include "hwconfig.h"

#include "avcodec.h"

#include "axcl.h"

typedef axclError (*pFnaxclrtMemcpy)(void *dstPtr, const void *srcPtr, size_t count, axclrtMemcpyKind kind);

typedef AX_S32 (*pFnAXCL_VENC_Init)(const AX_VENC_MOD_ATTR_T *pstModAttr);
typedef AX_S32 (*pFnAXCL_VENC_Deinit)();
typedef AX_S32 (*pFnAXCL_VENC_CreateChnEx)(VENC_CHN *pVeChn, const AX_VENC_CHN_ATTR_T *pstAttr);
typedef AX_S32 (*pFnAXCL_VENC_DestroyChn)(VENC_CHN VeChn);
typedef AX_S32 (*pFnAXCL_VENC_ResetChn)(VENC_CHN VeChn);
typedef AX_S32 (*pFnAXCL_VENC_SetChnAttr)(VENC_CHN VeChn, const AX_VENC_CHN_ATTR_T *pstChnAttr);
typedef AX_S32 (*pFnAXCL_VENC_GetChnAttr)(VENC_CHN VeChn, AX_VENC_CHN_ATTR_T *pstChnAttr);
typedef AX_S32 (*pFnAXCL_VENC_StartRecvFrame)(VENC_CHN VeChn, const AX_VENC_RECV_PIC_PARAM_T *pstRecvParam);
typedef AX_S32 (*pFnAXCL_VENC_StopRecvFrame)(VENC_CHN VeChn);
typedef AX_S32 (*pFnAXCL_VENC_SendFrame)(VENC_CHN VeChn, const AX_VIDEO_FRAME_INFO_T *pstFrame, AX_S32 s32MilliSec);
typedef AX_S32 (*pFnAXCL_VENC_GetStream)(VENC_CHN VeChn, AX_VENC_STREAM_T *pstStream, AX_S32 s32MilliSec);
typedef AX_S32 (*pFnAXCL_VENC_ReleaseStream)(VENC_CHN VeChn, const AX_VENC_STREAM_T *pstStream);

typedef struct {
#if defined(WIN32)
    HMODULE lib;
#else
    void *lib;
#endif
    pFnAXCL_VENC_Init venc_init;
    pFnAXCL_VENC_Deinit venc_deinit;
    pFnAXCL_VENC_CreateChnEx venc_create_chn_ex;
    pFnAXCL_VENC_DestroyChn venc_destroy_chn;
    pFnAXCL_VENC_ResetChn venc_reset_chn;
    pFnAXCL_VENC_SetChnAttr venc_set_chn_attr;
    pFnAXCL_VENC_GetChnAttr venc_get_chn_attr;
    pFnAXCL_VENC_StartRecvFrame venc_start_recv_frame;
    pFnAXCL_VENC_StopRecvFrame venc_stop_recv_frame;
    pFnAXCL_VENC_SendFrame venc_send_frame;
    pFnAXCL_VENC_GetStream venc_get_stream;
    pFnAXCL_VENC_ReleaseStream venc_release_stream;

#if defined(WIN32)
    HMODULE lib_rt;
#else
    void *lib_rt;
#endif
    pFnaxclrtMemcpy rt_memcpy;
} AXVencDlFuncs;

typedef struct {
    const AVClass *class;

    VENC_CHN chn;
    AX_VENC_MOD_ATTR_T mod_attr;
    AX_VENC_CHN_ATTR_T chn_attr;
    AX_VENC_RECV_PIC_PARAM_T recv_param;
    AX_VIDEO_FRAME_INFO_T frame_internal;
    AX_VENC_STREAM_T stream;

    AXVencDlFuncs dl_funcs;

    int (*attr_init)(AVCodecContext *ctx);

    int profile;
    int level;
    int rc;
    int i_qmin;
    int i_qmax;

    int intraQpDelta; /* Range:[-51, 51]; QP difference between target QP and intra frame QP */
    int idrQpDeltaRange; /* Range:[2, 10]; QP rang between CU QP and I frame QP */
    int maxIprop; /* Range:[1, 100]; the max I P size ratio */
    int minIprop; /* Range:[1, u32MaxIprop]; the min I P size ratio */
    unsigned int changePos;              /* Range:[20, 100] */
    AX_VENC_VBR_QUALITY_LEVEL_E enVQ; /* def AX_VENC_VBR_QUALITY_LEVEL_INV */
    unsigned int minStillPercent;        /* Range:[10, 100] */
    unsigned int maxStillQp;             /* Range:[u32MinIQp, u32MaxIQp]; def 36 */
    /**
     * Range[-1, 51]; Start QP value of the first frame
     * -1: Encoder calculates initial QP.
     */
    int firstFrameStartQp;

    AVFrame *buffer_frame;
    AVFrame *hw_frame;
} AXVencContext;

int ff_axvenc_init(AVCodecContext *ctx);
int ff_axvenc_receive_packet(AVCodecContext *ctx, AVPacket *pkt);
int ff_axvenc_close(AVCodecContext *ctx);
void ff_axvenc_flush(AVCodecContext *ctx);

#endif /* AVCODEC_AXVENC_H */
