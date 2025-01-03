/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UWorkStealingQueue.h
@Time: 2021/7/2 11:29 下午
@Desc: 实现了一个包含盗取功能的安全队列
***************************/


#ifndef CGRAPH_UWORKSTEALINGQUEUE_H
#define CGRAPH_UWORKSTEALINGQUEUE_H

#include <deque>

#include "UQueueObject.h"

CGRAPH_NAMESPACE_BEGIN

/**
 * @brief 工作窃取队列，实现为双端队列(deque_)；
 *        push在队尾发生，可以一次性写入一个或多个任务，也支持tryPush和带锁的push；
 *        pop在队头发生，可以一次性拿出一个或多个任务，也支持tryPop；
 *        trySteal在队尾发生，可以一次性窃取一个或多个任务；
 *        1. 工作的拿取发生在队头、窃取发生在队尾，可以显著减小工作线程和窃取线程竞争的概率；
 *        2. 主线程的工作拿取优先发生在第一队列、其次第二队列；窃取优先发生在第二队列、其次发生在第一队列，又可以进一步降低竞争概率；
 * @tparam T 
 */
template<typename T>
class UWorkStealingQueue : public UQueueObject {
public:
    /**
     * 向队列中写入信息
     * @param value
     */
    CVoid push(T&& value) {
        while (true) {
            if (mutex_.try_lock()) {
                deque_.emplace_back(std::forward<T>(value));
                mutex_.unlock();
                break;
            } else {
                CGRAPH_YIELD();
            }
        }
    }


    /**
     * 有条件的写入数据信息
     * @param value
     * @param enable
     * @param state
     * @return
     */
    CVoid push(T&& value, CBool enable, CBool lockable) {
        if (enable && lockable) {
            mutex_.lock();
        }
        deque_.emplace_back(std::forward<T>(value));
        if (enable && !lockable) {
            mutex_.unlock();
        }
    }


    /**
     * 尝试往队列里写入信息
     * @param value
     * @return
     */
    CBool tryPush(T&& value) {
        CBool result = false;
        if (mutex_.try_lock()) {
            deque_.emplace_back(std::forward<T>(value));
            mutex_.unlock();
            result = true;
        }
        return result;
    }


    /**
     * 向队列中写入信息
     * @param values
     */
    CVoid push(const std::vector<T>& values) {
        while (true) {
            if (mutex_.try_lock()) {
                for (auto& value : values) {
                    deque_.emplace_back(value);
                }
                mutex_.unlock();
                break;
            } else {
                CGRAPH_YIELD();
            }
        }
    }


    /**
     * 弹出节点，从头部进行
     * @param value
     * @return
     */
    CBool tryPop(T& value) {
        // 这里不使用raii锁，主要是考虑到多线程的情况下，可能会重复进入
        // raii锁指的是类似于lock_guard的用法，锁和对象的构造时获取、析构时释放
        // 此处直接使用mutex_的try_lock和unlock，手动管理加锁和释放，因此非RAII锁
        bool result = false;
        if (!deque_.empty() && mutex_.try_lock()) {
            if (!deque_.empty()) {
                value = std::forward<T>(deque_.front());    // 从前方弹出
                deque_.pop_front();
                result = true;
            }
            mutex_.unlock();
        }

        return result;
    }


    /**
     * 从头部开始批量获取可执行任务信息
     * @param values
     * @param maxLocalBatchSize
     * @return
     */
    CBool tryPop(std::vector<T>& values, int maxLocalBatchSize) {
        bool result = false;
        if (!deque_.empty() && mutex_.try_lock()) {
            while (!deque_.empty() && maxLocalBatchSize--) {
                values.emplace_back(std::forward<T>(deque_.front()));
                deque_.pop_front();
                result = true;
            }
            mutex_.unlock();
        }

        return result;
    }


    /**
     * 窃取节点，从尾部进行
     * @param task
     * @return
     */
    CBool trySteal(T& value) {
        bool result = false;
        if (!deque_.empty() && mutex_.try_lock()) {
            if (!deque_.empty()) {
                value = std::forward<T>(deque_.back());    // 从后方窃取
                deque_.pop_back();
                result = true;
            }
            mutex_.unlock();
        }

        return result;
    }


    /**
     * 批量窃取节点，从尾部进行
     * @param values
     * @return
     */
    CBool trySteal(std::vector<T>& values, int maxStealBatchSize) {
        bool result = false;
        if (!deque_.empty() && mutex_.try_lock()) {
            while (!deque_.empty() && maxStealBatchSize--) {
                values.emplace_back(std::forward<T>(deque_.back()));
                deque_.pop_back();
                result = true;
            }
            mutex_.unlock();
        }

        return result;    // 如果非空，表示盗取成功
    }

    UWorkStealingQueue() = default;

    CGRAPH_NO_ALLOWED_COPY(UWorkStealingQueue)

private:
    std::deque<T> deque_;            // 存放任务的双向队列
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UWORKSTEALINGQUEUE_H
