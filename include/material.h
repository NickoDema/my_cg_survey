#ifndef MATERIALH
#define MATERIALH

#include "ray.h"
#include "hitable.h"
#include "my_random.h"

struct hit_record;

vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(v,n)*n;
}

class Material {
    public:
        virtual bool scatter(const ray& ray_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};

class Lambertian : public Material {
public:
    Lambertian(const vec3& a) : albedo(a) {};
    virtual bool scatter(const ray& ray_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
        vec3 target = rec.p + rec.n + rand_in_unit_sphere();
        scattered = ray(rec.p, target-rec.p);
        attenuation = albedo;
        return true;
    }

    vec3 albedo;
};

class Metal : public Material {
public:
    Metal(const vec3& a) : albedo(a) {};
    virtual bool scatter(const ray& ray_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
        vec3 reflected = reflect(make_unit(ray_in.direction()), rec.n);
        scattered = ray(rec.p, reflected);
        attenuation = albedo;
        return (dot(scattered.direction(), rec.n) > 0);
    }

    vec3 albedo;
};

#endif
