#ifndef HITTABLE_HPP_
#define HITTABLE_HPP_
#include <memory>

#include "math_util.hpp"
#include "ray.hpp"
#include "vec3.hpp"

class material;
struct HitRecord {
    Point3 p;
    Vec3 normal;
    material *material_;
    RealType t;
    bool front_face;

    inline void set_face_normal(const ray& r, const Vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
public:
    virtual ~Hittable() = default;
    virtual bool hit(const ray& r, RealType t_min, RealType t_max,
                     HitRecord& rec) const = 0;
};

#endif  // HITTABLE_HPP_
