/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "skel.hpp"
#include <string.h>
#include <unistd.h>
#include "cv.hpp"
#include "detection.hpp"
#include "elapsed.hpp"
#include "file.hpp"
#include "io.hpp"
#include "axcl_rt_context.h"

static const int NUM_CLASS = SKEL_CLASS_NUM;

static const char *strSkelModelType[AX_ENGINE_MODEL_TYPE_BUTT] = {"3.6T", "7.2T", "10.8T"};

namespace {
static AX_S32 CheckModelVNpu(const std::string &strModel, const AX_ENGINE_MODEL_TYPE_T &eModelType, const AX_S32 &nNpuType, AX_U32 &nNpuSet) {
    AX_ENGINE_NPU_ATTR_T stNpuAttr;
    memset(&stNpuAttr, 0x00, sizeof(stNpuAttr));

    auto ret = AXCL_ENGINE_GetVNPUAttr(&stNpuAttr);
    if (ret == 0) {
        // VNPU DISABLE
        if (stNpuAttr.eHardMode == AX_ENGINE_VIRTUAL_NPU_DISABLE) {
            nNpuSet = 0x07; // NON-VNPU (0b111)
            ALOGN("{} will run under VNPU-DISABLE [{}]", strModel.c_str(), strSkelModelType[eModelType]);
        }
        // STD VNPU
        else if (stNpuAttr.eHardMode == AX_ENGINE_VIRTUAL_NPU_STD) {
            // 7.2T & 10.8T no allow
            if (eModelType == AX_ENGINE_MODEL_TYPE1
                || eModelType == AX_ENGINE_MODEL_TYPE2) {
                ALOGE("{} model type{}: [{}], no allow run under STD VNPU", strModel.c_str(), static_cast<int>(eModelType), strSkelModelType[eModelType]);
                return AXCL_ERR_SKEL_ILLEGAL_PARAM;
            }

            // default STD VNPU2
            if (nNpuType == 0) {
                nNpuSet = 0x02; // VNPU2 (0b010)
                ALOGN("{} will run under default STD-VNPU2 [{}]", strModel.c_str(), strSkelModelType[eModelType]);
            }
            else {
                if (nNpuType & AXCL_SKEL_STD_VNPU_1) {
                    nNpuSet |= 0x01; // VNPU1 (0b001)
                    ALOGN("{} will run under STD-VNPU1 [{}]", strModel.c_str(), strSkelModelType[eModelType]);
                }
                if (nNpuType & AXCL_SKEL_STD_VNPU_2) {
                    nNpuSet |= 0x02; // VNPU2 (0b010)
                    ALOGN("{} will run under STD-VNPU2 [{}]", strModel.c_str(), strSkelModelType[eModelType]);
                }
                if (nNpuType & AXCL_SKEL_STD_VNPU_3) {
                    nNpuSet |= 0x04; // VNPU3 (0b100)
                    ALOGN("{} will run under STD-VNPU3 [{}]", strModel.c_str(), strSkelModelType[eModelType]);
                }
            }
        }
        // BL VNPU
        else if (stNpuAttr.eHardMode == AX_ENGINE_VIRTUAL_NPU_BIG_LITTLE) {
            // 10.8T no allow
            if (eModelType == AX_ENGINE_MODEL_TYPE2) {
                ALOGE("{} model type{}: [{}], no allow run under BL VNPU", strModel.c_str(), static_cast<int>(eModelType), strSkelModelType[eModelType]);
                return AXCL_ERR_SKEL_ILLEGAL_PARAM;
            }

            // default BL VNPU
            if (nNpuType == 0) {
                // 7.2T default BL VNPU1
                if (eModelType == AX_ENGINE_MODEL_TYPE1) {
                    nNpuSet = 0x01; // VNPU1 (0b001)
                    ALOGN("{} will run under default BL-VNPU1 [{}]", strModel.c_str(), strSkelModelType[eModelType]);
                }
                // 3.6T default BL VNPU2
                else {
                    nNpuSet = 0x02; // VNPU2 (0b010)
                    ALOGN("{} will run under default BL-VNPU2 [{}]", strModel.c_str(), strSkelModelType[eModelType]);
                }
            }
            else {
                // 7.2T
                if (eModelType == AX_ENGINE_MODEL_TYPE1) {
                    // no allow set to BL VNPU2
                    if (nNpuType & AXCL_SKEL_BL_VNPU_2) {
                        ALOGE("{} model type{}: [{}], no allow run under BL VNPU2", strModel.c_str(), static_cast<int>(eModelType), strSkelModelType[eModelType]);
                        return AXCL_ERR_SKEL_ILLEGAL_PARAM;
                    }
                    if (nNpuType & AXCL_SKEL_BL_VNPU_1) {
                        nNpuSet |= 0x01; // VNPU1 (0b001)
                        ALOGN("{} will run under BL-VNPU1 [{}]", strModel.c_str(), strSkelModelType[eModelType]);
                    }
                }
                // 3.6T
                else {
                    if (nNpuType & AXCL_SKEL_BL_VNPU_1) {
                        nNpuSet |= 0x01; // VNPU1 (0b001)
                        ALOGN("{} will run under BL-VNPU1 [{}]", strModel.c_str(), strSkelModelType[eModelType]);
                    }
                    if (nNpuType & AXCL_SKEL_BL_VNPU_2) {
                        nNpuSet |= 0x02; // VNPU2 (0b010)
                        ALOGN("{} will run under BL-VNPU2 [{}]", strModel.c_str(), strSkelModelType[eModelType]);
                    }
                }
            }
        }
    }
    else {
        ALOGE("AXCL_ENGINE_GetVNPUAttr fail ret = {:#x}", ret);
    }

    return ret;
}
}  // namespace

