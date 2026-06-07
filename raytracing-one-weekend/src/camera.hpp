#ifndef INCLUDE_SRC_CAMERA_HPP_
#define INCLUDE_SRC_CAMERA_HPP_

#include <cmath>

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
    Camera(int aspect_ratio_x, int aspect_ratio_y, double fov, CameraPos pos_)
        : pos(pos_) {
        const auto aspect_ratio = (double)aspect_ratio_x / aspect_ratio_y;
        auto h = std::tan(fov / 2);

        auto viewport_height = 2 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        auto w = unit_vector(pos.lookfrom - pos.lookat);
        auto u = unit_vector(cross(pos.vup, w));
        auto v = cross(w, u);

        origin = pos.lookfrom;
        horizontal = viewport_height * u;
        vertical = viewport_height * v;
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - w;
    }

    ray get_ray(double u, double v) const {
        return ray(origin,
                   lower_left_corner + u * horizontal + v * vertical - origin);
    }

    Point3 origin, lower_left_corner;
    Vec3 horizontal, vertical;
    CameraPos pos;
};

#endif  // INCLUDE_SRC_CAMERA_HPP_
