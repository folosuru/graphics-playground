#include <chrono>
#include <iostream>
#include <ostream>
#include <thread>

#include "color.hpp"
#include "ray.hpp"

Color ray_color(const ray& r) {
    Vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0, 0, 1.0);
}

int main(int argc, char *argv[]) {
    constexpr auto aspect_ratio = 16.0 / 9.0;
    constexpr int image_width = 384;
    constexpr int image_height = image_width / aspect_ratio;
    std::cerr << "image size: " << image_width << 'x' << image_height << "\n";

    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = Point3(0, 0, 0);
    auto horizontal = Vec3(viewport_width, 0, 0);
    auto vertical = Vec3(0, viewport_height, 0);
    auto lower_left_corner =
        origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focal_length);

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rremaining " << j << " " << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width - 1);
            auto v = double(j) / (image_height - 1);
            ray r(origin,
                  lower_left_corner + u * horizontal + v * vertical - origin);
            Color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }
    std::cerr << '\n';
}
