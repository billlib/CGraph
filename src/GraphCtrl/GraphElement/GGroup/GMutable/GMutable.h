/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GMutable.h
@Time: 2023/11/13 22:21
@Desc: 
***************************/

#ifndef CGRAPH_GMUTABLE_H
#define CGRAPH_GMUTABLE_H

#include "../GGroup.h"
#include "../../GElementManager.h"

CGRAPH_NAMESPACE_BEGIN

/**
 * @brief 常规的GGroup及其派生类在运行时是不允许再修改前序后继关系的，但是GMutable可以
 * 具体做法是GMutable在每次run的时候会先调用setup初始化自己这张子图(实际上就是清空)，再调用reshape修改前序后继关系，最后才是正常运行子图
 * GMutable的reshape是纯虚函数，派生类必须实现，可以参考MyMutable示例
 */
class GMutable : public GGroup {
protected:
    /**
     * 重新设定内部数据的结构信息
     * @param elements
     * @return
     */
    virtual CStatus reshape(GElementPtrArr& elements) = 0;

    explicit GMutable();

    ~GMutable() override;

private:
    CStatus init() final;

    CStatus run() final;

    CStatus destroy() final;

    CBool isSerializable() const override;

    /**
     * 将数据进行恢复
     * @return
     */
    CVoid setup();

private:
    GElementManagerPtr manager_ = nullptr;

    friend class GPipeline;
    friend class CAllocator;
};

using GMutablePtr = GMutable *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GMUTABLE_H
