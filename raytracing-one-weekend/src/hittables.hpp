#ifndef INCLUDE_SRC_HITTABLES_HPP_
#define INCLUDE_SRC_HITTABLES_HPP_
#include <memory>
#include <vector>

#include "aabb.hpp"
#include "hittable.hpp"
#include "math_util.hpp"

using HittablesRecord = std::vector<std::shared_ptr<Hittable>>;

class Hittables : public Hittable {
public:
    Hittables() {}
    Hittables(std::vector<std::shared_ptr<Hittable>>&& vec) : objects(vec) {
        for (const auto& i : objects) {
            bbox = aabb(bbox, i->bounding_box());
        }
    }

    void add(std::shared_ptr<Hittable> o) {
        bbox = aabb(bbox, o->bounding_box());
        objects.emplace_back(std::move(o));
    }

    bool hit(const ray& r, RealType tmin, RealType tmax,
             HitRecord& rec) const override;

    aabb bounding_box() const override { return bbox; }

    HittablesRecord objects;
    aabb bbox;
};

inline bool Hittables::hit(const ray& r, RealType tmin, RealType tmax,
                           HitRecord& rec) const {
    HitRecord tmp;
    bool hitted = false;
    auto closet_so_far = tmax;

    for (const auto& i : objects) {
        if (i->hit(r, tmin, closet_so_far, tmp)) {
            hitted = true;
            closet_so_far = tmp.t;
            rec = tmp;
        }
    }
    return hitted;
}

#endif  // INCLUDE_SRC_HITTABLES_HPP_
