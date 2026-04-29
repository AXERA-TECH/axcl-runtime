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

#include "ax_global_type.h"

namespace axcl::skel {
class CSkelFrame;
class IFrameRelease {
public:
    virtual AX_VOID SkelFrameRelease(CSkelFrame *pSkelFrame) = 0;
};

class CSkelFrame {
public:
    AX_BOOL bUsed;
    AX_U64 nFrameId;
    AX_U32 nStreamId;
    AX_U32 nOriginalWidth;
    AX_U32 nOriginalHeight;
    AX_U32 nWidth;
    AX_U32 nHeight;
    AX_IMG_FORMAT_E enImgFormat;
    AX_U32 nSize;
    AX_U32 nStride[AX_MAX_COLOR_COMPONENT];
    AX_U64 nPhy[AX_MAX_COLOR_COMPONENT];
    AX_VOID *pVir[AX_MAX_COLOR_COMPONENT];
    AX_VOID *pUserData;
    AX_VIDEO_FRAME_T stFrame;
    IFrameRelease *pFrameRelease;

    CSkelFrame() {
        bUsed = AX_FALSE;
        nFrameId = 0;
        nStreamId = 0;
        nOriginalWidth = 0;
        nOriginalHeight = 0;
        nWidth = 0;
        nHeight = 0;
        enImgFormat = AX_FORMAT_YUV420_SEMIPLANAR;
        nSize = 0;
        nStride[0] = 0;
        nStride[1] = 0;
        nStride[2] = 0;
        nPhy[0] = 0;
        nPhy[1] = 0;
        nPhy[2] = 0;
        pVir[0] = NULL;
        pVir[1] = NULL;
        pVir[2] = NULL;
        pUserData = NULL;
        pFrameRelease = NULL;
    }

    virtual ~CSkelFrame() {
    }

    AX_VOID FreeMem(void) {
        if (pFrameRelease) {
            pFrameRelease->SkelFrameRelease(this);
        }
    }
};
}

using namespace axcl::skel;
