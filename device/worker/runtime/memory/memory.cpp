/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "memory.hpp"
#include "dispatcher.hpp"
#include "axcl_logger.hpp"
#include "ax_dmadim_api.h"
#include "axcl_comm.hpp"

#include "os.hpp"

#define TAG "memory"


namespace axcl::worker {

template<>
bool runtime_api<axcl::pkg::runtime::memory::API,
                axcl::pkg::device::memory_response,
                axcl::pkg::device::memory_request>::handle_ack_err(
    const axcl::pkg::device::memory_response& decoder, int32_t ret) {

    auto noresp_flag = decoder.get_noresp_flag();
    if (noresp_flag == 0) {
        return false;
    }

    auto api = decoder.get_api();
    if (ret != 0) {
        throw RuntimeApiException(ret, m_module, static_cast<int32_t>(api));
    }

    return true;
}

memory::memory(dispatcher *dispatcher) : base(dispatcher), m_api(TAG) {
    m_api.append({
        RUNTIME_MEMORY_API_APPEND(ALLOC),
        RUNTIME_MEMORY_API_APPEND(FREE),
        RUNTIME_MEMORY_API_APPEND(FLUSH),
        RUNTIME_MEMORY_API_APPEND(INVALIDATE),
        RUNTIME_MEMORY_API_APPEND(SET),
        RUNTIME_MEMORY_API_APPEND(WRITE),
        RUNTIME_MEMORY_API_APPEND(READ),
        RUNTIME_MEMORY_API_APPEND(SYNC),
        RUNTIME_MEMORY_API_APPEND(CMP),
    });
}

void memory::run(const void *data, const size_t& size) {
    m_api.run(get_dispatcher(), data, size);
}

void memory::insert(const uint64_t& phyaddr, void* pviraddr) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_map[phyaddr] = pviraddr;
}

int32_t memory::free(const uint64_t& phyaddr) {
    AX_VOID *pviraddr = nullptr;

    {
        std::lock_guard<std::mutex> lock(m_mutex);

        auto it = m_map.find(phyaddr);
        if (it == m_map.end()) {
            return -1;
        }

        pviraddr = it->second;

        m_map.erase(phyaddr);
    }

    return ::AX_SYS_MemFree(phyaddr, pviraddr);
}

void* memory::find(const uint64_t& phyaddr) {
    std::lock_guard<std::mutex> lock(m_mutex);

    auto it = m_map.find(phyaddr);
    if (it == m_map.end()) {
        return nullptr;
    }

    return it->second;
}

RUNTIME_MEMORY_API_IMPL(ALLOC) {
    uint32_t size = response->get_size();
    uint32_t policy = response->get_policy();

    axcl::pkg::runtime::memory::CHACHE_TYPE CacheType = response->get_cache_type();

    std::string token = "axcl_p"
                        + std::to_string(policy)
                        + "_c"
                        + std::to_string(static_cast<int32_t>(CacheType))
                        + "_s"
                        + std::to_string(size);

    AX_S32 ret = -1;
    AX_U64 phyaddr;
    AX_VOID *pviraddr;

    if (axcl::pkg::runtime::memory::CHACHE_TYPE::CACHED == CacheType) {
        if (3 == policy) {
            ret = ::AX_SYS_MemAllocCachedAlign(&phyaddr, &pviraddr, size, size, (AX_S8 *)token.c_str());
        } else {
            ret = ::AX_SYS_MemAllocCached(&phyaddr, &pviraddr, size, 128, (AX_S8 *)token.c_str());
        }

    } else {
        if (3 == policy) {
            ret = ::AX_SYS_MemAllocAlign(&phyaddr, &pviraddr, size, size, (AX_S8 *)token.c_str());
        } else {
            ret = ::AX_SYS_MemAlloc(&phyaddr, &pviraddr, size, 128, (AX_S8 *)token.c_str());
        }
    }

    if (!ret && phyaddr && pviraddr) {
        request->set_addr(reinterpret_cast<AX_VOID *>(phyaddr));
        insert(phyaddr, pviraddr);
    } else {
        request->set_addr(0);
    }

    return ret;
}

RUNTIME_MEMORY_API_IMPL(FREE) {
    uint64_t phyaddr = response->get_addr();
    return free(phyaddr);
}

RUNTIME_MEMORY_API_IMPL(FLUSH) {

    AX_S32 ret = -1;
    uint64_t phyaddr = response->get_addr();
    uint32_t size = response->get_size();

    bool mapped = false;
    void* pviraddr = find(phyaddr);

    if (!pviraddr) {
        pviraddr = ::AX_SYS_Mmap(phyaddr, size);
        mapped = true;
    }

    if (pviraddr) {
        ret = ::AX_SYS_MflushCache(phyaddr, pviraddr, size);

        if (mapped) {
            ret = ::AX_SYS_Munmap(pviraddr, size);
        }
    }

    return ret;
}

RUNTIME_MEMORY_API_IMPL(INVALIDATE) {

    AX_S32 ret = -1;
    uint64_t phyaddr = response->get_addr();
    uint32_t size = response->get_size();

    bool mapped = false;
    void* pviraddr = find(phyaddr);

    if (!pviraddr) {
        pviraddr = ::AX_SYS_Mmap(phyaddr, size);
        mapped = true;
    }

    if (pviraddr) {
        ret = ::AX_SYS_MinvalidateCache(phyaddr, pviraddr, size);

        if (mapped) {
            ret = ::AX_SYS_Munmap(pviraddr, size);
        }
    }

    return ret;
}

