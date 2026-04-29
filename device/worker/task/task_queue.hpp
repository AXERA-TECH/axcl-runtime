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

#include <mutex>
#include <queue>
#include <memory>
#include <stdexcept>
#include <string>
#include "task.hpp"
#include "axcl_logger.hpp"
#include "runtime_api.hpp"
#include "axcl_package.hpp"

namespace axcl::worker {

class stream;
struct channel_data;
class TaskError {
public:
    TaskError() = default;

    TaskError(const std::string& error_type, const std::string& error_message, int error_code = 0)
        : m_error_type(error_type), m_error_message(error_message), m_error_code(error_code) {}

    bool has_error() const { return !m_error_type.empty(); }
    const std::string& error_type() const { return m_error_type; }
    const std::string& error_message() const { return m_error_message; }
    int error_code() const { return m_error_code; }

    void clear() {
        m_error_type.clear();
        m_error_message.clear();
        m_error_code = 0;
    }

private:
    std::string m_error_type;
    std::string m_error_message;
    int m_error_code = 0;
};

class task_queue {
public:
    virtual ~task_queue() = default;
    virtual void push(std::shared_ptr<task> task) = 0;
    virtual std::shared_ptr<task> pop() = 0;
    virtual bool empty() const = 0;
    virtual size_t size() const = 0;
    virtual void clear() = 0;
    virtual TaskError last_error() const = 0;
    virtual void execute_task(std::shared_ptr<task> task) = 0;
    virtual void set_error(const std::string& error_type, const std::string& error_message, int error_code) = 0;
    virtual void clear_error() = 0;
    virtual bool should_execute_task() const = 0;
    virtual void set_stream(stream* stream_ptr) = 0;
    virtual stream* get_stream() const = 0;
};

class standard_task_queue : public task_queue {
public:
    standard_task_queue() = default;

    void push(std::shared_ptr<task> task) override;

    std::shared_ptr<task> pop() override {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_tasks.empty()) {
            return nullptr;
        }
        auto task = m_tasks.front();
        m_tasks.pop();
        return task;
    }

    bool empty() const override {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_tasks.empty();
    }

    size_t size() const override {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_tasks.size();
    }

    void clear() override {
        std::unique_lock<std::mutex> lock(m_mutex);
        std::queue<std::shared_ptr<task>> empty;
        std::swap(m_tasks, empty);
    }

    TaskError last_error() const override {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_last_error;
    }

    void set_error(const std::string& error_type, const std::string& error_message, int error_code) override {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_last_error = TaskError(error_type, error_message, error_code);
    }

    void clear_error() override {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_last_error.clear();
    }

    bool should_execute_task() const override {
        std::unique_lock<std::mutex> lock(m_mutex);
        return !m_last_error.has_error();
    }

    void execute_task(std::shared_ptr<task> task) override;

    void set_stream(stream* stream_ptr) override {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_stream = stream_ptr;
    }

    stream* get_stream() const override {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_stream;
    }

private:
    std::queue<std::shared_ptr<task>> m_tasks;
    mutable std::mutex m_mutex;
    TaskError m_last_error;
    stream* m_stream{nullptr};
};

} // namespace axcl