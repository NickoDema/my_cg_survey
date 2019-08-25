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
    // std::cout << "in shpere hit" << "\n";
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - a*c;
    // std::cout << "dis: " << discriminant << "\n";
    if (discriminant > 0.0) {
        float temp = (-b - sqrt(discriminant))/a;
        // std::cout << "temp: " << temp << "\n";
        if (temp < t_max && t_min < temp) {
            rec.t = temp;
            rec.p = r.point_at_param(temp);
            rec.n = (rec.p - center)/radius;
            // std::cout << "-b-" << "\n";
            return true;
        }
        // else {
        //     std::cout << "no b- | temp :"<< temp << "\n";
        // }
        temp = (-b + sqrt(discriminant))/a;
        if (temp < t_max && t_min < temp) {
            rec.t = temp;
            rec.p = r.point_at_param(temp);
            rec.n = (rec.p - center)/radius;
            // std::cout << "-b+" << "\n";
            return true;
        }
        // else {
        //     std::cout << temp << "\n";
        // }
    }
    // std::cout << "f" << "\n";
    return false;
}

#endif