CSKEL::CSKEL(AXCL_SKEL_CTX_T ctx) : CStage("skel_thread", ctx.nDeviceId), m_ctx(ctx) {
}

CSKEL::CSKEL(AXCL_SKEL_CTX_T ctx, const string& strName) : CStage(strName, ctx.nDeviceId), m_ctx(ctx) {
}

AX_S32 CSKEL::CreateEngine(const std::string &strModel, std::array<int, 2> &input_size, AXCL_SKEL_PRIV_CTX_T &stCtx, const std::string& token) {
    AX_S32 ret = 0;

    // 1. load model
    AX_CHAR *pModelBufferVirAddr = nullptr;
    AX_U32 nModelBufferSize = 0;
    std::vector<char> model_buffer;

    if (!load_file(strModel, (AX_VOID **)&pModelBufferVirAddr, nModelBufferSize)) {
        ALOGE("SKEL read model({}) fail", strModel.c_str());
        return AXCL_ERR_SKEL_ILLEGAL_PARAM;
    }

    auto freeModelBuffer = [&]() {
        if (pModelBufferVirAddr) {
            unload_file(pModelBufferVirAddr, nModelBufferSize);
        }
        return;
    };

    // 1.1 Get Model Type
    AX_ENGINE_MODEL_TYPE_T eModelType = AX_ENGINE_MODEL_TYPE0;
    ret = AXCL_ENGINE_GetModelType(pModelBufferVirAddr, nModelBufferSize, &eModelType);

    if (0 != ret || eModelType >= AX_ENGINE_MODEL_TYPE_BUTT) {
        ALOGE("{} AX_ENGINE_GetModelType fail ret={:#x}, eModelType={}", strModel.c_str(), ret, static_cast<int>(eModelType));

        freeModelBuffer();

        return AXCL_ERR_SKEL_ILLEGAL_PARAM;
    }

    ALOGN("SKEL {}: model is type{}: [{}]", strModel.c_str(), static_cast<int>(eModelType), strSkelModelType[eModelType]);

    // 1.2 Check VNPU
    AX_ENGINE_NPU_SET_T nNpuSet = 0;
    ret = CheckModelVNpu(strModel, eModelType, m_ctx.stParam.nNpuType, nNpuSet);

    if (0 != ret) {
        ALOGE("SKEL CheckModelVNpu fail");

        freeModelBuffer();

        return AXCL_ERR_SKEL_ILLEGAL_PARAM;
    }

    // 2. create handle
    AX_ENGINE_HANDLE handle = nullptr;
    START_RECORD_TIME;
    ret = AXCL_ENGINE_CreateHandle(&handle, pModelBufferVirAddr, nModelBufferSize);
    PRINT_ELAPSE_TIME_WITH_FUNC("AX_ENGINE_CreateHandle");

    auto deinit_handle = [&handle]() {
        if (handle) {
            AXCL_ENGINE_DestroyHandle(handle);
        }
        return AXCL_ERR_SKEL_ILLEGAL_PARAM;
    };

    freeModelBuffer();

    if (0 != ret || !handle) {
        ALOGE("SKEL Create model({}) handle fail", strModel.c_str());

        return deinit_handle();
    }

    // 3. create context
    ret = AXCL_ENGINE_CreateContext(handle);
    if (0 != ret) {
        return deinit_handle();
    }

    // 4. set io
    auto &io_info = stCtx.io_info;
    ret = AXCL_ENGINE_GetIOInfo(handle, &io_info);
    if (0 != ret) {
        return deinit_handle();
    }

    // 4.1 query io
    AX_IMG_FORMAT_E eDtype = AX_FORMAT_YUV420_SEMIPLANAR;
    ret = axcl::skel::query_model_input_size(io_info, input_size, eDtype);

    if (0 != ret) {
        ALOGE("SKEL model({}) query model input size fail", strModel.c_str());
        return deinit_handle();
    }

    if (eDtype == AX_FORMAT_YUV420_SEMIPLANAR || eDtype == AX_FORMAT_RGB888) {
        ALOGN("SKEL model({}) data type is {:#x}", strModel.c_str(), static_cast<int>(eDtype));
    }
    else {
        ALOGE("SKEL model({}) data type is: {:#x}, unsupport", strModel.c_str(), static_cast<int>(eDtype));
        return deinit_handle();
    }

    // 4.2 brief io
#ifdef __AXCL_SKEL_DEBUG__
    axcl::skel::brief_io_info(strModel, io_info);
#endif

    //5. Config VNPU
    ALOGN("SKEL model({}) nNpuSet: {:#x}", strModel.c_str(), nNpuSet);
    if (nNpuSet != 0x07){
        ret = AXCL_ENGINE_SetAffinity(handle, (AX_ENGINE_NPU_SET_T)nNpuSet);

        if (0 != ret) {
            ALOGE("SKEL model({}) AX_ENGINE_SetAffinity fail ret={:#x}", strModel.c_str(), ret);
            return deinit_handle();
        }

    }

    // 6. prepare io
    AX_U32 nIoDepth = (stCtx.vecOutputBufferFlag.size() == 0) ? 1 : stCtx.vecOutputBufferFlag.size();
    ret = axcl::skel::prepare_io(token, io_info, stCtx.stIO, nIoDepth,
                            stCtx.vecOutputBuffer,
                            stCtx.vecSkelOutput,
                            SKEL_IO_BUFFER_STRATEGY_CACHED);

    if (0 != ret) {
        return deinit_handle();
    }

    stCtx.pHandle = handle;

    stCtx.eModelType = eModelType;

    stCtx.arrInputSize = input_size;

    stCtx.eDtype = eDtype;

    {
        AX_CHAR *pEnv = NULL;
        pEnv = getenv(SKEL_DUMP_SRC_ENV_STR);
        if (pEnv) {
            m_bDumpSrcImage = (AX_BOOL)atoi(pEnv);
        }

        pEnv = getenv(SKEL_DUMP_BODY_ENV_STR);
        if (pEnv) {
            m_bDumpBodyImage = (AX_BOOL)atoi(pEnv);
        }

        pEnv = getenv(SKEL_DUMP_POSE_ENV_STR);
        if (pEnv) {
            m_bDumpPoseImage = (AX_BOOL)atoi(pEnv);
        }

        GetCvIntfType(SKEL_CSC_INTF_TYPE_ENV_STR, m_eCscIntfType);

        GetCvIntfType(SKEL_RESIZE_INTF_TYPE_ENV_STR, m_eResizeIntfType);
    }

    return AXCL_SKEL_SUCC;
}

