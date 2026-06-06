#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <ostream>
#include <thread>

#include "camera.hpp"
#include "color.hpp"
#include "hittable.hpp"
#include "hittables.hpp"
#include "math_util.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "vec3.hpp"

using std::sqrt;

Color ray_color(const ray& r, const Hittable& hittable, int depth = 10) {
    HitRecord rec;
    Vec3 sphere_center{0, 0, -1};
    if (depth <= 0) {
        return Color{0, 0, 0};
    }
    if (hittable.hit(r, 0.001, Infinity, rec)) {
        ray new_ray{rec.p, rec.p + rec.normal + random_unit_sphere_vector()};
        return 0.5 * ray_color(new_ray, hittable, depth - 1);
        //       return 0.5 * (rec.normal + Vec3(1, 1, 1));
    }
    Vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

int main(int argc, char *argv[]) {
    constexpr auto aspect_ratio = 16.0 / 9.0;
    constexpr int image_width = 384;
    constexpr int image_height = image_width / aspect_ratio;
    constexpr int samples_per_pixel = 100;
    std::cerr << "image size: " << image_width << 'x' << image_height << "\n";

    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;
    Hittables world;
    world.add(std::make_shared<Sphere>(Vec3{-1.5, 0, -1}, 0.5));
    world.add(std::make_shared<Sphere>(Vec3{-1, 0, -1.5}, 0.5));
    world.add(std::make_shared<Sphere>(Vec3{-0.5, 0, -2}, 0.5));
    world.add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100));

    Camera camera(16, 9);

    std::chrono::time_point<std::chrono::system_clock> start =
        std::chrono::system_clock::now();

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rremaining " << j << " " << std::flush;
        for (int i = 0; i < image_width; ++i) {
            Color pixel_color{0, 0, 0};
            for (int k = 0; k < samples_per_pixel; k++) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                pixel_color += ray_color(camera.get_ray(u, v), world);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    std::chrono::time_point<std::chrono::system_clock> stop =
        std::chrono::system_clock::now();

    std::cerr << "\nerapsed: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(stop -
                                                                       start)
                         .count() /
                     1000.0
              << "s" << '\n';
}
