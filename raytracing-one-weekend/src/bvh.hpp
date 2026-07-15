#ifndef INCLUDE_SRC_BVH_HPP_
#define INCLUDE_SRC_BVH_HPP_

#include <algorithm>
#include <cstddef>
#include <memory>
#include <vector>

#include "aabb.hpp"
#include "hittable.hpp"
#include "hittables.hpp"
#include "math_util.hpp"
class bvh_node : public Hittable {
public:
    bvh_node(std::unique_ptr<Hittable> l, std::unique_ptr<Hittable> r)
        : left(std::move(l)),
          right(std::move(r)),
          bbox(left->bounding_box(), right->bounding_box()) {}

    bool hit(const ray& r, RealType t_min, RealType t_max,
             HitRecord& rec) const override {
        if (!bbox.contains(r, {t_min, t_max})) {
            return false;
        }
        bool l_result = left->hit(r, t_min, t_max, rec);
        bool r_result = l_result ? right->hit(r, t_min, rec.t, rec)
                                 : right->hit(r, t_min, t_max, rec);

        return l_result || r_result;
    }

    aabb bounding_box() const override { return bbox; }

    using HittableIter = HittablesRecord::iterator;
    static std::unique_ptr<Hittable> create(HittableIter begin,
                                            HittableIter end) {
        size_t size = end - begin;
        if (size <= 2) {
            return std::make_unique<Hittables>(HittablesRecord(begin, end));
        }
        aabb all;
        for (auto iter = begin; iter != end; iter++) {
            all = aabb(all, (*iter)->bounding_box());
        }
        auto comp = aabb::compare_min(0);  //random_double() * 3);
        std::sort(begin, end, [&comp](auto l, auto r) {
            return comp((*l).bounding_box(), (*r).bounding_box());
        });

        HittableIter mid = begin + (size / 2);

        return std::make_unique<bvh_node>(create(begin, mid), create(mid, end));
    }

private:
    std::unique_ptr<Hittable> left;
    std::unique_ptr<Hittable> right;
    aabb bbox;
};

#endif  // INCLUDE_SRC_BVH_HPP_
