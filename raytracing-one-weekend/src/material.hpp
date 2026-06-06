#ifndef INCLUDE_SRC_MATERIAL_HPP_
#define INCLUDE_SRC_MATERIAL_HPP_

#include "hittable.hpp"
#include "ray.hpp"
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

class metal : public material {
public:
    metal(const Color& a, double f) : albedo(a), fuzziness(f < 1 ? f : 1) {}
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
    double fuzziness;
};

class dielectric : public material {
public:
    dielectric(double refraction_index) : refraction_index(refraction_index) {}

    bool scatter(const ray& r_in, const HitRecord& rec, Color& attenuation,
                 ray& scattered) const override {
        attenuation = Color(1.0, 1.0, 1.0);
        double ri =
            rec.front_face ? (1.0 / refraction_index) : refraction_index;

        Vec3 unit_direction = unit_vector(r_in.direction());
        double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

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
    double refraction_index;

    static double reflectance(double cosine, double refraction_index) {
        auto r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0 * r0;
        return r0 + (1 - r0) * std::pow((1 - cosine), 5);
    }
};

#endif  // INCLUDE_SRC_MATERIAL_HPP_
