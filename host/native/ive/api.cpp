/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "axcl_base.h"
#include "axcl_ive.h"
#include "axcl_module_version.h"
#include "ive.hpp"
#include "axcl_logger.hpp"

#define TAG "ive"
#define CHECK_NULL_POINTER(p)                           \
    if (!(p)) {                                         \
        LOG_MM_E(TAG, "nil pointer");                   \
        return AXCL_DEF_IVE_ERR(AXCL_ERR_NULL_POINTER); \
    }

AXCL_EXPORT AX_S32 AXCL_IVE_Init(AX_VOID) {
    ive api;
    return api.AX_IVE_Init();
}

AXCL_EXPORT AX_VOID AXCL_IVE_Exit(AX_VOID) {
    ive api;
    api.AX_IVE_Exit();
}

AXCL_EXPORT AX_S32 AXCL_IVE_Query(AX_IVE_HANDLE IveHandle, AX_BOOL *pbFinish, AX_BOOL bBlock) {
    CHECK_NULL_POINTER(IveHandle)
    CHECK_NULL_POINTER(pbFinish)

    ive api;
    return api.AX_IVE_Query(IveHandle, pbFinish, bBlock);
}

AXCL_EXPORT AX_S32 AXCL_IVE_DMA(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_DATA_T *pstSrc, AX_IVE_DST_DATA_T *pstDst,
                                AX_IVE_DMA_CTRL_T *pstDmaCtrl, AX_BOOL bInstant) {
    CHECK_NULL_POINTER(pIveHandle)
    CHECK_NULL_POINTER(pstSrc)
    CHECK_NULL_POINTER(pstDst)
    CHECK_NULL_POINTER(pstDmaCtrl)

    ive api;
    return api.AX_IVE_DMA(pIveHandle, pstSrc, pstDst, pstDmaCtrl, bInstant);
}

AXCL_EXPORT AX_S32 AXCL_IVE_Add(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc1, AX_IVE_SRC_IMAGE_T *pstSrc2,
                                AX_IVE_DST_IMAGE_T *pstDst, AX_IVE_ADD_CTRL_T *pstAddCtrl, AX_BOOL bInstant) {
    CHECK_NULL_POINTER(pIveHandle)
    CHECK_NULL_POINTER(pstSrc1)
    CHECK_NULL_POINTER(pstSrc2)
    CHECK_NULL_POINTER(pstDst)
    CHECK_NULL_POINTER(pstAddCtrl)
    ive api;
    return api.AX_IVE_Add(pIveHandle, pstSrc1, pstSrc2, pstDst, pstAddCtrl, bInstant);
}

AXCL_EXPORT AX_S32 AXCL_IVE_Sub(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc1, AX_IVE_SRC_IMAGE_T *pstSrc2,
                                AX_IVE_DST_IMAGE_T *pstDst, AX_IVE_SUB_CTRL_T *pstSubCtrl, AX_BOOL bInstant) {
    CHECK_NULL_POINTER(pIveHandle)
    CHECK_NULL_POINTER(pstSrc1)
    CHECK_NULL_POINTER(pstSrc2)
    CHECK_NULL_POINTER(pstDst)
    CHECK_NULL_POINTER(pstSubCtrl)

    ive api;
    return api.AX_IVE_Sub(pIveHandle, pstSrc1, pstSrc2, pstDst, pstSubCtrl, bInstant);
}

AXCL_EXPORT AX_S32 AXCL_IVE_And(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc1, AX_IVE_SRC_IMAGE_T *pstSrc2,
                                AX_IVE_DST_IMAGE_T *pstDst, AX_BOOL bInstant) {
    CHECK_NULL_POINTER(pIveHandle)
    CHECK_NULL_POINTER(pstSrc1)
    CHECK_NULL_POINTER(pstSrc2)
    CHECK_NULL_POINTER(pstDst)

    ive api;
    return api.AX_IVE_And(pIveHandle, pstSrc1, pstSrc2, pstDst, bInstant);
}

