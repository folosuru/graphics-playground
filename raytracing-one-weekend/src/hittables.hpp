#ifndef INCLUDE_SRC_HITTABLES_HPP_
#define INCLUDE_SRC_HITTABLES_HPP_
#include <memory>
#include <vector>

#include "aabb.hpp"
#include "hittable.hpp"
#include "math_util.hpp"

class Hittables : public Hittable {
public:
    Hittables() {}

    void add(std::shared_ptr<Hittable> o) {
        bbox = aabb(bbox, o->bounding_box());
        objects.emplace_back(std::move(o));
    }

    bool hit(const ray& r, RealType tmin, RealType tmax,
             HitRecord& rec) const override;

    aabb bounding_box() const override { return bbox; }

    std::vector<std::shared_ptr<Hittable>> objects;
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
