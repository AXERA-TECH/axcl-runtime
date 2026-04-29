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

#include "axcl_base.h"
#include <cstddef>
#include <cstdint>
#include <memory>
#include <variant>

namespace axcl::comm {

enum class COMM_TYPE {
    COMM_TYPE_PCIE = 0,
    COMM_TYPE_SOCKET = 1,
    COMM_TYPE_BUTT,
};

enum class SOCKET_TYPE {
    SOCKET_TYPE_SERVER = 0,
    SOCKET_TYPE_CLIENT = 1,
    SOCKET_TYPE_BUTT,
};

enum {
    PCIE_MSG = 0,
    PCIE_DMA = 1,
};

#define MAX_PCIE_MSG_SIZE (1024)

typedef struct {
    union {
        struct {
            bool handshake;
            int32_t target;  //!< rc: slot id of ep, ep: <= 0
            int32_t port[4];
        } pcie;
        struct {
            SOCKET_TYPE type;
            char ip[24];
            int32_t port;
        } socket;
    };
} comm_args_t;

using attr = std::variant<int32_t, uint32_t, COMM_TYPE>;

/**
 * @brief
 *
 */
class icomm {
public:
    virtual ~icomm() = default;

    virtual bool open(const comm_args_t &args) = 0;
    virtual void close() = 0;

    /**
     * @brief send data
     * @param buf [IN] specifies buffer of data to send
     * @param size [IN] data size in bytes to send
     * @param flag [IN] PCIE_MSG/PCIE_DMA
     * @param timeout [IN] millseconds
     * @return bytes sent, if <= size, usually indicates transmit error happen
     */
    virtual size_t send(const void *buf, size_t size, int64_t flag, int32_t timeout) = 0;

    /**
     * @brief recv data
     * @param buf [OUT] buffer contains recv data, remind to invoke free API to free memory
     * @param flag [IN] PCIE_MSG/PCIE_DMA
     * @param timeout [IN]  millseconds
     * @return bytes recived, if <= 0, usually indicates transmit error happen
     */
    virtual size_t recv(void **buf, int64_t flag, int32_t timeout) = 0;

    /**
     * @brief Perform asynchronous memory copy between host and device.
     * @param flag [IN] Operation flag:
     *                  0 - Start the asynchronous memory copy operation.
     *                  1 - Wait for the previously started asynchronous copy to complete.
     * @param timeout [IN] Timeout in milliseconds:
     *                  0  - Return immediately without waiting.
     *                 -1  - Wait indefinitely until the operation completes.
     *                 >0  - Wait for the specified number of milliseconds.
     * @return true if the operation succeeds, false otherwise.
     */
    virtual bool memcpy_async(void* hd, int flag, int32_t timeout) = 0;

    /**
     * @brief free memory of recv buffer
     */
    virtual void free(void *buf) = 0;

    virtual void clear() = 0;

    virtual bool get_attr(const char *name, attr &value) = 0;
    virtual bool set_attr(const char *name, const attr &value) = 0;

public:
    /**
     * attr name: "remote_devices", value: std::vector<int32_t>
     */
    static constexpr const char *COMM_TYPE_ATTR = "comm.type";
    static constexpr int64_t FLAG_RAWDATA = 0x82658768658465; /* 'R'A'W'D'A'T'A' */

    static constexpr const char *DMA_BUF_SIZE_ATTR = "comm.attr.dma_buf_size";
};

}  // namespace axcl::comm

/**
 * @brief export API
 */
AXCL_EXPORT std::shared_ptr<axcl::comm::icomm> axcl_create_comm_instance(axcl::comm::COMM_TYPE type);
