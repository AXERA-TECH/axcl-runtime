/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "hvcp.hpp"
#include <string.h>
#include <unistd.h>
#include "cv.hpp"
#include "detection.hpp"
#include "elapsed.hpp"
#include "file.hpp"
#include "io.hpp"

static const int DEFAULT_IMG_H = 576;
static const int DEFAULT_IMG_W = 1024;

static const int NUM_CLASS = HVCP_CLASS_NUM;
static const char *CLASS_NAMES[] = {"body", "vehicle", "cycle", "plate"};

static const float PROB_THRESHOLD_BODY_MIN = 0.3f;
static const float PROB_THRESHOLD_VEHICLE_MIN = 0.3f;
static const float PROB_THRESHOLD_CYCLE_MIN = 0.3f;
static const float PROB_THRESHOLD_PLATE_MIN = 0.3f;
static const float PROB_THRESHOLD_MIN = 0.3f;
static const float NMS_THRESHOLD = 0.45f;

static const int DEFAULT_IO_DEPTH = HVCP_DEFAULT_IO_DEPTH;
static const int MAX_IO_DEPTH = HVCP_MAX_IO_DEPTH;
static const int DEFAULT_POST_THREAD_NUMS = HVCP_DEFAULT_POST_THREAD_NUMS;

static const AXCL_SKEL_FILTER_CONFIG_T MIN_FILTER_CONFIG[] = {
    {0, 0, PROB_THRESHOLD_BODY_MIN},
    {0, 0, PROB_THRESHOLD_VEHICLE_MIN},
    {0, 0, PROB_THRESHOLD_CYCLE_MIN},
    {0, 0, PROB_THRESHOLD_PLATE_MIN}
};

static const int TRACK_BUFFER = 30;  // frame number of tracking states buffers
static const float HIGH_DET_THRESH = 0.5f;  // 0.5f > m_track_thresh as high(1st)
static const float NEW_TRACK_THRESH = PROB_THRESHOLD_MIN;//(HIGH_DET_THRESH + 0.1f);   // > m_high_thresh as new track

// 3 Matching thresholds
static const float HIGH_MATCH_THRESH = 0.8f;  // 0.8f first match threshold
static const float LOW_MATCH_THRESH = 0.5f;  // 0.5f second match threshold
static const float UNCONFIRMED_MATCH_THRESH = 0.7f;  // 0.7: unconfirmed track match to remain dets

