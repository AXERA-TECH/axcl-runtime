/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "dsp.hpp"

#include "dispatcher.hpp"
#include "res_guard.hpp"

#include "ax_dsp_api.h"

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#define TAG "dsp"
#define SHM_NAME_BASE "/axcl_dsp_"

#define CHECK_HALT(exp, ret, msg, ...) {                        \
    if (!(exp)) {                                               \
        LOG_MM_E(TAG, msg, ##__VA_ARGS__);                      \
        std::ignore = request->pack(response->get_api(), ret);  \
        return false;                                           \
    }                                                           \
}

namespace axcl::worker {

dsp::dsp(dispatcher *dispatcher) : base(dispatcher), m_api(TAG) {
    LOG_MM_D(TAG, "axcl dsp constructed...");
    m_api.append({
        NATIVE_DSP_API_APPEND(AX_DSP_PowerOn),
        NATIVE_DSP_API_APPEND(AX_DSP_PowerOff),
        NATIVE_DSP_API_APPEND(AX_DSP_LoadBin),
        NATIVE_DSP_API_APPEND(AX_DSP_EnableCore),
        NATIVE_DSP_API_APPEND(AX_DSP_DisableCore),
        NATIVE_DSP_API_APPEND(AX_DSP_PRC),
        NATIVE_DSP_API_APPEND(AX_DSP_Query),
    });
}

void dsp::run(const void *data, const size_t& size) {
    LOG_MM_D(TAG, "axcl dsp run...");
    m_api.run(get_dispatcher(), data, size);
}

NATIVE_DSP_API_IMPL(AX_DSP_PowerOn) {
    AX_DSP_ID_E enDspId;

    if (!response->unpack(response->get_api(), &enDspId)) {
        return false;
    }

    if (const AX_S32 ret = ::AX_DSP_PowerOn(enDspId); !request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_DSP_API_IMPL(AX_DSP_PowerOff) {
    AX_DSP_ID_E enDspId;

    if (!response->unpack(response->get_api(), &enDspId)) {
        return false;
    }

    if (const AX_S32 ret = ::AX_DSP_PowerOff(enDspId); !request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_DSP_API_IMPL(AX_DSP_LoadBin) {
    AX_DSP_ID_E enDspId;
    uint64_t addr;
    uint32_t size;
    AX_DSP_MEM_TYPE_E enMemType;

    if (!response->unpack(response->get_api(), &enDspId, &addr, &size, &enMemType)) {
        return false;
    }

    // 0. get pid and generate shm name
    const auto pid = response->get_env().pid;
    const auto shm_name = std::string(SHM_NAME_BASE) + std::to_string(pid);

    // 1. shm open lambda
    using shm_pair = std::pair<std::string, int>;
    auto make_shm_pair = [](const std::string& name) {
        auto fd = ::shm_open(name.c_str(), O_CREAT | O_RDWR, 0666);
        return std::make_pair(name, fd);
    };

    // 2. open shm
    auto shm_fd_holder = res_guard<std::pair<std::string, int>>(
        make_shm_pair(shm_name),
        [](const shm_pair& pair) { if (pair.second != -1) ::shm_unlink(pair.first.c_str()); }
    );

    // 3. check shm
    CHECK_HALT(-1 != shm_fd_holder.get().second, AX_DSP_READ_FAIL, "Call shm_open() failed.");

    // 4. truncate shm
    CHECK_HALT(0 == ::ftruncate(shm_fd_holder.get().second, size), AX_DSP_READ_FAIL, "Call ftruncate() for shm failed.");

    // 5. mmap shm
    auto shm_map_holder = res_guard<void*>(
        ::mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd_holder.get().second, 0),
        [&size](void* ptr) {
            if (nullptr != ptr) {::munmap(ptr, size);}
        }
    );

    // 6. check mmap for shm
    CHECK_HALT(MAP_FAILED != shm_map_holder.get(), AX_DSP_READ_FAIL, "Call mmap for shm failed.");

    // 7. map phy to vir
    auto vir_map_holder = res_guard<void*>(
        ::AX_SYS_Mmap(addr, size),
        [&size](void* ptr) {
            if (nullptr != ptr) {::AX_SYS_Munmap(ptr, size);}
        }
    );

    // 8. check mmap for vir
    CHECK_HALT(nullptr != vir_map_holder.get(), AX_DSP_READ_FAIL, "Call AX_SYS_Mmap() for {:#x} failed.", addr);

    // 9. copy data
    ::memcpy(shm_map_holder.get(), vir_map_holder.get(), size);

    // 10. load bin
    const auto shm_file_name = std::string("/dev/shm") + shm_name;
    if (const AX_S32 ret = ::AX_DSP_LoadBin(enDspId, shm_file_name.c_str(), enMemType);
        !request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_DSP_API_IMPL(AX_DSP_EnableCore) {
    AX_DSP_ID_E enDspId;

    if (!response->unpack(response->get_api(), &enDspId)) {
        return false;
    }

    if (const AX_S32 ret = ::AX_DSP_EnableCore(enDspId); !request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_DSP_API_IMPL(AX_DSP_DisableCore) {
    AX_DSP_ID_E enDspId;

    if (!response->unpack(response->get_api(), &enDspId)) {
        return false;
    }

    if (const AX_S32 ret = ::AX_DSP_DisableCore(enDspId); !request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_DSP_API_IMPL(AX_DSP_PRC) {
    AX_DSP_MESSAGE_T pstMsg;
    AX_DSP_ID_E enDspId;
    AX_DSP_PRI_E enPri;

    if (!response->unpack(response->get_api(), &pstMsg, &enDspId, &enPri)) {
        return false;
    }

    AX_DSP_HANDLE handle;
    if (const AX_S32 ret = ::AX_DSP_PRC(&handle, &pstMsg, enDspId, enPri); !request->pack(response->get_api(), ret, handle)) {
        return false;
    }

    return true;
}

NATIVE_DSP_API_IMPL(AX_DSP_Query) {
    AX_DSP_ID_E enDspId;
    AX_DSP_HANDLE hHandle;
    AX_BOOL bBlock;

    if (!response->unpack(response->get_api(), &enDspId, &hHandle, &bBlock)) {
        return false;
    }

    AX_DSP_MESSAGE_T pstMsg{};
    if (const AX_S32 ret = ::AX_DSP_Query(enDspId, hHandle, &pstMsg, bBlock); !request->pack(response->get_api(), ret, pstMsg)) {
        return false;
    }

    return true;
}

}  // namespace axcl::worker
