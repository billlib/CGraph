/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GPerfDefine.h
@Time: 2023/7/29 22:38
@Desc: 
***************************/

#ifndef CGRAPH_GPERFDEFINE_H
#define CGRAPH_GPERFDEFINE_H

#include "../../GraphDefine.h"

CGRAPH_NAMESPACE_BEGIN

/**
 * @brief 性能分析数据，目前只支持耗时和最长连路分析
 */
struct GPerfInfo : public CStruct {
    CUInt loop_ = 0;                       // 循环数
    CFMSec first_start_ts_ = 0.0;          // 开始的时间戳
    CFMSec last_finish_ts_ = 0.0;          // 最后一次结束的时间（需要考虑多次执行，或者多次被循环执行的情况）
    CFMSec accu_cost_ts_ = 0.0;            // 总体的耗时信息(累计值)
    CBool in_longest_path_ = false;        // 是否在最长耗时链路上
};

using GPerfInfoPtr = GPerfInfo *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GPERFDEFINE_H
