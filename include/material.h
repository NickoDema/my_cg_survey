#ifndef MATERIALH
#define MATERIALH

#include <math.h>
#include "ray.h"
#include "hitable.h"
#include "my_random.h"

struct hit_record;

vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(v,n)*n;
}


bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) {
    vec3 uv = make_unit(v);
    float dt = dot(uv, n);
    float discriminant = 1.0 - ni_over_nt*ni_over_nt*(1-dt*dt);
    if (discriminant > 0) {
        refracted = ni_over_nt*(uv - n*dt) - n*sqrt(discriminant);
        return true;
    }
    else
        return false;
}


float schlick(float cosine, float ref_idx) {
    float r0 = (1 - ref_idx)/(1 + ref_idx);
    r0 = r0*r0;
    return r0 + (1 - r0)*pow((1 - cosine), 5);
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
    Metal(const vec3& a, float f) : albedo(a) { if ((f < 1) && (0 <= f)) fuzz = f; else fuzz = 0; }
    virtual bool scatter(const ray& ray_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
        vec3 reflected = reflect(make_unit(ray_in.direction()), rec.n);
        scattered = ray(rec.p, reflected + fuzz*rand_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.n) > 0);
    }

    vec3 albedo;
    float fuzz;
};


class Dielectric : public Material {
public:
    Dielectric(float ri) : ref_idx(ri) {};
    virtual bool scatter(const ray& ray_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
        vec3 outward_normal;
        vec3 reflected = reflect(ray_in.direction(), rec.n);
        float ni_over_nt;
        attenuation = vec3(1.0, 1.0, 1.0);
        vec3 refracted;
        float reflect_prob;
        float cosine;

        if (dot(ray_in.direction(), rec.n) > 0) {
            outward_normal = -rec.n;
            ni_over_nt = ref_idx;
            cosine = ref_idx * dot(ray_in.direction(), rec.n)/ray_in.direction().length();
        }
        else {
            outward_normal = rec.n;
            ni_over_nt = 1.0/ref_idx;
            cosine = -dot(ray_in.direction(), rec.n)/ray_in.direction().length();
        }

        if (refract(ray_in.direction(), outward_normal, ni_over_nt, refracted)) {
            reflect_prob = schlick(cosine, ref_idx);
        }
        else {
            scattered = ray(rec.p, reflected);
            reflect_prob = 1.0;
        }

        if (random_0to1() < reflect_prob) {
            scattered = ray(rec.p, reflected);
        }
        else {
            scattered = ray(rec.p, refracted);
        }

        return true;
    }

    float ref_idx;
};

#endif