namespace {
static AX_VOID generate_yolox_proposals(std::vector<axcl::skel::GridAndStride> grid_strides, float *feat_ptr, const std::array<AXCL_SKEL_FILTER_CONFIG_T, NUM_CLASS> &stFilterArr,
                                    std::vector<axcl::skel::Object> &objects, int wxc) {
    //const int num_grid = 3549;
    const int num_class = NUM_CLASS;
    const int num_anchors = grid_strides.size();

    float* feat_ptr_objectness = feat_ptr + 4 * wxc;
    float* feat_ptr_x_center = feat_ptr;
    float* feat_ptr_y_center = feat_ptr + wxc;
    float* feat_ptr_w = feat_ptr + 2 * wxc;
    float* feat_ptr_h = feat_ptr + 3 * wxc;

    for (int anchor_idx = 0; anchor_idx < num_anchors; anchor_idx++) {
        float box_objectness = feat_ptr_objectness[anchor_idx];
        if (box_objectness >= PROB_THRESHOLD_MIN) {
            for (int class_idx = 0; class_idx < num_class; class_idx ++) {
                float box_cls_score = feat_ptr[(5 + class_idx) * wxc + anchor_idx];
                if (box_cls_score >= stFilterArr[class_idx].fConfidence) {
                    float box_prob = box_objectness * box_cls_score;
                    if (box_prob >= stFilterArr[class_idx].fConfidence) {
                        axcl::skel::Object obj;
                        // printf("%d,%d\n",num_anchors,anchor_idx);
                        const int grid0 = grid_strides[anchor_idx].grid0; // 0
                        const int grid1 = grid_strides[anchor_idx].grid1; // 0
                        const int stride = grid_strides[anchor_idx].stride; // 8
                        // yolox/models/yolo_head.py decode logic
                        //  outputs[..., :2] = (outputs[..., :2] + grids) * strides
                        //  outputs[..., 2:4] = torch.exp(outputs[..., 2:4]) * strides
                        float x_center = (feat_ptr_x_center[anchor_idx] + grid0) * stride;
                        float y_center = (feat_ptr_y_center[anchor_idx] + grid1) * stride;
                        float w = exp(feat_ptr_w[anchor_idx]) * stride;
                        float h = exp(feat_ptr_h[anchor_idx]) * stride;
                        float x0 = x_center - w * 0.5f;
                        float y0 = y_center - h * 0.5f;
                        obj.rect.x = x0;
                        obj.rect.y = y0;
                        obj.rect.width = w;
                        obj.rect.height = h;
                        obj.label = class_idx;
                        obj.prob = box_prob;

                        if (w >= MIN_FILTER_CONFIG[class_idx].nWidth
                            && h >= MIN_FILTER_CONFIG[class_idx].nHeight) {
                            objects.push_back(obj);
                        }
                    }
                }
            }
        }
    } // point anchor loop
}

static AX_S32 HvcpPostProcess(AXCL_SKEL_PRIV_CTX_T &stHvcpCtx, AX_U32 nSrcHeight, AX_U32 nSrcWidth, AX_U32 nOutBufferId,
                        std::vector<axcl::skel::Object> &object_bbox, const std::array<AXCL_SKEL_FILTER_CONFIG_T, NUM_CLASS> &stFilterArr) {
    std::vector<axcl::skel::Object> proporsal;
    auto &io_info = stHvcpCtx.io_info;

    if (io_info) {
        for (uint32_t i = 0; i < io_info->nOutputSize; ++i) {
            auto &output = io_info->pOutputs[i];
            auto &info = stHvcpCtx.vecOutputBuffer[nOutBufferId][i];
            auto &io_skel = stHvcpCtx.vecSkelOutput[nOutBufferId][i];
            axcl::skel::cache_io_flush(&info, &io_skel);
            auto ptr = (float *)io_skel.pVirAddr;
            int wxc = output.pShape[2] * output.pShape[3];
            generate_yolox_proposals(stHvcpCtx.arrGridStride[i], ptr, stFilterArr, proporsal, wxc);
        }

        axcl::skel::get_out_bbox(proporsal, object_bbox, NMS_THRESHOLD, stHvcpCtx.arrInputSize[0], stHvcpCtx.arrInputSize[1], nSrcHeight, nSrcWidth);

        return AXCL_SKEL_SUCC;
    }

    return -1;
}

#define IMPL_MEM_RESULT 0

static AX_VOID impl_mem_callback(MEMMGR_ADDR pAddr, AX_VOID *pUserData, AX_VOID *pParam) {
    CSKEL *__this = (CSKEL *)pUserData;

    if (__this) {
        __this->ImplMemCallback(pAddr, pParam);
    }
    else {
        ALOGE("SKEL instance lose, will memory leak!");
    }
}
}  // namespace

CHVCP::CHVCP(AXCL_SKEL_CTX_T ctx) : CSKEL(ctx, "hvcp_thread") {
}