AXCL_EXPORT AX_S32 AXCL_IVE_Or(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc1, AX_IVE_SRC_IMAGE_T *pstSrc2,
                               AX_IVE_DST_IMAGE_T *pstDst, AX_BOOL bInstant) {
    CHECK_NULL_POINTER(pIveHandle)
    CHECK_NULL_POINTER(pstSrc1)
    CHECK_NULL_POINTER(pstSrc2)
    CHECK_NULL_POINTER(pstDst)

    ive api;
    return api.AX_IVE_Or(pIveHandle, pstSrc1, pstSrc2, pstDst, bInstant);
}

AXCL_EXPORT AX_S32 AXCL_IVE_Xor(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc1, AX_IVE_SRC_IMAGE_T *pstSrc2,
                                AX_IVE_DST_IMAGE_T *pstDst, AX_BOOL bInstant) {
    CHECK_NULL_POINTER(pIveHandle)
    CHECK_NULL_POINTER(pstSrc1)
    CHECK_NULL_POINTER(pstSrc2)
    CHECK_NULL_POINTER(pstDst)

    ive api;
    return api.AX_IVE_Xor(pIveHandle, pstSrc1, pstSrc2, pstDst, bInstant);
}

AXCL_EXPORT AX_S32 AXCL_IVE_Mse(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc1, AX_IVE_SRC_IMAGE_T *pstSrc2,
                                AX_IVE_DST_IMAGE_T *pstDst, AX_IVE_MSE_CTRL_T *pstMseCtrl, AX_BOOL bInstant) {
    CHECK_NULL_POINTER(pIveHandle)
    CHECK_NULL_POINTER(pstSrc1)
    CHECK_NULL_POINTER(pstSrc2)
    CHECK_NULL_POINTER(pstDst)
    CHECK_NULL_POINTER(pstMseCtrl)

    ive api;
    return api.AX_IVE_Mse(pIveHandle, pstSrc1, pstSrc2, pstDst, pstMseCtrl, bInstant);
}

AXCL_EXPORT AX_S32 AXCL_IVE_CannyHysEdge(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc1, AX_IVE_SRC_IMAGE_T *pstSrc2,
                                         AX_IVE_DST_IMAGE_T *pstDst, AX_IVE_HYS_EDGE_CTRL_T *pstCannyHysEdgeCtrl, AX_BOOL bInstant) {
    CHECK_NULL_POINTER(pIveHandle)
    CHECK_NULL_POINTER(pstSrc1)
    CHECK_NULL_POINTER(pstSrc2)
    CHECK_NULL_POINTER(pstDst)
    CHECK_NULL_POINTER(pstCannyHysEdgeCtrl)

    ive api;
    return api.AX_IVE_CannyHysEdge(pIveHandle, pstSrc1, pstSrc2, pstDst, pstCannyHysEdgeCtrl, bInstant);
}

AXCL_EXPORT AX_S32 AXCL_IVE_CannyEdge(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pstDst,
                                      AX_IVE_CANNY_EDGE_CTRL_T *pstCannyEdgeCtrl, AX_BOOL bInstant) {
    CHECK_NULL_POINTER(pIveHandle)
    CHECK_NULL_POINTER(pstSrc)
    CHECK_NULL_POINTER(pstDst)
    CHECK_NULL_POINTER(pstCannyEdgeCtrl)

    ive api;
    return api.AX_IVE_CannyEdge(pIveHandle, pstSrc, pstDst, pstCannyEdgeCtrl, bInstant);
}

AXCL_EXPORT AX_S32 AXCL_IVE_CCL(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pstDst,
                                AX_IVE_DST_MEM_INFO_T *pstBlob, AX_IVE_CCL_CTRL_T *pstCclCtrl, AX_BOOL bInstant) {
    CHECK_NULL_POINTER(pIveHandle)
    CHECK_NULL_POINTER(pstSrc)
    CHECK_NULL_POINTER(pstDst)
    CHECK_NULL_POINTER(pstBlob)
    CHECK_NULL_POINTER(pstCclCtrl)

    ive api;
    return api.AX_IVE_CCL(pIveHandle, pstSrc, pstDst, pstBlob, pstCclCtrl, bInstant);
}

