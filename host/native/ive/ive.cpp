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
#include <stdlib.h>
#include <string.h>
#include <memory>
#include "axcl_logger.hpp"

#define TAG "ive"

AX_S32 ive::AX_IVE_Init() {
    axcl::pkg::native::IVE_API api(axcl::pkg::native::IVE_API::AX_IVE_Init);
    AX_S32 ret;

    if (!m_req.pack(api)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_VOID ive::AX_IVE_Exit() {
    axcl::pkg::native::IVE_API api(axcl::pkg::native::IVE_API::AX_IVE_Exit);
    AX_S32 ret;

    if (!m_req.pack(api)) {
        return;
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return;
    }

    if (!m_rsp.unpack(api)) {
        return;
    }
}

AX_S32 ive::AX_IVE_Query(AX_IVE_HANDLE IveHandle, AX_BOOL *pbFinish, AX_BOOL bBlock) {
    axcl::pkg::native::IVE_API api(axcl::pkg::native::IVE_API::AX_IVE_Query);
    AX_S32 ret;

    if (!m_req.pack(api, IveHandle, bBlock)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pbFinish)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ive::AX_IVE_DMA(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_DATA_T *pstSrc, AX_IVE_DST_DATA_T *pstDst, AX_IVE_DMA_CTRL_T *pstDmaCtrl,
                       AX_BOOL bInstant) {
    axcl::pkg::native::IVE_API api(axcl::pkg::native::IVE_API::AX_IVE_DMA);
    AX_S32 ret;

    if (!m_req.pack(api, pstSrc, pstDst, pstDmaCtrl, bInstant)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pIveHandle)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ive::AX_IVE_Add(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc1, AX_IVE_SRC_IMAGE_T *pstSrc2, AX_IVE_DST_IMAGE_T *pstDst,
                       AX_IVE_ADD_CTRL_T *pstAddCtrl, AX_BOOL bInstant) {
    axcl::pkg::native::IVE_API api(axcl::pkg::native::IVE_API::AX_IVE_Add);
    AX_S32 ret;

    if (!m_req.pack(api, pstSrc1, pstSrc2, pstDst, pstAddCtrl, bInstant)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pIveHandle)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ive::AX_IVE_Sub(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc1, AX_IVE_SRC_IMAGE_T *pstSrc2, AX_IVE_DST_IMAGE_T *pstDst,
                       AX_IVE_SUB_CTRL_T *pstSubCtrl, AX_BOOL bInstant) {
    axcl::pkg::native::IVE_API api(axcl::pkg::native::IVE_API::AX_IVE_Sub);
    AX_S32 ret;

    if (!m_req.pack(api, pstSrc1, pstSrc2, pstDst, pstSubCtrl, bInstant)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pIveHandle)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ive::AX_IVE_And(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc1, AX_IVE_SRC_IMAGE_T *pstSrc2, AX_IVE_DST_IMAGE_T *pstDst,
                       AX_BOOL bInstant) {
    axcl::pkg::native::IVE_API api(axcl::pkg::native::IVE_API::AX_IVE_And);
    AX_S32 ret;

    if (!m_req.pack(api, pstSrc1, pstSrc2, pstDst, bInstant)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pIveHandle)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ive::AX_IVE_Or(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc1, AX_IVE_SRC_IMAGE_T *pstSrc2, AX_IVE_DST_IMAGE_T *pstDst,
                      AX_BOOL bInstant) {
    axcl::pkg::native::IVE_API api(axcl::pkg::native::IVE_API::AX_IVE_Or);
    AX_S32 ret;

    if (!m_req.pack(api, pstSrc1, pstSrc2, pstDst, bInstant)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pIveHandle)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ive::AX_IVE_Xor(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc1, AX_IVE_SRC_IMAGE_T *pstSrc2, AX_IVE_DST_IMAGE_T *pstDst,
                       AX_BOOL bInstant) {
    axcl::pkg::native::IVE_API api(axcl::pkg::native::IVE_API::AX_IVE_Xor);
    AX_S32 ret;

    if (!m_req.pack(api, pstSrc1, pstSrc2, pstDst, bInstant)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pIveHandle)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ive::AX_IVE_Mse(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc1, AX_IVE_SRC_IMAGE_T *pstSrc2, AX_IVE_DST_IMAGE_T *pstDst,
                       AX_IVE_MSE_CTRL_T *pstMseCtrl, AX_BOOL bInstant) {
    axcl::pkg::native::IVE_API api(axcl::pkg::native::IVE_API::AX_IVE_Mse);
    AX_S32 ret;

    if (!m_req.pack(api, pstSrc1, pstSrc2, pstDst, pstMseCtrl, bInstant)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pIveHandle)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ive::AX_IVE_CannyHysEdge(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc1, AX_IVE_SRC_IMAGE_T *pstSrc2,
                                AX_IVE_DST_IMAGE_T *pstDst, AX_IVE_HYS_EDGE_CTRL_T *pstCannyHysEdgeCtrl, AX_BOOL bInstant) {
    axcl::pkg::native::IVE_API api(axcl::pkg::native::IVE_API::AX_IVE_CannyHysEdge);
    AX_S32 ret;

    if (!m_req.pack(api, pstSrc1, pstSrc2, pstDst, pstCannyHysEdgeCtrl, bInstant)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pIveHandle)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ive::AX_IVE_CannyEdge(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pstDst,
                             AX_IVE_CANNY_EDGE_CTRL_T *pstCannyEdgeCtrl, AX_BOOL bInstant) {
    axcl::pkg::native::IVE_API api(axcl::pkg::native::IVE_API::AX_IVE_CannyEdge);
    AX_S32 ret;

    if (!m_req.pack(api, pstSrc, pstDst, pstCannyEdgeCtrl, bInstant)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pIveHandle)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ive::AX_IVE_CCL(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pstDst, AX_IVE_DST_MEM_INFO_T *pstBlob,
                       AX_IVE_CCL_CTRL_T *pstCclCtrl, AX_BOOL bInstant) {
    axcl::pkg::native::IVE_API api(axcl::pkg::native::IVE_API::AX_IVE_CCL);
    AX_S32 ret;

    if (!m_req.pack(api, pstSrc, pstDst, pstBlob, pstCclCtrl, bInstant)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pIveHandle)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ive::AX_IVE_Erode(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pstDst,
                         AX_IVE_ERODE_CTRL_T *pstErodeCtrl, AX_BOOL bInstant) {
    axcl::pkg::native::IVE_API api(axcl::pkg::native::IVE_API::AX_IVE_Erode);
    AX_S32 ret;

    if (!m_req.pack(api, pstSrc, pstDst, pstErodeCtrl, bInstant)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pIveHandle)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ive::AX_IVE_Dilate(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pstDst,
                          AX_IVE_DILATE_CTRL_T *pstDilateCtrl, AX_BOOL bInstant) {
    axcl::pkg::native::IVE_API api(axcl::pkg::native::IVE_API::AX_IVE_Dilate);
    AX_S32 ret;

    if (!m_req.pack(api, pstSrc, pstDst, pstDilateCtrl, bInstant)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pIveHandle)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ive::AX_IVE_Filter(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pstDst,
                          AX_IVE_FILTER_CTRL_T *pstFltCtrl, AX_BOOL bInstant) {
    axcl::pkg::native::IVE_API api(axcl::pkg::native::IVE_API::AX_IVE_Filter);
    AX_S32 ret;

    if (!m_req.pack(api, pstSrc, pstDst, pstFltCtrl, bInstant)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pIveHandle)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ive::AX_IVE_Hist(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_MEM_INFO_T *pstDst, AX_BOOL bInstant) {
    axcl::pkg::native::IVE_API api(axcl::pkg::native::IVE_API::AX_IVE_Hist);
    AX_S32 ret;

    if (!m_req.pack(api, pstSrc, pstDst, bInstant)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pIveHandle)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ive::AX_IVE_EqualizeHist(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_MEM_INFO_T *pstDst,
                                AX_IVE_EQUALIZE_HIST_CTRL_T *pstEqualizeHistCtrl, AX_BOOL bInstant) {
    axcl::pkg::native::IVE_API api(axcl::pkg::native::IVE_API::AX_IVE_EqualizeHist);
    AX_S32 ret;

    if (!m_req.pack(api, pstSrc, pstDst, pstEqualizeHistCtrl, bInstant)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pIveHandle)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ive::AX_IVE_Integ(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pstDst,
                         AX_IVE_INTEG_CTRL_T *pstIntegCtrl, AX_BOOL bInstant) {
    axcl::pkg::native::IVE_API api(axcl::pkg::native::IVE_API::AX_IVE_Integ);
    AX_S32 ret;

    if (!m_req.pack(api, pstSrc, pstDst, pstIntegCtrl, bInstant)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pIveHandle)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ive::AX_IVE_MagAndAng(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc1, AX_IVE_SRC_IMAGE_T *pstSrc2,
                             AX_IVE_DST_IMAGE_T *pstDstMag, AX_IVE_DST_IMAGE_T *pstDstAng, AX_BOOL bInstant) {
    axcl::pkg::native::IVE_API api(axcl::pkg::native::IVE_API::AX_IVE_MagAndAng);
    AX_S32 ret;

    if (!m_req.pack(api, pstSrc1, pstSrc2, pstDstMag, pstDstAng, bInstant)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pIveHandle)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ive::AX_IVE_Sobel(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pstDst,
                         AX_IVE_SOBEL_CTRL_T *pstSobelCtrl, AX_BOOL bInstant) {
    axcl::pkg::native::IVE_API api(axcl::pkg::native::IVE_API::AX_IVE_Sobel);
    AX_S32 ret;

    if (!m_req.pack(api, pstSrc, pstDst, pstSobelCtrl, bInstant)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pIveHandle)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ive::AX_IVE_GMM(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pstFg, AX_IVE_DST_IMAGE_T *pstBg,
                       AX_IVE_MEM_INFO_T *pstModel, AX_IVE_GMM_CTRL_T *pstGmmCtrl, AX_BOOL bInstant) {
    axcl::pkg::native::IVE_API api(axcl::pkg::native::IVE_API::AX_IVE_GMM);
    AX_S32 ret;

    if (!m_req.pack(api, pstSrc, pstFg, pstBg, pstModel, pstGmmCtrl, bInstant)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pIveHandle)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ive::AX_IVE_GMM2(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pstFg, AX_IVE_DST_IMAGE_T *pstBg,
                        AX_IVE_MEM_INFO_T *pstModel, AX_IVE_GMM2_CTRL_T *pstGmm2Ctrl, AX_BOOL bInstant) {
    axcl::pkg::native::IVE_API api(axcl::pkg::native::IVE_API::AX_IVE_GMM2);
    AX_S32 ret;

    if (!m_req.pack(api, pstSrc, pstFg, pstBg, pstModel, pstGmm2Ctrl, bInstant)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pIveHandle)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ive::AX_IVE_Thresh(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pstDst,
                          AX_IVE_THRESH_CTRL_T *pstThrCtrl, AX_BOOL bInstant) {
    axcl::pkg::native::IVE_API api(axcl::pkg::native::IVE_API::AX_IVE_Thresh);
    AX_S32 ret;

    if (!m_req.pack(api, pstSrc, pstDst, pstThrCtrl, bInstant)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pIveHandle)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ive::AX_IVE_16BitTo8Bit(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pstDst,
                               AX_IVE_16BIT_TO_8BIT_CTRL_T *pst16BitTo8BitCtrl, AX_BOOL bInstant) {
    axcl::pkg::native::IVE_API api(axcl::pkg::native::IVE_API::AX_IVE_16BitTo8Bit);
    AX_S32 ret;

    if (!m_req.pack(api, pstSrc, pstDst, pst16BitTo8BitCtrl, bInstant)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pIveHandle)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ive::AX_IVE_CropImage(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pastDst[],
                             AX_IVE_RECT_U16_T *pastSrcBoxs[], AX_IVE_CROP_IMAGE_CTRL_T *pstCropImageCtrl, AX_IVE_ENGINE_E enEngine,
                             AX_BOOL bInstant) {
    axcl::pkg::native::IVE_API api(axcl::pkg::native::IVE_API::AX_IVE_CropImage);
    AX_S32 ret;

    axcl::pkg::uint8_array boxes;
    boxes.size = sizeof(AX_IVE_RECT_U16_T) * pstCropImageCtrl->u16Num;
    std::unique_ptr<AX_U8[]> spbox = std::make_unique<AX_U8[]>(boxes.size);
    if (!spbox) {
        LOG_MM_E(TAG, "malloc boxes serialize memory fail");
        return AXCL_DEF_IVE_ERR(AXCL_ERR_NO_MEMORY);
    } else {
        boxes.data = spbox.get();
        AX_IVE_RECT_U16_T *box = reinterpret_cast<AX_IVE_RECT_U16_T *>(boxes.data);
        for (AX_U16 i = 0; i < pstCropImageCtrl->u16Num; ++i) {
            *box++ = *pastSrcBoxs[i];
        }
    }

    axcl::pkg::uint8_array dsts_input;
    dsts_input.size = sizeof(AX_IVE_DST_IMAGE_T) * pstCropImageCtrl->u16Num;
    std::unique_ptr<AX_U8[]> spdsts = std::make_unique<AX_U8[]>(dsts_input.size);
    if (!spdsts) {
        LOG_MM_E(TAG, "malloc image serialize memory fail");
        return AXCL_DEF_IVE_ERR(AXCL_ERR_NO_MEMORY);
    } else {
        dsts_input.data = spdsts.get();
        AX_IVE_DST_IMAGE_T *img = reinterpret_cast<AX_IVE_DST_IMAGE_T *>(dsts_input.data);
        for (AX_U16 i = 0; i < pstCropImageCtrl->u16Num; ++i) {
            *img++ = *pastDst[i];
        }
    }

    if (!m_req.pack(api, pstSrc, dsts_input, boxes, pstCropImageCtrl, enEngine, bInstant)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pIveHandle)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ive::AX_IVE_CropResize(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pastDst[],
                              AX_IVE_RECT_U16_T *pastSrcBoxs[], AX_IVE_CROP_RESIZE_CTRL_T *pstCropResizeCtrl, AX_IVE_ENGINE_E enEngine,
                              AX_BOOL bInstant) {
    axcl::pkg::native::IVE_API api(axcl::pkg::native::IVE_API::AX_IVE_CropResize);
    AX_S32 ret;

    axcl::pkg::uint8_array boxes;
    boxes.size = sizeof(AX_IVE_RECT_U16_T) * pstCropResizeCtrl->u16Num;
    std::unique_ptr<AX_U8[]> spbox = std::make_unique<AX_U8[]>(boxes.size);
    if (!spbox) {
        LOG_MM_E(TAG, "malloc boxes serialize memory fail");
        return AXCL_DEF_IVE_ERR(AXCL_ERR_NO_MEMORY);
    } else {
        boxes.data = spbox.get();
        AX_IVE_RECT_U16_T *box = reinterpret_cast<AX_IVE_RECT_U16_T *>(boxes.data);
        for (AX_U16 i = 0; i < pstCropResizeCtrl->u16Num; ++i) {
            *box++ = *pastSrcBoxs[i];
        }
    }

    axcl::pkg::uint8_array dsts_input;
    dsts_input.size = sizeof(AX_IVE_DST_IMAGE_T) * pstCropResizeCtrl->u16Num;
    std::unique_ptr<AX_U8[]> spdst = std::make_unique<AX_U8[]>(dsts_input.size);
    if (!spdst) {
        LOG_MM_E(TAG, "malloc image serialize memory fail");
        return AXCL_DEF_IVE_ERR(AXCL_ERR_NO_MEMORY);
    } else {
        dsts_input.data = spdst.get();
        AX_IVE_DST_IMAGE_T *img = reinterpret_cast<AX_IVE_DST_IMAGE_T *>(dsts_input.data);
        for (AX_U16 i = 0; i < pstCropResizeCtrl->u16Num; ++i) {
            *img++ = *pastDst[i];
        }
    }

    if (!m_req.pack(api, pstSrc, dsts_input, boxes, pstCropResizeCtrl, enEngine, bInstant)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pIveHandle)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ive::AX_IVE_CropResizeForSplitYUV(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc1, AX_IVE_SRC_IMAGE_T *pstSrc2,
                                         AX_IVE_DST_IMAGE_T *pastDst1[], AX_IVE_DST_IMAGE_T *pastDst2[], AX_IVE_RECT_U16_T *pastSrcBoxs[],
                                         AX_IVE_CROP_RESIZE_CTRL_T *pstCropResizeCtrl, AX_IVE_ENGINE_E enEngine, AX_BOOL bInstant) {
    axcl::pkg::native::IVE_API api(axcl::pkg::native::IVE_API::AX_IVE_CropResizeForSplitYUV);
    AX_S32 ret;

    axcl::pkg::uint8_array boxes;
    boxes.size = sizeof(AX_IVE_RECT_U16_T) * pstCropResizeCtrl->u16Num;
    std::unique_ptr<AX_U8[]> spbox = std::make_unique<AX_U8[]>(boxes.size);
    if (!spbox) {
        LOG_MM_E(TAG, "malloc boxes serialize memory fail");
        return AXCL_DEF_IVE_ERR(AXCL_ERR_NO_MEMORY);
    } else {
        boxes.data = spbox.get();
        AX_IVE_RECT_U16_T *box = reinterpret_cast<AX_IVE_RECT_U16_T *>(boxes.data);
        for (AX_U16 i = 0; i < pstCropResizeCtrl->u16Num; ++i) {
            *box++ = *pastSrcBoxs[i];
        }
    }

    axcl::pkg::uint8_array dsts1_input;
    dsts1_input.size = sizeof(AX_IVE_DST_IMAGE_T) * pstCropResizeCtrl->u16Num;
    std::unique_ptr<AX_U8[]> spdst1 = std::make_unique<AX_U8[]>(dsts1_input.size);
    if (!spdst1) {
        LOG_MM_E(TAG, "malloc image1 serialize memory fail");
        return AXCL_DEF_IVE_ERR(AXCL_ERR_NO_MEMORY);
    } else {
        dsts1_input.data = spdst1.get();
        AX_IVE_DST_IMAGE_T *img1 = reinterpret_cast<AX_IVE_DST_IMAGE_T *>(dsts1_input.data);
        for (AX_U16 i = 0; i < pstCropResizeCtrl->u16Num; ++i) {
            *img1++ = *pastDst1[i];
        }
    }

    axcl::pkg::uint8_array dsts2_input;
    dsts2_input.size = sizeof(AX_IVE_DST_IMAGE_T) * pstCropResizeCtrl->u16Num;
    std::unique_ptr<AX_U8[]> spdst2 = std::make_unique<AX_U8[]>(dsts2_input.size);
    if (!spdst2) {
        LOG_MM_E(TAG, "malloc image2 serialize memory fail");
        return AXCL_DEF_IVE_ERR(AXCL_ERR_NO_MEMORY);
    } else {
        dsts2_input.data = spdst2.get();
        AX_IVE_DST_IMAGE_T *img2 = reinterpret_cast<AX_IVE_DST_IMAGE_T *>(dsts2_input.data);
        for (AX_U16 i = 0; i < pstCropResizeCtrl->u16Num; ++i) {
            *img2++ = *pastDst2[i];
        }
    }

    if (!m_req.pack(api, pstSrc1, pstSrc2, dsts1_input, dsts2_input, boxes, pstCropResizeCtrl, enEngine, bInstant)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pIveHandle)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ive::AX_IVE_CSC(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_DST_IMAGE_T *pstDst, AX_IVE_ENGINE_E enEngine,
                       AX_BOOL bInstant) {
    axcl::pkg::native::IVE_API api(axcl::pkg::native::IVE_API::AX_IVE_CSC);
    AX_S32 ret;

    if (!m_req.pack(api, pstSrc, pstDst, enEngine, bInstant)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pIveHandle)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ive::AX_IVE_CropResize2(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc, AX_IVE_IMAGE_T *pastDst[],
                               AX_IVE_RECT_U16_T *pastSrcBoxs[], AX_IVE_RECT_U16_T *pastDstBoxs[],
                               AX_IVE_CROP_IMAGE_CTRL_T *pstCropResize2Ctrl, AX_IVE_ENGINE_E enEngine, AX_BOOL bInstant) {
    axcl::pkg::native::IVE_API api(axcl::pkg::native::IVE_API::AX_IVE_CropResize2);
    AX_S32 ret;

    axcl::pkg::uint8_array boxes1;
    boxes1.size = sizeof(AX_IVE_RECT_U16_T) * pstCropResize2Ctrl->u16Num;
    std::unique_ptr<AX_U8[]> spbox1 = std::make_unique<AX_U8[]>(boxes1.size);
    if (!spbox1) {
        LOG_MM_E(TAG, "malloc boxes1 serialize memory fail");
        return AXCL_DEF_IVE_ERR(AXCL_ERR_NO_MEMORY);
    } else {
        boxes1.data = spbox1.get();
        AX_IVE_RECT_U16_T *box1 = reinterpret_cast<AX_IVE_RECT_U16_T *>(boxes1.data);
        for (AX_U16 i = 0; i < pstCropResize2Ctrl->u16Num; ++i) {
            *box1++ = *pastSrcBoxs[i];
        }
    }

    axcl::pkg::uint8_array boxes2;
    boxes2.size = sizeof(AX_IVE_RECT_U16_T) * pstCropResize2Ctrl->u16Num;
    std::unique_ptr<AX_U8[]> spbox2 = std::make_unique<AX_U8[]>(boxes2.size);
    if (!spbox2) {
        LOG_MM_E(TAG, "malloc boxes2 serialize memory fail");
        return AXCL_DEF_IVE_ERR(AXCL_ERR_NO_MEMORY);
    } else {
        boxes2.data = spbox2.get();
        AX_IVE_RECT_U16_T *box2 = reinterpret_cast<AX_IVE_RECT_U16_T *>(boxes2.data);
        for (AX_U16 i = 0; i < pstCropResize2Ctrl->u16Num; ++i) {
            *box2++ = *pastDstBoxs[i];
        }
    }

    axcl::pkg::uint8_array dsts_input;
    dsts_input.size = sizeof(AX_IVE_IMAGE_T) * pstCropResize2Ctrl->u16Num;
    std::unique_ptr<AX_U8[]> spdst = std::make_unique<AX_U8[]>(dsts_input.size);
    if (!spdst) {
        LOG_MM_E(TAG, "malloc image serialize memory fail");
        return AXCL_DEF_IVE_ERR(AXCL_ERR_NO_MEMORY);
    } else {
        dsts_input.data = spdst.get();
        AX_IVE_IMAGE_T *img = reinterpret_cast<AX_IVE_IMAGE_T *>(dsts_input.data);
        for (AX_U16 i = 0; i < pstCropResize2Ctrl->u16Num; ++i) {
            *img++ = *pastDst[i];
        }
    }

    if (!m_req.pack(api, pstSrc, dsts_input, boxes1, boxes2, pstCropResize2Ctrl, enEngine, bInstant)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pIveHandle)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ive::AX_IVE_CropResize2ForSplitYUV(AX_IVE_HANDLE *pIveHandle, AX_IVE_SRC_IMAGE_T *pstSrc1, AX_IVE_SRC_IMAGE_T *pstSrc2,
                                          AX_IVE_IMAGE_T *pastDst1[], AX_IVE_IMAGE_T *pastDst2[], AX_IVE_RECT_U16_T *pastSrcBoxs[],
                                          AX_IVE_RECT_U16_T *pastDstBoxs[], AX_IVE_CROP_IMAGE_CTRL_T *pstCropResize2Ctrl,
                                          AX_IVE_ENGINE_E enEngine, AX_BOOL bInstant) {
    axcl::pkg::native::IVE_API api(axcl::pkg::native::IVE_API::AX_IVE_CropResize2ForSplitYUV);
    AX_S32 ret;

    axcl::pkg::uint8_array boxes1;
    boxes1.size = sizeof(AX_IVE_RECT_U16_T) * pstCropResize2Ctrl->u16Num;
    std::unique_ptr<AX_U8[]> spbox1 = std::make_unique<AX_U8[]>(boxes1.size);
    if (!spbox1) {
        LOG_MM_E(TAG, "malloc boxes1 serialize memory fail");
        return AXCL_DEF_IVE_ERR(AXCL_ERR_NO_MEMORY);
    } else {
        boxes1.data = spbox1.get();
        AX_IVE_RECT_U16_T *box1 = reinterpret_cast<AX_IVE_RECT_U16_T *>(boxes1.data);
        for (AX_U16 i = 0; i < pstCropResize2Ctrl->u16Num; ++i) {
            *box1++ = *pastSrcBoxs[i];
        }
    }

    axcl::pkg::uint8_array boxes2;
    boxes2.size = sizeof(AX_IVE_RECT_U16_T) * pstCropResize2Ctrl->u16Num;
    std::unique_ptr<AX_U8[]> spbox2 = std::make_unique<AX_U8[]>(boxes2.size);
    if (!spbox2) {
        LOG_MM_E(TAG, "malloc boxes2 serialize memory fail");
        return AXCL_DEF_IVE_ERR(AXCL_ERR_NO_MEMORY);
    } else {
        boxes2.data = spbox2.get();
        AX_IVE_RECT_U16_T *box2 = reinterpret_cast<AX_IVE_RECT_U16_T *>(boxes2.data);
        for (AX_U16 i = 0; i < pstCropResize2Ctrl->u16Num; ++i) {
            *box2++ = *pastDstBoxs[i];
        }
    }

    axcl::pkg::uint8_array dsts1_input;
    dsts1_input.size = sizeof(AX_IVE_IMAGE_T) * pstCropResize2Ctrl->u16Num;
    std::unique_ptr<AX_U8[]> spdst1 = std::make_unique<AX_U8[]>(dsts1_input.size);
    if (!spdst1) {
        LOG_MM_E(TAG, "malloc image1 serialize memory fail");
        return AXCL_DEF_IVE_ERR(AXCL_ERR_NO_MEMORY);
    } else {
        dsts1_input.data = spdst1.get();
        AX_IVE_IMAGE_T *img1 = reinterpret_cast<AX_IVE_IMAGE_T *>(dsts1_input.data);
        for (AX_U16 i = 0; i < pstCropResize2Ctrl->u16Num; ++i) {
            *img1++ = *pastDst1[i];
        }
    }

    axcl::pkg::uint8_array dsts2_input;
    dsts2_input.size = sizeof(AX_IVE_IMAGE_T) * pstCropResize2Ctrl->u16Num;
    std::unique_ptr<AX_U8[]> spdst2 = std::make_unique<AX_U8[]>(dsts2_input.size);
    if (!spdst2) {
        LOG_MM_E(TAG, "malloc image2 serialize memory fail");
        return AXCL_DEF_IVE_ERR(AXCL_ERR_NO_MEMORY);
    } else {
        dsts2_input.data = spdst2.get();
        AX_IVE_IMAGE_T *img2 = reinterpret_cast<AX_IVE_IMAGE_T *>(dsts2_input.data);
        for (AX_U16 i = 0; i < pstCropResize2Ctrl->u16Num; ++i) {
            *img2++ = *pastDst2[i];
        }
    }

    if (!m_req.pack(api, pstSrc1, pstSrc2, dsts1_input, dsts2_input, boxes1, boxes2, pstCropResize2Ctrl, enEngine, bInstant)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pIveHandle)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 ive::AX_IVE_MAU_MatMul(AX_IVE_HANDLE *pIveHandle, AX_IVE_MAU_MATMUL_INPUT_T *pstSrc, AX_IVE_MAU_MATMUL_OUTPUT_T *pstDst,
                              AX_IVE_MAU_MATMUL_CTRL_T *pstMatMulCtrl, AX_IVE_ENGINE_E enEngine, AX_BOOL bInstant) {
    axcl::pkg::native::IVE_API api(axcl::pkg::native::IVE_API::AX_IVE_MAU_MatMul);
    AX_S32 ret;

    axcl::pkg::uint8_array mat_q;
    mat_q.data = reinterpret_cast<AX_U8 *>(pstSrc->stMatQ.pShape);
    mat_q.size = pstSrc->stMatQ.u8ShapeSize * sizeof(AX_S32);
    axcl::pkg::uint8_array mat_b;
    mat_b.data = reinterpret_cast<AX_U8 *>(pstSrc->stMatB.pShape);
    mat_b.size = pstSrc->stMatB.u8ShapeSize * sizeof(AX_S32);
    axcl::pkg::uint8_array mat_mul;
    mat_mul.data = reinterpret_cast<AX_U8 *>(pstDst->stMulRes.pShape);
    mat_mul.size = pstDst->stMulRes.u8ShapeSize * sizeof(AX_S32);
    axcl::pkg::uint8_array mat_top;
    mat_top.data = reinterpret_cast<AX_U8 *>(pstDst->stTopNRes.pShape);
    mat_top.size = pstDst->stTopNRes.u8ShapeSize * sizeof(AX_S32);

    if (!m_req.pack(api, pstSrc, pstDst, pstMatMulCtrl, enEngine, bInstant, mat_q, mat_b, mat_mul, mat_top)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pIveHandle, pstDst, &mat_mul, &mat_top)) {
        return AXCL_DEF_IVE_ERR(AXCL_ERR_DECODE);
    }

    if (mat_mul.size < pstDst->stMulRes.u8ShapeSize * sizeof(AX_S32)) {
        LOG_MM_E(TAG, "mulres shape size {} is less than  {}", mat_mul.size, pstDst->stMulRes.u8ShapeSize * sizeof(AX_S32));
        return AXCL_DEF_IVE_ERR(AXCL_ERR_UNEXPECT_RESPONSE);
    } else {
        ::memcpy(pstDst->stMulRes.pShape, mat_mul.data, pstDst->stMulRes.u8ShapeSize * sizeof(AX_S32));
    }

    if (mat_top.size < pstDst->stTopNRes.u8ShapeSize * sizeof(AX_S32)) {
        LOG_MM_E(TAG, "topnres shape size {} is less than to {}", mat_top.size, pstDst->stTopNRes.u8ShapeSize * sizeof(AX_S32));
        return AXCL_DEF_IVE_ERR(AXCL_ERR_UNEXPECT_RESPONSE);
    } else {
        ::memcpy(pstDst->stTopNRes.pShape, mat_top.data, pstDst->stTopNRes.u8ShapeSize * sizeof(AX_S32));
    }

    return ret;
}

AX_S32 ive::AX_IVE_NPU_CreateMatMulHandle(AX_IVE_MATMUL_HANDLE *pHandle, AX_IVE_NPU_MATMUL_CTRL_T *pstMatMulCtrl) {
    LOG_MM_E(TAG, "unsupported");
    return AXCL_DEF_IVE_ERR(AXCL_ERR_UNSUPPORT);
}

AX_S32 ive::AX_IVE_NPU_DestroyMatMulHandle(AX_IVE_MATMUL_HANDLE *pHandle) {
    LOG_MM_E(TAG, "unsupported");
    return AXCL_DEF_IVE_ERR(AXCL_ERR_UNSUPPORT);
}

AX_S32 ive::AX_IVE_NPU_MatMul(AX_IVE_MATMUL_HANDLE hHandle, AX_IVE_MAU_MATMUL_INPUT_T *pstSrc, AX_IVE_MAU_MATMUL_OUTPUT_T *pstDst,
                              AX_IVE_ENGINE_E enEngine, AX_BOOL bInstant) {
    LOG_MM_E(TAG, "unsupported");
    return AXCL_DEF_IVE_ERR(AXCL_ERR_UNSUPPORT);
}
