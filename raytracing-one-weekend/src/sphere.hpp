#ifndef INCLUDE_SRC_SPHERE_HPP_
#define INCLUDE_SRC_SPHERE_HPP_
#include <cmath>
#include <memory>

#include "hittable.hpp"
#include "vec3.hpp"

class Sphere : public Hittable {
public:
    Sphere() {}
    Sphere(Point3 cen, RealType r, std::shared_ptr<material> m)
        : center(cen), radius(r), material_(m) {}

    bool hit(const ray& r, RealType tmin, RealType tmax,
             HitRecord& rec) const override;

    aabb bounding_box() const override;

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

    if (0 < D) {
        auto D_sqrt = sqrt(D);
        auto temp_t = (-b - D_sqrt) / a;
        if (tmin < temp_t && temp_t < tmax) {
            rec.t = temp_t;
            rec.p = r.at(temp_t);
            rec.set_face_normal(r, (rec.p - center) / radius);
            rec.material_ = material_.get();
            return true;
        }
        temp_t = (-b + D_sqrt) / a;
        if (tmin < temp_t && temp_t < tmax) {
            rec.t = temp_t;
            rec.p = r.at(temp_t);
            rec.set_face_normal(r, (rec.p - center) / radius);
            rec.material_ = material_.get();
            return true;
        }
    }
    return false;
}

inline aabb Sphere::bounding_box() const {
    Vec3 rad = {radius, radius, radius};
    return aabb(center + rad, center - rad);
}

#endif  // INCLUDE_SRC_SPHERE_HPP_
