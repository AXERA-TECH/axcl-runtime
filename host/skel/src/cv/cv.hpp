/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * License); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * AS IS BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#pragma once

#include <cstdio>
#include <memory>
#include <vector>
#include "axcl_sys.h"
#include "axcl_ivps.h"
#include "io.hpp"
#include "utils.hpp"

namespace axcl::skel {
static inline AX_VIDEO_FRAME_T* alloc_cv_image(const std::string& token, std::array<int, 2> &input_size, AX_IMG_FORMAT_E eDtype, SKEL_IO_BUFFER_STRATEGY_T eStrategy = SKEL_IO_BUFFER_STRATEGY_DEFAULT) {
    int ret = 0;
    auto dst_image = new AX_VIDEO_FRAME_T;
    if (!dst_image) {
        fprintf(stderr, "[ERR] error AX_VIDEO_FRAME_T \n");
        return nullptr;
    }
    memset(dst_image, 0x00, sizeof(AX_VIDEO_FRAME_T));
    dst_image->u32Width = input_size[1];
    dst_image->u32Height = input_size[0];
    dst_image->u32PicStride[0] = dst_image->u32Width;
    dst_image->u32PicStride[1] = dst_image->u32PicStride[0];
    dst_image->u32PicStride[2] = dst_image->u32PicStride[0];
    dst_image->enImgFormat = eDtype;
    dst_image->u32FrameSize = axcl::skel::get_image_data_size(dst_image);

    const std::string token_name = "skel_" + token + "_in";

    if (eStrategy == SKEL_IO_BUFFER_STRATEGY_CACHED) {
        ret = AXCL_SYS_MemAllocCached((AX_U64*)&dst_image->u64PhyAddr[0], (AX_VOID **)&dst_image->u64VirAddr[0], dst_image->u32FrameSize, SKEL_IO_CMM_ALIGN_SIZE, (AX_S8*)token_name.c_str());
    }
    else {
        ret = AXCL_SYS_MemAlloc((AX_U64*)&dst_image->u64PhyAddr[0], (AX_VOID **)&dst_image->u64VirAddr[0], dst_image->u32FrameSize, SKEL_IO_CMM_ALIGN_SIZE, (AX_S8*)token_name.c_str());
    }

    if (ret != 0) {
        fprintf(stderr, "[ERR] error alloc image sys mem %x \n", ret);
        return nullptr;
    }

    dst_image->u64PhyAddr[1] = dst_image->u64PhyAddr[0] + dst_image->u32PicStride[0] * dst_image->u32Height;

    return dst_image;
}

static inline void flush_cv_image(AX_VIDEO_FRAME_T *pstFrame) {
    if (pstFrame->u64PhyAddr[0] != 0) {
        AXCL_SYS_MflushCache(pstFrame->u64PhyAddr[0], (AX_VOID *)&pstFrame->u64VirAddr[0], pstFrame->u32FrameSize);
    }
}

static inline int free_cv_image(AX_VIDEO_FRAME_T* image) {
    int ret = AXCL_SYS_MemFree((AX_U64)image->u64PhyAddr[0], (AX_VOID *)image->u64VirAddr[0]);

    delete image;

    if (ret != 0) {
        fprintf(stderr, "[ERR] error free %x \n", ret);
        return ret;
    }

    return 0;
}

static inline int crop_resize(const AX_VIDEO_FRAME_T* input_image, AX_VIDEO_FRAME_T* output_image, AX_IVPS_RECT_T* tRect,
                                    AX_IVPS_ASPECT_RATIO_ALIGN_E horizontal, AX_IVPS_ASPECT_RATIO_ALIGN_E vertical, AXCL_SKEL_CV_INTF_TYPE_E eResizeInfType = AXCL_SKEL_CV_INTF_TYPE_VPP) {
    AX_IVPS_ASPECT_RATIO_T tAspectRatio;

    memset(&tAspectRatio, 0x00, sizeof(tAspectRatio));
    tAspectRatio.eAligns[0] = horizontal;
    tAspectRatio.eAligns[1] = vertical;
    tAspectRatio.nBgColor = 0x00000000;

    tAspectRatio.eMode = AX_IVPS_ASPECT_RATIO_AUTO;
    if (tRect) {
        tAspectRatio.tRect = *tRect;
    }

    AX_S32 ret = 0;

    if (eResizeInfType == AXCL_SKEL_CV_INTF_TYPE_VPP) {
         ret = AXCL_IVPS_CropResizeVpp(input_image, output_image, &tAspectRatio);
    }
    else if (eResizeInfType == AXCL_SKEL_CV_INTF_TYPE_TDP) {
        ret = AXCL_IVPS_CropResizeTdp(input_image, output_image, &tAspectRatio);
    }
    else if (eResizeInfType == AXCL_SKEL_CV_INTF_TYPE_VGP) {
        ret = AXCL_IVPS_CropResizeVgp(input_image, output_image, &tAspectRatio);
    }
    else {
        ret = AXCL_IVPS_CropResizeVpp(input_image, output_image, &tAspectRatio);
    }

    return ret;
}

static inline int FrameCropResize(const CSkelFrame *pSkelFrame, AX_VIDEO_FRAME_T *pCvImage, AXCL_SKEL_CV_INTF_TYPE_E eResizeInfType = AXCL_SKEL_CV_INTF_TYPE_VPP) {
    auto ConvFrame = [](const CSkelFrame *pSrc) -> AX_VIDEO_FRAME_T {
        AX_VIDEO_FRAME_T t = {0};
        if (pSrc->nPhy[0] != 0) {
            t.u32Width = pSrc->nOriginalWidth;
            t.u32Height = pSrc->nOriginalHeight;
            t.enImgFormat = pSrc->enImgFormat;
            t.u32PicStride[0] = (0 == pSrc->nStride[0]) ? t.u32Width : pSrc->nStride[0];
            t.u32FrameSize = pSrc->nSize;
            t.u64PhyAddr[0] = pSrc->nPhy[0];
            t.u64VirAddr[0] = (AX_U64)pSrc->pVir[0];
        }
        else {
            t = pSrc->stFrame;
        }

        return t;
    };

    AX_VIDEO_FRAME_T tSrcFrame = ConvFrame(pSkelFrame);

    auto ret = axcl::skel::crop_resize(&tSrcFrame, pCvImage, NULL, AX_IVPS_ASPECT_RATIO_HORIZONTAL_CENTER, AX_IVPS_ASPECT_RATIO_VERTICAL_CENTER, eResizeInfType);

    return ret;
}

static inline int FrameCropResize(const AX_VIDEO_FRAME_T *pSrcFrame, AX_VIDEO_FRAME_T *pCvImage, AXCL_SKEL_CV_INTF_TYPE_E eResizeInfType = AXCL_SKEL_CV_INTF_TYPE_VPP) {
    auto ret = axcl::skel::crop_resize(pSrcFrame, pCvImage, NULL, AX_IVPS_ASPECT_RATIO_HORIZONTAL_CENTER, AX_IVPS_ASPECT_RATIO_VERTICAL_CENTER, eResizeInfType);

    return ret;
}

static inline int FrameCsc(const AX_VIDEO_FRAME_T* input_image, AX_VIDEO_FRAME_T* output_image, AXCL_SKEL_CV_INTF_TYPE_E eCscInfType = AXCL_SKEL_CV_INTF_TYPE_VPP) {
    AX_S32 ret = 0;
    if (eCscInfType == AXCL_SKEL_CV_INTF_TYPE_VPP) {
        ret = AXCL_IVPS_CscVpp(input_image, output_image);
    }
    else if (eCscInfType == AXCL_SKEL_CV_INTF_TYPE_TDP) {
        ret = AXCL_IVPS_CscTdp(input_image, output_image);
    }
    else if (eCscInfType == AXCL_SKEL_CV_INTF_TYPE_VGP) {
        ret = AXCL_IVPS_CscVgp(input_image, output_image);
    }
    else {
        ret = AXCL_IVPS_CscVpp(input_image, output_image);
    }

    return ret;
}

static inline AX_S32 GetCvIntfType(const AX_CHAR *pstrEnv, AXCL_SKEL_CV_INTF_TYPE_E &eCvInfType) {
    AX_CHAR *pEnv = NULL;

    pEnv = getenv(pstrEnv);

    if (pEnv) {
        std::string strEnv = pEnv;

        ALOGW("CV Interface: {}={}", pstrEnv, strEnv.c_str());

        if (strEnv == "VPP") {
            eCvInfType = AXCL_SKEL_CV_INTF_TYPE_VPP;
        }
        else if (strEnv == "TDP") {
            eCvInfType = AXCL_SKEL_CV_INTF_TYPE_TDP;
        }
        else if (strEnv == "VGP") {
            eCvInfType = AXCL_SKEL_CV_INTF_TYPE_VGP;
        }
        else {
            eCvInfType = AXCL_SKEL_CV_INTF_TYPE_VPP;
        }
    }

    return AXCL_SKEL_SUCC;
}

}  // namespace axcl::skel

using namespace axcl::skel;