AX_S32 CHVCP::Create(AX_VOID) {
    AX_S32 nRet = AXCL_SKEL_SUCC;

    // check param
    if ((m_ctx.stParam.nWidth % 2) || (m_ctx.stParam.nHeight % 2)) {
        ALOGE("SKEL WxH({},{}) should be even", m_ctx.stParam.nWidth, m_ctx.stParam.nHeight);
        return AXCL_ERR_SKEL_ILLEGAL_PARAM;
    }

    std::array<int, 2> model_input_size = {DEFAULT_IMG_H, DEFAULT_IMG_W};

    // 0. check param
    if (m_ctx.stParam.ePPL != AXCL_SKEL_PPL_HVCP) {
        nRet = AXCL_ERR_SKEL_NOT_SUPPORT;
        ALOGE("Unsupport SKEL PPL: {}", static_cast<int>(m_ctx.stParam.ePPL));
        return nRet;
    }

    if (m_ctx.stParam.nWidth == 0) {
        m_ctx.stParam.nWidth = DEFAULT_SRC_W;
    }
    if (m_ctx.stParam.nHeight == 0) {
        m_ctx.stParam.nHeight = DEFAULT_SRC_H;
    }

    if (m_ctx.stParam.nIoDepth == 0) {
        m_ctx.stParam.nIoDepth = DEFAULT_IO_DEPTH;
    }
    else if (m_ctx.stParam.nIoDepth > MAX_IO_DEPTH) {
        m_ctx.stParam.nIoDepth = MAX_IO_DEPTH;
    }

    SetPrivParam();

    // 1. create a runtime handle and load the model
    m_stHvcpCtx.vecOutputBufferFlag.resize(m_ctx.stParam.nIoDepth);
    nRet = CreateEngine(m_ctx.stInitParam.strHvcpModel, model_input_size, m_stHvcpCtx, "hvcp");

    if (AXCL_SKEL_SUCC != nRet) {
        ALOGE("SKEL Hvcp CreateEngine fail");
        goto EXIT;
    }

    nRet = CreateGridStride(m_stHvcpCtx);

    if (AXCL_SKEL_SUCC != nRet) {
        ALOGE("SKEL Hvcp CreateGridStride fail");
        goto EXIT;
    }

    if (!m_stHvcpCtx.pstIOBuffer) {
        m_stHvcpCtx.pstIOBuffer = axcl::skel::alloc_cv_image("hvcp", model_input_size, m_stHvcpCtx.eDtype);
    }

    if (!m_stHvcpCtx.pstIOBuffer) {
        ALOGE("SKEL alloc hvcp dst fail");
        nRet = AXCL_ERR_SKEL_NOMEM;
        goto EXIT;
    }

    // 1.1 Warm up
    WarmUpEngine(m_stHvcpCtx);

    CreateThreadPool(m_stHvcpCtx, DEFAULT_POST_THREAD_NUMS, "hvcp_post");

    if (m_ctx.stParam.bTrackEnable && !m_pTracker) {
        m_pTracker = new CBYTETracker(NUM_CLASS, 30, TRACK_BUFFER,
                                        HIGH_DET_THRESH, NEW_TRACK_THRESH,
                                        HIGH_MATCH_THRESH, LOW_MATCH_THRESH,
                                        UNCONFIRMED_MATCH_THRESH);
    }

EXIT:
    if (AXCL_SKEL_SUCC != nRet) {
        Destroy();
    }

    return nRet;
}

AX_S32 CHVCP::Destroy(AX_VOID) {
    auto DestroyEngine = [&](AXCL_SKEL_PRIV_CTX_T &stCtx) {
        if (stCtx.pThreadPool) {
            delete(stCtx.pThreadPool);
            stCtx.pThreadPool = nullptr;
        }

        axcl::skel::free_io(stCtx.stIO, stCtx.vecOutputBuffer, stCtx.vecSkelOutput);

        if (stCtx.pstIOBuffer) {
            axcl::skel::free_cv_image(stCtx.pstIOBuffer);
            stCtx.pstIOBuffer = nullptr;
        }

        stCtx.vecOutputBuffer.clear();
        stCtx.vecSkelOutput.clear();

        AXCL_ENGINE_DestroyHandle(stCtx.pHandle);
    };

    if (m_stHvcpCtx.pHandle) {
        DestroyEngine(m_stHvcpCtx);
        m_stHvcpCtx.pHandle = nullptr;
    }

    return CSKEL::Destroy();
}

