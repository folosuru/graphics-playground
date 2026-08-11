#ifndef INCLUDE_SRC_RENDRER_HPP_
#define INCLUDE_SRC_RENDRER_HPP_
#include <chrono>
#include <iomanip>
#include <iostream>
#include <memory>
#include <ostream>

#include "bvh.hpp"
#include "camera.hpp"
#include "color.hpp"
#include "hittable.hpp"
#include "hittables.hpp"
#include "material.hpp"
#include "timer.hpp"

class Renderer {
    static constexpr auto aspect_ratio = 16.0 / 9.0;
    static constexpr int image_width = 320;
    static constexpr int image_height = image_width / aspect_ratio;
    static constexpr int samples_per_pixel = 25;

public:
    struct RenderResult {
        double take_seconds;
    };

    Renderer() : buffer(image_height * image_width * 3) {
        std::cerr << "image size: " << image_width << 'x' << image_height
                  << "\n";
    }

    void render(HittablesRecord objs, Camera& camera) {
        std::unique_ptr<Hittable> world;

        auto time = timer([&, this]() {
            world = bvh_node::create(objs.begin(), objs.end());
        });
        std::cerr << "\ncreate bvh take: " << time << "sec \n";
        render(world, camera);
    }

    RenderResult render(std::unique_ptr<Hittable>& bvh, Camera& camera) {
        using std::chrono::system_clock;
        using std::chrono::time_point;

        auto time = timer([&, this]() {
#pragma omp parallel for
            for (int j = image_height - 1; j >= 0; --j) {
                /*
                std::cerr << "\rremaining " << j << " " << std::setprecision(2)
                          << ((100 * j) / (image_height - 1)) << "%    "
                          << std::flush;
                */
                for (int i = 0; i < image_width; ++i) {
                    Color pixel_color{0, 0, 0};
                    for (int k = 0; k < samples_per_pixel; k++) {
                        auto u = (i + random_double()) / (image_width - 1);
                        auto v = (j + random_double()) / (image_height - 1);
                        pixel_color += ray_color(camera.get_ray(u, v), *bvh);
                    }
                    write_colorbuf(
                        buffer.data() +
                            ((image_height - j - 1) * image_width + i) * 3,
                        pixel_color, samples_per_pixel);
                }
            }
        });
        std::cout.write(reinterpret_cast<const char *>(buffer.data()),
                        image_height * image_width * 3);
        return {time.seconds()};
    }

    static Color ray_color(const ray& r, const Hittable& hittable,
                           int depth = 10) {
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
        }
        Vec3 unit_direction = unit_vector(r.direction());
        auto t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
    }

private:
    std::vector<unsigned char> buffer;
};

#endif  // INCLUDE_SRC_RENDRER_HPP_
