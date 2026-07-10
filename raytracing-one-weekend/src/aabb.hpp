#ifndef INCLUDE_SRC_AABB_HPP_
#define INCLUDE_SRC_AABB_HPP_
#include "math_util.hpp"
#include "range.hpp"
#include "ray.hpp"
#include "vec3.hpp"

class aabb {
public:
    bool contains(const ray& ray, range ray_t_range) {
        range current_t_range = ray_t_range;

        // x = ray.origin() + ray.direction() * t;
        // x - ray.origin() = ray.direction() * t;
        // (x - ray.origin()) / ray.direction() = t;

        Vec3 ray_dir_rcp = ray.direction().rcp();

        // start_t および end_t はNaN になりえる。
        // (start[n] - ray.origin()[n]) = 0 && ray_dir_rcp[n] = Infinity のとき
        // 0 / Infinity = NaN .
        //
        // これは何かと言うと、レイがスラブの平面上に存在する時。
        // 例えばmin はどの値でもスラブ上なのでNaN 、max はどの値でもスラブ上にならないのでInfinity になる。
        //
        // のだが、extract_min/maxでNaN は除去されInfinityだけが残り、min_t にInfinityが混入するので戻り値はfalseになる。
        Vec3 start_t = (start - ray.origin()) * ray_dir_rcp;
        Vec3 end_t = (end - ray.origin()) * ray_dir_rcp;

        Vec3 min_vec = start_t.extract_min(end_t);
        Vec3 max_vec = start_t.extract_max(end_t);

        RealType min_t =
            fold_max(min_vec.x(), min_vec.y(), min_vec.z(), ray_t_range.min());
        RealType max_t =
            fold_min(max_vec.x(), max_vec.y(), max_vec.z(), ray_t_range.max());

        return min_t <= max_t;
    }

    aabb(Point3 p1, Point3 p2)
        : start(p1.extract_min(p2)), end(p1.extract_max(p2)) {}

    aabb(const aabb& v1, const aabb& v2)
        : start(v1.start.extract_min(v2.start)),
          end(v1.end.extract_max(v2.start)) {}

    aabb()
        : start(Infinity, Infinity, Infinity),
          end(-Infinity, -Infinity, -Infinity) {}

private:
    Vec3 start, end;
};

#endif  // INCLUDE_SRC_AABB_HPP_
