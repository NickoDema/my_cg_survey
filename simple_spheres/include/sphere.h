#ifndef SPHEREH
#define SPHEREH

#include "hitable.h"

class sphere: public hitable {
    public:
        sphere() {};
        sphere(vec3 cen, float r): center(cen), radius(r) {};
        virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
        vec3 center;
        float radius;
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius*radius;
    float d = b*b - a*c;
    if (d > 0) {
        float temp = (-b - sqrt(d))/a;
        if (temp < t_max && t_min < temp) {
            rec.t = temp;
            rec.p = r.point_at_param(temp);
            rec.n = (rec.p - center)/radius;
            return true;
        }
        float temp = (-b + sqrt(d))/a;
        if (temp < t_max && t_min < temp) {
            rec.t = temp;
            rec.p = r.point_at_param(temp);
            rec.n = (rec.p - center)/radius;
            return true;
        }
    }
    return false;
}

#endif
