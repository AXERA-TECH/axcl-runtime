/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "face.hpp"
#include <string.h>
#include <unistd.h>
#include "cv.hpp"
#include "detection.hpp"
#include "elapsed.hpp"
#include "file.hpp"
#include "io.hpp"

static const int DEFAULT_IMG_H = 576;
static const int DEFAULT_IMG_W = 1024;
static const int FACEWHAT_H = 128;
static const int FACEWHAT_W = 128;
static const int FACEWHAT_JOINTS = 106;

static const int NUM_CLASS = FACE_CLASS_NUM;
static const char *CLASS_NAMES[] = {"face"};

static const float PROB_THRESHOLD = 0.5f;
static const float PROB_THRESHOLD_MIN = 0.5f;
static const float NMS_THRESHOLD = 0.5f;

static const int DEFAULT_IO_DEPTH = FACE_DEFAULT_IO_DEPTH;
static const int MAX_IO_DEPTH = FACE_MAX_IO_DEPTH;
static const int DEFAULT_POST_THREAD_NUMS = FACE_DEFAULT_POST_THREAD_NUMS;

static const AXCL_SKEL_FILTER_CONFIG_T MIN_FILTER_CONFIG[] = {
    {0, 0, PROB_THRESHOLD_MIN}
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
    // const int num_grid = 3549;
    //const int num_class = NUM_CLASS;
    const int num_anchors = grid_strides.size();

    float* feat_ptr_objectness = feat_ptr + 4 * wxc;
    float* feat_ptr_x_center = feat_ptr;
    float* feat_ptr_y_center = feat_ptr + wxc;
    float* feat_ptr_w = feat_ptr + 2 * wxc;
    float* feat_ptr_h = feat_ptr + 3 * wxc;

    const int class_idx = 0;
    for (int anchor_idx = 0; anchor_idx < num_anchors; anchor_idx++) {
        float box_objectness = feat_ptr_objectness[anchor_idx];
        if (box_objectness >= PROB_THRESHOLD_MIN) {
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
                    obj.label = class_idx; //for face
                    obj.prob = box_prob;

                    if (w >= MIN_FILTER_CONFIG[class_idx].nWidth
                        && h >= MIN_FILTER_CONFIG[class_idx].nHeight) {
                        // face Landmark
                        for (int i = 0; i < 5; ++i) {
                            float x = (feat_ptr[(2 * i + 6) * wxc + anchor_idx] + grid0) * stride;
                            float y = (feat_ptr[(2 * i + 7) * wxc + anchor_idx] + grid1) * stride;
                            ai_point_t point = {.x = x,
                                                .y = y,
                                                .score = box_prob};
                            obj.points.push_back(point);
                        }

                        objects.push_back(obj);
                    }
                }
            }
        }
    } // point anchor loop
}

static void face_get_out_bbox(std::vector<Object>& proposals, std::vector<Object>& objects, const float nms_threshold, int letterbox_rows, int letterbox_cols, int src_rows, int src_cols) {
    qsort_descent_inplace(proposals);
    std::vector<int> picked;
    nms_sorted_bboxes(proposals, picked, nms_threshold);

    /* yolov5 draw the result */
    float scale_letterbox;
    int resize_rows;
    int resize_cols;
    if ((letterbox_rows * 1.0 / src_rows) < (letterbox_cols * 1.0 / src_cols)) {
        scale_letterbox = letterbox_rows * 1.0 / src_rows;
    }
    else
    {
        scale_letterbox = letterbox_cols * 1.0 / src_cols;
    }
    resize_cols = int(scale_letterbox * src_cols);
    resize_rows = int(scale_letterbox * src_rows);

    int tmp_h = (letterbox_rows - resize_rows) / 2;
    int tmp_w = (letterbox_cols - resize_cols) / 2;

    float ratio_x = (float)src_rows / resize_rows;
    float ratio_y = (float)src_cols / resize_cols;

    int count = picked.size();

    objects.resize(count);

    for (int i = 0; i < count; i++){
        objects[i] = proposals[picked[i]];
        float x0 = (objects[i].rect.x);
        float y0 = (objects[i].rect.y);
        float x1 = (objects[i].rect.x + objects[i].rect.width);
        float y1 = (objects[i].rect.y + objects[i].rect.height);

        x0 = (x0 - tmp_w) * ratio_x;
        y0 = (y0 - tmp_h) * ratio_y;
        x1 = (x1 - tmp_w) * ratio_x;
        y1 = (y1 - tmp_h) * ratio_y;

        x0 = std::max(std::min(x0, (float)(src_cols - 1)), 0.f);
        y0 = std::max(std::min(y0, (float)(src_rows - 1)), 0.f);
        x1 = std::max(std::min(x1, (float)(src_cols - 1)), 0.f);
        y1 = std::max(std::min(y1, (float)(src_rows - 1)), 0.f);

        objects[i].rect.x = x0;
        objects[i].rect.y = y0;
        objects[i].rect.width = x1 - x0;
        objects[i].rect.height = y1 - y0;

        // face
        for (int j = 0; j < 5; ++j) {
            objects[i].points[j].x = std::max(std::min((objects[i].points[j].x - tmp_w) * ratio_x, (float)(src_cols - 1)), 0.f);
            objects[i].points[j].y = std::max(std::min((objects[i].points[j].y - tmp_h) * ratio_y, (float)(src_rows - 1)), 0.f);
        }
    }
}