AXCL_EXPORT AX_S32 AXCL_IVE_Erode(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pstDst,
                                  AX_IVE_ERODE_CTRL_T *pstErodeCtrl, AX_BOOL bInstant) {
    CHECK_NULL_POINTER(pIveHandle)
    CHECK_NULL_POINTER(pstSrc)
    CHECK_NULL_POINTER(pstDst)
    CHECK_NULL_POINTER(pstErodeCtrl)

    ive api;
    return api.AX_IVE_Erode(pIveHandle, pstSrc, pstDst, pstErodeCtrl, bInstant);
}

AXCL_EXPORT AX_S32 AXCL_IVE_Dilate(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pstDst,
                                   AX_IVE_DILATE_CTRL_T *pstDilateCtrl, AX_BOOL bInstant) {
    CHECK_NULL_POINTER(pIveHandle)
    CHECK_NULL_POINTER(pstSrc)
    CHECK_NULL_POINTER(pstDst)
    CHECK_NULL_POINTER(pstDilateCtrl)

    ive api;
    return api.AX_IVE_Dilate(pIveHandle, pstSrc, pstDst, pstDilateCtrl, bInstant);
}

AXCL_EXPORT AX_S32 AXCL_IVE_Filter(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pstDst,
                                   AX_IVE_FILTER_CTRL_T *pstFltCtrl, AX_BOOL bInstant) {
    CHECK_NULL_POINTER(pIveHandle)
    CHECK_NULL_POINTER(pstSrc)
    CHECK_NULL_POINTER(pstDst)
    CHECK_NULL_POINTER(pstFltCtrl)

    ive api;
    return api.AX_IVE_Filter(pIveHandle, pstSrc, pstDst, pstFltCtrl, bInstant);
}

AXCL_EXPORT AX_S32 AXCL_IVE_Hist(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_MEM_INFO_T *pstDst, AX_BOOL bInstant) {
    CHECK_NULL_POINTER(pIveHandle)
    CHECK_NULL_POINTER(pstSrc)
    CHECK_NULL_POINTER(pstDst)

    ive api;
    return api.AX_IVE_Hist(pIveHandle, pstSrc, pstDst, bInstant);
}

AXCL_EXPORT AX_S32 AXCL_IVE_EqualizeHist(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_MEM_INFO_T *pstDst,
                                         AX_IVE_EQUALIZE_HIST_CTRL_T *pstEqualizeHistCtrl, AX_BOOL bInstant) {
    CHECK_NULL_POINTER(pIveHandle)
    CHECK_NULL_POINTER(pstSrc)
    CHECK_NULL_POINTER(pstDst)
    CHECK_NULL_POINTER(pstEqualizeHistCtrl)

    ive api;
    return api.AX_IVE_EqualizeHist(pIveHandle, pstSrc, pstDst, pstEqualizeHistCtrl, bInstant);
}

AXCL_EXPORT AX_S32 AXCL_IVE_Integ(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pstDst,
                                  AX_IVE_INTEG_CTRL_T *pstIntegCtrl, AX_BOOL bInstant) {
    CHECK_NULL_POINTER(pIveHandle)
    CHECK_NULL_POINTER(pstSrc)
    CHECK_NULL_POINTER(pstDst)
    CHECK_NULL_POINTER(pstIntegCtrl)

    ive api;
    return api.AX_IVE_Integ(pIveHandle, pstSrc, pstDst, pstIntegCtrl, bInstant);
}

AXCL_EXPORT AX_S32 AXCL_IVE_MagAndAng(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc1, AX_IVE_SRC_IMAGE_T *pstSrc2,
                                      AX_IVE_DST_IMAGE_T *pstDstMag, AX_IVE_DST_IMAGE_T *pstDstAng, AX_BOOL bInstant) {
    CHECK_NULL_POINTER(pIveHandle)
    CHECK_NULL_POINTER(pstSrc1)
    CHECK_NULL_POINTER(pstSrc2)
    CHECK_NULL_POINTER(pstDstMag)
    CHECK_NULL_POINTER(pstDstAng)

    ive api;
    return api.AX_IVE_MagAndAng(pIveHandle, pstSrc1, pstSrc2, pstDstMag, pstDstAng, bInstant);
}

