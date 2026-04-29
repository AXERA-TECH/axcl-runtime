/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/
#ifndef USRWORKER_IPC_H
#define USRWORKER_IPC_H

#include <cstdint>
#include <sys/types.h>
#include "usrworker_def.h"


class IPCBase {
public:
    IPCBase();
    virtual ~IPCBase();

    IPCBase(const IPCBase&) = delete;
    IPCBase& operator=(const IPCBase&) = delete;

protected:
    int32_t create_queue(uint32_t key_id);
    int32_t connect_queue(uint32_t key_id);
    void close_queue();

    int32_t send_message(const IPC_MSG_T& msg);
    int32_t recv_message(IPC_MSG_T& msg, uint32_t target_pid, bool blocking = false);

protected:
    int m_msgid{-1};

private:
    key_t generate_key(uint32_t key_id);
};

class ParentIPC : public IPCBase {
public:
    explicit ParentIPC(uint32_t parent_pid);
    ~ParentIPC();

    int32_t init();
    void deinit();

    int32_t send_init_message(uint32_t child_pid, const IPC_MSG_INIT_T& init_data);

    int32_t receive_message(IPC_MSG_T& msg);

    int32_t send_ack(uint32_t child_pid, IPC_MSG_TYPE_E ack_type, int32_t status);

private:
    uint32_t m_parent_pid;
};

class ChildIPC : public IPCBase {
public:
    ChildIPC();
    ~ChildIPC();

    int32_t init();
    void deinit();

    int32_t receive_init_message(IPC_MSG_T& msg);

    int32_t send_ack(IPC_MSG_TYPE_E ack_type, int32_t status);

    int32_t send_heartbeat(int32_t timestamp, int32_t status);

    int32_t receive_ack(IPC_MSG_T& msg);

private:
    uint32_t m_parent_pid;
    uint32_t m_child_pid;
};

#endif // USRWORKER_IPC_H