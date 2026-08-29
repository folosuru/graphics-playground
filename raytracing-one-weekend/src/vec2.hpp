#ifndef INCLUDE_SRC_VEC2_HPP_
#define INCLUDE_SRC_VEC2_HPP_

#include <cmath>
struct Vec2 {
    float e[2];

    Vec2() : e{0, 0} {}
    Vec2(float v1, float v2) : e{v1, v2} {}

    template<typename T, typename U>
    Vec2(T v1, U v2) : Vec2((float)v1, (float)v2) {}

    inline float operator[](int i) const { return e[i]; };
    inline float& operator[](int i) { return e[i]; }

    Vec2 operator-() const { return {-e[0], -e[1]}; }
    Vec2& operator+=(const Vec2& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        return *this;
    }
    Vec2& operator*=(float t) {
        e[0] *= t;
        e[1] *= t;
        return *this;
    }
    Vec2& operator/=(const float t) { return *this *= 1 / t; }

    float length() const { return std::sqrt(length_squared()); }
    float length_squared() const { return e[0] * e[0] + e[1] * e[1]; }
};

inline Vec2 operator+(const Vec2& l, const Vec2& r) {
    return {l[0] + r[0], l[1] + r[1]};
}
inline Vec2 operator-(const Vec2& l, const Vec2& r) {
    return {l[0] - r[0], l[1] - r[1]};
}
inline Vec2 operator*(const Vec2& l, const Vec2& r) {
    return {l[0] * r[0], l[1] * r[1]};
}
inline Vec2 operator*(const Vec2& v, float t) {
    return {v[0] * t, v[1] * t};
}
inline Vec2 operator*(float t, const Vec2& v) {
    return v * t;
}
#endif  // INCLUDE_SRC_VEC2_HPP_
