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

#define ENGINE_ERR_NULL_PTR       "Null pointer({})."

#define ENGINE_ERR_ENCODE_FAIL    "Encode api({}) failed."
#define ENGINE_ERR_DECODE_FAIL    "Decode api({}) response failed."
#define ENGINE_ERR_REQUEST_FAIL   "Request api({}) return failed({})."

#define ENGINE_ERR_MODEL_ID       "Invalid model id({})."
#define ENGINE_ERR_MODEL_SIZE     "Invalid model size({})."
#define ENGINE_ERR_MODEL_PATH     "Model file({}) not found."
#define ENGINE_ERR_MODEL_MALLOC   "Malloc for model({}) failed."
#define ENGINE_ERR_MODEL_COPY     "Send model({}) to device failed."
#define ENGINE_ERR_LOAD_FAIL      "Load model failed."
#define ENGINE_ERR_LOAD_FILE_FAIL "Load model file({}) failed."
#define ENGINE_ERR_MAP_FILE_FAIL  "Map model file({}) failed."

#define ENGINE_ERR_CONTEXT_ID     "Invalid context id({})."

#define ENGINE_ERR_PARAM          "Invalid {}({})."

#define ENGINE_ERR_BUFFER_SIZE    "Invalid buffer size({})."