AX_S32 CHVCP::SendFrame(const AXCL_SKEL_FRAME_T *pstFrame, AX_S32 nTimeout) {
    auto start_send_time = std::chrono::steady_clock::now();
    if (pstFrame->stFrame.u32Width == 0 || pstFrame->stFrame.u32Height == 0 || pstFrame->stFrame.u32FrameSize == 0) {
        ALOGE("Invalid param: W:{}, H:{}, Size:{}", pstFrame->stFrame.u32Width, pstFrame->stFrame.u32Height, pstFrame->stFrame.u32FrameSize);

        return AXCL_ERR_SKEL_ILLEGAL_PARAM;
    }

    if ((pstFrame->stFrame.u32Width % 2) || (pstFrame->stFrame.u32Height % 2)) {
        ALOGE("SKEL WxH({},{}) should be even", pstFrame->stFrame.u32Width, pstFrame->stFrame.u32Height);
        return AXCL_ERR_SKEL_ILLEGAL_PARAM;
    }

    if (pstFrame->stFrame.u32PicStride[0] % 16) {
        ALOGE("SKEL stride should be Align(16)", pstFrame->stFrame.u32PicStride[0]);
        return AXCL_ERR_SKEL_ILLEGAL_PARAM;
    }

    if (pstFrame->stFrame.u64PhyAddr[0] == 0) {
        ALOGE("SKEL u64PhyAddr({:#x}) should be set", pstFrame->stFrame.u64PhyAddr[0]);
        return AXCL_ERR_SKEL_ILLEGAL_PARAM;
    }

    // check enImgFormat
    if (AX_FORMAT_YUV420_SEMIPLANAR != pstFrame->stFrame.enImgFormat) {
        ALOGE("SKEL Only support NV12");
        return AXCL_ERR_SKEL_NOT_SUPPORT;
    }

    AX_S32 nRet = AXCL_ERR_SKEL_QUEUE_FULL;
    CSkelFrame *pSkelFrame = new CSkelFrame;

    if (pSkelFrame) {
        inc_io_ref_cnt(pstFrame->stFrame);

        do {
            pSkelFrame->nOriginalWidth = pstFrame->stFrame.u32Width;
            pSkelFrame->nOriginalHeight = pstFrame->stFrame.u32Height;
            pSkelFrame->nWidth = m_stHvcpCtx.arrInputSize[1];
            pSkelFrame->nHeight = m_stHvcpCtx.arrInputSize[0];
            pSkelFrame->enImgFormat = pstFrame->stFrame.enImgFormat;
            pSkelFrame->nFrameId = pstFrame->nFrameId;
            pSkelFrame->nStreamId = pstFrame->nStreamId;
            pSkelFrame->pUserData = pstFrame->pUserData;
            pSkelFrame->stFrame = pstFrame->stFrame;
            pSkelFrame->pFrameRelease = this;
        } while(0);

        nRet = SkelEnqueue(pSkelFrame, nTimeout);
    }

    if (m_bDumpSrcImage) {
        AX_CHAR strName[50] = {0};
        sprintf(strName, "src_%dx%d_%lld.yuv", pstFrame->stFrame.u32Width, pstFrame->stFrame.u32Height, pstFrame->nFrameId);
        axcl::skel::dump_file(strName, pstFrame->stFrame);
    }

    if (pSkelFrame && AXCL_SKEL_SUCC != nRet) {
        dec_io_ref_cnt(pstFrame->stFrame);

        pSkelFrame->FreeMem();
    }

    ADD_ELAPSE_TIME("hvcp sendFrame", start_send_time);

    return nRet;
}