AX_S32 CSKEL::CreateGridStride(AXCL_SKEL_PRIV_CTX_T &stCtx) {
    auto &io_info = stCtx.io_info;

    if (!io_info) {
        return AXCL_ERR_SKEL_INVALID_HANDLE;
    }

    std::vector<std::vector<int>> stride = {{8}, {16}, {32}};
    for (uint32_t i = 0; i < io_info->nOutputSize; ++i) {
        axcl::skel::generate_grids_and_stride(stCtx.arrInputSize[1], stCtx.arrInputSize[0], stride[i], stCtx.arrGridStride[i]);
    }

    return AXCL_SKEL_SUCC;
}

AX_S32 CSKEL::WarmUpEngine(AXCL_SKEL_PRIV_CTX_T &stCtx) {
    if (stCtx.pstIOBuffer) {
        START_RECORD_TIME;
        auto ret = axcl::skel::push_io_input(stCtx.pstIOBuffer, stCtx.stIO);

        if (0 == ret) {
            ret = AXCL_ENGINE_RunSync(stCtx.pHandle, &stCtx.stIO);

            if (0 != ret) {
                ALOGE("AX_ENGINE_RunSync failed.");
                return AXCL_ERR_SKEL_INVALID_HANDLE;
            }
        }
        PRINT_ELAPSE_TIME_WITH_FUNC("Engine WarmUp");
    }

    return AXCL_SKEL_SUCC;
}

