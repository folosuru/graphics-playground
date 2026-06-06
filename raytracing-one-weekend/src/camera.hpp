#ifndef INCLUDE_SRC_CAMERA_HPP_
#define INCLUDE_SRC_CAMERA_HPP_

#include "ray.hpp"
#include "vec3.hpp"
class Camera {
public:
    Camera(int aspect_ratio_x, int aspect_ratio_y) {
        const auto aspect_ratio = (double)aspect_ratio_x / aspect_ratio_y;

        auto viewport_height = 2.0;
        auto viewport_width = aspect_ratio * viewport_height;
        auto focal_length = 1.0;
        origin = Point3{0, 0, 0};
        horizontal = Vec3{viewport_width, 0, 0};
        vertical = Vec3{0, viewport_height, 0};
        lower_left_corner =
            origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focal_length);
    }

    ray get_ray(double u, double v) const {
        return ray(origin,
                   lower_left_corner + u * horizontal + v * vertical - origin);
    }

    Point3 origin, lower_left_corner;
    Vec3 horizontal, vertical;
};

#endif  // INCLUDE_SRC_CAMERA_HPP_