AXCL_SKEL_STAGE_STAT_E CHVCP::ProcessFrame(const CSkelFrame *pstFrame) {
    if (!pstFrame || !m_stHvcpCtx.pHandle) {
        if (pstFrame) {
            dec_io_ref_cnt(pstFrame->stFrame);
        }

        SKEL_RESULT_NOTIFY_ONE();
        return AXCL_SKEL_STAGE_STAT_FINALIZE;
    }

    auto start_process_time = std::chrono::steady_clock::now();

    AXCL_SKEL_RESULT_T *pstResult = new AXCL_SKEL_RESULT_T;

    if (!pstResult) {
        dec_io_ref_cnt(pstFrame->stFrame);
        SKEL_RESULT_NOTIFY_ONE();
        ALOGE("SKEL alloc fail");
        return AXCL_SKEL_STAGE_STAT_FINALIZE;
    }

    auto start_pre_process_time = std::chrono::steady_clock::now();
    MEMMGROBJ->Add(pstResult, this, (AX_VOID *)IMPL_MEM_RESULT, impl_mem_callback);
    memset(pstResult, 0x00, sizeof(AXCL_SKEL_RESULT_T));

    pstResult->nFrameId = pstFrame->nFrameId;
    pstResult->nStreamId = pstFrame->nStreamId;
    pstResult->nOriginalWidth = pstFrame->nOriginalWidth;
    pstResult->nOriginalHeight = pstFrame->nOriginalHeight;
    pstResult->pUserData = pstFrame->pUserData;

    AX_S32 nRet = AXCL_SKEL_SUCC;
    AX_U32 nOutBufferId = 0;

    // pre process
    AX_VIDEO_FRAME_T stDstFrame;
    nRet = PreProcess(m_stHvcpCtx, pstFrame, stDstFrame);
    ADD_ELAPSE_TIME("Hvcp PreProcess", start_pre_process_time);

    if (AXCL_SKEL_SUCC != nRet) {
        dec_io_ref_cnt(pstFrame->stFrame);
        goto EXIT;
    }

    if (m_bDumpBodyImage) {
        AX_CHAR strName[50] = {0};
        sprintf(strName, "body_%dx%d_%lld.yuv", pstFrame->nWidth, pstFrame->nHeight, pstFrame->nFrameId);
        axcl::skel::dump_file(strName, stDstFrame);
    }

    // 6. run engine model
    // run hvcp process
    {
        auto start_run_engine_time = std::chrono::steady_clock::now();
        nRet = RunEngine(m_stHvcpCtx, &stDstFrame, nOutBufferId);
        ADD_ELAPSE_TIME("Hvcp RunEngine", start_run_engine_time);

        if (AXCL_SKEL_SUCC != nRet) {
            dec_io_ref_cnt(pstFrame->stFrame);
            goto EXIT;
        }
    }

    // only push should keep ref cnt
    dec_io_ref_cnt(pstFrame->stFrame);

    if (m_stHvcpCtx.pThreadPool) {
        m_stHvcpCtx.pThreadPool->enqueue([pstFrame, pstResult, nOutBufferId, start_process_time, this] {
                this->PostProcessHandler(pstFrame, pstResult, nOutBufferId, AX_FALSE);

                ADD_ELAPSE_TIME("processFrame", start_process_time);
            }
        );

        return AXCL_SKEL_STAGE_STAT_CONTINUE;
    }
    else {
        PostProcessHandler(pstFrame, pstResult, nOutBufferId, AX_TRUE);

        ADD_ELAPSE_TIME("processFrame", start_process_time);

        return AXCL_SKEL_STAGE_STAT_FINALIZE;
    }

EXIT:
    SetOutputBufferStat(m_stHvcpCtx, nOutBufferId, AX_FALSE);

    NOTIFY_SKEL_RESULT(pstResult);

    return AXCL_SKEL_STAGE_STAT_FINALIZE;
}

