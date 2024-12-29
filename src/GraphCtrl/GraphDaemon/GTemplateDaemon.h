/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GTemplateDaemon.h
@Time: 2022/12/23 23:39
@Desc: 
***************************/

#ifndef CGRAPH_GTEMPLATEDAEMON_H
#define CGRAPH_GTEMPLATEDAEMON_H

#include "GDaemon.h"

CGRAPH_NAMESPACE_BEGIN

/**
 * @brief GTemplateDaemon(空) = GDaemon，但是实现成模板类，可以自己用模板参数加以实现
 * 
 * @tparam Args 
 */
template<class ...Args>
class GTemplateDaemon : public GDaemon {
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GTEMPLATEDAEMON_H