AX_S32 CSKEL::PreProcess(AXCL_SKEL_PRIV_CTX_T &stCtx, const CSkelFrame *pstFrame, AX_VIDEO_FRAME_T &stDstFrame)
{
    AX_S32 nRet = AXCL_SKEL_SUCC;
    const AX_VIDEO_FRAME_T *pstSrcFrame = nullptr;

    // need crop resize (NV12)
    pstSrcFrame = &pstFrame->stFrame;
    if (pstSrcFrame->u32PicStride[0] != stCtx.pstIOBuffer->u32PicStride[0]
        || pstSrcFrame->u32Width != stCtx.pstIOBuffer->u32Width
        || pstSrcFrame->u32Height != stCtx.pstIOBuffer->u32Height) {
        auto start_frame_crop_resize_time = std::chrono::steady_clock::now();
        stDstFrame = *stCtx.pstIOBuffer;
        nRet = FrameCropResize(pstSrcFrame, &stDstFrame, m_eResizeIntfType);
        ADD_ELAPSE_TIME("FrameCropResize", start_frame_crop_resize_time);
    }
    else {
        stDstFrame = pstFrame->stFrame;
    }

    if (nRet != 0) {
        ALOGE("SKEL crop resize fail");
        return AXCL_ERR_SKEL_ILLEGAL_PARAM;
    }

    // need csc (TODO:)
    if (pstSrcFrame->enImgFormat != stCtx.eDtype) {
        ALOGE("CSC no match({}, {})", static_cast<int>(pstSrcFrame->enImgFormat), static_cast<int>(stCtx.eDtype));
        return AXCL_ERR_SKEL_ILLEGAL_PARAM;
    }

    return nRet;
}

AX_S32 CSKEL::RunEngine(AXCL_SKEL_PRIV_CTX_T &stCtx, AX_VIDEO_FRAME_T *pstFrame, AX_U32 &nOutBufferId) {
    // 7.1 fill input & prepare to inference
    auto ret = axcl::skel::push_io_input(pstFrame, stCtx.stIO);

    if (0 != ret) {
        ALOGE("push_io_input failed.");
        ret = AXCL_ERR_SKEL_ILLEGAL_PARAM;
        return ret;
    }

    // 7.2 fill output
    {
        auto &io_info = stCtx.io_info;

        if (!io_info) {
            ALOGE("AX_ENGINE_GetIOInfo failed.");
            ret = AXCL_ERR_SKEL_ILLEGAL_PARAM;
            return ret;
        }

        nOutBufferId = GetOutputBufferUnUseId(stCtx);

        std::vector<AX_ENGINE_IO_BUFFER_T> outputBuffer = stCtx.vecOutputBuffer[nOutBufferId];
        ret = axcl::skel::push_io_output(io_info, stCtx.stIO, outputBuffer);

        if (0 != ret) {
            SetOutputBufferStat(stCtx, nOutBufferId, AX_FALSE);
            ALOGE("push_io_output failed.");
            ret = AXCL_ERR_SKEL_ILLEGAL_PARAM;
            return ret;
        }
    }

    // 7.3 run & benchmark
    {
        ret = AXCL_ENGINE_RunSync(stCtx.pHandle, &stCtx.stIO);

        if (0 != ret) {
            SetOutputBufferStat(stCtx, nOutBufferId, AX_FALSE);
            ALOGE("AX_ENGINE_RunSync failed.");
            ret = AXCL_ERR_SKEL_INVALID_HANDLE;
            return ret;
        }
    }

    return ret;
}

