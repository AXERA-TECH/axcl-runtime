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
#include <string>
#include "axcl_sys.h"

namespace axcl::skel {
static inline uint32_t get_image_stride_w(const AX_VIDEO_FRAME_T* pImg) {
    if (pImg->u32PicStride[0] == 0) {
        return pImg->u32Width;
    }

    return pImg->u32PicStride[0];
}

static inline int get_image_data_size(const AX_VIDEO_FRAME_T* img) {
    int stride_w = get_image_stride_w(img);
    switch (img->enImgFormat) {
        case AX_FORMAT_YUV420_SEMIPLANAR:  // FIXME
        case AX_FORMAT_YUV420_SEMIPLANAR_VU:
            return int(stride_w * img->u32Height * 3 / 2);

        case AX_FORMAT_RGB888:
        case AX_FORMAT_BGR888:
        case AX_FORMAT_YUV444_SEMIPLANAR:
        case AX_FORMAT_YUV444_SEMIPLANAR_VU:
            return int(stride_w * img->u32Height * 3);

        case AX_FORMAT_ARGB8888:
        case AX_FORMAT_RGBA8888:
            return int(stride_w * img->u32Height * 4);

        case AX_FORMAT_YUV400:
            return int(stride_w * img->u32Height * 1);

        default:
            fprintf(stderr, "[ERR] unsupported color space %d to calculate image data size\n", (int)img->enImgFormat);
            return 0;
    }
}

typedef struct axTHREAD_ATTR_T
{
    AX_BOOL bExplicitSched;
    AX_S32 nSchedPolicy;
    AX_U32 nSchedPriority;  /* Range:[1, 99]; encode thread scheduling priority.*/

    axTHREAD_ATTR_T() {
        bExplicitSched = AX_FALSE;
        nSchedPolicy = SCHED_OTHER;
        nSchedPriority = 0;
    }
} THREAD_ATTR_T;

static inline void set_thread_setschedparam(std::thread *pthread, const THREAD_ATTR_T &stThreadAttr) {
    if (pthread) {
        if (stThreadAttr.bExplicitSched) {
            sched_param sch;
            int policy;
            pthread_getschedparam(pthread->native_handle(), &policy, &sch);
            sch.sched_priority = stThreadAttr.nSchedPriority;
            pthread_setschedparam(pthread->native_handle(), stThreadAttr.nSchedPolicy, &sch);
        }
    }
}
}  // namespace axcl::skel

using namespace axcl::skel;
