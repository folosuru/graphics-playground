#ifndef INCLUDE_SRC_SPHERE_HPP_
#define INCLUDE_SRC_SPHERE_HPP_
#include <cassert>
#include <cmath>
#include <memory>

#include "hittable.hpp"
#include "math_util.hpp"
#include "vec2.hpp"
#include "vec3.hpp"

class Sphere : public Hittable {
public:
    Sphere() {}
    Sphere(Point3 cen, RealType r, std::shared_ptr<material> m)
        : center(cen), radius(r), material_(m) {}

    bool hit(const ray& r, RealType tmin, RealType tmax,
             HitRecord& rec) const override;

    aabb bounding_box() const override;

    static Vec2 get_texture_uv(const Vec3& point) {
        // 球面座標系に変換する。
        // theta: y軸から見た時の角度。上(y=1)の時0, 下(y=-1)のとき1
        // phi: XZ軸の角度。Z=-1、X=1 -> Z=1 -> X=-1の向きで回転する。
        //      なお、定義域が-pi <= std::atan2() <= pi のため、piを加算して半周ずらす。
        //
        auto theta = std::acos(-point.y()) / pi;
        auto phi = (std::atan2(-point.z(), point.x()) + pi) / (2 * pi);
        assert(0 <= phi);
        assert(0 <= theta);
        assert(theta <= 1);
        assert(phi <= 1);

        return {phi, theta};
    }

public:
    Point3 center;
    RealType radius;
    std::shared_ptr<material> material_;
};

inline bool Sphere::hit(const ray& r, RealType tmin, RealType tmax,
                        HitRecord& rec) const {
    // 式は
    // b^2 * t^w + 2b * (A-C) * t  + (A-C) * (A-C) - r^2
    // = A-C
    Vec3 oc = r.origin() - center;
    // b^2
    auto a = r.direction().length_squared();
    // 2b * (A - C)
    auto b = dot(oc, r.direction());
    // (A-C) * (A-C) - r^2
    auto c = dot(oc, oc) - radius * radius;
    auto D = b * b - (a * c);

    if (!(0 < D)) {
        return false;
    }

    auto D_sqrt = sqrt(D);

    auto temp_t = (-b - D_sqrt) / a;
    if (!(tmin < temp_t && temp_t < tmax)) {
        temp_t = (-b + D_sqrt) / a;
        if (!(tmin < temp_t && temp_t < tmax)) {
            return false;
        }
    }

    rec.t = temp_t;
    rec.p = r.at(temp_t);
    Vec3 normalize_pos = (rec.p - center) / radius;
    rec.set_face_normal(r, normalize_pos);
    rec.texture_uv = get_texture_uv(normalize_pos);
    rec.material_ = material_.get();
    return true;
}

inline aabb Sphere::bounding_box() const {
    Vec3 rad = {radius, radius, radius};
    return aabb(center + rad, center - rad);
}

#endif  // INCLUDE_SRC_SPHERE_HPP_