RUNTIME_MEMORY_API_IMPL(SET) {
    AX_S32 ret = -1;
    uint64_t phyaddr = response->get_addr();
    uint32_t size = response->get_size();
    uint8_t value = response->get_value();

    if ((phyaddr % 8) == 0 && (size % 8) == 0) {
        ret = ::AX_DMA_MemSet(phyaddr, value, size);
    } else {
        bool mapped = false;
        void* pviraddr = find(phyaddr);

        if (!pviraddr) {
            pviraddr = ::AX_SYS_Mmap(phyaddr, size);
            mapped = true;
        }

        if (pviraddr) {
            memset(pviraddr, value, size);
            ret = 0;

            if (mapped) {
                ret = ::AX_SYS_Munmap(pviraddr, size);
            }
        }
    }

    return ret;
}

// AXCL_MEMCPY_HOST_TO_DEVICE
RUNTIME_MEMORY_API_IMPL(WRITE) {

    AX_S32 ret = -1;
    uint64_t dst = response->get_addr();
    uint64_t src = response->get_from();
    uint64_t phy = response->get_phy();
    uint32_t size = response->get_size();
    uint64_t kind = response->get_kind();

    axcl::pkg::head* hd = const_cast<axcl::pkg::head*>(static_cast<const axcl::pkg::head*>(data));
    auto comm = this->get_dispatcher()->get_comm();
    if (comm) {


        comm->memcpy_async(hd, 0, -1);

        m_memcpy_package.hd = *hd;
        m_memcpy_package.hd.type = RUNTIME_MEMCPY_ASYNC_HANDLER_TYPE;
        m_memcpy_package.kind = static_cast<axcl::pkg::memcpy_kind>(kind);
        m_memcpy_package.dst_phy = dst;
        m_memcpy_package.size = size;
        m_memcpy_package.src_phy = phy;
        if (axcl::pkg::memcpy_kind::HOST_VIR_TO_DEVICE == m_memcpy_package.kind) {
            m_memcpy_package.data = reinterpret_cast<void*>(src);
        } else {
            m_memcpy_package.src_phy = src;
        }
        comm->send(reinterpret_cast<void*>(&m_memcpy_package), sizeof(m_memcpy_package), axcl::comm::PCIE_DMA, -1);

        if (!comm->memcpy_async(hd, 1, -1)) {
            ret = -1;
        } else {
            ret = 0;
        }
    }
    return ret;
}

// AXCL_MEMCPY_DEVICE_TO_HOST
RUNTIME_MEMORY_API_IMPL(READ) {

    AX_S32 ret = -1;
    uint64_t dst = response->get_addr();
    uint64_t src = response->get_from();
    uint64_t phy = response->get_phy();
    uint32_t size = response->get_size();
    uint64_t kind = response->get_kind();

    axcl::pkg::head* hd = const_cast<axcl::pkg::head*>(static_cast<const axcl::pkg::head*>(data));
    auto comm = this->get_dispatcher()->get_comm();
    if (comm) {
        comm->memcpy_async(hd, 0, -1);
        m_memcpy_package.hd = *hd;
        m_memcpy_package.hd.type = RUNTIME_MEMCPY_ASYNC_HANDLER_TYPE;
        m_memcpy_package.kind = static_cast<axcl::pkg::memcpy_kind>(kind);
        m_memcpy_package.src_phy = src;
        m_memcpy_package.size = size;
        m_memcpy_package.dst_phy = phy;

        if (axcl::pkg::memcpy_kind::DEVICE_TO_HOST_VIR == m_memcpy_package.kind) {
            m_memcpy_package.data = reinterpret_cast<void*>(dst);
        } else {
            m_memcpy_package.dst_phy = dst;
        }
        comm->send(reinterpret_cast<void*>(&m_memcpy_package), sizeof(m_memcpy_package), axcl::comm::PCIE_DMA, -1);
        if (!comm->memcpy_async(hd, 1, -1)) {
            ret = -1;
        } else {
            ret = 0;
        }
    }
    return ret;
}

// AXCL_MEMCPY_DEVICE_TO_DEVICE
RUNTIME_MEMORY_API_IMPL(SYNC) {
    AX_S32 ret = -1;
    uint64_t from = response->get_from();
    uint64_t to = response->get_addr();
    uint32_t size = response->get_size();

    ret = ::AX_DMA_MemCopy(to, from, size);

    return ret;
}

RUNTIME_MEMORY_API_IMPL(CMP) {
    uint64_t s1 = response->get_from();
    uint64_t s2 = response->get_addr();
    uint32_t size = response->get_size();

    AX_VOID *v1 = ::AX_SYS_Mmap(s1, size);
    AX_VOID *v2 = ::AX_SYS_Mmap(s2, size);

    AX_S32 ret = ::memcmp(v1, v2, size);

    ::AX_SYS_Munmap(v1, size);
    ::AX_SYS_Munmap(v2, size);

    return ret;
}

}  // namespace axcl::worker
