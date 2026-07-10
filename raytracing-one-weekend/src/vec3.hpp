#ifndef VEC3_HPP
#define VEC3_HPP

#include <immintrin.h>
#include <xmmintrin.h>

#include <cmath>
#include <csignal>
#include <iostream>
#include <ostream>

#include "math_util.hpp"
using std::cos;
using std::sin;
using std::sqrt;
using std::tan;
struct alignas(16) Vec3 {
    union {
        RealType e[4];
        __m128 v;
    };
    Vec3() : e{0, 0, 0} {}
    Vec3(RealType e0, RealType e1, RealType e2) : e{e0, e1, e2} {}
    Vec3(RealType e0, RealType e1, RealType e2, RealType e3)
        : e{e0, e1, e2, e3} {}
    Vec3(__m128 v_) : v(v_) {}

    template<typename T, typename U, typename V>
    explicit Vec3(T e0, U e1, V e2)
        : Vec3(static_cast<RealType>(e0), static_cast<RealType>(e1),
               static_cast<RealType>(e2)) {}

    auto x() const { return e[0]; }
    auto y() const { return e[1]; }
    auto z() const { return e[2]; }

    Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2], -e[3]); }
    RealType operator[](int i) const { return e[i]; }
    RealType& operator[](int i) { return e[i]; }

    Vec3& operator+=(const Vec3& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        e[3] += v.e[3];
        return *this;
    }

    Vec3& operator*=(const RealType t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        e[3] *= t;
        return *this;
    }

    Vec3& operator/=(const RealType t) { return *this *= 1 / t; }

    RealType length() const { return sqrt(length_squared()); }

    RealType length_squared() const {
        return _mm_cvtss_f32(_mm_dp_ps(v, v, 0x77));
        // return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    Vec3 rcp() const {
        return _mm_rcp_ps(v);
        // return {1 / e[0], 1 / e[1], 1 / e[2]};
    }

    /**
     * 2 つのベクトルから小さい方の値を取る。
     * なお、一方にNaN がある時はNaN でない側が取られる。
     **/
    Vec3 extract_min(const Vec3& other) const { return _mm_min_ps(v, other.v); }

    /**
     * 2 つのベクトルから大きい方の値を取る。
     * なお、一方にNaN がある時はNaN でない側が取られる。
     **/
    Vec3 extract_max(const Vec3& other) const { return _mm_max_ps(v, other.v); }

    inline static Vec3 random() {
        return {random_double(), random_double(), random_double()};
    }
    inline static Vec3 random(RealType min, RealType max) {
        return {random_double(min, max), random_double(min, max),
                random_double(min, max)};
    }
};
using Color = Vec3;
using Point3 = Vec3;

inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
    return out << "{" << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2] << '}';
}

inline Vec3 operator+(const Vec3& u, const Vec3& v) {
    return Vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2],
                u.e[3] + v.e[3]);
}

inline Vec3 operator-(const Vec3& u, const Vec3& v) {
    return Vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2],
                u.e[3] - v.e[3]);
}

inline Vec3 operator*(const Vec3& u, const Vec3& v) {
    return Vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2],
                u.e[3] * v.e[3]);
}

inline Vec3 operator*(RealType t, const Vec3& v) {
    return Vec3(t * v.e[0], t * v.e[1], t * v.e[2], t * v.e[3]);
}

inline Vec3 operator*(const Vec3& v, RealType t) {
    return t * v;
}

inline Vec3 operator/(const Vec3& v, RealType t) {
    return (1 / t) * v;
}

// 内積。
// a・b。
inline RealType dot(const Vec3& u, const Vec3& v) {
    return _mm_cvtss_f32(_mm_dp_ps(u.v, v.v, 0x77));
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

// 外積。 cross productって言うらしい。
// a × b。
inline Vec3 cross(const Vec3& u, const Vec3& v) {
    __m128 a_1203 = _mm_shuffle_ps(u.v, u.v, _MM_SHUFFLE(3, 0, 2, 1));
    __m128 b_2013 = _mm_shuffle_ps(v.v, v.v, _MM_SHUFFLE(3, 1, 0, 2));
    __m128 a_2013 = _mm_shuffle_ps(u.v, u.v, _MM_SHUFFLE(3, 1, 0, 2));
    __m128 b_1203 = _mm_shuffle_ps(v.v, v.v, _MM_SHUFFLE(3, 0, 2, 1));
    return {_mm_sub_ps(_mm_mul_ps(a_1203, b_2013), _mm_mul_ps(a_2013, b_1203))};

    return Vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline Vec3 unit_vector(const Vec3& v) {
    return {_mm_mul_ps(v.v, _mm_rsqrt_ps(_mm_dp_ps(v.v, v.v, 0x77)))};

    return v / v.length();
}

inline Vec3 random_unit_sphere_vector() {
    auto a = random_double(0, 2 * pi);
    auto z = random_double(-1, 1);
    auto r = sqrt(1 - z * z);
    return Vec3(r * cos(a), r * sin(a), z);
}

inline Vec3 random_unit_disk_vector() {
    auto r = random_double(0, 2 * pi);
    auto l = random_double();
    return Vec3{sin(r) * l, cos(r) * l, 0};
}

inline Vec3 reflect(const Vec3& v, const Vec3& n) {
    return v - 2 * dot(v, n) * n;
}

inline Vec3 refract(const Vec3& uv, const Vec3& n, RealType etai_over_etat) {
    auto cos_theta = dot(-uv, n);
    Vec3 r_out_parallel = etai_over_etat * (uv + cos_theta * n);
    Vec3 r_out_perp =
        -sqrt(std::fabs(1.0 - r_out_parallel.length_squared())) * n;
    Vec3 r = r_out_parallel + r_out_perp;
    return r;
}

#endif  // VEC3_HPP
