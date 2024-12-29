/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GAspectManager.h
@Time: 2021/10/2 10:17 下午
@Desc: 
***************************/

#ifndef CGRAPH_GASPECTMANAGER_H
#define CGRAPH_GASPECTMANAGER_H

#include "../GraphManager.h"
#include "GAspectObject.h"
#include "GAspect.h"

CGRAPH_NAMESPACE_BEGIN

/**
 * 继承GraphManager<T>的add/remove/find/create/get/reset/run等针对具体T类型对象的管理能力
 * 继承GAspectObject的GetAParam/setAParam获取GAspectParam的能力，以及setBelong设置所属GElement的能力
 */
class GAspectManager : public GAspectObject,
                       public GraphManager<GAspect> {
protected:
    explicit GAspectManager();

    ~GAspectManager() override ;

    /**
     * 执行切面逻辑
     * @param type
     * @param curStatus
     * @return
     */
    CStatus reflect(const internal::GAspectType& type,
                    const CStatus& curStatus = CStatus());

    CStatus add(GAspectPtr aspect) final;

    CSize getSize() const final;

    CStatus clear() final;

    /**
     * 弹出去最后一个
     * @return
     */
    CStatus popLast();

    CVoidPtr setGParamManager(GParamManagerPtr pm) final;

    CVoidPtr setGEventManager(GEventManagerPtr em) final;

    CGRAPH_NO_ALLOWED_COPY(GAspectManager)

private:
    GAspectPtrArr aspect_arr_;     // 存储aspect的容器

    friend class GElement;
    friend class CAllocator;
};

using GAspectManagerPtr = GAspectManager *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GASPECTMANAGER_H