AXCL_EXPORT AX_S32 AXCL_IVE_Sobel(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pstDst,
                                  AX_IVE_SOBEL_CTRL_T *pstSobelCtrl, AX_BOOL bInstant) {
    CHECK_NULL_POINTER(pIveHandle)
    CHECK_NULL_POINTER(pstSrc)
    CHECK_NULL_POINTER(pstDst)
    CHECK_NULL_POINTER(pstSobelCtrl)

    ive api;
    return api.AX_IVE_Sobel(pIveHandle, pstSrc, pstDst, pstSobelCtrl, bInstant);
}

AXCL_EXPORT AX_S32 AXCL_IVE_GMM(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pstFg, AX_IVE_DST_IMAGE_T *pstBg,
                                AX_IVE_MEM_INFO_T *pstModel, AX_IVE_GMM_CTRL_T *pstGmmCtrl, AX_BOOL bInstant) {
    CHECK_NULL_POINTER(pIveHandle)
    CHECK_NULL_POINTER(pstSrc)
    CHECK_NULL_POINTER(pstFg)
    CHECK_NULL_POINTER(pstBg)
    CHECK_NULL_POINTER(pstModel)
    CHECK_NULL_POINTER(pstGmmCtrl)

    ive api;
    return api.AX_IVE_GMM(pIveHandle, pstSrc, pstFg, pstBg, pstModel, pstGmmCtrl, bInstant);
}

AXCL_EXPORT AX_S32 AXCL_IVE_GMM2(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pstFg,
                                 AX_IVE_DST_IMAGE_T *pstBg, AX_IVE_MEM_INFO_T *pstModel, AX_IVE_GMM2_CTRL_T *pstGmm2Ctrl,
                                 AX_BOOL bInstant) {
    CHECK_NULL_POINTER(pIveHandle)
    CHECK_NULL_POINTER(pstSrc)
    CHECK_NULL_POINTER(pstFg)
    CHECK_NULL_POINTER(pstBg)
    CHECK_NULL_POINTER(pstModel)
    CHECK_NULL_POINTER(pstGmm2Ctrl)

    ive api;
    return api.AX_IVE_GMM2(pIveHandle, pstSrc, pstFg, pstBg, pstModel, pstGmm2Ctrl, bInstant);
}

AXCL_EXPORT AX_S32 AXCL_IVE_Thresh(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pstDst,
                                   AX_IVE_THRESH_CTRL_T *pstThrCtrl, AX_BOOL bInstant) {
    CHECK_NULL_POINTER(pIveHandle)
    CHECK_NULL_POINTER(pstSrc)
    CHECK_NULL_POINTER(pstDst)
    CHECK_NULL_POINTER(pstThrCtrl)

    ive api;
    return api.AX_IVE_Thresh(pIveHandle, pstSrc, pstDst, pstThrCtrl, bInstant);
}

AXCL_EXPORT AX_S32 AXCL_IVE_16BitTo8Bit(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pstDst,
                                        AX_IVE_16BIT_TO_8BIT_CTRL_T *pst16BitTo8BitCtrl, AX_BOOL bInstant) {
    CHECK_NULL_POINTER(pIveHandle)
    CHECK_NULL_POINTER(pstSrc)
    CHECK_NULL_POINTER(pstDst)
    CHECK_NULL_POINTER(pst16BitTo8BitCtrl)

    ive api;
    return api.AX_IVE_16BitTo8Bit(pIveHandle, pstSrc, pstDst, pst16BitTo8BitCtrl, bInstant);
}

