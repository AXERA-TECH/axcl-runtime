/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#ifndef WORKER_DEF_H
#define WORKER_DEF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define MAX_PORT_NUM 4

// ACK wait flags for IPC communication
#define ACK_FLAG_INIT     (1 << 0)  // Configuration ACK flag (bit 0)

// IPC message types for parent-child communication
typedef enum {
    IPC_MSG_TYPE_INIT = 1,        // Parent -> Child: Initialize worker
    IPC_MSG_TYPE_HEARTBEAT = 2,   // Child -> Parent: Heartbeat message
    IPC_MSG_TYPE_ACK = 3,         // Child -> Parent: Acknowledgment
    IPC_MSG_TYPE_MAX = 4,
} IPC_MSG_TYPE_E;

// Child -> Parent: Acknowledgment message
typedef struct {
    IPC_MSG_TYPE_E type;          // Type of message being acknowledged
    int32_t status;               // 0: success, other: failed
} IPC_MSG_ACK_T;

// Parent -> Child: Initialization message
typedef struct {
    uint32_t heartbeat_interval;  // Heartbeat interval in milliseconds
    uint32_t port_num;            // Number of ports to configure
    uint32_t ports[MAX_PORT_NUM]; // Port configuration array
} IPC_MSG_INIT_T;

// Child -> Parent: Heartbeat message
typedef struct {
    int32_t timestamp;            // Heartbeat packet timestamp
    int32_t status;               // 0: success, other: failed
} IPC_MSG_HEARTBEAT_T;

// Union for different message body types
typedef struct {
    int32_t pid;                 // Sender process ID
    IPC_MSG_TYPE_E type;
    union
    {
        IPC_MSG_INIT_T init;
        IPC_MSG_HEARTBEAT_T heartbeat;
        IPC_MSG_ACK_T ack;
    };
} IPC_MSG_BODY_T;

// Complete IPC message structure
typedef struct {
    long pid;                     // Receiver process ID
    IPC_MSG_BODY_T body;          // Message body
} IPC_MSG_T;

#ifdef __cplusplus
}
#endif

#endif // WORKER_DEF_H