/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GTemplateNode.h
@Time: 2022/12/24 12:21
@Desc: 
***************************/

#ifndef CGRAPH_GTEMPLATENODE_H
#define CGRAPH_GTEMPLATENODE_H

#include "GNode.h"

CGRAPH_NAMESPACE_BEGIN

/**
 * @brief GTemplateNode(空) = GNode，但是实现成模板类，可以自己用模板参数加以实现
 * 
 * @tparam Args 
 */
template<typename ...Args>
class GTemplateNode : public GNode {
};

template<typename ...Args>
using GTemplateNodePtr = GTemplateNode<Args ...> *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GTEMPLATENODE_H
