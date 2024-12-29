/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: SingletonProc.h
@Time: 2021/6/11 7:06 下午
@Desc: 
***************************/

#ifndef CGRAPH_USINGLETON_H
#define CGRAPH_USINGLETON_H

#include "USingletonDefine.h"
#include "../UtilsObject.h"

CGRAPH_NAMESPACE_BEGIN

/**
 * @brief 支持默认构造一个T类型的对象、封装成单例使用
 *        目前实际上只支持CObject及其派生类作为单例要维护的对象类型，其他类型在init/destroy阶段啥都不做
 * 
 * @tparam T 单例实际维护的对象类型，在USingleton内部维护为T *句柄
 * @tparam type 单例模式，支持懒汉(LAZY)和饿汉(HUNGRY)模式，默认HUNGRY
 * @tparam autoInit 是否在构造时自动初始化，默认false
 */
template<typename T,
        USingletonType type = USingletonType::HUNGRY,
        CBool autoInit = false>
class USingleton : public UtilsObject {
public:
    explicit USingleton() noexcept {
        if (USingletonType::HUNGRY == type || autoInit) {
            /* 如果是饥汉模式，则直接构造 */
            create();
        }

        if (autoInit) {
            init();
        }
    }

    ~USingleton() override {
        clear();
    }

    /**
     * 获取singleton句柄信息
     * @return
     */
    T* get() {
        /* 懒汉模式：get的时候才create */
        if (USingletonType::LAZY == type) {
            create();
        }

        T* handle = handle_;
        return handle;
    }

protected:
    CStatus init() override {
        CGRAPH_FUNCTION_BEGIN

        // 如果传入的是CObject类型的对象的话，则调用其init接口
        if (std::is_base_of<CObject, T>::value) {
            status = this->get()->init();
        }
        CGRAPH_FUNCTION_END
    }

    CStatus destroy() override {
        CGRAPH_FUNCTION_BEGIN
        if (std::is_base_of<CObject, T>::value) {
            status = this->get()->destroy();
        }
        CGRAPH_FUNCTION_END
    }

    /**
     * 单例的创建一个句柄
     * @return
     */
    CVoid create() {
        // unlikely只用于编译器优化，告诉编译器这个条件不太可能为真，帮忙编译器重排代码
        // 实际上无论该条件是否满足，下面的语句块都会执行
        if (unlikely(nullptr == handle_)) {
            CGRAPH_LOCK_GUARD lock(lock_);
            if (nullptr == handle_) {
                handle_ = CGRAPH_SAFE_MALLOC_COBJECT(T)
            }
        }
    }

    /**
     * 销毁单例句柄
     * @return
     */
    CStatus clear() {
        CGRAPH_FUNCTION_BEGIN
        CGRAPH_LOCK_GUARD lock(lock_);
        CGRAPH_DELETE_PTR(handle_)
        CGRAPH_FUNCTION_END
    }

    CGRAPH_NO_ALLOWED_COPY(USingleton);

private:
    T* handle_ { nullptr };             // 对应的单例句柄信息
    std::mutex lock_;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_USINGLETON_H
