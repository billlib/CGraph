/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GElementObject.h
@Time: 2022/12/18 19:32
@Desc: 
***************************/

#ifndef CGRAPH_GELEMENTOBJECT_H
#define CGRAPH_GELEMENTOBJECT_H

#include "../GraphObject.h"
#include "../GraphParam/GParamInclude.h"
#include "../GraphAspect/GAspectInclude.h"
#include "../GraphEvent/GEventInclude.h"
#include "../GraphStage/GStageInclude.h"

CGRAPH_NAMESPACE_BEGIN

/**
 * @brief GElementObject = GraphObject = CObject，没有新增或删除任何成员
 */
class GElementObject : public GraphObject {
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GELEMENTOBJECT_H