static AX_S32 FacePostProcess(AXCL_SKEL_PRIV_CTX_T &stFaceCtx, AX_U32 nSrcHeight, AX_U32 nSrcWidth, AX_U32 nOutBufferId,
                        std::vector<axcl::skel::Object> &object_bbox, const std::array<AXCL_SKEL_FILTER_CONFIG_T, NUM_CLASS> &stFilterArr) {
    std::vector<axcl::skel::Object> proporsal;
    auto &io_info = stFaceCtx.io_info;

    if (io_info) {
        for (uint32_t i = 0; i < io_info->nOutputSize; ++i) {
            auto &output = io_info->pOutputs[i];
            auto &info = stFaceCtx.vecOutputBuffer[nOutBufferId][i];
            auto &io_skel = stFaceCtx.vecSkelOutput[nOutBufferId][i];
            axcl::skel::cache_io_flush(&info, &io_skel);
            auto ptr = (float *)io_skel.pVirAddr;
            int wxc = output.pShape[2] * output.pShape[3];
            generate_yolox_proposals(stFaceCtx.arrGridStride[i], ptr, stFilterArr, proporsal, wxc);
        }

        face_get_out_bbox(proporsal, object_bbox, NMS_THRESHOLD, stFaceCtx.arrInputSize[0], stFaceCtx.arrInputSize[1], nSrcHeight, nSrcWidth);

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

CFACE::CFACE(AXCL_SKEL_CTX_T ctx) : CSKEL(ctx, "face_thread") {
}

AX_S32 CFACE::Create(AX_VOID) {
    AX_S32 nRet = AXCL_SKEL_SUCC;

    // check param
    if ((m_ctx.stParam.nWidth % 2) || (m_ctx.stParam.nHeight % 2)) {
        ALOGE("SKEL WxH({},{}) should be even", m_ctx.stParam.nWidth, m_ctx.stParam.nHeight);
        return AXCL_ERR_SKEL_ILLEGAL_PARAM;
    }

    std::array<int, 2> face_input_size = {DEFAULT_IMG_H, DEFAULT_IMG_W};

    // 0. check param
    if (m_ctx.stParam.ePPL != AXCL_SKEL_PPL_FACE) {
        nRet = AXCL_ERR_SKEL_NOT_SUPPORT;
        ALOGE("Invalid SKEL PPL: {}", static_cast<int>(m_ctx.stParam.ePPL));
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
    m_stFaceCtx.vecOutputBufferFlag.resize(m_ctx.stParam.nIoDepth);
    nRet = CreateEngine(m_ctx.stInitParam.strFaceModel, face_input_size, m_stFaceCtx, "face");

    if (AXCL_SKEL_SUCC != nRet) {
        ALOGE("SKEL Face CreateEngine fail");
        goto EXIT;
    }

    nRet = CreateGridStride(m_stFaceCtx);

    if (AXCL_SKEL_SUCC != nRet) {
        ALOGE("SKEL face CreateGridStride fail");
        goto EXIT;
    }

    if (!m_stFaceCtx.pstIOBuffer) {
        m_stFaceCtx.pstIOBuffer = axcl::skel::alloc_cv_image("face", face_input_size, m_stFaceCtx.eDtype);
    }

    if (!m_stFaceCtx.pstIOBuffer) {
        ALOGE("SKEL alloc face dst fail");
        nRet = AXCL_ERR_SKEL_NOMEM;
        goto EXIT;
    }

    // 1.1 Warm up
    WarmUpEngine(m_stFaceCtx);

    CreateThreadPool(m_stFaceCtx, DEFAULT_POST_THREAD_NUMS, "face_post");

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

AX_S32 CFACE::Destroy(AX_VOID) {
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

    if (m_stFaceCtx.pHandle) {
        DestroyEngine(m_stFaceCtx);
        m_stFaceCtx.pHandle = nullptr;
    }

    return CSKEL::Destroy();
}

AX_S32 CFACE::SendFrame(const AXCL_SKEL_FRAME_T *pstFrame, AX_S32 nTimeout) {
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
            pSkelFrame->nWidth = m_stFaceCtx.arrInputSize[1];
            pSkelFrame->nHeight = m_stFaceCtx.arrInputSize[0];
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

    ADD_ELAPSE_TIME("face sendFrame", start_send_time);

    return nRet;
}

AXCL_SKEL_STAGE_STAT_E CFACE::ProcessFrame(const CSkelFrame *pstFrame) {
    if (!pstFrame || !m_stFaceCtx.pHandle) {
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
    pstResult->nOriginalWidth = pstFrame->nOriginalWidth;
    pstResult->nOriginalHeight = pstFrame->nOriginalHeight;
    pstResult->pUserData = pstFrame->pUserData;

    AX_S32 nRet = AXCL_SKEL_SUCC;
    AX_U32 nOutBufferId = 0;

    // pre process
    AX_VIDEO_FRAME_T stDstFrame;
    nRet = PreProcess(m_stFaceCtx, pstFrame, stDstFrame);
    ADD_ELAPSE_TIME("face PreProcess", start_pre_process_time);

    if (AXCL_SKEL_SUCC != nRet) {
        dec_io_ref_cnt(pstFrame->stFrame);
        goto EXIT;
    }

    if (m_bDumpBodyImage) {
        AX_CHAR strName[50] = {0};
        sprintf(strName, "face_%dx%d_%lld.yuv", pstFrame->nWidth, pstFrame->nHeight, pstFrame->nFrameId);
        axcl::skel::dump_file(strName, stDstFrame);
    }

    {
        // run face process
        auto start_run_face_time = std::chrono::steady_clock::now();
        nRet = RunEngine(m_stFaceCtx, &stDstFrame, nOutBufferId);
        ADD_ELAPSE_TIME("Face RunEngine", start_run_face_time);

        if (AXCL_SKEL_SUCC != nRet) {
            dec_io_ref_cnt(pstFrame->stFrame);
            goto EXIT;
        }
    }

    // only push should keep ref cnt
    dec_io_ref_cnt(pstFrame->stFrame);

    if (m_stFaceCtx.pThreadPool) {
        m_stFaceCtx.pThreadPool->enqueue([pstFrame, pstResult, nOutBufferId, start_process_time, this] {
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
    SetOutputBufferStat(m_stFaceCtx, nOutBufferId, AX_FALSE);

    NOTIFY_SKEL_RESULT(pstResult);

    return AXCL_SKEL_STAGE_STAT_FINALIZE;
}

AX_VOID CFACE::PostProcessHandler(const CSkelFrame *pstFrame,
                                            AXCL_SKEL_RESULT_T *pstResult,
                                            AX_U32 nOutBufferId,
                                            AX_BOOL bSync) {
    AX_S32 nRet = AXCL_SKEL_SUCC;
    AX_U32 nOriginalWidth = pstFrame->nOriginalWidth;
    AX_U32 nOriginalHeight = pstFrame->nOriginalHeight;
    const std::array<AXCL_SKEL_FILTER_CONFIG_T, NUM_CLASS> &stFilterArr = m_sPrivFaceParam.stFilterArr;
    const AXCL_SKEL_PARAM_T &stParam = m_ctx.stParam;
    std::vector<axcl::skel::Object> faceOutObject;
    std::vector<AXCL_SKEL_OBJECT_ITEM_T> vecObjectItem;
    std::array<AX_U32, NUM_CLASS> nObjectCount = {0};

    if (!pstResult) {
        SetOutputBufferStat(m_stFaceCtx, nOutBufferId, AX_FALSE);
        goto EXIT;
    }

    {
        // face post process
        auto start_post_process_time = std::chrono::steady_clock::now();
        nRet = FacePostProcess(m_stFaceCtx, nOriginalHeight, nOriginalWidth, nOutBufferId, faceOutObject, stFilterArr);
        SetOutputBufferStat(m_stFaceCtx, nOutBufferId, AX_FALSE);
        ADD_ELAPSE_TIME("FacePostProcess", start_post_process_time);

        if (AXCL_SKEL_SUCC != nRet) {
            goto EXIT;
        }
    }

    // track
    if (stParam.bTrackEnable && m_pTracker) {
        auto start_face_tracker_time = std::chrono::steady_clock::now();
        track_map<AX_U32, vector<CTrack>> output_tracks_dict;
        output_tracks_dict = m_pTracker->update(pstFrame->nStreamId, pstFrame->nFrameId, faceOutObject);
        ADD_ELAPSE_TIME("face tracker", start_face_tracker_time);

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
        for (size_t i = 0; i < faceOutObject.size(); i++) {
            const axcl::skel::Object &obj = faceOutObject[i];
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

            objectItem.pstrObjectCategory = pstrObjectCategory;
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

AX_VOID CFACE::SkelFrameRelease(CSkelFrame *pSkelFrame) {
    if (pSkelFrame) {
        delete pSkelFrame;
    }
}

AX_VOID CFACE::ImplMemCallback(MEMMGR_ADDR pAddr, AX_VOID *pParam) {
    MEMMGR_PARAM_T nType = (MEMMGR_PARAM_T)pParam;

    switch (nType) {
    case IMPL_MEM_RESULT: {
            AXCL_SKEL_RESULT_T *p = (AXCL_SKEL_RESULT_T *)pAddr;

            if (p) {
                if (p->pstObjectItems) {
                    for (size_t i = 0; i < p->nObjectSize; i++) {
                        if (p->pstObjectItems[i].pstFeatureItem) {
                            delete[] p->pstObjectItems[i].pstFeatureItem;
                        }
                        if (p->pstObjectItems[i].pstMetaInfo) {
                            for (size_t j = 0; j < p->pstObjectItems[i].nMetaInfoSize; j++) {
                                free(p->pstObjectItems[i].pstMetaInfo[j].pstrValue);
                            }
                            delete[] p->pstObjectItems[i].pstMetaInfo;
                        }
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

AX_BOOL CFACE::ObjectFliterProcess(const AX_CHAR *pstrObjectCategory,
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
    // face
    case 0:
        {
            // check max target count
            if (stParam.stMaxTargetCount.nBodyTargetCount > 0 && nObjectCount >= stParam.stMaxTargetCount.nBodyTargetCount) {
                ALOGI("SKEL body target count({}) exceed", stParam.stMaxTargetCount.nBodyTargetCount);
                return AX_TRUE;
            }
        }
        break;

    default:
        break;
    }

    return AX_FALSE;
}

AX_S32 CFACE::SetPrivParam(AX_VOID) {
    m_sPrivFaceParam.stFilterArr[0] = m_ctx.stParam.stFilterMaps["face"];

    for (int i = 0; i < NUM_CLASS; i++) {
        if (m_sPrivFaceParam.stFilterArr[i].nWidth < MIN_FILTER_CONFIG[i].nWidth) {
            m_sPrivFaceParam.stFilterArr[i].nWidth = MIN_FILTER_CONFIG[i].nWidth;
        }
        if (m_sPrivFaceParam.stFilterArr[i].nHeight < MIN_FILTER_CONFIG[i].nHeight) {
            m_sPrivFaceParam.stFilterArr[i].nHeight = MIN_FILTER_CONFIG[i].nHeight;
        }
        if (m_sPrivFaceParam.stFilterArr[i].fConfidence < MIN_FILTER_CONFIG[i].fConfidence) {
            m_sPrivFaceParam.stFilterArr[i].fConfidence = MIN_FILTER_CONFIG[i].fConfidence;
        }
    }

    return AXCL_SKEL_SUCC;
}
