/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GStaticEngine.h
@Time: 2024/7/27 10:40
@Desc: 
***************************/

#ifndef CGRAPH_GSTATICENGINE_H
#define CGRAPH_GSTATICENGINE_H

#include "../GEngine.h"

CGRAPH_NAMESPACE_BEGIN

/**
 * @brief 将待执行的图中的所有结点进行拓扑排序(setup)，排序结果存储在element_mat_；
 * 然后将element_mat_各个拓扑排序好的结点提交到微线程或者线程池，等待执行完成获取结果(run)
 */
class GStaticEngine : public GEngine {
protected:
    explicit GStaticEngine() = default;

    CStatus setup(const GSortedGElementPtrSet& elements) override;

    CStatus run() override;

private:
    GElementPtrMat2D element_mat_ {};        // 解图后的elements信息

    friend class CAllocator;
    friend class GElementManager;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GSTATICENGINE_H
