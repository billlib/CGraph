/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: USpinLock.h
@Time: 2023/2/21 22:17
@Desc: 
***************************/

#ifndef CGRAPH_USPINLOCK_H
#define CGRAPH_USPINLOCK_H

#include <atomic>

#include "../UThreadObject.h"

CGRAPH_NAMESPACE_BEGIN

/**
 * @brief 基于std::atomic_flag实现的用户态自旋锁，std::atomic_flag只有true/false两种状态；
 *        USpinLock提供以下三个方法：lock为忙等，tryLock为单次尝试，unlock为解锁；
 *        (基于std::atomic_flag实现的用户态自旋锁是一种采用的做法，)
 */
class USpinLock : public UThreadObject {
public:
    /**
     * 加锁
     */
    CVoid lock() {
        // memory_order_acquire 后面访存指令勿重排至此条指令之前
        // test_and_set将std::atomic_flag置为true，但返回设置之前的状态:
        // flag_为false的情况下，test_and_set将其置为true，但返回false
        // flag_为true的情况下，test_and_set直接返回true
        while (flag_.test_and_set(std::memory_order_acquire)) {
        }
    }

    /**
     * 解锁
     */
    CVoid unlock() {
        // memory_order_release 前面访存指令勿重排到此条指令之后
        // clear无条件将flag_置为false
        flag_.clear(std::memory_order_release);
    }

    /**
     * 尝试加锁。若未加锁，会上锁
     * @return
     */
    CBool tryLock() {
        return !flag_.test_and_set();
    }

private:
    std::atomic_flag flag_ = ATOMIC_FLAG_INIT;         // 标志位
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_USPINLOCK_H
