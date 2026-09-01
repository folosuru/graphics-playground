#include <iomanip>
#include <ios>
#include <iostream>
#include <memory>

#include "camera.hpp"
#include "hittables.hpp"
#include "image/image_file.hpp"
#include "material.hpp"
#include "math_util.hpp"
#include "obj_loader.hpp"
#include "rendrer.hpp"
#include "sphere.hpp"
#include "texture.hpp"
#include "timer.hpp"
#include "triangle.hpp"
#include "vec3.hpp"

void scene1() {
    HittablesRecord objs;

    std::shared_ptr<material> mats[] = {
        std::make_shared<lambertian>(Color{0.7, 0.7, 0.7}),
        std::make_shared<lambertian>(Color{0.7, 0.2, 0.3}),
        std::make_shared<metal>(Color{0.8, 0.8, 0.8}, 0),
        std::make_shared<dielectric>(1.5)};

    objs.push_back(
        std::make_shared<Sphere>(Vec3{-1.5, 0.5, -1}, 0.25, mats[2]));
    objs.push_back(
        std::make_shared<Sphere>(Vec3{1, 0.25, -1.5}, 0.25, mats[1]));
    objs.push_back(
        std::make_shared<Sphere>(Vec3{0.5, 0.25, -0.5}, 0.25, mats[3]));
    objs.push_back(std::make_shared<Sphere>(Point3(0, -100, -1), 100, mats[0]));

    std::shared_ptr<material> smallmats[] = {
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
        objs.push_back(std::make_shared<Sphere>(
            pos, 0.1,
            smallmats[rand() % (sizeof(smallmats) / sizeof(smallmats[0]))]));
    }

    for (int frame = 0; frame < 1; frame++) {
        Camera camera(16, 9, degrees_to_radians(90),
                      {Vec3{0, 0.5, 0}, Vec3{0, 0, -1}, Vec3{0, 1, 0}},
                      degrees_to_radians(0.5), 0.5);

        Renderer().render(objs, camera);
    }
}

Vec3 frame_rotation(int total, int current, RealType distance,
                    RealType y = 1.5) {
    const auto radian = (current * pi * 2) / total;
    return Vec3{sin(radian) * distance, y, cos(radian) * distance};
}

void scene2() {
    HittablesRecord objs;
    objs.push_back(std::make_shared<triangle>(
        Vec3{-1, -0.5, 0}, Vec3{0, -0.5, 0}, Vec3{0, 0.5, 0.2}));
    std::shared_ptr<material> mats[] = {
        std::make_shared<lambertian>(Color{0.7, 0.7, 0.7}),
        std::make_shared<lambertian>(Color{0.7, 0.2, 0.3}),
        std::make_shared<metal>(Color{0.8, 0.8, 0.8}, 0),
        std::make_shared<dielectric>(1.5)};

    objs.push_back(std::make_shared<Sphere>(Vec3{-1, -0.5, 0}, 0.05, mats[1]));
    auto bvh = bvh_node::create(objs.begin(), objs.end());
    for (int frame = 0; frame < 1; frame++) {
        Camera camera(
            16, 9, degrees_to_radians(45),
            {frame_rotation(89, frame, 2.0), Vec3{0, 0, 0}, Vec3{0, 1, 0}},
            degrees_to_radians(0.5), 0.5);

        Renderer().render(bvh, camera);
    }
}

void scene3() {
    constexpr int frame_count = 30;
    HittablesRecord objs;
    load_obj(objs, "models/utah_teapot.obj");
    std::cerr << "object count = " << objs.size() << ".\n";

    auto r = Renderer();
    auto bvh = bvh_node::create(objs.begin(), objs.end());
    auto time = timer([&]() {
        for (int frame = 0; frame < frame_count; frame++) {
            Camera camera(16, 9, degrees_to_radians(45),
                          {frame_rotation(frame_count + 1, frame, 6.0, 3),
                           Vec3{0, 1, 0}, Vec3{0, 1, 0}},
                          degrees_to_radians(0.5), 0.5);

            auto time = r.render(bvh, camera);
            std::cerr << "frame " << std::setw(4) << std::right << frame
                      << ", take " << time.take_seconds << "sec \n";
        }
    });
    std::cerr << "total take: " << time << " s       \n";
}

void scene4() {
    HittablesRecord objs;
    objs.push_back(std::make_shared<Sphere>(
        Point3(0, -100, -1), 100,
        std::make_shared<lambertian>(Color{0.7, 0.7, 0.7})));
    std::shared_ptr<material> mat =
        std::make_shared<lambertian_texture>(new test_texture());

    objs.push_back(std::make_shared<triangle>(Vec3{0, 0, 0}, Vec3{1, 0, 0},
                                              Vec3{1, 1, 0}, Vec2{0, 0},
                                              Vec2{1, 0}, Vec2{1, 1}, mat));

    objs.push_back(std::make_shared<triangle>(Vec3{0, 1, 0}, Vec3{0, 0, 0},
                                              Vec3{1, 1, 0}, Vec2{0, 1},
                                              Vec2{0, 0}, Vec2{1, 1}, mat));

    objs.push_back(std::make_shared<Sphere>(Vec3{-1, 0.5, 0}, 0.5, mat));
    auto bvh = bvh_node::create(objs.begin(), objs.end());
    for (int frame = 0; frame < 30; frame++) {
        Camera camera(
            16, 9, degrees_to_radians(45),
            {frame_rotation(29, frame, 3.0, 0.5), Vec3{0, 0, 0}, Vec3{0, 1, 0}},
            degrees_to_radians(0.5), 0.5);

        Renderer().render(bvh, camera);
    }
}

void scene5() {
    HittablesRecord objs;

    auto earth = image_file::create("models/Equirectangular-projection.jpg");
    std::shared_ptr<material> mat =
        std::make_shared<lambertian_texture>(new image_texture(earth));

    objs.push_back(std::make_shared<Sphere>(Vec3{0, 0, 0}, 0.75, mat));
    auto bvh = bvh_node::create(objs.begin(), objs.end());
    for (int frame = 0; frame < 30; frame++) {
        Camera camera(
            16, 9, degrees_to_radians(45),
            {frame_rotation(31, frame, 3.0, 0.5), Vec3{0, 0, 0}, Vec3{0, 1, 0}},
            degrees_to_radians(0.5), 0.5);

        Renderer().render(bvh, camera);
    }
}