AX_S32 CSKEL::CreateThreadPool(AXCL_SKEL_PRIV_CTX_T &stCtx, AX_S32 nThreadNums, const string& threadName) {
    if (stCtx.vecOutputBufferFlag.size() > 1) {
        if (!stCtx.pThreadPool) {
            stCtx.pThreadPool = new CThreadPool(nThreadNums, threadName, m_stThreadAttr, m_ctx.nDeviceId);
            stCtx.nThreadNums = nThreadNums;
        }
    }

    return AXCL_SKEL_SUCC;
}

AX_U32 CSKEL::GetOutputBufferUnUseId(AXCL_SKEL_PRIV_CTX_T &stCtx) {
    if (stCtx.vecOutputBufferFlag.size() > 1) {
        while (1) {
            const std::vector<AX_BOOL> &vecOutputBufferFlag = stCtx.vecOutputBufferFlag;

            for (size_t i = 0; i < vecOutputBufferFlag.size(); i++) {
                if (!vecOutputBufferFlag[i]) {
                    std::unique_lock<std::mutex> lck(stCtx.mtxOutputBuffer);
                    stCtx.vecOutputBufferFlag[i] = AX_TRUE;
                    return i;
                }
            }

            CTimeUtils::usSleep(500);
        };
    }

    return 0;
}

AX_U32 CSKEL::SetOutputBufferStat(AXCL_SKEL_PRIV_CTX_T &stCtx, AX_U32 nId, AX_BOOL bStat) {
    if (stCtx.vecOutputBufferFlag.size() > 1
        && nId < stCtx.vecOutputBufferFlag.size()) {
        std::unique_lock<std::mutex> lck(stCtx.mtxOutputBuffer);
        stCtx.vecOutputBufferFlag[nId] = bStat;
    }

    return AXCL_SKEL_SUCC;
}

AX_S32 CSKEL::Create(AX_VOID) {
    return AXCL_SKEL_SUCC;
}

AX_S32 CSKEL::Destroy(AX_VOID) {
    SAFE_DELETE_PTR(m_pTracker);

    m_bResultThreadRunning = AX_FALSE;

    PRINT_ELAPSE_INFO;
    RESET_ELAPSE_INFO;

    SKEL_RESULT_NOTIFY_ONE();

    if (m_pResultThread) {
        m_pResultThread->join();
        delete m_pResultThread;
        m_pResultThread = nullptr;
    }

    return AXCL_SKEL_SUCC;
}

AX_BOOL CSKEL::ObjectFliterProcess(const AX_CHAR *pstrObjectCategory,
                                            const AX_U32 &nClassId,
                                            const AXCL_SKEL_PARAM_T &stParam,
                                            const std::array<AXCL_SKEL_FILTER_CONFIG_T, NUM_CLASS> &stFilterArr,
                                            const vector<float> &tlwh,
                                            const AX_U32 &nObjectCount) {
    return AX_TRUE;
}

AX_BOOL CSKEL::SkelQueueEmpty(AX_VOID) {
    if (QueueEmpty()) {
        std::lock_guard<std::mutex> lck(m_mtxStatus);
        AX_U64 &nFrameProc = m_ctx.stRunningStatus.nFrameProc;
        AX_U64 &nFrameTrans = m_ctx.stRunningStatus.nFrameTrans;

        // processed >= transmitted
        if (nFrameProc >= nFrameTrans) {
            return AX_TRUE;
        }
    }

    return AX_FALSE;
}

