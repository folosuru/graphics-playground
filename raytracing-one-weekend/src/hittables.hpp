#ifndef INCLUDE_SRC_HITTABLES_HPP_
#define INCLUDE_SRC_HITTABLES_HPP_
#include <memory>
#include <vector>

#include "hittable.hpp"
#include "math_util.hpp"

class Hittables : public Hittable {
public:
    Hittables() {}

    void add(std::shared_ptr<Hittable> o) {
        objects.emplace_back(std::move(o));
    }

    virtual bool hit(const ray& r, RealType tmin, RealType tmax,
                     HitRecord& rec) const override;

    std::vector<std::shared_ptr<Hittable>> objects;
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
