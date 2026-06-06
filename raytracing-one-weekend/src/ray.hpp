#ifndef RAY_HPP_
#define RAY_HPP_
#include "vec3.hpp"

struct ray {
    ray() {}
    ray(const Point3& orig, const Vec3& dir) : origin_(orig), direction_(dir) {}

    Point3 origin() const { return origin_; }
    Vec3 direction() const { return direction_; }

    Point3 at(double t) const { return origin_ + (t * direction_); }

    Point3 origin_;
    Vec3 direction_;
};

#endif  // RAY_HPP_