AX_BOOL CSKEL::SkelQueueFull(AX_VOID) {
    std::lock_guard<std::mutex> lck(m_mtxStatus);
    AX_U64 &nFrameProc = m_ctx.stRunningStatus.nFrameProc;
    AX_U64 &nFrameTrans = m_ctx.stRunningStatus.nFrameTrans;
    AX_U32 &nFrameDepth = m_ctx.stParam.nFrameDepth;

    // processed + depth <= transmitted
    if (nFrameProc + nFrameDepth <= nFrameTrans) {
        return AX_TRUE;
    }

    return AX_FALSE;
}

AX_VOID CSKEL::WaitQueueEmpty(AX_VOID) {
    WaitSkelQueueEmpty(-1);
}

AX_S32 CSKEL::SkelEnqueue(CSkelFrame *pSkelFrame, AX_S32 nTimeout/* = -1*/) {
    AX_S32 nRet = AXCL_ERR_SKEL_QUEUE_FULL;

    auto startTime = std::chrono::steady_clock::now();

    do {
        if (!SkelQueueFull()) {
            if (EnqueueFrame(pSkelFrame)) {
                {
                    std::lock_guard<std::mutex> lck(m_mtxStatus);
                    m_ctx.stRunningStatus.nFrameTrans ++;
                }

                nRet = AXCL_SKEL_SUCC;
            }
            break;
        }

        auto endTime = std::chrono::steady_clock::now();

        AX_S32 nElapsed = (AX_S32)(std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count());

        if ((nTimeout > 0) && (nElapsed >= nTimeout)) {
            nRet = AXCL_ERR_SKEL_TIMEOUT;
            break;
        }

        if (nTimeout != 0) {
            CTimeUtils::usSleep(1000);
        }
    } while (nTimeout != 0);

    return nRet;
}

AX_VOID CSKEL::WaitSkelQueueEmpty(AX_S32 nTimeout/* = -1*/) {
    // Wait frame empty
    auto waitStart = std::chrono::steady_clock::now();

    do {
        if (SkelQueueEmpty()) {
            break;
        }

        if (nTimeout >= 0) {
            auto waitEnd = std::chrono::steady_clock::now();
            AX_S32 nElapsed = (AX_U32)(std::chrono::duration_cast<std::chrono::milliseconds>(waitEnd - waitStart).count());

            if (nElapsed >= nTimeout) {
                break;
            }
        }

        CTimeUtils::msSleep(10);
    } while(1);
}

AX_S32 CSKEL::SendFrame(const AXCL_SKEL_FRAME_T *pstFrame, AX_S32 nTimeout) {
    return AXCL_SKEL_SUCC;
}

AXCL_SKEL_STAGE_STAT_E CSKEL::ProcessFrame(const CSkelFrame *pstFrame) {
    return AXCL_SKEL_STAGE_STAT_FINALIZE;
}

AX_VOID CSKEL::PostProcessHandler(const CSkelFrame *pstFrame,
                                            AXCL_SKEL_RESULT_T *pstResult,
                                            AX_U32 nOutBufferId,
                                            AX_BOOL bSync) {
}

AX_VOID CSKEL::SkelFrameRelease(CSkelFrame *pSkelFrame) {
}

AX_VOID CSKEL::ImplMemCallback(MEMMGR_ADDR pAddr, AX_VOID *pParam) {
}