AX_VOID CHVCP::PostProcessHandler(const CSkelFrame *pstFrame,
                                            AXCL_SKEL_RESULT_T *pstResult,
                                            AX_U32 nOutBufferId,
                                            AX_BOOL bSync) {
    AX_S32 nRet = AXCL_SKEL_SUCC;
    AX_U32 nOriginalWidth = pstFrame->nOriginalWidth;
    AX_U32 nOriginalHeight = pstFrame->nOriginalHeight;
    const std::array<AXCL_SKEL_FILTER_CONFIG_T, NUM_CLASS> &stFilterArr = m_stPrivHvcpParam.stFilterArr;
    const AXCL_SKEL_PARAM_T &stParam = m_ctx.stParam;
    std::vector<axcl::skel::Object> hvcpOutObject;
    std::vector<AXCL_SKEL_OBJECT_ITEM_T> vecObjectItem;
    std::array<AX_U32, NUM_CLASS> nObjectCount = {0};

    if (!pstResult) {
        SetOutputBufferStat(m_stHvcpCtx, nOutBufferId, AX_FALSE);
        goto EXIT;
    }

    // 7. post process
    {
        auto start_post_process_time = std::chrono::steady_clock::now();
        nRet = HvcpPostProcess(m_stHvcpCtx, nOriginalHeight, nOriginalWidth, nOutBufferId, hvcpOutObject, stFilterArr);
        SetOutputBufferStat(m_stHvcpCtx, nOutBufferId, AX_FALSE);
        ADD_ELAPSE_TIME("Hvcp PostProcess", start_post_process_time);

        if (AXCL_SKEL_SUCC != nRet) {
            goto EXIT;
        }
    }

    // track
    if (stParam.bTrackEnable && m_pTracker) {
        auto start_hvcp_tracker_time = std::chrono::steady_clock::now();
        track_map<AX_U32, vector<CTrack>> output_tracks_dict;
        output_tracks_dict = m_pTracker->update(pstFrame->nStreamId, pstFrame->nFrameId, hvcpOutObject);
        ADD_ELAPSE_TIME("hvcp tracker", start_hvcp_tracker_time);

        // hash table traversing
        for (auto it = output_tracks_dict.begin(); it != output_tracks_dict.end(); it++) {
            const vector<CTrack>& output_tracks = it->second;

            for (AX_U32 i = 0; i < output_tracks.size(); ++ i) {
                const axcl::skel::CTrack &obj = output_tracks[i];
                AX_U32 nClassId = obj.class_id;
                const AX_CHAR *pstrObjectCategory = CLASS_NAMES[nClassId];
                AXCL_SKEL_OBJECT_ITEM_T stObjectItem;
                memset(&stObjectItem, 0x00, sizeof(stObjectItem));

                if (obj.state == TrackState::New) {
                    stObjectItem.eTrackState = AXCL_SKEL_TRACK_STATUS_NEW;
                }
                else if (obj.state == TrackState::Tracked) {
                    stObjectItem.eTrackState = AXCL_SKEL_TRACK_STATUS_UPDATE;
                }
                else if (obj.state == TrackState::Removed) {
                    stObjectItem.eTrackState = AXCL_SKEL_TRACK_STATUS_DIE;
                }
                else {
                    continue;
                }

                stObjectItem.pstrObjectCategory = (const AX_CHAR *)pstrObjectCategory;
                stObjectItem.stRect.fX = (float)obj._tlwh[0];
                stObjectItem.stRect.fY = (float)obj._tlwh[1];
                stObjectItem.stRect.fW = (float)obj._tlwh[2];
                stObjectItem.stRect.fH = (float)obj._tlwh[3];
                stObjectItem.fConfidence = (float)obj.score;
                stObjectItem.nFrameId = obj.real_frame_id;

                // track
                stObjectItem.nTrackId = obj.track_id;

                // fliter process
                if (ObjectFliterProcess(pstrObjectCategory,
                                        nClassId,
                                        stParam,
                                        stFilterArr,
                                        obj._tlwh,
                                        nObjectCount[nClassId])) {
                    continue;
                }

                nObjectCount[nClassId] ++;

                vecObjectItem.push_back(stObjectItem);
           }
        }
    }
    else {
        auto start_hvcpOutObject_time = std::chrono::steady_clock::now();
        for (size_t i = 0; i < hvcpOutObject.size(); i++) {
            const axcl::skel::Object &obj = hvcpOutObject[i];
            AX_U32 nClassId = (AX_U32)obj.label;
            const AX_CHAR *pstrObjectCategory = CLASS_NAMES[nClassId];
            AXCL_SKEL_OBJECT_ITEM_T objectItem;
            memset(&objectItem, 0x00, sizeof(objectItem));

            // fliter process
            vector<float> _tlwh;
            _tlwh.resize(4);
            _tlwh[0] = obj.rect.x;
            _tlwh[1] = obj.rect.y;
            _tlwh[2] = obj.rect.width;
            _tlwh[3] = obj.rect.height;
            if (ObjectFliterProcess(pstrObjectCategory,
                                    nClassId,
                                    stParam,
                                    stFilterArr,
                                    _tlwh,
                                    nObjectCount[nClassId])) {
                continue;
            }

            objectItem.pstrObjectCategory = (const AX_CHAR *)pstrObjectCategory;
            objectItem.stRect.fX = (float)_tlwh[0];
            objectItem.stRect.fY = (float)_tlwh[1];
            objectItem.stRect.fW = (float)_tlwh[2];
            objectItem.stRect.fH = (float)_tlwh[3];
            objectItem.fConfidence = (float)obj.prob;
            objectItem.nFrameId = pstFrame->nFrameId;
            objectItem.eTrackState = AXCL_SKEL_TRACK_STATUS_UPDATE;

            nObjectCount[nClassId] ++;

            vecObjectItem.push_back(objectItem);
        }
        ADD_ELAPSE_TIME("hvcpOutObject", start_hvcpOutObject_time);
    }

    pstResult->nObjectSize = vecObjectItem.size();

    if (pstResult->nObjectSize > 0) {
        pstResult->pstObjectItems = new AXCL_SKEL_OBJECT_ITEM_T[pstResult->nObjectSize];
        if (!pstResult->pstObjectItems) {
            pstResult->nObjectSize = 0;
            ALOGE("SKEL alloc Object Item fail");
            nRet = AXCL_ERR_SKEL_NOMEM;
            goto EXIT;
        }

        memcpy(pstResult->pstObjectItems, vecObjectItem.data(), sizeof(AXCL_SKEL_OBJECT_ITEM_T) * pstResult->nObjectSize);
    }

EXIT:
    if (!bSync) {
        SkelFrameRelease((CSkelFrame *)pstFrame);
    }

    NOTIFY_SKEL_RESULT(pstResult);
}

