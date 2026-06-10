#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <memory>
#include <ostream>
#include <thread>
#include <vector>

#include "camera.hpp"
#include "color.hpp"
#include "hittable.hpp"
#include "hittables.hpp"
#include "material.hpp"
#include "math_util.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "vec3.hpp"

using std::sqrt;

Color ray_color(const ray& r, const Hittable& hittable, int depth = 10) {
    HitRecord rec;
    if (depth <= 0) {
        return Color{0, 0, 0};
    }
    if (hittable.hit(r, 0.001, Infinity, rec)) {
        ray new_ray;
        Color attenuation;
        if (rec.material_->scatter(r, rec, attenuation, new_ray)) {
            return attenuation * ray_color(new_ray, hittable, depth - 1);
        } else {
            return Color{0, 0, 0};  // コレは何？
        }
        //       ray new_ray{rec.p, rec.p + rec.normal + random_unit_sphere_vector()};
        //       return 0.5 * ray_color(new_ray, hittable, depth - 1);
        //       return 0.5 * (rec.normal + Vec3(1, 1, 1));
    }
    Vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

void add_cubes(Hittables& world) {
    std::shared_ptr<material> mats[] = {
        std::make_shared<lambertian>(Color{0.9, 0.3, 0.3}),
        std::make_shared<lambertian>(Color{0.3, 0.7, 0.9}),
        std::make_shared<lambertian>(Color{0.1, 0.9, 0.1}),
        std::make_shared<lambertian>(Color{0.1, 0.5, 0.5}),
        std::make_shared<lambertian>(Color{0.1, 0.1, 0.2}),
        std::make_shared<metal>(Color{0.8, 0.8, 0.8}, 0),
        std::make_shared<metal>(Color{0.8, 0.8, 0.3}, 0.5),
        std::make_shared<metal>(Color{0.1, 0.8, 0.3}, 0.5),
        std::make_shared<dielectric>(1.5),
    };

    for (int i = 0; i < 128; i++) {
        Vec3 pos{random_double(-3, 3), 0.1, random_double(-5, 1)};
        world.add(std::make_shared<Sphere>(
            pos, 0.1, mats[rand() % (sizeof(mats) / sizeof(mats[0]))]));
    }
}

int main(int argc, char *argv[]) {
    constexpr auto aspect_ratio = 16.0 / 9.0;
    constexpr int image_width = 640;
    constexpr int image_height = image_width / aspect_ratio;
    constexpr int samples_per_pixel = 30;
    std::cerr << "image size: " << image_width << 'x' << image_height << "\n";

    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;
    Hittables world;

    std::shared_ptr<material> mats[] = {
        std::make_shared<lambertian>(Color{0.7, 0.7, 0.7}),
        std::make_shared<lambertian>(Color{0.7, 0.2, 0.3}),
        std::make_shared<metal>(Color{0.8, 0.8, 0.8}, 0),
        std::make_shared<dielectric>(1.5)};
    world.add(std::make_shared<Sphere>(Vec3{-1.5, 0.5, -1}, 0.25, mats[2]));
    world.add(std::make_shared<Sphere>(Vec3{1, 0.25, -1.5}, 0.25, mats[1]));
    world.add(std::make_shared<Sphere>(Vec3{0.5, 0.25, -0.5}, 0.25, mats[3]));
    world.add(std::make_shared<Sphere>(Point3(0, -100, -1), 100, mats[0]));

    add_cubes(world);
    std::chrono::time_point<std::chrono::system_clock> start =
        std::chrono::system_clock::now();

    std::vector<unsigned char> buffer;
    buffer.resize(image_height * image_width * 3);

    for (int frame = 0; frame < 1; frame++) {
        std::cerr << "\rframe " << frame << "  ";
        Camera camera(16, 9, degrees_to_radians(90),
                      {Vec3{0, 0.5, 0}, Vec3{0, 0, -1}, Vec3{0, 1, 0}},
                      degrees_to_radians(0.5), 0.5);
        //#pragma omp parallel for
        for (int j = image_height - 1; j >= 0; --j) {
            /*   std::cerr << "\rremaining " << j << " " << std::setprecision(2)
                      << ((100 * j) / (image_height - 1)) << "%    "
                      << std::flush;*/
            for (int i = 0; i < image_width; ++i) {
                Color pixel_color{0, 0, 0};
                for (int k = 0; k < samples_per_pixel; k++) {
                    auto u = (i + random_double()) / (image_width - 1);
                    auto v = (j + random_double()) / (image_height - 1);
                    pixel_color += ray_color(camera.get_ray(u, v), world);
                }
                write_colorbuf(
                    buffer.data() +
                        ((image_height - j - 1) * image_width + i) * 3,
                    pixel_color, samples_per_pixel);
            }
        }
        std::cout.write(reinterpret_cast<const char *>(buffer.data()),
                        image_height * image_width * 3);
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
