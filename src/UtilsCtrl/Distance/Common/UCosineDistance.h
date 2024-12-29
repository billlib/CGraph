/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UCosineDistance.h
@Time: 2023/1/25 23:17
@Desc: 
***************************/

#ifndef CGRAPH_UCOSINEDISTANCE_H
#define CGRAPH_UCOSINEDISTANCE_H

#include "../UDistance.h"

CGRAPH_NAMESPACE_BEGIN

/**
 * @brief 计算v1和v2两个向量的cos距离
 *        默认v1和v2的维度相同(dim1 ==  dim2)且非零 (此处calc实现未做检查，可能出现除零错误)
 * 
 * @tparam TSrc 
 * @tparam TRes 
 */
template<typename TSrc, typename TRes = TSrc>
class UCosineDistance : public UDistance<TSrc, TRes> {
public:
    CStatus calc(const TSrc* v1, const TSrc* v2, CSize dim1, CSize dim2, TRes& result, CVoidPtr ext) override {
        CGRAPH_FUNCTION_BEGIN

        result = 0;     // 先清空
        TSrc val1 = 0;    // 用于记录vec1的长度
        TSrc val2 = 0;
        for (CSize i = 0; i < dim1; i++) {
            result += (v1[i] * v2[i]);
            val1 += (v1[i] * v1[i]);
            val2 += (v2[i] * v2[i]);
        }

        result = result / (std::sqrt(val1) * std::sqrt(val2));
        CGRAPH_FUNCTION_END
    }
};

CGRAPH_NAMESPACE_END


#endif //CGRAPH_UCOSINEDISTANCE_H
