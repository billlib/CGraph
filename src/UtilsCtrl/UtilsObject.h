/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UtilsObject.h
@Time: 2021/9/19 12:00 上午
@Desc: 
***************************/

#ifndef CGRAPH_UTILSOBJECT_H
#define CGRAPH_UTILSOBJECT_H

#include "UtilsDefine.h"

CGRAPH_NAMESPACE_BEGIN

/**
 * @brief UtilsObject单独继承CObject，run()标记为CGRAPH_NO_SUPPORT
 * 
 */
class UtilsObject : public CObject {
protected:
    /**
     * 将基类CObject的run设置为不支持
     * 继承为一个只支持init/destroy的工具类
     */
    CStatus run() override {
        CGRAPH_NO_SUPPORT
    }
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UTILSOBJECT_H