AX_VOID CHVCP::SkelFrameRelease(CSkelFrame *pSkelFrame) {
    if (pSkelFrame) {
        delete pSkelFrame;
    }
}

AX_VOID CHVCP::ImplMemCallback(MEMMGR_ADDR pAddr, AX_VOID *pParam) {
    MEMMGR_PARAM_T nType = (MEMMGR_PARAM_T)pParam;

    switch (nType) {
    case IMPL_MEM_RESULT: {
            AXCL_SKEL_RESULT_T *p = (AXCL_SKEL_RESULT_T *)pAddr;

            if (p) {
                if (p->pstObjectItems) {
                    for (size_t i = 0; i < p->nObjectSize; i++) {
                        if (p->pstObjectItems[i].pstPointSet) {
                            delete[] p->pstObjectItems[i].pstPointSet;
                        }
                    }
                    delete[] p->pstObjectItems;
                }

                if (p->pstCacheList) {
                    delete[] p->pstCacheList;
                }

                delete p;
            }
        }
        break;

    default:
        ALOGE("SKEL Release invalid memory type: {}", nType);
        break;
    }
}

AX_BOOL CHVCP::ObjectFliterProcess(const AX_CHAR *pstrObjectCategory,
                                            const AX_U32 &nClassId,
                                            const AXCL_SKEL_PARAM_T &stParam,
                                            const std::array<AXCL_SKEL_FILTER_CONFIG_T, NUM_CLASS> &stFilterArr,
                                            const vector<float> &tlwh,
                                            const AX_U32 &nObjectCount) {
    if (nClassId >= NUM_CLASS) {
        return AX_TRUE;
    }

    // check roi
    if (stParam.stRoi.bEnable) {
        float x1 = (float)tlwh[0];
        float y1 = (float)tlwh[1];
        float x2 = x1 + (float)tlwh[2];
        float y2 = y1 + (float)tlwh[3];

        float x1_roi = (float)stParam.stRoi.stRect.fX;
        float y1_roi = (float)stParam.stRoi.stRect.fY;
        float x2_roi = x1_roi + (float)stParam.stRoi.stRect.fW;
        float y2_roi = y1_roi + (float)stParam.stRoi.stRect.fH;

        if (!((x1 >= x1_roi && x1 <= x2_roi) && (x2 >= x1_roi && x2 <= x2_roi) && (y1 >= y1_roi && y1 <= y2_roi) &&
            (y2 >= y1_roi && y2 <= y2_roi))) {
            return AX_TRUE;
        }
    }

    // check filter size
    if (!(tlwh[2] >= stFilterArr[nClassId].nWidth && tlwh[3] >= stFilterArr[nClassId].nHeight)) {
        ALOGI("SKEL {} filter({}x{}:{}x{})", pstrObjectCategory, tlwh[2], tlwh[3], stFilterArr[nClassId].nWidth, stFilterArr[nClassId].nHeight);
        return AX_TRUE;
    }

    switch (nClassId) {
    // body
    case 0:
        {
            // check max target count
            if (stParam.stMaxTargetCount.nBodyTargetCount > 0 && nObjectCount >= stParam.stMaxTargetCount.nBodyTargetCount) {
                ALOGI("SKEL body target count({}) exceed", stParam.stMaxTargetCount.nBodyTargetCount);
                return AX_TRUE;
            }
        }
        break;

    // vehicle
    case 1:
        {
            // check max target count
            if (stParam.stMaxTargetCount.nVehicleTargetCount > 0 && nObjectCount >= stParam.stMaxTargetCount.nVehicleTargetCount) {
                ALOGI("SKEL vehicle target count({}) exceed", stParam.stMaxTargetCount.nVehicleTargetCount);
                return AX_TRUE;
            }
        }
        break;

    // cycle
    case 2:
        {
            // check max target count
            if (stParam.stMaxTargetCount.nCycleTargetCount > 0 && nObjectCount >= stParam.stMaxTargetCount.nCycleTargetCount) {
                ALOGI("SKEL cycle target count({}) exceed", stParam.stMaxTargetCount.nCycleTargetCount);
                return AX_TRUE;
            }
        }
        break;

    // plate
    case 3:
        {
            // check max target count
            if (stParam.stMaxTargetCount.nVehicleTargetCount > 0 && nObjectCount >= stParam.stMaxTargetCount.nVehicleTargetCount) {
                ALOGI("SKEL vehicle target count({}) exceed", stParam.stMaxTargetCount.nVehicleTargetCount);
                return AX_TRUE;
            }
        }
        break;

    default:
        break;
    }

    return AX_FALSE;
}

