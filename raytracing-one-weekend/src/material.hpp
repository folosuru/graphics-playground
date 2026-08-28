#ifndef INCLUDE_SRC_MATERIAL_HPP_
#define INCLUDE_SRC_MATERIAL_HPP_

#include "hittable.hpp"
#include "math_util.hpp"
#include "ray.hpp"
#include "texture.hpp"
#include "vec3.hpp"
class material {
public:
    virtual ~material() = default;
    virtual bool scatter(const ray& in_ray, const HitRecord& record,
                         Color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
public:
    lambertian(const Color& a) : albedo(a) {}
    bool scatter(const ray& r_in, const HitRecord& rec, Color& attenuation,
                 ray& scattered) const override {
        Vec3 scatter_direction = rec.normal + random_unit_sphere_vector();
        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

    Color albedo;
};

class checker : public material {
public:
    checker(const Color& a) : albedo(a) {}
    bool scatter(const ray& r_in, const HitRecord& rec, Color& attenuation,
                 ray& scattered) const override {
        Vec3 scatter_direction = rec.normal + random_unit_sphere_vector();
        scattered = ray(rec.p, scatter_direction);
        attenuation = tex.get({(floor(rec.p.x() * 10) + floor(rec.p.y() * 10) +
                                floor(rec.p.z() * 10)),
                               0});
        return true;
    }

    Color albedo;
    checker_texture tex;
};

class lambertian_texture : public material {
public:
    lambertian_texture(texture *texture) : texture_(texture) {}
    bool scatter(const ray& r_in, const HitRecord& rec, Color& attenuation,
                 ray& scattered) const override {
        Vec3 scatter_direction = rec.normal + random_unit_sphere_vector();
        scattered = ray(rec.p, scatter_direction);
        attenuation = texture_->get(rec.texture_uv);
        return true;
    }
    texture *texture_;
};

class metal : public material {
public:
    metal(const Color& a, RealType f) : albedo(a), fuzziness(f < 1 ? f : 1) {}
    bool scatter(const ray& r_in, const HitRecord& rec, Color& attenuation,
                 ray& scattered) const override {
        Vec3 scatter_direction = rec.normal + random_unit_sphere_vector();
        auto reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = {rec.p,
                     reflected + fuzziness * random_unit_sphere_vector()};
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

    Color albedo;
    RealType fuzziness;
};

class dielectric : public material {
public:
    dielectric(RealType refraction_index)
        : refraction_index(refraction_index) {}

    bool scatter(const ray& r_in, const HitRecord& rec, Color& attenuation,
                 ray& scattered) const override {
        attenuation = Color(1.0, 1.0, 1.0);
        RealType ri =
            rec.front_face ? (1.0 / refraction_index) : refraction_index;

        Vec3 unit_direction = unit_vector(r_in.direction());
        RealType cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
        RealType sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = ri * sin_theta > 1.0;
        Vec3 direction;

        if (cannot_refract || reflectance(cos_theta, ri) > random_double())
            direction = reflect(unit_direction, rec.normal);
        else
            direction = refract(unit_direction, rec.normal, ri);

        scattered = ray(rec.p, direction);
        return true;
    }

private:
    // Refractive index in vacuum or air, or the ratio of the material's refractive index over
    // the refractive index of the enclosing media
    RealType refraction_index;

    static RealType reflectance(double cosine, double refraction_index) {
        auto r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0 * r0;
        return r0 + (1 - r0) * std::pow((1 - cosine), 5);
    }
};

#endif  // INCLUDE_SRC_MATERIAL_HPP_
