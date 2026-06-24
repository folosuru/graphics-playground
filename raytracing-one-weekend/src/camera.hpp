#ifndef INCLUDE_SRC_CAMERA_HPP_
#define INCLUDE_SRC_CAMERA_HPP_

#include <cmath>

#include "math_util.hpp"
#include "ray.hpp"
#include "vec3.hpp"

struct CameraPos {
    Point3 lookfrom;
    Point3 lookat;
    Vec3 vup;
};

class Camera {
public:
    // fov is radian
    Camera(int aspect_ratio_x, int aspect_ratio_y, RealType fov, CameraPos pos_,
           RealType defocus_angle_, RealType focus_dist_)
        : pos(pos_), defocus_angle(defocus_angle_), focus_dist(focus_dist_) {
        const auto aspect_ratio = (RealType)aspect_ratio_x / aspect_ratio_y;
        auto h = std::tan(fov / 2);

        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = aspect_ratio * viewport_height;

        auto w = unit_vector(pos.lookfrom - pos.lookat);
        auto u = unit_vector(cross(pos.vup, w));
        auto v = cross(w, u);

        origin = pos.lookfrom;
        horizontal = viewport_width * u;
        vertical = viewport_height * v;
        lower_left_corner =
            origin - horizontal / 2 - vertical / 2 - (w * focus_dist);

        auto defocus_r = focus_dist * tan(defocus_angle / 2);
        defocus_disk_u = defocus_r * u;
        defocus_disk_v = defocus_r * v;
    }

    ray get_ray(RealType u, double v) const {
        if (defocus_angle <= 0) {
            return ray(origin, lower_left_corner + u * horizontal +
                                   v * vertical - origin);
        } else {
            auto p = random_unit_disk_vector();
            auto new_origin =
                origin + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
            return ray(new_origin,
                       (lower_left_corner + u * horizontal + v * vertical) -
                           new_origin);
        }
    }

    Point3 origin, lower_left_corner;
    Vec3 horizontal, vertical;
    Vec3 defocus_disk_u, defocus_disk_v;
    RealType defocus_angle, focus_dist;
    CameraPos pos;
};

#endif  // INCLUDE_SRC_CAMERA_HPP_