AX_VOID CSKEL::ResultCallbackThread(AX_VOID)
{
    prctl(PR_SET_NAME, "skel_res_thread");

    /* create thread context */
    axclrtContext context;
    if (axclError ret = axclrtCreateContext(&context, m_ctx.nDeviceId); AXCL_SUCC != ret) {
        return;
    }

    AXCL_SKEL_RESULT_T *pstResult = nullptr;

    m_bResultThreadRunning = AX_TRUE;

    while (m_bResultThreadRunning) {
        pstResult = nullptr;

        {
            std::unique_lock<std::mutex> lck(m_mtxResultQueue);
            m_cvResultCome.wait(lck, [this]() { return (!m_qResult.empty() || !m_bResultThreadRunning); });

            if (m_qResult.size() > 0) {
                pstResult = m_qResult.front();
                m_qResult.pop();
            }
        }

        if (pstResult) {
            AXCL_SKEL_CALLBACK_T &stCallback = m_ctx.stCallback;

            if (stCallback.callback) {
                stCallback.callback(m_ctx.pHandle, pstResult, stCallback.pUserData);
            }

            {
                std::lock_guard<std::mutex> lck(m_mtxStatus);
                m_ctx.stRunningStatus.nFrameProc ++;
            }
        }
    }

    /* destory thread context */
    axclrtDestroyContext(context);
}

AX_S32 CSKEL::RegisterResultCallback(AXCL_SKEL_RESULT_CALLBACK_FUNC callback, AX_VOID *pUserData) {
    if (callback) {
        SKEL_RESULT_NOTIFY_ONE();

        if (!m_pResultThread) {
            m_pResultThread = new thread(&CSKEL::ResultCallbackThread, this);
        }
    }
    else {
        m_bResultThreadRunning = AX_FALSE;

        SKEL_RESULT_NOTIFY_ONE();

        if (m_pResultThread) {
            m_pResultThread->join();
            delete m_pResultThread;
            m_pResultThread = nullptr;
        }
    }

    {
        std::unique_lock<std::mutex> lck(m_mtxSetting);
        m_ctx.stCallback.callback = callback;
        m_ctx.stCallback.pUserData = pUserData;
    }

    return AXCL_SKEL_SUCC;
}

AX_S32 CSKEL::GetResult(AXCL_SKEL_RESULT_T **ppstResult, AX_S32 nTimeout) {
    CHECK_PTR(ppstResult);

    std::unique_lock<std::mutex> lck(m_mtxResultQueue);
    if (m_qResult.size() > 0) {
        *ppstResult = m_qResult.front();
        m_qResult.pop();

        if (*ppstResult) {
            std::lock_guard<std::mutex> lck(m_mtxStatus);
            m_ctx.stRunningStatus.nFrameProc ++;
        }

        return AXCL_SKEL_SUCC;
    } else if (nTimeout == 0) {
        return AXCL_ERR_SKEL_QUEUE_EMPTY;
    } else if (nTimeout < 0) {
        m_cvResultCome.wait(lck, [this]() { return (!m_qResult.empty() || !m_bProcessFrameWorking); });

        if (m_qResult.size() == 0) {
            return AXCL_ERR_SKEL_QUEUE_EMPTY;
        }

        *ppstResult = m_qResult.front();
        m_qResult.pop();

        if (*ppstResult) {
            std::lock_guard<std::mutex> lck(m_mtxStatus);
            m_ctx.stRunningStatus.nFrameProc ++;
        }
    } else {
        m_cvResultCome.wait_for(lck, std::chrono::milliseconds(nTimeout), [this]() { return (!m_qResult.empty()); });

        if (m_qResult.size() == 0) {
            return AXCL_ERR_SKEL_QUEUE_EMPTY;
        }

        *ppstResult = m_qResult.front();
        m_qResult.pop();

        if (*ppstResult) {
            std::lock_guard<std::mutex> lck(m_mtxStatus);
            m_ctx.stRunningStatus.nFrameProc ++;
        }
    }

    return AXCL_SKEL_SUCC;
}

AXCL_SKEL_PARAM_T CSKEL::GetConfig(AX_VOID) {
    std::unique_lock<std::mutex> lck(m_mtxSetting);

    return m_ctx.stParam;
}

AX_S32 CSKEL::SetConfig(AXCL_SKEL_PARAM_T &stConfig) {
    std::unique_lock<std::mutex> lck(m_mtxSetting);

    m_ctx.stParam = stConfig;

    SetPrivParam();

    return AXCL_SKEL_SUCC;
}

AX_S32 CSKEL::SetPrivParam(AX_VOID) {
    return AXCL_SKEL_SUCC;
}
