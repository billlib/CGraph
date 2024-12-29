/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: CObject.h
@Time: 2021/4/26 8:12 下午
@Desc: 所有类型的父节点，其中run()方法必须实现
***************************/

#ifndef CGRAPH_COBJECT_H
#define CGRAPH_COBJECT_H

#include "CBasicDefine.h"
#include "CValType.h"
#include "CFuncType.h"

CGRAPH_NAMESPACE_BEGIN

/**
 * 各类功能单元的最底层的基类，包含虚函数init/destroy和纯虚函数run
 * 派生类如果不支持run也要显式写NO_SUPPORT(比如UtilsObject)
 * 具体可派生出GElementManager/GAsptectManager/GEventManager/GDaemonManager/
 * GPipelineManager/GMessageManager/GParamManager等manager类、GEngine/GAspect/
 * GPerf/GDaemon/GSchedule/GElement/GElementRepository/DAnnNode/DAnnParam等结点类
 * 以及UtilsObject的派生类等
 */
class CObject {
public:
    /**
     * 默认构造函数
     */
    explicit CObject() = default;

    /**
     * 初始化函数
     */
    virtual CStatus init() {
        CGRAPH_EMPTY_FUNCTION
    }

    /**
     * 流程处理函数
     */
    virtual CStatus run() = 0;

    /**
     * 释放函数
     */
    virtual CStatus destroy() {
        CGRAPH_EMPTY_FUNCTION
    }

    /**
     * 默认析构函数
     */
    virtual ~CObject() = default;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_COBJECT_H
