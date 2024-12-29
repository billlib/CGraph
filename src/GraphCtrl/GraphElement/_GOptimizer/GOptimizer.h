/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GOptimizer.h
@Time: 2024/4/7 21:27
@Desc: 
***************************/

#ifndef CGRAPH_GOPTIMIZER_H
#define CGRAPH_GOPTIMIZER_H

#include <vector>

#include "../GElementObject.h"

CGRAPH_NAMESPACE_BEGIN

/**
 * @brief GOptimizer是用来干嘛的?
 */
class GOptimizer : public GElementObject {
protected:
    /**
     * 记录 path 信息
     * @param element
     * @param curPath
     * @param paths
     * @return
     */
    static CVoid collect(GElementPtr element,
                         std::vector<GElementPtr>& curPath,
                         std::vector<std::vector<GElementPtr>>& paths) {
        // 前驱结点已加入curPath的，下次递归调用将后继节点(入度不一定为1)依次加入curPath
        curPath.emplace_back(element);
        if (element->run_before_.empty()) {
            // 如果是最后一个信息了，说明到达当前正在搜索的路径的终点，将这条curPath记录下来
            paths.emplace_back(curPath);
        } else {
            // 广度优先搜索，对当前入度为0的结点的所有后继结点递归调用collect
            for (auto& cur : element->run_before_) {
                collect(cur, curPath, paths);
            }
        }
        curPath.pop_back();
    }

    /**
     * 记录所有的path信息
     * @param elements
     * @return
     */
    static std::vector<std::vector<GElementPtr>> collectPaths(const GSortedGElementPtrSet& elements) {
        std::vector<std::vector<GElementPtr>> paths;
        for (const auto& element : elements) {
            std::vector<GElementPtr> curPath;
            // 从入度为0的结点开始，通过collect递归搜索所有以该结点为头结点的路径存入curPath
            if (element->dependence_.empty()) {
                collect(element, curPath, paths);
            }
        }

        return paths;
    }

    /**
     * 构造对应的二维矩阵图
     * 将每条path的可达关系构造成一个二维矩阵图，father可以直接或通过串联的多个元素到达son，反之不行
     * @param elements
     * @param paths
     * @param father
     * @param son
     * @param unlink
     * @return
     */
    static std::vector<std::vector<int>> buildGraph(const GSortedGElementPtrSet& elements,
                            const std::vector<std::vector<GElementPtr>>& paths,
                            int father, int son, int unlink) {
        const CSize size = elements.size();
        std::vector<std::vector<int>> graph(size, std::vector<int>(size, unlink));
        if (0 == size || 0 == paths.size()) {
            return graph;
        }

        for (const auto& path : paths) {
            for (CSize i = 0; i < path.size(); i++) {
                // 这里的 find是一定能找到的。因为path的数据，是从elements中记录的
                CSize height = std::distance(elements.begin(), elements.find(path[i]));
                for (CSize j = i + 1; j < path.size(); j++) {
                    CSize column = std::distance(elements.begin(), elements.find(path[j]));
                    graph[height][column] = father;
                    graph[column][height] = son;
                }
            }
        }
        return graph;
    }

    friend class GPerf;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GOPTIMIZER_H
