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

#include "npu_io_holder.hpp"

#include <mutex>
#include <memory>

extern std::mutex g_npu_map_mutex;
extern std::shared_ptr<info_map_holder> g_npu_map;