AX_S32 CHVCP::SetPrivParam(AX_VOID) {
    m_stPrivHvcpParam.stFilterArr[0] = m_ctx.stParam.stFilterMaps["body"];
    m_stPrivHvcpParam.stFilterArr[1] = m_ctx.stParam.stFilterMaps["vehicle"];
    m_stPrivHvcpParam.stFilterArr[2] = m_ctx.stParam.stFilterMaps["cycle"];
    m_stPrivHvcpParam.stFilterArr[3] = m_ctx.stParam.stFilterMaps["plate"];

    for (int i = 0; i < NUM_CLASS; i++) {
        if (m_stPrivHvcpParam.stFilterArr[i].nWidth < MIN_FILTER_CONFIG[i].nWidth) {
            m_stPrivHvcpParam.stFilterArr[i].nWidth = MIN_FILTER_CONFIG[i].nWidth;
        }
        if (m_stPrivHvcpParam.stFilterArr[i].nHeight < MIN_FILTER_CONFIG[i].nHeight) {
            m_stPrivHvcpParam.stFilterArr[i].nHeight = MIN_FILTER_CONFIG[i].nHeight;
        }
        if (m_stPrivHvcpParam.stFilterArr[i].fConfidence < MIN_FILTER_CONFIG[i].fConfidence) {
            m_stPrivHvcpParam.stFilterArr[i].fConfidence = MIN_FILTER_CONFIG[i].fConfidence;
        }
    }

    return AXCL_SKEL_SUCC;
}
