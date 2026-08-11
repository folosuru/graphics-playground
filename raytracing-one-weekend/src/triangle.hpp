#ifndef INCLUDE_SRC_TRIANGLE_HPP_
#define INCLUDE_SRC_TRIANGLE_HPP_

#include <memory>

#include "aabb.hpp"
#include "hittable.hpp"
#include "material.hpp"
#include "vec3.hpp"
class triangle : public Hittable {
public:
    triangle(Point3 a_, Point3 b_, Point3 c_)
        : a(a_),
          u_vec(a_ - b_),
          v_vec(a_ - c_),
          normal(unit_vector(cross(u_vec, v_vec))) {
        mat = std::make_shared<lambertian>(Color{0.8, 0.8, 0.8});
        box = aabb::from_points({a_, b_, c_});
        box.padding();
    }

    aabb bounding_box() const override { return box; }

    bool hit(const ray& r, RealType t_min, RealType t_max,
             HitRecord& rec) const override {
        // P = origin + t * D = A + u(B- A) + v(C - A)
        // origin - A = u(B-A) + v(C-A) - t * D
        //
        // u, v, tの符号を揃えるために符号を反転
        // A-origin =  u(A-B) + v(A-C) + t * D
        //

        auto ao = a - r.origin();

        auto matrix_det = det(u_vec, v_vec, r.direction());
        auto u = det(ao, v_vec, r.direction()) / matrix_det;
        auto v = det(u_vec, ao, r.direction()) / matrix_det;
        auto t = det(u_vec, v_vec, ao) / matrix_det;
        if (!(t_min <= t && t <= t_max)) return false;
        if (!(0 <= u && u <= 1)) return false;
        if (!(0 <= v && v <= 1)) return false;
        if (!(u + v < 1)) return false;

        rec.t = t;
        rec.p = r.at(t);
        rec.set_face_normal(r, normal);
        rec.material_ = mat.get();
        return true;
    }

private:
    Point3 a;
    Vec3 u_vec, v_vec;
    Vec3 normal;
    aabb box;
    std::shared_ptr<material> mat;
};

#endif  // INCLUDE_SRC_TRIANGLE_HPP_
