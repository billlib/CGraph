/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GAdapter.h
@Time: 2021/10/30 10:22 下午
@Desc:
***************************/

#ifndef CGRAPH_GADAPTER_H
#define CGRAPH_GADAPTER_H

#include "../GElement.h"

CGRAPH_NAMESPACE_BEGIN
/**
 * 适配器，可以派生出GCoordinator/GFence/GFunction/GSingleton等
 * 作为具体适配某些场景的具体类型的共同基类
 */
class GAdapter : public GElement {
protected:
    explicit GAdapter() {
        element_type_ = GElementType::ADAPTER;
    }

    friend class GPipeline;
};

using GAdapterPtr = GAdapter *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GADAPTER_H
