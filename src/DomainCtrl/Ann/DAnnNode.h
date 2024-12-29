/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: DAnnNode.h
@Time: 2022/4/4 21:17
@Desc: ann领域算子模型抽象
***************************/

#ifndef CGRAPH_DANNNODE_H
#define CGRAPH_DANNNODE_H

#include "DAnnDefine.h"
#include "DAnnObject.h"

CGRAPH_NAMESPACE_BEGIN

/**
 * @brief ANN领域算子模型抽象，说人话，就是抽象了一套跑NN相关的范式
 *        prepareParam准备要接下来要run的函数类型
 *        函数类型和DAnnNode的成员函数一一对应，分别有
 *        train/search/insert/update/remove/loadModel/saveModel/edition；
 *        上述接口在DAnnNode这个基类都为空，由派生类自行实现，在run统一调用；
 * 
 *        run的调用逻辑：
 *        prepareParam为纯虚函数，派生类必须重写，返回的是接下来run要跑的函数类型
 *        接着run会调用这个函数类型在ann_func_arr_实际注册的函数
 *        最后再调用一次refreshParam刷新参数
 * 
 *        P.S. 感觉用处不大，和目前常用的 前处理 -> NN -> 后处理 不太match，倒是比较契合算法的模型训练流程?
 */
class DAnnNode : public DAnnObject, public GNode {
protected:
    /**
     * 预处理参数信息，并且返回需要执行的函数信息
     * @return
     */
    virtual DAnnFuncType prepareParam() = 0;

    /**
     * 训练
     * @return
     */
    virtual CStatus train();

    /**
     * 查询
     * @return
     */
    virtual CStatus search();

    /**
     * 插入
     * @return
     */
    virtual CStatus insert();

    /**
     * 修改
     * @return
     */
    virtual CStatus update();

    /**
     * 删除
     * @return
     */
    virtual CStatus remove();

    /**
     * 加载模型信息
     * @return
     */
    virtual CStatus loadModel();

    /**
     * 保存模型信息
     * @return
     */
    virtual CStatus saveModel();

    /**
     * 其他方法
     */
    virtual CStatus edition();

    /**
     * 将参数更新，作用于所有功能函数之后
     * @return
     */
    virtual CStatus refreshParam();

protected:
    /**
     * 构造函数信息
     */
    explicit DAnnNode();

    CGRAPH_NO_ALLOWED_COPY(DAnnNode)

    /**
     * 分拆执行函数
     * @return
     */
    CStatus run() override;

private:
    typedef	CStatus (DAnnNode::*DAnnNodeFuncPtr)();
    DAnnNodeFuncPtr ann_func_arr_[static_cast<CUInt>(DAnnFuncType::ANN_MAX_SIZE)]{};    // ann函数映射关系
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_DANNNODE_H