AXCL_EXPORT AX_S32 AXCL_IVE_CropImage(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pastDst[],
                                      AX_IVE_RECT_U16_T *pastSrcBoxs[], AX_IVE_CROP_IMAGE_CTRL_T *pstCropImageCtrl,
                                      AX_IVE_ENGINE_E enEngine, AX_BOOL bInstant) {
    CHECK_NULL_POINTER(pIveHandle)
    CHECK_NULL_POINTER(pstSrc)
    CHECK_NULL_POINTER(pastDst)
    CHECK_NULL_POINTER(pastSrcBoxs)
    CHECK_NULL_POINTER(pstCropImageCtrl)

    ive api;
    return api.AX_IVE_CropImage(pIveHandle, pstSrc, pastDst, pastSrcBoxs, pstCropImageCtrl, enEngine, bInstant);
}

AXCL_EXPORT AX_S32 AXCL_IVE_CropResize(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pastDst[],
                                       AX_IVE_RECT_U16_T *pastSrcBoxs[], AX_IVE_CROP_RESIZE_CTRL_T *pstCropResizeCtrl,
                                       AX_IVE_ENGINE_E enEngine, AX_BOOL bInstant) {
    CHECK_NULL_POINTER(pIveHandle)
    CHECK_NULL_POINTER(pstSrc)
    CHECK_NULL_POINTER(pastDst)
    CHECK_NULL_POINTER(pastSrcBoxs)
    CHECK_NULL_POINTER(pstCropResizeCtrl)

    ive api;
    return api.AX_IVE_CropResize(pIveHandle, pstSrc, pastDst, pastSrcBoxs, pstCropResizeCtrl, enEngine, bInstant);
}

AXCL_EXPORT AX_S32 AXCL_IVE_CropResizeForSplitYUV(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc1, AX_IVE_SRC_IMAGE_T *pstSrc2,
                                                  AX_IVE_DST_IMAGE_T *pastDst1[], AX_IVE_DST_IMAGE_T *pastDst2[],
                                                  AX_IVE_RECT_U16_T *pastSrcBoxs[], AX_IVE_CROP_RESIZE_CTRL_T *pstCropResizeCtrl,
                                                  AX_IVE_ENGINE_E enEngine, AX_BOOL bInstant) {
    CHECK_NULL_POINTER(pIveHandle)
    CHECK_NULL_POINTER(pstSrc1)
    CHECK_NULL_POINTER(pstSrc2)
    CHECK_NULL_POINTER(pastDst1)
    CHECK_NULL_POINTER(pastDst2)
    CHECK_NULL_POINTER(pastSrcBoxs)
    CHECK_NULL_POINTER(pstCropResizeCtrl)

    ive api;
    return api.AX_IVE_CropResizeForSplitYUV(pIveHandle, pstSrc1, pstSrc2, pastDst1, pastDst2, pastSrcBoxs, pstCropResizeCtrl, enEngine,
                                            bInstant);
}

AXCL_EXPORT AX_S32 AXCL_IVE_CSC(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pstDst, AX_IVE_ENGINE_E enEngine,
                                AX_BOOL bInstant) {
    CHECK_NULL_POINTER(pIveHandle)
    CHECK_NULL_POINTER(pstSrc)
    CHECK_NULL_POINTER(pstDst)

    ive api;
    return api.AX_IVE_CSC(pIveHandle, pstSrc, pstDst, enEngine, bInstant);
}

AXCL_EXPORT AX_S32 AXCL_IVE_CropResize2(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_IMAGE_T *pastDst[],
                                        AX_IVE_RECT_U16_T *pastSrcBoxs[], AX_IVE_RECT_U16_T *pastDstBoxs[],
                                        AX_IVE_CROP_IMAGE_CTRL_T *pstCropResize2Ctrl, AX_IVE_ENGINE_E enEngine, AX_BOOL bInstant) {
    CHECK_NULL_POINTER(pIveHandle)
    CHECK_NULL_POINTER(pstSrc)
    CHECK_NULL_POINTER(pastDst)
    CHECK_NULL_POINTER(pastSrcBoxs)
    CHECK_NULL_POINTER(pastDstBoxs)
    CHECK_NULL_POINTER(pstCropResize2Ctrl)

    ive api;
    return api.AX_IVE_CropResize2(pIveHandle, pstSrc, pastDst, pastSrcBoxs, pastDstBoxs, pstCropResize2Ctrl, enEngine, bInstant);
}

