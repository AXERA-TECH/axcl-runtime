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

#include <sys/prctl.h>
#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <future>
#include <functional>
#include <stdexcept>
#include <condition_variable>
#include "utils.hpp"
#include "axcl_rt_context.h"

namespace axcl::skel {
class CThreadPool {
public:
    CThreadPool(size_t, const std::string&, const THREAD_ATTR_T &, const AX_S32&);
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args)
        -> std::future<typename std::result_of<F(Args...)>::type>;
    ~CThreadPool();
private:
    // need to keep track of threads so we can join them
    std::vector< std::thread > workers;
    // the task queue
    std::queue< std::function<void()> > tasks;

    // synchronization
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

// the constructor just launches some amount of workers
inline CThreadPool::CThreadPool(size_t threads, const std::string& strName, const THREAD_ATTR_T &stThreadAttr, const AX_S32& nDeviceId)
    :   stop(false)
{
    for(size_t i = 0;i<threads;++i) {
        workers.emplace_back(
            [i, threads, strName, nDeviceId, this]
            {
                std::string threadName;

                /* create thread context */
                axclrtContext context;
                if (axclError ret = axclrtCreateContext(&context, nDeviceId); AXCL_SUCC != ret) {
                    return;
                }

                if (threads > 1) {
                    threadName = strName + "_" + std::to_string(i + 1);
                }
                else {
                    threadName = strName;
                }

                prctl(PR_SET_NAME, threadName.c_str());

                for(;;)
                {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->condition.wait(lock,
                            [this]{ return this->stop || !this->tasks.empty(); });
                        if(this->stop && this->tasks.empty())
                            break;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }

                    task();
                }

                /* destory thread context */
                axclrtDestroyContext(context);
            }
        );

        set_thread_setschedparam(&workers[i], stThreadAttr);
    }
}

// add new work item to the pool
template<class F, class... Args>
auto CThreadPool::enqueue(F&& f, Args&&... args)
    -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared< std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);

        // don't allow enqueueing after stopping the pool
        if(stop) {
            throw std::runtime_error("enqueue on stopped CThreadPool");
        }

        tasks.emplace([task](){ (*task)(); });
    }
    condition.notify_one();
    return res;
}

// the destructor joins all threads
inline CThreadPool::~CThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }

    condition.notify_all();
    for (std::thread &worker: workers) {
        worker.join();
    }
}
}
