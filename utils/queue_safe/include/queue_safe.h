//
// Created by sauci on 21/11/2018.
//

#ifndef XCPUI_QUEUE_SAFE_H
#define XCPUI_QUEUE_SAFE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <condition_variable>
#include <mutex>
#include <queue>

template<typename T>
class Queue
{
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable cond_;

    bool signal_ = false;

public:

    T pop(bool &signal)
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        while (queue_.empty())
        {
            cond_.wait(mlock);
        }
        auto item = queue_.front();
        signal = signal_;
        queue_.pop();
        return item;
    }

    void pop(T &item, bool &signal)
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        while (queue_.empty())
        {
            cond_.wait(mlock);
        }
        item = queue_.front();
        signal = signal_;
        queue_.pop();
    }

    void push(const T &item)
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        queue_.push(item);
        mlock.unlock();
        cond_.notify_one();
    }

    void push(T &&item)
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        queue_.push(std::move(item));
        mlock.unlock();
        cond_.notify_one();
    }

    void signal()
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        signal_ = true;
        T dummy;
        queue_.push(dummy);
        mlock.unlock();
        cond_.notify_one();
    }

    bool empty()
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        bool empty = queue_.empty();
        mlock.unlock();
        cond_.notify_one();
        return empty;
    }
};

#endif //XCPUI_QUEUE_SAFE_H
