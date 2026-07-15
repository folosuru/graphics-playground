#ifndef INCLUDE_SRC_MATH_UTIL_HPP_
#define INCLUDE_SRC_MATH_UTIL_HPP_

#include <cmath>
#include <limits>
#include <random>

using RealType = float;
// 定数
const RealType Infinity = std::numeric_limits<RealType>::infinity();
const RealType pi = 3.1415926535897932385;

// ユーティリティ関数
inline RealType degrees_to_radians(double degrees) {
    return degrees * pi / 180;
}

inline RealType fold_min(RealType v, RealType v2) {
    return v < v2 ? v : v2;
}
inline RealType fold_max(RealType v, RealType v2) {
    return v > v2 ? v : v2;
}

/**
 * 最右の引数がNaN でない場合、戻り値はNaN にならない。 
 **/
template<typename... Args>
inline RealType fold_min(RealType v, Args... val) {
    RealType right = fold_min(val...);
    return v < right ? v : right;
}

/**
 * 最右の引数がNaN でない場合、戻り値はNaN にならない。 
 **/
template<typename... Args>
inline RealType fold_max(RealType v, Args... val) {
    RealType right = fold_max(val...);
    return v > right ? v : right;
}

inline float fast_rand_float() {
    thread_local uint32_t x = 2463534242;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    uint32_t tmp = (x >> 9) | 0x3F800000;
    return *(float *)&tmp - 1.0f;
}

inline RealType random_double() {
    //mt19937ぶん回すのはちょいおそい　精度いらなそうだしXORShiftで済ます
    return fast_rand_float();
    static std::uniform_real_distribution<RealType> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline RealType random_double(double min, double max) {
    // [min,max) の実数乱数を返す
    return min + (max - min) * random_double();
}

#endif  // INCLUDE_SRC_MATH_UTIL_HPP_
