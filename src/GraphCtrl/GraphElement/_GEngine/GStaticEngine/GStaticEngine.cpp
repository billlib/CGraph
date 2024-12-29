/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GStaticEngine.cpp
@Time: 2024/7/27 10:40
@Desc: 
***************************/

#include "GStaticEngine.h"

CGRAPH_NAMESPACE_BEGIN

/**
 * @brief 对elements内的元素进行拓扑排序，排序结果存入element_mat_
 * 
 * @param elements 
 * @return CStatus 
 */
CStatus GStaticEngine::setup(const GSortedGElementPtrSet& elements) {
    CGRAPH_FUNCTION_BEGIN
    element_mat_.clear();

    GElementPtrArr curArr;
    CSize totalSize = 0;
    for (auto element : elements) {
        CGRAPH_ASSERT_NOT_NULL(element)
        // 初始化：刷新所有结点的left_depend_(待执行的前序元素个数)
        element->refresh();
        // 初始化：前序依赖为空(可执行)的element加入curArr
        if (element->dependence_.empty()) {
            curArr.push_back(element);
            totalSize++;
        }
    }

    // (假设curArr全部执行完成，那么curArr中所有元素的后继的left_depend_-1)
    // 在这个前提下，对curArr现有元素的后继的left_depend_(肯定≥1)统一减1
    // 如果后继元素的left_depend_变0了则说明该后继的所有前驱都已经加入了curArr
    // 那么这个后继也加入curArr的尾部；通过这种方式，实现类似于拓扑排序的效果
    while (!curArr.empty()) {
        element_mat_.emplace_back(curArr);
        GElementPtrArr runnableArr = curArr;
        curArr.clear();
        for (auto element : runnableArr) {
            for (auto cur : element->run_before_) {
                if (0 == (--cur->left_depend_)) {
                    curArr.push_back(cur);
                    totalSize++;
                }
            }
        }
    }

    CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(totalSize != elements.size(),
                                            "static engine parse error");
    CGRAPH_FUNCTION_END
}


/**
 * @brief 未绑定线程的微任务在macros排队执行；其他任务提交到线程池
 * TODO(bill.lee): 拓扑排序完，但是提交到线程池和微线程的线程怎么保证按照拓扑排序的顺序执行?
 * 如果不能保证按照拓扑排序的顺序串行执行，那setup的意义在哪?
 * 
 * @return CStatus 
 */
CStatus GStaticEngine::run() {
    CGRAPH_FUNCTION_BEGIN

    for (const auto& arr : element_mat_) {
        std::vector<std::future<CStatus>> futures;
        GElementPtrArr macros;
        for (auto* element : arr) {
            if (element->isMacro() && element->isDefaultBinding()) {
                // 未绑定线程的微任务，直接放到 macros 中，减少线程切换
                macros.emplace_back(element);
            } else {
                auto fut = thread_pool_->commit([element] {
                    return element->fatProcessor(CFunctionType::RUN);
                }, element->binding_index_);
                futures.emplace_back(std::move(fut));
            }
        }

        for (GElementPtr macro : macros) {
            status += macro->fatProcessor(CFunctionType::RUN);
        }

        for (auto& future : futures) {
            status += future.get();
        }
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    CGRAPH_FUNCTION_END
}

CGRAPH_NAMESPACE_END
