#ifndef HITABLEH
#define HITABLEH

#include "ray.h"

class Material;

struct hit_record {
    float t;
    vec3 p;
    vec3 n;
    Material *m;
};

class Hitable {
    public:
        virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;

};

#endif
