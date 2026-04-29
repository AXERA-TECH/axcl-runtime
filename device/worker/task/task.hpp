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

#include <functional>
#include <future>
#include <memory>
#include <tuple>
#include <type_traits>

namespace axcl::worker {

class task {
public:
    virtual ~task() = default;
    virtual void execute() = 0;
    virtual bool is_data_task() const { return false; }
    virtual void* get_data_ptr() const { return nullptr; }
};

template<typename DataType, typename ReturnType = void>
class data_task : public task {
public:
    template<class F, class... Args>
    data_task(F&& f, std::shared_ptr<DataType> data, Args&&... args)
        : m_data(data),
          m_packaged_task([func = std::forward<F>(f),
                          data_ptr = data,
                          args_tuple = std::make_tuple(std::forward<Args>(args)...)]() mutable -> ReturnType {
              return std::apply([&func, &data_ptr](auto&&... args) -> ReturnType {
                  return func(data_ptr, std::forward<decltype(args)>(args)...);
              }, args_tuple);
          }) {}

    void execute() override {
        if (m_data) {
            m_packaged_task();
        }
    }

    std::future<ReturnType> get_future() {
        return m_packaged_task.get_future();
    }

    void* get_data() const {
        return m_data.get();
    }

    size_t get_data_size() const {
        return sizeof(DataType);
    }

    std::shared_ptr<DataType> get_shared_data() const {
        return m_data;
    }

    bool is_data_task() const override { return true; }
    void* get_data_ptr() const override { return m_data.get(); }

private:
    std::shared_ptr<DataType> m_data;
    std::packaged_task<ReturnType()> m_packaged_task;
};

template<typename DataType>
class data_task<DataType, void> : public task {
public:
    template<class F, class... Args>
    data_task(F&& f, std::shared_ptr<DataType> data, Args&&... args)
        : m_data(data),
          m_func([func = std::forward<F>(f),
                 data_ptr = data,
                 args_tuple = std::make_tuple(std::forward<Args>(args)...)]() mutable {
              std::apply([&func, &data_ptr](auto&&... args) {
                  func(data_ptr, std::forward<decltype(args)>(args)...);
              }, args_tuple);
          }) {}

    void execute() override {
        if (m_data) {
            m_func();
        }
    }

    void* get_data() const {
        return m_data.get();
    }

    size_t get_data_size() const {
        return sizeof(DataType);
    }

    std::shared_ptr<DataType> get_shared_data() const {
        return m_data;
    }

    bool is_data_task() const override {
        return true;
    }

    void* get_data_ptr() const override {
        return m_data.get();
    }

private:
    std::shared_ptr<DataType> m_data;
    std::function<void()> m_func;
};

template<typename DataType, typename ReturnType = void, class F, class... Args>
auto make_data_task(F&& f, std::shared_ptr<DataType> data, Args&&... args)
    -> std::shared_ptr<data_task<DataType, ReturnType>>
{
    return std::make_shared<data_task<DataType, ReturnType>>(
        std::forward<F>(f), data, std::forward<Args>(args)...
    );
}

} // namespace axcl::worker