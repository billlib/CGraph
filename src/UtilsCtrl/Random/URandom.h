/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: URandom.h
@Time: 2022/5/13 23:15
@Desc: 
***************************/

#ifndef CGRAPH_URANDOM_H
#define CGRAPH_URANDOM_H

#include <random>

#include "../UtilsObject.h"
#include "URandomDefine.h"

CGRAPH_NAMESPACE_BEGIN

/**
 * @brief 随机数生成类，支持模板入参T(默认CFloat)、SEED(0为完全随机值，其他为固定随机值)、
 *        TEngine为随机数的生成方式(梅森旋转法/线性同余法/滞后Fibonacci/...)
 *        generate: 基于std::uniform_real_distribution生成指定T类型、一维或二维、指定个数的均匀分布的随机数存入data
 *        generateSession：生成随机session_信息，常用于CDescInfo的成员，作为CDescInfo派生类(GNode/GAdapter/GElement等)的唯一标识
 * 
 * @tparam T 随机数类型，默认CFloat
 * @tparam SEED 0为完全随机值，其他为固定随机值
 * @tparam TEngine 随机数的生成方式(梅森旋转法/线性同余法/滞后Fibonacci/...)
 */
template<typename T = CFloat,
        CInt SEED = CGRAPH_REAL_RANDOM,    /** 当传入的seed为 CGRAPH_REAL_RANDOM 的时候，表示完全随机。其他值为固定随机值 */
        typename TEngine = CGRAPH_RANDOM_MT19937>
class URandom : public UtilsObject {
public:
    /**
     * 构建随机一维向量
     * @param data
     * @param dim
     * @param minValue
     * @param maxValue
     * @return
     * @notice 暂时不支持int类型的随机数生成
     */
    static CStatus generate(std::vector<T>& data, CSize dim,
                            const T& minValue, const T& maxValue) {
        std::random_device rd;
        TEngine eng(CGRAPH_REAL_RANDOM == SEED ? rd() : SEED);
        std::uniform_real_distribution<T> urd(minValue, maxValue);

        data.clear();
        data.reserve(dim);
        for (CSize i = 0; i < dim; i++) {
            data.emplace_back(urd(eng));
        }

        return CStatus();
    }

    /**
     * 构建随机二维向量
     * @param data
     * @param height
     * @param column
     * @param minValue
     * @param maxValue
     * @return
     */
    static CStatus generate(std::vector<std::vector<T> >& data, CSize height, CSize column,
                            const T& minValue, const T& maxValue) {
        std::random_device rd;
        TEngine eng(CGRAPH_REAL_RANDOM == SEED ? rd() : SEED);
        std::uniform_real_distribution<T> urd(minValue, maxValue);

        data.clear();
        data.reserve(height);
        std::vector<T> arr;

        for (CSize i = 0; i < height; i++) {
            arr.reserve(column);
            for (CSize j = 0; j < column; j++) {
                arr.emplace_back(urd(eng));
            }
            data.emplace_back(std::move(arr));
        }

        return CStatus();
    }

    /**
     * 生成随机session信息
     * @param key
     * @param size
     * @return
     */
    static std::string generateSession(const std::string& key = CGRAPH_UNKNOWN, CSize size = 3) {
        std::string session;    // 形式是 a-b-c-key，其中 a表示6位随机数字
        std::vector<CFloat> vec;
        URandom<CFloat>::generate(vec, size, 100000, 999999);
        for (CSize i = 0; i < size; i++) {
            session += std::to_string(int(vec[i]));
            session += '-';
        }
        session += key;
        return session;
    }
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_URANDOM_H
