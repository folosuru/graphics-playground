#ifndef INCLUDE_SRC_TRIANGLE_HPP_
#define INCLUDE_SRC_TRIANGLE_HPP_

#include "hittable.hpp"
#include "vec3.hpp"
class triangle : public Hittable {
public:
    bool hit(const ray& r, RealType t_min, RealType t_max,
             HitRecord& rec) const override {
        // P = origin + t * D = A + u(B- a) + v(C - A)
        // origin - A = u(B-A) + v(C-A) - t * D

        auto oa = r.origin() - a;

        auto matrix_det = det(b - a, c - a, r.direction());
    }

private:
    Point3 a, b, c;
};

#endif  // INCLUDE_SRC_TRIANGLE_HPP_
