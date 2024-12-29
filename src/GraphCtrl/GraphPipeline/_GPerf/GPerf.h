/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GPerf.h
@Time: 2023/7/29 22:38
@Desc: 
***************************/

#ifndef CGRAPH_GPERF_H
#define CGRAPH_GPERF_H

#include "../../GraphObject.h"
#include "GPerfAspect.h"
#include "GPerfDefine.h"

CGRAPH_NAMESPACE_BEGIN

class GPipeline;

/**
 * @brief pipeline性能分析，具体做法是通过inject注入统计性能的切面，然后运行pipeline，
 * 接着输出最长路径和整个pipeline的可视化，最后通过recover删除切面恢复原来的pipeline
 */
class GPerf : public GraphObject {
protected:
    /**
     * 针对pipeline进行性能分析
     * @param pipeline
     * @return
     */
    static CStatus perf(GPipeline* pipeline, std::ostream& oss);

    /**
     * 注入对应的切面信息
     * @param pipeline
     * @return
     */
    static CStatus inject(GPipeline* pipeline);

    /**
     * 查询最长链路信息
     * @param pipeline
     * @return
     */
    static CStatus markLongestPath(GPipeline* pipeline);

    /**
     * 恢复原来的pipeline信息
     * @param pipeline
     * @return
     */
    static CStatus recover(GPipeline* pipeline);

    friend class GPipeline;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GPERF_H
