/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "usrworker_ipc.h"
#include <sys/msg.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <cstring>
#include <errno.h>
#include <ctime>
#include "axcl_logger.hpp"

#define TAG "ipc"

IPCBase::IPCBase() = default;

IPCBase::~IPCBase() {
    close_queue();
}

key_t IPCBase::generate_key(uint32_t key_id) {
    key_t key = ftok("/tmp", key_id);
    if (key == -1) {
        LOG_MM_E(TAG, "Failed to create IPC key: {}", strerror(errno));
    }
    return key;
}

int32_t IPCBase::create_queue(uint32_t key_id) {
    key_t key = generate_key(key_id);
    if (key == -1) {
        return -1;
    }

    m_msgid = msgget(key, IPC_CREAT | 0666);
    if (m_msgid == -1) {
        LOG_MM_E(TAG, "Failed to create message queue: {}", strerror(errno));
        return -1;
    }

    LOG_MM_I(TAG, "Created message queue, msgid: {}", m_msgid);
    return 0;
}

int32_t IPCBase::connect_queue(uint32_t key_id) {
    key_t key = generate_key(key_id);
    if (key == -1) {
        return -1;
    }

    m_msgid = msgget(key, 0666);
    if (m_msgid == -1) {
        LOG_MM_E(TAG, "Failed to connect to message queue: {}", strerror(errno));
        return -1;
    }

    LOG_MM_I(TAG, "Connected to message queue, msgid: {}", m_msgid);
    return 0;
}

void IPCBase::close_queue() {
    if (m_msgid != -1) {
        LOG_MM_I(TAG, "Closing message queue, msgid: {}", m_msgid);
        m_msgid = -1;
    }
}

int32_t IPCBase::send_message(const IPC_MSG_T& msg) {
    if (m_msgid == -1) {
        LOG_MM_E(TAG, "Message queue not initialized");
        return -1;
    }

    if (msgsnd(m_msgid, &msg, sizeof(msg) - sizeof(msg.pid), 0) == -1) {
        LOG_MM_E(TAG, "msgsnd failed: {}", strerror(errno));
        return -1;
    }

    return 0;
}

int32_t IPCBase::recv_message(IPC_MSG_T& msg, uint32_t target_pid, bool blocking) {
    if (m_msgid == -1) {
        LOG_MM_E(TAG, "Message queue not initialized");
        return -1;
    }

    if (msgrcv(m_msgid, &msg, sizeof(msg) - sizeof(msg.pid), target_pid, blocking ? 0 : IPC_NOWAIT) == -1) {
        if (errno == ENOMSG || errno == EAGAIN) {
            return -2; // No message available
        } else if (errno == EIDRM) {
            LOG_MM_I(TAG, "Message queue was removed");
            return -1;
        }
        LOG_MM_E(TAG, "msgrcv failed: {}", strerror(errno));
        return -1;
    }

    return 0;
}

ParentIPC::ParentIPC(uint32_t parent_pid) : m_parent_pid(parent_pid) {
}

ParentIPC::~ParentIPC() {
    deinit();
}

int32_t ParentIPC::init() {
    return create_queue(m_parent_pid);
}

void ParentIPC::deinit() {
    if (m_msgid != -1) {
        if (msgctl(m_msgid, IPC_RMID, nullptr) == -1) {
            LOG_MM_E(TAG, "Failed to remove message queue: {}", strerror(errno));
        } else {
            LOG_MM_I(TAG, "Message queue removed successfully");
        }
    }
    close_queue();
}

int32_t ParentIPC::send_init_message(uint32_t child_pid, const IPC_MSG_INIT_T& init_data) {
    IPC_MSG_T msg;
    memset(&msg, 0, sizeof(msg));

    msg.pid = child_pid;
    msg.body.pid = m_parent_pid;
    msg.body.type = IPC_MSG_TYPE_INIT;
    msg.body.init = init_data;

    return send_message(msg);
}

int32_t ParentIPC::receive_message(IPC_MSG_T& msg) {
    return recv_message(msg, m_parent_pid, false);
}

int32_t ParentIPC::send_ack(uint32_t child_pid, IPC_MSG_TYPE_E ack_type, int32_t status) {
    IPC_MSG_T ack_msg;
    memset(&ack_msg, 0, sizeof(ack_msg));

    ack_msg.pid = child_pid;
    ack_msg.body.pid = m_parent_pid;
    ack_msg.body.type = IPC_MSG_TYPE_ACK;
    ack_msg.body.ack.type = ack_type;
    ack_msg.body.ack.status = status;

    return send_message(ack_msg);
}

ChildIPC::ChildIPC() : m_parent_pid(0), m_child_pid(0) {
}

ChildIPC::~ChildIPC() {
    deinit();
}

int32_t ChildIPC::init() {
    pid_t parent_pid = getppid();
    if (parent_pid == -1) {
        LOG_MM_E(TAG, "Failed to get parent process ID: {}", strerror(errno));
        return -1;
    }

    m_parent_pid = static_cast<uint32_t>(parent_pid);
    m_child_pid = getpid();

    return connect_queue(m_parent_pid);
}

void ChildIPC::deinit() {
    close_queue();
}

int32_t ChildIPC::receive_init_message(IPC_MSG_T& msg) {
    return recv_message(msg, m_child_pid, false);
}

int32_t ChildIPC::send_ack(IPC_MSG_TYPE_E ack_type, int32_t status) {
    IPC_MSG_T ack_msg;
    memset(&ack_msg, 0, sizeof(ack_msg));

    ack_msg.pid = m_parent_pid;
    ack_msg.body.pid = m_child_pid;
    ack_msg.body.type = IPC_MSG_TYPE_ACK;
    ack_msg.body.ack.type = ack_type;
    ack_msg.body.ack.status = status;

    return send_message(ack_msg);
}

int32_t ChildIPC::send_heartbeat(int32_t timestamp, int32_t status) {
    IPC_MSG_T heartbeat_msg;
    memset(&heartbeat_msg, 0, sizeof(heartbeat_msg));

    heartbeat_msg.pid = m_parent_pid;
    heartbeat_msg.body.pid = m_child_pid;
    heartbeat_msg.body.type = IPC_MSG_TYPE_HEARTBEAT;
    heartbeat_msg.body.heartbeat.timestamp = timestamp;
    heartbeat_msg.body.heartbeat.status = status;

    return send_message(heartbeat_msg);
}

int32_t ChildIPC::receive_ack(IPC_MSG_T& msg) {
    return recv_message(msg, m_child_pid, false);
}