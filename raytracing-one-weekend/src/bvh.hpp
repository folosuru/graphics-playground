#ifndef INCLUDE_SRC_BVH_HPP_
#define INCLUDE_SRC_BVH_HPP_

#include <memory>

#include "aabb.hpp"
#include "hittable.hpp"
class bvh_node : public Hittable {
public:
    bool hit(const ray& r, RealType t_min, RealType t_max,
             HitRecord& rec) const override;

    aabb bounding_box() const override { return bbox; }

private:
    std::unique_ptr<Hittable> left;
    std::unique_ptr<Hittable> right;
    aabb bbox;
};

#endif  // INCLUDE_SRC_BVH_HPP_
