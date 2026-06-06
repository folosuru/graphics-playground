#include <chrono>
#include <iostream>
#include <ostream>
#include <thread>

#include "color.hpp"
#include "ray.hpp"
#include "vec3.hpp"

bool hit_sphere(const Point3& center, double r, const ray& ray) {
    // 式は
    // b^2 * t^w + 2b * (A-C) * t  + (A-C) * (A-C) - r^2

    // = A-C
    Vec3 oc = ray.origin() - center;
    // b^2
    auto a = dot(ray.direction(), ray.direction());
    // 2b * (A - C)
    auto b = 2.0 * dot(oc, ray.direction());
    // (A-C) * (A-C) - r^2
    auto c = dot(oc, oc) - r * r;

    auto D = b * b - (4 * a * c);
    return (D > 0);
}

Color ray_color(const ray& r) {
    if (hit_sphere(Point3{0, 0, -1}, 0.5, r)) {
        return Color{1, 0, 0};
    }
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