AXCL_EXPORT AX_S32 AXCL_IVE_CropResize2ForSplitYUV(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc1, AX_IVE_SRC_IMAGE_T *pstSrc2,
                                                   AX_IVE_IMAGE_T *pastDst1[], AX_IVE_IMAGE_T *pastDst2[], AX_IVE_RECT_U16_T *pastSrcBoxs[],
                                                   AX_IVE_RECT_U16_T *pastDstBoxs[], AX_IVE_CROP_IMAGE_CTRL_T *pstCropResize2Ctrl,
                                                   AX_IVE_ENGINE_E enEngine, AX_BOOL bInstant) {
    CHECK_NULL_POINTER(pIveHandle)
    CHECK_NULL_POINTER(pstSrc1)
    CHECK_NULL_POINTER(pstSrc2)
    CHECK_NULL_POINTER(pastDst1)
    CHECK_NULL_POINTER(pastDst2)
    CHECK_NULL_POINTER(pastSrcBoxs)
    CHECK_NULL_POINTER(pastDstBoxs)
    CHECK_NULL_POINTER(pstCropResize2Ctrl)

    ive api;
    return api.AX_IVE_CropResize2ForSplitYUV(pIveHandle, pstSrc1, pstSrc2, pastDst1, pastDst2, pastSrcBoxs, pastDstBoxs, pstCropResize2Ctrl,
                                             enEngine, bInstant);
}

AXCL_EXPORT AX_S32 AXCL_IVE_MAU_MatMul(AX_IVE_HANDLE *pIveHandle, AX_IVE_MAU_MATMUL_INPUT_T *pstSrc, AX_IVE_MAU_MATMUL_OUTPUT_T *pstDst,
                                       AX_IVE_MAU_MATMUL_CTRL_T *pstMatMulCtrl, AX_IVE_ENGINE_E enEngine, AX_BOOL bInstant) {
    CHECK_NULL_POINTER(pIveHandle)
    CHECK_NULL_POINTER(pstSrc)
    CHECK_NULL_POINTER(pstDst)
    CHECK_NULL_POINTER(pstMatMulCtrl)

    ive api;
    return api.AX_IVE_MAU_MatMul(pIveHandle, pstSrc, pstDst, pstMatMulCtrl, enEngine, bInstant);
}

AXCL_EXPORT AX_S32 AXCL_IVE_NPU_CreateMatMulHandle(AX_IVE_MATMUL_HANDLE *pHandle, AX_IVE_NPU_MATMUL_CTRL_T *pstMatMulCtrl) {
    CHECK_NULL_POINTER(pHandle)
    CHECK_NULL_POINTER(pstMatMulCtrl)

    ive api;
    return api.AX_IVE_NPU_CreateMatMulHandle(pHandle, pstMatMulCtrl);
}

AXCL_EXPORT AX_S32 AXCL_IVE_NPU_DestroyMatMulHandle(AX_IVE_MATMUL_HANDLE *pHandle) {
    CHECK_NULL_POINTER(pHandle)

    ive api;
    return api.AX_IVE_NPU_DestroyMatMulHandle(pHandle);
}

AXCL_EXPORT AX_S32 AXCL_IVE_NPU_MatMul(AX_IVE_MATMUL_HANDLE hHandle, AX_IVE_MAU_MATMUL_INPUT_T *pstSrc, AX_IVE_MAU_MATMUL_OUTPUT_T *pstDst,
                                       AX_IVE_ENGINE_E enEngine, AX_BOOL bInstant) {
    CHECK_NULL_POINTER(hHandle)
    CHECK_NULL_POINTER(pstSrc)
    CHECK_NULL_POINTER(pstDst)

    ive api;
    return api.AX_IVE_NPU_MatMul(hHandle, pstSrc, pstDst, enEngine, bInstant);
}
