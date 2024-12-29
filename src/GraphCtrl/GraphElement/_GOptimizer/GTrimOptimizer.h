/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GTrimOptimizer.h
@Time: 2024/9/1 13:59
@Desc: 
***************************/

#ifndef CGRAPH_GTRIMOPTIMIZER_H
#define CGRAPH_GTRIMOPTIMIZER_H

#include <algorithm>
#include <iterator>

#include "GOptimizer.h"
#include "../GGroup/GGroupInclude.h"

CGRAPH_NAMESPACE_BEGIN

class GTrimOptimizer : public GOptimizer {
    /**
     * 针对图结构，做剪裁；具体算法为：
     * (1) collectPaths：以入度为0的结点开始，通过递归，收集整个elements集合中的路径paths
     * (2) buildGraph：对paths进行分析，构建一个[s, s]的矩阵graph，s为elements所有元素的个数，其中
     *                 graph[x][y]表示elements中index为x的元素是否可以达到index为y的元素
     * (3) 剪枝: 遍历elements，如果发现elements的前序结点x/y存在可达关系x->y，类似于x -> y
     *           那么可以把x->cur的依赖关系删除                                   ↘  ↙
     *                                                                           cur
     *  (4) 如果elements存在某个元素是REGION，那么继续调用REGION自己的trim (本质上就是上述算法)
     *  (5) 记录总共被剪枝的元素个数
     * @param elements
     * @return
     */
    static CSize trim(GSortedGElementPtrSet& elements) {
        CSize trimNum = 0;
        // 记录所有path信息
        const auto& paths = collectPaths(elements);
        // 将所有path信息的父子关系(前后关系)构造成1个二维矩阵
        // graph[x][y]表示elements中index为x的元素和index为y的元素的关系
        // 如果为1，说明elements[x]是elements[y]的father(可以是直接前驱也可以是间接前驱，即可以是父结点也可以是祖先结点)，即存在一条从x到y的路径
        // 如果为0，则不是，说明不存在一条路径可以从x到y
        auto graph = buildGraph(elements, paths, 1, 0, 0);

        for (auto* cur : elements) {
            CGRAPH_ASSERT_NOT_NULL_THROW_ERROR(cur)
            GElementPtrArr candidates;
            for (CSize i = 0; i < cur->dependence_.size(); i++) {
                CSize x = std::distance(elements.begin(), elements.find(cur->dependence_[i]));
                for (CSize j = 0; j < cur->dependence_.size(); j++) {
                    // 这里必须是 n^2 的循环
                    // 通过n^2的循环两两遍历cur的所有前序元素，如果发现某两个前序元素之前存在可达关系
                    // 则把father元素加入待删除的前序依赖列表candidates
                    // 举例：x -> y，这种情况下，可以把x->cur这层依赖关系删除，不影响拓扑顺序
                    //       ↘  ↙
                    //        cur
                    CSize y = std::distance(elements.begin(), elements.find(cur->dependence_[j]));
                    if (1 == graph[x][y]) {
                        candidates.push_back(cur->dependence_[i]);
                    }
                }
            }

            // 执行剪枝动作
            for (auto* candidate : candidates) {
                if (cur->removeDepend(candidate).isOK()) {
                    trimNum++;
                }
            }

            // 如果当前正在剪枝的元素属于REGION，则对REGION内的所有元素也做一遍剪枝
            // 计入当前元素集合elements的总剪枝数
            // 注：GCluster纯串行、GCondition条件、GMutable可变、GSome纯并行，所以都不用剪枝
            if (GElementType::REGION == cur->element_type_) {
                trimNum += ((GRegionPtr)cur)->trim();
            }
        }

        return trimNum;
    }

    friend class GElementManager;
    friend class GRegion;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GTRIMOPTIMIZER_H
