#ifndef INCLUDE_SRC_SPHERE_HPP_
#define INCLUDE_SRC_SPHERE_HPP_
#include <cmath>

#include "hittable.hpp"

class Sphere : public Hittable {
public:
    Sphere() {}
    Sphere(Point3 cen, double r) : center(cen), radius(r) {}

    bool hit(const ray& r, double tmin, double tmax,
             HitRecord& rec) const override;

public:
    Point3 center;
    double radius;
};

bool Sphere::hit(const ray& r, double tmin, double tmax, HitRecord& rec) const {
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
            return true;
        }
        temp_t = (-b + D_sqrt) / a;
        if (tmin < temp_t && temp_t < tmax) {
            rec.t = temp_t;
            rec.p = r.at(temp_t);
            rec.set_face_normal(r, (rec.p - center) / radius);
            return true;
        }
    }
    return false;
}

#endif  // INCLUDE_SRC_SPHERE_HPP_
