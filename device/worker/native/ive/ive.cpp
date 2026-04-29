/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "ive.hpp"
#include "dispatcher.hpp"
#include "axcl_logger.hpp"

#define TAG "ive"

namespace axcl::worker {
ive::ive(dispatcher *dispatcher) : base(dispatcher), m_api(TAG) {
    m_api.append({
        NATIVE_IVE_API_APPEND(AX_IVE_Init),
        NATIVE_IVE_API_APPEND(AX_IVE_Exit),
        NATIVE_IVE_API_APPEND(AX_IVE_Query),
        NATIVE_IVE_API_APPEND(AX_IVE_DMA),
        NATIVE_IVE_API_APPEND(AX_IVE_Add),
        NATIVE_IVE_API_APPEND(AX_IVE_Sub),
        NATIVE_IVE_API_APPEND(AX_IVE_And),
        NATIVE_IVE_API_APPEND(AX_IVE_Or),
        NATIVE_IVE_API_APPEND(AX_IVE_Xor),
        NATIVE_IVE_API_APPEND(AX_IVE_Mse),
        NATIVE_IVE_API_APPEND(AX_IVE_CannyHysEdge),
        NATIVE_IVE_API_APPEND(AX_IVE_CannyEdge),
        NATIVE_IVE_API_APPEND(AX_IVE_CCL),
        NATIVE_IVE_API_APPEND(AX_IVE_Erode),
        NATIVE_IVE_API_APPEND(AX_IVE_Dilate),
        NATIVE_IVE_API_APPEND(AX_IVE_Filter),
        NATIVE_IVE_API_APPEND(AX_IVE_Hist),
        NATIVE_IVE_API_APPEND(AX_IVE_EqualizeHist),
        NATIVE_IVE_API_APPEND(AX_IVE_Integ),
        NATIVE_IVE_API_APPEND(AX_IVE_MagAndAng),
        NATIVE_IVE_API_APPEND(AX_IVE_Sobel),
        NATIVE_IVE_API_APPEND(AX_IVE_GMM),
        NATIVE_IVE_API_APPEND(AX_IVE_GMM2),
        NATIVE_IVE_API_APPEND(AX_IVE_Thresh),
        NATIVE_IVE_API_APPEND(AX_IVE_16BitTo8Bit),
        NATIVE_IVE_API_APPEND(AX_IVE_CropImage),
        NATIVE_IVE_API_APPEND(AX_IVE_CropResize),
        NATIVE_IVE_API_APPEND(AX_IVE_CropResizeForSplitYUV),
        NATIVE_IVE_API_APPEND(AX_IVE_CSC),
        NATIVE_IVE_API_APPEND(AX_IVE_CropResize2),
        NATIVE_IVE_API_APPEND(AX_IVE_CropResize2ForSplitYUV),
        NATIVE_IVE_API_APPEND(AX_IVE_MAU_MatMul),
        NATIVE_IVE_API_APPEND(AX_IVE_NPU_CreateMatMulHandle),
        NATIVE_IVE_API_APPEND(AX_IVE_NPU_DestroyMatMulHandle),
        NATIVE_IVE_API_APPEND(AX_IVE_NPU_MatMul),
    });
}

void ive::run(const void *data, const size_t& size) {
    m_api.run(get_dispatcher(), data, size);
}

NATIVE_IVE_API_IMPL(AX_IVE_Init) {
    if (!response->unpack(response->get_api())) {
        return false;
    }

    AX_S32 ret = ::AX_IVE_Init();

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVE_API_IMPL(AX_IVE_Exit) {
    if (!response->unpack(response->get_api())) {
        return false;
    }

    ::AX_IVE_Exit();

    if (!request->pack(response->get_api())) {
        return false;
    }

    return true;
}

NATIVE_IVE_API_IMPL(AX_IVE_Query) {
    AX_IVE_HANDLE IveHandle;
    AX_BOOL bBlock;

    if (!response->unpack(response->get_api(), &IveHandle, &bBlock)) {
        return false;
    }

    AX_BOOL bFinish;
    AX_S32 ret = ::AX_IVE_Query(IveHandle, &bFinish, bBlock);

    if (!request->pack(response->get_api(), ret, bFinish)) {
        return false;
    }

    return true;
}

NATIVE_IVE_API_IMPL(AX_IVE_DMA) {
    AX_IVE_SRC_DATA_T stSrc;
    AX_IVE_DST_DATA_T stDst;
    AX_IVE_DMA_CTRL_T stDmaCtrl;
    AX_BOOL bInstant;

    if (!response->unpack(response->get_api(), &stSrc, &stDst, &stDmaCtrl, &bInstant)) {
        return false;
    }

    AX_IVE_HANDLE IveHandle;
    AX_S32 ret = ::AX_IVE_DMA(&IveHandle, &stSrc, &stDst, &stDmaCtrl, bInstant);

    if (!request->pack(response->get_api(), ret, IveHandle)) {
        return false;
    }

    return true;
}

NATIVE_IVE_API_IMPL(AX_IVE_Add) {
    AX_IVE_SRC_IMAGE_T stSrc1;
    AX_IVE_SRC_IMAGE_T stSrc2;
    AX_IVE_DST_IMAGE_T stDst;
    AX_IVE_ADD_CTRL_T stAddCtrl;
    AX_BOOL bInstant;

    if (!response->unpack(response->get_api(), &stSrc1, &stSrc2, &stDst, &stAddCtrl, &bInstant)) {
        return false;
    }

    AX_IVE_HANDLE IveHandle;
    AX_S32 ret = ::AX_IVE_Add(&IveHandle, &stSrc1, &stSrc2, &stDst, &stAddCtrl, bInstant);

    if (!request->pack(response->get_api(), ret, IveHandle)) {
        return false;
    }

    return true;
}

NATIVE_IVE_API_IMPL(AX_IVE_Sub) {
    AX_IVE_SRC_IMAGE_T stSrc1;
    AX_IVE_SRC_IMAGE_T stSrc2;
    AX_IVE_DST_IMAGE_T stDst;
    AX_IVE_SUB_CTRL_T stSubCtrl;
    AX_BOOL bInstant;

    if (!response->unpack(response->get_api(), &stSrc1, &stSrc2, &stDst, &stSubCtrl, &bInstant)) {
        return false;
    }

    AX_IVE_HANDLE IveHandle;
    AX_S32 ret = ::AX_IVE_Sub(&IveHandle, &stSrc1, &stSrc2, &stDst, &stSubCtrl, bInstant);

    if (!request->pack(response->get_api(), ret, IveHandle)) {
        return false;
    }

    return true;
}

NATIVE_IVE_API_IMPL(AX_IVE_And) {
    AX_IVE_SRC_IMAGE_T stSrc1;
    AX_IVE_SRC_IMAGE_T stSrc2;
    AX_IVE_DST_IMAGE_T stDst;
    AX_BOOL bInstant;

    if (!response->unpack(response->get_api(), &stSrc1, &stSrc2, &stDst, &bInstant)) {
        return false;
    }

    AX_IVE_HANDLE IveHandle;
    AX_S32 ret = ::AX_IVE_And(&IveHandle, &stSrc1, &stSrc2, &stDst, bInstant);

    if (!request->pack(response->get_api(), ret, IveHandle)) {
        return false;
    }

    return true;
}

NATIVE_IVE_API_IMPL(AX_IVE_Or) {
    AX_IVE_SRC_IMAGE_T stSrc1;
    AX_IVE_SRC_IMAGE_T stSrc2;
    AX_IVE_DST_IMAGE_T stDst;
    AX_BOOL bInstant;

    if (!response->unpack(response->get_api(), &stSrc1, &stSrc2, &stDst, &bInstant)) {
        return false;
    }

    AX_IVE_HANDLE IveHandle;
    AX_S32 ret = ::AX_IVE_Or(&IveHandle, &stSrc1, &stSrc2, &stDst, bInstant);

    if (!request->pack(response->get_api(), ret, IveHandle)) {
        return false;
    }

    return true;
}

NATIVE_IVE_API_IMPL(AX_IVE_Xor) {
    AX_IVE_SRC_IMAGE_T stSrc1;
    AX_IVE_SRC_IMAGE_T stSrc2;
    AX_IVE_DST_IMAGE_T stDst;
    AX_BOOL bInstant;

    if (!response->unpack(response->get_api(), &stSrc1, &stSrc2, &stDst, &bInstant)) {
        return false;
    }

    AX_IVE_HANDLE IveHandle;
    AX_S32 ret = ::AX_IVE_Xor(&IveHandle, &stSrc1, &stSrc2, &stDst, bInstant);

    if (!request->pack(response->get_api(), ret, IveHandle)) {
        return false;
    }

    return true;
}

NATIVE_IVE_API_IMPL(AX_IVE_Mse) {
    AX_IVE_SRC_IMAGE_T stSrc1;
    AX_IVE_SRC_IMAGE_T stSrc2;
    AX_IVE_DST_IMAGE_T stDst;
    AX_IVE_MSE_CTRL_T stMseCtrl;
    AX_BOOL bInstant;

    if (!response->unpack(response->get_api(), &stSrc1, &stSrc2, &stDst, &stMseCtrl, &bInstant)) {
        return false;
    }

    AX_IVE_HANDLE IveHandle;
    AX_S32 ret = ::AX_IVE_Mse(&IveHandle, &stSrc1, &stSrc2, &stDst, &stMseCtrl, bInstant);

    if (!request->pack(response->get_api(), ret, IveHandle)) {
        return false;
    }

    return true;
}

NATIVE_IVE_API_IMPL(AX_IVE_CannyHysEdge) {
    AX_IVE_SRC_IMAGE_T stSrc1;
    AX_IVE_SRC_IMAGE_T stSrc2;
    AX_IVE_DST_IMAGE_T stDst;
    AX_IVE_HYS_EDGE_CTRL_T stCannyHysEdgeCtrl;
    AX_BOOL bInstant;

    if (!response->unpack(response->get_api(), &stSrc1, &stSrc2, &stDst, &stCannyHysEdgeCtrl, &bInstant)) {
        return false;
    }

    AX_IVE_HANDLE IveHandle;
    AX_S32 ret = ::AX_IVE_CannyHysEdge(&IveHandle, &stSrc1, &stSrc2, &stDst, &stCannyHysEdgeCtrl, bInstant);

    if (!request->pack(response->get_api(), ret, IveHandle)) {
        return false;
    }

    return true;
}

NATIVE_IVE_API_IMPL(AX_IVE_CannyEdge) {
    AX_IVE_SRC_IMAGE_T stSrc;
    AX_IVE_DST_IMAGE_T stDst;
    AX_IVE_CANNY_EDGE_CTRL_T stCannyEdgeCtrl;
    AX_BOOL bInstant;

    if (!response->unpack(response->get_api(), &stSrc, &stDst, &stCannyEdgeCtrl, &bInstant)) {
        return false;
    }

    AX_IVE_HANDLE IveHandle;
    AX_S32 ret = ::AX_IVE_CannyEdge(&IveHandle, &stSrc, &stDst, &stCannyEdgeCtrl, bInstant);

    if (!request->pack(response->get_api(), ret, IveHandle)) {
        return false;
    }

    return true;
}

NATIVE_IVE_API_IMPL(AX_IVE_CCL) {
    AX_IVE_SRC_IMAGE_T stSrc;
    AX_IVE_DST_IMAGE_T stDst;
    AX_IVE_DST_MEM_INFO_T stBlob;
    AX_IVE_CCL_CTRL_T stCclCtrl;
    AX_BOOL bInstant;

    if (!response->unpack(response->get_api(), &stSrc, &stDst, &stBlob, &stCclCtrl, &bInstant)) {
        return false;
    }

    AX_IVE_HANDLE IveHandle;
    AX_S32 ret = ::AX_IVE_CCL(&IveHandle, &stSrc, &stDst, &stBlob, &stCclCtrl, bInstant);

    if (!request->pack(response->get_api(), ret, IveHandle)) {
        return false;
    }

    return true;
}

NATIVE_IVE_API_IMPL(AX_IVE_Erode) {
    AX_IVE_SRC_IMAGE_T stSrc;
    AX_IVE_DST_IMAGE_T stDst;
    AX_IVE_ERODE_CTRL_T stErodeCtrl;
    AX_BOOL bInstant;

    if (!response->unpack(response->get_api(), &stSrc, &stDst, &stErodeCtrl, &bInstant)) {
        return false;
    }

    AX_IVE_HANDLE IveHandle;
    AX_S32 ret = ::AX_IVE_Erode(&IveHandle, &stSrc, &stDst, &stErodeCtrl, bInstant);

    if (!request->pack(response->get_api(), ret, IveHandle)) {
        return false;
    }

    return true;
}

NATIVE_IVE_API_IMPL(AX_IVE_Dilate) {
    AX_IVE_SRC_IMAGE_T stSrc;
    AX_IVE_DST_IMAGE_T stDst;
    AX_IVE_DILATE_CTRL_T stDilateCtrl;
    AX_BOOL bInstant;

    if (!response->unpack(response->get_api(), &stSrc, &stDst, &stDilateCtrl, &bInstant)) {
        return false;
    }

    AX_IVE_HANDLE IveHandle;
    AX_S32 ret = ::AX_IVE_Dilate(&IveHandle, &stSrc, &stDst, &stDilateCtrl, bInstant);

    if (!request->pack(response->get_api(), ret, IveHandle)) {
        return false;
    }

    return true;
}

NATIVE_IVE_API_IMPL(AX_IVE_Filter) {
    AX_IVE_SRC_IMAGE_T stSrc;
    AX_IVE_DST_IMAGE_T stDst;
    AX_IVE_FILTER_CTRL_T stFltCtrl;
    AX_BOOL bInstant;

    if (!response->unpack(response->get_api(), &stSrc, &stDst, &stFltCtrl, &bInstant)) {
        return false;
    }

    AX_IVE_HANDLE IveHandle;
    AX_S32 ret = ::AX_IVE_Filter(&IveHandle, &stSrc, &stDst, &stFltCtrl, bInstant);

    if (!request->pack(response->get_api(), ret, IveHandle)) {
        return false;
    }

    return true;
}

NATIVE_IVE_API_IMPL(AX_IVE_Hist) {
    AX_IVE_SRC_IMAGE_T stSrc;
    AX_IVE_DST_MEM_INFO_T stDst;
    AX_BOOL bInstant;

    if (!response->unpack(response->get_api(), &stSrc, &stDst, &bInstant)) {
        return false;
    }

    AX_IVE_HANDLE IveHandle;
    AX_S32 ret = ::AX_IVE_Hist(&IveHandle, &stSrc, &stDst, bInstant);

    if (!request->pack(response->get_api(), ret, IveHandle)) {
        return false;
    }

    return true;
}

NATIVE_IVE_API_IMPL(AX_IVE_EqualizeHist) {
    AX_IVE_SRC_IMAGE_T stSrc;
    AX_IVE_DST_MEM_INFO_T stDst;
    AX_IVE_EQUALIZE_HIST_CTRL_T stEqualizeHistCtrl;
    AX_BOOL bInstant;

    if (!response->unpack(response->get_api(), &stSrc, &stDst, &stEqualizeHistCtrl, &bInstant)) {
        return false;
    }

    AX_IVE_HANDLE IveHandle;
    AX_S32 ret = ::AX_IVE_EqualizeHist(&IveHandle, &stSrc, &stDst, &stEqualizeHistCtrl, bInstant);

    if (!request->pack(response->get_api(), ret, IveHandle)) {
        return false;
    }

    return true;
}

NATIVE_IVE_API_IMPL(AX_IVE_Integ) {
    AX_IVE_SRC_IMAGE_T stSrc;
    AX_IVE_DST_IMAGE_T stDst;
    AX_IVE_INTEG_CTRL_T stIntegCtrl;
    AX_BOOL bInstant;

    if (!response->unpack(response->get_api(), &stSrc, &stDst, &stIntegCtrl, &bInstant)) {
        return false;
    }

    AX_IVE_HANDLE IveHandle;
    AX_S32 ret = ::AX_IVE_Integ(&IveHandle, &stSrc, &stDst, &stIntegCtrl, bInstant);

    if (!request->pack(response->get_api(), ret, IveHandle)) {
        return false;
    }

    return true;
}

NATIVE_IVE_API_IMPL(AX_IVE_MagAndAng) {
    AX_IVE_SRC_IMAGE_T stSrc1;
    AX_IVE_SRC_IMAGE_T stSrc2;
    AX_IVE_DST_IMAGE_T stDstMag;
    AX_IVE_DST_IMAGE_T stDstAng;
    AX_BOOL bInstant;

    if (!response->unpack(response->get_api(), &stSrc1, &stSrc2, &stDstMag, &stDstAng, &bInstant)) {
        return false;
    }

    AX_IVE_HANDLE IveHandle;
    AX_S32 ret = ::AX_IVE_MagAndAng(&IveHandle, &stSrc1, &stSrc2, &stDstMag, &stDstAng, bInstant);

    if (!request->pack(response->get_api(), ret, IveHandle)) {
        return false;
    }

    return true;
}

NATIVE_IVE_API_IMPL(AX_IVE_Sobel) {
    AX_IVE_SRC_IMAGE_T stSrc;
    AX_IVE_DST_IMAGE_T stDst;
    AX_IVE_SOBEL_CTRL_T stSobelCtrl;
    AX_BOOL bInstant;

    if (!response->unpack(response->get_api(), &stSrc, &stDst, &stSobelCtrl, &bInstant)) {
        return false;
    }

    AX_IVE_HANDLE IveHandle;
    AX_S32 ret = ::AX_IVE_Sobel(&IveHandle, &stSrc, &stDst, &stSobelCtrl, bInstant);

    if (!request->pack(response->get_api(), ret, IveHandle)) {
        return false;
    }

    return true;
}

NATIVE_IVE_API_IMPL(AX_IVE_GMM) {
    AX_IVE_SRC_IMAGE_T stSrc;
    AX_IVE_DST_IMAGE_T stFg;
    AX_IVE_DST_IMAGE_T stBg;
    AX_IVE_MEM_INFO_T stModel;
    AX_IVE_GMM_CTRL_T stGmmCtrl;
    AX_BOOL bInstant;

    if (!response->unpack(response->get_api(), &stSrc, &stFg, &stBg, &stModel, &stGmmCtrl, &bInstant)) {
        return false;
    }

    AX_IVE_HANDLE IveHandle;
    AX_S32 ret = ::AX_IVE_GMM(&IveHandle, &stSrc, &stFg, &stBg, &stModel, &stGmmCtrl, bInstant);

    if (!request->pack(response->get_api(), ret, IveHandle)) {
        return false;
    }

    return true;
}

NATIVE_IVE_API_IMPL(AX_IVE_GMM2) {
    AX_IVE_SRC_IMAGE_T stSrc;
    AX_IVE_DST_IMAGE_T stFg;
    AX_IVE_DST_IMAGE_T stBg;
    AX_IVE_MEM_INFO_T stModel;
    AX_IVE_GMM2_CTRL_T stGmm2Ctrl;
    AX_BOOL bInstant;

    if (!response->unpack(response->get_api(), &stSrc, &stFg, &stBg, &stModel, &stGmm2Ctrl, &bInstant)) {
        return false;
    }

    AX_IVE_HANDLE IveHandle;
    AX_S32 ret = ::AX_IVE_GMM2(&IveHandle, &stSrc, &stFg, &stBg, &stModel, &stGmm2Ctrl, bInstant);

    if (!request->pack(response->get_api(), ret, IveHandle)) {
        return false;
    }

    return true;
}

NATIVE_IVE_API_IMPL(AX_IVE_Thresh) {
    AX_IVE_SRC_IMAGE_T stSrc;
    AX_IVE_DST_IMAGE_T stDst;
    AX_IVE_THRESH_CTRL_T stThrCtrl;
    AX_BOOL bInstant;

    if (!response->unpack(response->get_api(), &stSrc, &stDst, &stThrCtrl, &bInstant)) {
        return false;
    }

    AX_IVE_HANDLE IveHandle;
    AX_S32 ret = ::AX_IVE_Thresh(&IveHandle, &stSrc, &stDst, &stThrCtrl, bInstant);

    if (!request->pack(response->get_api(), ret, IveHandle)) {
        return false;
    }

    return true;
}

NATIVE_IVE_API_IMPL(AX_IVE_16BitTo8Bit) {
    AX_IVE_SRC_IMAGE_T stSrc;
    AX_IVE_DST_IMAGE_T stDst;
    AX_IVE_16BIT_TO_8BIT_CTRL_T st16BitTo8BitCtrl;
    AX_BOOL bInstant;

    if (!response->unpack(response->get_api(), &stSrc, &stDst, &st16BitTo8BitCtrl, &bInstant)) {
        return false;
    }

    AX_IVE_HANDLE IveHandle;
    AX_S32 ret = ::AX_IVE_16BitTo8Bit(&IveHandle, &stSrc, &stDst, &st16BitTo8BitCtrl, bInstant);

    if (!request->pack(response->get_api(), ret, IveHandle)) {
        return false;
    }

    return true;
}

NATIVE_IVE_API_IMPL(AX_IVE_CropImage) {
    AX_IVE_SRC_IMAGE_T stSrc;
    AX_IVE_CROP_IMAGE_CTRL_T stCropImageCtrl;
    AX_IVE_ENGINE_E enEngine;
    AX_BOOL bInstant;
    axcl::pkg::uint8_array adst;
    axcl::pkg::uint8_array abox;

    if (!response->unpack(response->get_api(), &stSrc, &adst, &abox, &stCropImageCtrl, &enEngine, &bInstant)) {
        return false;
    }

    AX_IVE_HANDLE IveHandle;
    auto spastDst = unpackArray2PArray<AX_IVE_DST_IMAGE_T>(adst);
    auto spastSrcBoxs = unpackArray2PArray<AX_IVE_RECT_U16_T>(abox);

    if (!spastDst || !spastSrcBoxs) {
        return false;
    }

    AX_S32 ret = ::AX_IVE_CropImage(&IveHandle, &stSrc, spastDst.get(), spastSrcBoxs.get(), &stCropImageCtrl, enEngine, bInstant);

    if (!request->pack(response->get_api(), ret, IveHandle)) {
        return false;
    }

    return true;
}

NATIVE_IVE_API_IMPL(AX_IVE_CropResize) {
    AX_IVE_SRC_IMAGE_T stSrc;
    AX_IVE_CROP_RESIZE_CTRL_T stCropResizeCtrl;
    AX_IVE_ENGINE_E enEngine;
    AX_BOOL bInstant;
    axcl::pkg::uint8_array adst;
    axcl::pkg::uint8_array abox;

    if (!response->unpack(response->get_api(), &stSrc, &adst, &abox, &stCropResizeCtrl, &enEngine, &bInstant)) {
        return false;
    }

    AX_IVE_HANDLE IveHandle;
    auto spastDst = unpackArray2PArray<AX_IVE_DST_IMAGE_T>(adst);
    auto spastSrcBoxs = unpackArray2PArray<AX_IVE_RECT_U16_T>(abox);

    if (!spastDst || !spastSrcBoxs) {
        return false;
    }

    AX_S32 ret = ::AX_IVE_CropResize(&IveHandle, &stSrc, spastDst.get(), spastSrcBoxs.get(), &stCropResizeCtrl, enEngine, bInstant);

    if (!request->pack(response->get_api(), ret, IveHandle)) {
        return false;
    }

    return true;
}

NATIVE_IVE_API_IMPL(AX_IVE_CropResizeForSplitYUV) {
    AX_IVE_SRC_IMAGE_T stSrc1;
    AX_IVE_SRC_IMAGE_T stSrc2;
    AX_IVE_CROP_RESIZE_CTRL_T stCropResizeCtrl;
    AX_IVE_ENGINE_E enEngine;
    AX_BOOL bInstant;
    axcl::pkg::uint8_array adst1;
    axcl::pkg::uint8_array adst2;
    axcl::pkg::uint8_array abox;

    if (!response->unpack(response->get_api(), &stSrc1, &stSrc2, &adst1, &adst2, &abox, &stCropResizeCtrl, &enEngine, &bInstant)) {
        return false;
    }

    AX_IVE_HANDLE IveHandle;
    auto spastDst1 = unpackArray2PArray<AX_IVE_DST_IMAGE_T>(adst1);
    auto spastDst2 = unpackArray2PArray<AX_IVE_DST_IMAGE_T>(adst2);
    auto spastSrcBoxs = unpackArray2PArray<AX_IVE_RECT_U16_T>(abox);

    if (!spastDst1 || !spastDst2 || !spastSrcBoxs) {
        return false;
    }

    AX_S32 ret = ::AX_IVE_CropResizeForSplitYUV(&IveHandle, &stSrc1, &stSrc2, spastDst1.get(), spastDst2.get(), spastSrcBoxs.get(), &stCropResizeCtrl, enEngine, bInstant);

    if (!request->pack(response->get_api(), ret, IveHandle)) {
        return false;
    }

    return true;
}

NATIVE_IVE_API_IMPL(AX_IVE_CSC) {
    AX_IVE_SRC_IMAGE_T stSrc;
    AX_IVE_DST_IMAGE_T stDst;
    AX_IVE_ENGINE_E enEngine;
    AX_BOOL bInstant;

    if (!response->unpack(response->get_api(), &stSrc, &stDst, &enEngine, &bInstant)) {
        return false;
    }

    AX_IVE_HANDLE IveHandle;
    AX_S32 ret = ::AX_IVE_CSC(&IveHandle, &stSrc, &stDst, enEngine, bInstant);

    if (!request->pack(response->get_api(), ret, IveHandle)) {
        return false;
    }

    return true;
}

NATIVE_IVE_API_IMPL(AX_IVE_CropResize2) {
    AX_IVE_SRC_IMAGE_T stSrc;
    AX_IVE_CROP_IMAGE_CTRL_T stCropResize2Ctrl;
    AX_IVE_ENGINE_E enEngine;
    AX_BOOL bInstant;
    axcl::pkg::uint8_array adst;
    axcl::pkg::uint8_array astSrcBoxs;
    axcl::pkg::uint8_array astDstBoxs;

    if (!response->unpack(response->get_api(), &stSrc, &adst, &astSrcBoxs, &astDstBoxs, &stCropResize2Ctrl, &enEngine, &bInstant)) {
        return false;
    }

    AX_IVE_HANDLE IveHandle;
    auto spastDst = unpackArray2PArray<AX_IVE_DST_IMAGE_T>(adst);
    auto spastSrcBoxs = unpackArray2PArray<AX_IVE_RECT_U16_T>(astSrcBoxs);
    auto spastDstBoxs = unpackArray2PArray<AX_IVE_RECT_U16_T>(astDstBoxs);

    if (!spastDst || !spastSrcBoxs || !spastDstBoxs) {
        return false;
    }

    AX_S32 ret = ::AX_IVE_CropResize2(&IveHandle, &stSrc, spastDst.get(), spastSrcBoxs.get(), spastDstBoxs.get(), &stCropResize2Ctrl, enEngine, bInstant);

    if (!request->pack(response->get_api(), ret, IveHandle)) {
        return false;
    }

    return true;
}

NATIVE_IVE_API_IMPL(AX_IVE_CropResize2ForSplitYUV) {
    AX_IVE_SRC_IMAGE_T stSrc1;
    AX_IVE_SRC_IMAGE_T stSrc2;
    AX_IVE_CROP_IMAGE_CTRL_T stCropResize2Ctrl;
    AX_IVE_ENGINE_E enEngine;
    AX_BOOL bInstant;
    axcl::pkg::uint8_array adst1;
    axcl::pkg::uint8_array adst2;
    axcl::pkg::uint8_array astSrcBoxs;
    axcl::pkg::uint8_array astDstBoxs;

    if (!response->unpack(response->get_api(), &stSrc1, &stSrc2, &adst1, &adst2, &astSrcBoxs, &astDstBoxs, &stCropResize2Ctrl, &enEngine, &bInstant)) {
        return false;
    }

    AX_IVE_HANDLE IveHandle;
    auto spastDst1 = unpackArray2PArray<AX_IVE_DST_IMAGE_T>(adst1);
    auto spastDst2 = unpackArray2PArray<AX_IVE_DST_IMAGE_T>(adst2);
    auto spastSrcBoxs = unpackArray2PArray<AX_IVE_RECT_U16_T>(astSrcBoxs);
    auto spastDstBoxs = unpackArray2PArray<AX_IVE_RECT_U16_T>(astDstBoxs);

    if (!spastDst1 || !spastDst2 || !spastSrcBoxs || !spastDstBoxs) {
        return false;
    }

    AX_S32 ret = ::AX_IVE_CropResize2ForSplitYUV(&IveHandle, &stSrc1, &stSrc2, spastDst1.get(), spastDst2.get(), spastSrcBoxs.get(), spastDstBoxs.get(), &stCropResize2Ctrl, enEngine, bInstant);

    if (!request->pack(response->get_api(), ret, IveHandle)) {
        return false;
    }

    return true;
}

NATIVE_IVE_API_IMPL(AX_IVE_MAU_MatMul) {
    AX_IVE_MAU_MATMUL_INPUT_T stSrc;
    AX_IVE_MAU_MATMUL_OUTPUT_T stDst;
    AX_IVE_MAU_MATMUL_CTRL_T stMatMulCtrl;
    AX_IVE_ENGINE_E enEngine;
    AX_BOOL bInstant;
    axcl::pkg::uint8_array mat_q;
    axcl::pkg::uint8_array mat_b;
    axcl::pkg::uint8_array mat_mul;
    axcl::pkg::uint8_array mat_top;

    if (!response->unpack(response->get_api(), &stSrc, &stDst, &stMatMulCtrl, &enEngine, &bInstant, &mat_q, &mat_b, &mat_mul, &mat_top)) {
        return false;
    }

    AX_IVE_MAU_MATMUL_OUTPUT_T stDstRet = stDst;

    stSrc.stMatQ.pShape = (AX_S32 *)mat_q.data;
    stSrc.stMatQ.u8ShapeSize = mat_q.size / sizeof(AX_S32);

    stSrc.stMatB.pShape = (AX_S32 *)mat_b.data;
    stSrc.stMatB.u8ShapeSize = mat_b.size / sizeof(AX_S32);

    stDst.stMulRes.pShape = (AX_S32 *)mat_mul.data;
    stDst.stMulRes.u8ShapeSize = mat_mul.size / sizeof(AX_S32);

    stDst.stTopNRes.pShape = (AX_S32 *)mat_top.data;
    stDst.stTopNRes.u8ShapeSize = mat_top.size / sizeof(AX_S32);

    AX_IVE_HANDLE IveHandle;
    AX_S32 ret = ::AX_IVE_MAU_MatMul(&IveHandle, &stSrc, &stDst, &stMatMulCtrl, enEngine, bInstant);

    axcl::pkg::uint8_array mat_mul_ret;
    mat_mul_ret.data = reinterpret_cast<AX_U8 *>(stDst.stMulRes.pShape);
    mat_mul_ret.size = stDst.stMulRes.u8ShapeSize * sizeof(AX_S32);
    axcl::pkg::uint8_array mat_top_ret;
    mat_top_ret.data = reinterpret_cast<AX_U8 *>(stDst.stTopNRes.pShape);
    mat_top_ret.size = stDst.stTopNRes.u8ShapeSize * sizeof(AX_S32);

    if (!request->pack(response->get_api(), ret, IveHandle, &stDstRet, mat_mul_ret, mat_top_ret)) {
        return false;
    }

    return true;
}

NATIVE_IVE_API_IMPL(AX_IVE_NPU_CreateMatMulHandle) {
    AX_IVE_NPU_MATMUL_CTRL_T stMatMulCtrl;
    axcl::pkg::uint8_array model;

    if (!response->unpack(response->get_api(), &stMatMulCtrl, &model)) {
        return false;
    }

    AX_IVE_MATMUL_HANDLE pHandle;

    stMatMulCtrl.pchModelDir = (AX_CHAR *)model.data;

    AX_S32 ret = ::AX_IVE_NPU_CreateMatMulHandle(&pHandle, &stMatMulCtrl);

    AX_U64 handle = reinterpret_cast<AX_U64>(pHandle);

    if (!request->pack(response->get_api(), ret, handle)) {
        return false;
    }

    return true;
}

NATIVE_IVE_API_IMPL(AX_IVE_NPU_DestroyMatMulHandle) {
    AX_U64 handle;

    if (!response->unpack(response->get_api(), &handle)) {
        return false;
    }

    AX_IVE_MATMUL_HANDLE pHandle = reinterpret_cast<AX_IVE_MATMUL_HANDLE>(handle);

    AX_S32 ret = ::AX_IVE_NPU_DestroyMatMulHandle(&pHandle);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_IVE_API_IMPL(AX_IVE_NPU_MatMul) {
    AX_U64 handle;
    AX_IVE_MAU_MATMUL_INPUT_T stSrc;
    AX_IVE_MAU_MATMUL_OUTPUT_T stDst;
    AX_IVE_ENGINE_E enEngine;
    AX_BOOL bInstant;
    axcl::pkg::uint8_array mat_q;
    axcl::pkg::uint8_array mat_b;

    if (!response->unpack(response->get_api(), &handle, &stSrc, &stDst, &enEngine, &bInstant, &mat_q, &mat_b)) {
        return false;
    }

    stSrc.stMatQ.pShape = reinterpret_cast<AX_S32 *>(mat_q.data);
    stSrc.stMatQ.u8ShapeSize = mat_q.size / sizeof(AX_S32);

    stSrc.stMatB.pShape = reinterpret_cast<AX_S32 *>(mat_b.data);
    stSrc.stMatB.u8ShapeSize = mat_b.size / sizeof(AX_S32);

    AX_IVE_MATMUL_HANDLE pHandle = reinterpret_cast<AX_IVE_MATMUL_HANDLE>(handle);

    AX_S32 ret = ::AX_IVE_NPU_MatMul(pHandle, &stSrc, &stDst, enEngine, bInstant);

    axcl::pkg::uint8_array mat_mul;
    axcl::pkg::uint8_array mat_top;

    mat_mul.data = reinterpret_cast<AX_U8 *>(stDst.stMulRes.pShape);
    mat_mul.size = stDst.stMulRes.u8ShapeSize * sizeof(AX_S32);

    mat_top.data = reinterpret_cast<AX_U8 *>(stDst.stTopNRes.pShape);
    mat_top.size = stDst.stMulRes.u8ShapeSize * sizeof(AX_S32);

    if (!request->pack(response->get_api(), ret, &stDst, &mat_mul, &mat_top)) {
        return false;
    }

    return true;
}

}  // namespace axcl::worker
