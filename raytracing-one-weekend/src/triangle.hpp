#ifndef INCLUDE_SRC_TRIANGLE_HPP_
#define INCLUDE_SRC_TRIANGLE_HPP_

#include <memory>

#include "aabb.hpp"
#include "hittable.hpp"
#include "material.hpp"
#include "vec2.hpp"
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

    triangle(Point3 a_, Point3 b_, Point3 c_, Vec2 a_uv, Vec2 b_uv, Vec2 c_uv,
             std::shared_ptr<material> mat_)
        : a(a_),
          u_vec(a_ - b_),
          v_vec(a_ - c_),
          normal(unit_vector(cross(u_vec, v_vec))),
          uv{a_uv, b_uv, c_uv},
          mat(mat_) {
        box = aabb::from_points({a_, b_, c_});
        box.padding();
    }
    triangle(Point3 a_, Point3 b_, Point3 c_, Point3 normal_, Vec2 a_uv,
             Vec2 b_uv, Vec2 c_uv)
        : a(a_),
          u_vec(a_ - b_),
          v_vec(a_ - c_),
          normal(normal_),
          uv{a_uv, b_uv, c_uv} {
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
        rec.texture_uv = get_texture_uv(u, v);
        rec.set_face_normal(r, normal);
        rec.material_ = mat.get();
        return true;
    }

    Vec2 get_texture_uv(float triangle_u, float triangle_v) const noexcept {
        auto A_weight = (1 - triangle_u - triangle_v);
        return uv[0] * A_weight + uv[1] * triangle_u + uv[2] * triangle_v;
    }

private:
    Point3 a;
    Vec3 u_vec, v_vec;
    Vec3 normal;
    Vec2 uv[3];
    aabb box;
    std::shared_ptr<material> mat;
};

#endif  // INCLUDE_SRC_TRIANGLE_HPP_
