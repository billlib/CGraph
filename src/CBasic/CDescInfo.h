/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: CDescInfo.h
@Time: 2023/2/19 15:56
@Desc: 通用描述信息
***************************/

#ifndef CGRAPH_CDESCINFO_H
#define CGRAPH_CDESCINFO_H

#include <string>

#include "CBasicDefine.h"

CGRAPH_NAMESPACE_BEGIN

/**
 * 广泛用于GAspect/GAspectManager/GDaemon/GDaemonManager/GEvent/GEventManager/GPipeline/GElement及其派生类
 * 这些类及其派生类基本覆盖CGraph的绝大部分场景，因此CDescInfo可以看作是CGraph各个类的描述信息基座
 * 具体：提供名称(name_)、描述(description_)、ID(session_)的set/get接口
 * 比如：
 * GElement又可派生出GAdapter/GGroup/GNode
 * GAdapter又可派生出GCoordinator/GFence/GFunction/GSingleton
 * GGroup又可派生出GCluster/GCondition/GMultiCondition/GMutable/GRegion/GSome
 */
class CDescInfo {
public:
    /**
     * 获取名称信息
     * @return
     */
    virtual const std::string& getName() const {
        return name_.empty() ? session_ : name_;
    }

    /**
     * 获取唯一id信息
     * @return
     */
    const std::string& getSession() const {
        return session_;
    }

    /**
     * 获取描述信息
     * @return
     */
    const std::string& getDescription() const {
        return description_;
    }

    /**
     * 设置名称信息
     * @param name
     * @return
     * @notice 部分windows编译器不支持函数继承改变返回值类型，故做此区分
     */
#ifdef _WIN32
    virtual CVoidPtr setName(const std::string& name) {
#else
    virtual auto setName(const std::string& name) -> decltype(this) {
#endif
        name_ = name;
        return this;
    }

    /**
     * 设置描述信息
     * @param description
     * @return
     */
    virtual auto setDescription(const std::string& description)
    -> decltype(this) {
        description_ = description;
        return this;
    }

    virtual ~CDescInfo() = default;

protected:
    std::string name_;                               // 名字
    std::string session_;                            // 唯一id信息
    std::string description_;                        // 描述信息
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_CDESCINFO_H
