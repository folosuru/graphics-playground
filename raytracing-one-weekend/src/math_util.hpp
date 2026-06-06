#ifndef INCLUDE_SRC_MATH_UTIL_HPP_
#define INCLUDE_SRC_MATH_UTIL_HPP_

#include <limits>
#include <random>

// 定数
const double Infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// ユーティリティ関数
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180;
}

inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double(double min, double max) {
    // [min,max) の実数乱数を返す
    return min + (max - min) * random_double();
}

#endif  // INCLUDE_SRC_MATH_UTIL_HPP_
