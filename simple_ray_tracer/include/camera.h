#ifndef CAMERAH
#define CAMERAH

#include <math.h>
#include "ray.h"
#include "my_random.h"

#define PI 3.14159265359

class Camera {
    public:
        Camera(vec3 lookfrom, vec3 lookat, vec3 vup, float v_aov, float aspect, float aperture, float focus_dist) {

            float theta = 1.570796326795;//90 deg

            if ((v_aov > 0) || (v_aov <= 160)) {
                theta = v_aov * PI / 180;
            }

            lens_radius = aperture / 2;

            float half_height = tan(theta/2);
            float half_width = aspect * half_height;

            origin = lookfrom;
            w = make_unit(lookfrom - lookat);
            u = make_unit(cross(vup, w));
            v = make_unit(cross(w, u));

            // lower_left_corner = vec3(-half_width, -half_height, -1.0);
            lower_left_corner = origin - half_width*u - half_height*v - w;
            horizontal = 2*half_width*u;
            vertical = 2*half_height*v;
        }

        ray get_ray( float s, float t) {
            vec3 rd = lens_radius*rand_in_unit_disk();
            vec3 offset = u * rd.x() + v * rd.y();
            return ray(origin, lower_left_corner + s*horizontal + t*vertical - origin - offset);
        }

        vec3 u, v, w;
        vec3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
        vec3 origin;

        float lens_radius;

};

#endif
