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

#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>
#include "def.hpp"
#include "axcl_rt_memory.h"

namespace axcl::skel {
static inline bool file_exist(const std::string& path) {
    auto flag = false;

    std::fstream fs(path, std::ios::in | std::ios::binary);
    flag = fs.is_open();
    fs.close();

    return flag;
}

static inline bool load_file(const std::string& path, AX_VOID **pModelBufferVirAddr, AX_U32 &nModelBufferSize) {
    int fd;

    if ((fd = open(path.c_str(), O_RDONLY)) < 0) {
        return false;
    }

    struct stat st;

    if (fstat(fd, &st) == -1) {
        close(fd);
        return false;
    }

    AX_VOID *buffer;

    if ((buffer = mmap(nullptr, st.st_size, PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED) {
        close(fd);
        return false;
    }

    AX_VOID* devMem = nullptr;
    axclrtMalloc(&devMem, st.st_size, AXCL_MEM_MALLOC_NORMAL_ONLY);

    if (!devMem) {
        munmap(buffer, st.st_size);
        close(fd);
        return false;
    }

    axclrtMemcpy(devMem, buffer, st.st_size, AXCL_MEMCPY_HOST_TO_DEVICE);

    *pModelBufferVirAddr = devMem;
    nModelBufferSize = (AX_U32)st.st_size;

    munmap(buffer, st.st_size);
    close(fd);

    return true;
}

static inline AX_VOID unload_file(AX_VOID *pModelBufferVirAddr, const AX_U32& nModelBufferSize) {
    axclrtFree(pModelBufferVirAddr);
}

static inline bool read_file(const std::string& path, std::vector<char>& data) {
    std::fstream fs(path, std::ios::in | std::ios::binary);

    if (!fs.is_open()) {
        return false;
    }

    fs.seekg(std::ios::end);
    auto fs_end = fs.tellg();
    fs.seekg(std::ios::beg);
    auto fs_beg = fs.tellg();

    auto file_size = static_cast<size_t>(fs_end - fs_beg);
    auto vector_size = data.size();

    data.reserve(vector_size + file_size);
    data.insert(data.end(), std::istreambuf_iterator<char>(fs), std::istreambuf_iterator<char>());

    fs.close();

    return true;
}

static inline bool dump_file(const std::string& path, const AX_VIDEO_FRAME_T &stFrame) {
    std::fstream fs(path, std::ios::out | std::ios::binary);

    if (!fs.is_open() || fs.fail()) {
        fprintf(stderr, "[ERR] cannot open file %s \n", path.c_str());
        return false;
    }

    AX_VOID *dst = nullptr;

    axclrtMallocHost(&dst, stFrame.u32FrameSize);

    if (!dst) {
        fs.close();
        return false;
    }

    AX_VOID *src = reinterpret_cast<AX_VOID *>(stFrame.u64VirAddr[0]);

    axclrtMemcpy(dst, src, stFrame.u32FrameSize, AXCL_MEMCPY_DEVICE_TO_HOST);

    fs.write((AX_CHAR *)dst, stFrame.u32FrameSize);

    fs.close();

    axclrtFreeHost(dst);

    return true;
}
}  // namespace axcl::skel

using namespace axcl::skel;
