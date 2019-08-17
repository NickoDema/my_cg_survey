#include <iostream>
#include "ray.h"

float hit_sphere(const vec3& sphere_center, float radius, const ray& r) {
    vec3 oc = r.origin() - sphere_center;
    float a = dot(r.direction(), r.direction());
    float b = 2.0 * dot(oc, r.direction());
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - 4*a*c;
    if (discriminant < 0) {
        return (-1.0);
    }
    else {
        return (-b - sqrt(discriminant)) / (2.0*a);
    }
}

vec3 color(const ray& r) {
    vec3 new_sphere = vec3(0, 0, -1);
    float t = (float)hit_sphere(new_sphere, 0.2, r);
    if (t > 0) {
        vec3 N = make_unit(r.point_at_param(t) - new_sphere);
        return 0.5*vec3(N.x()+1, N.y()+1, N.z()+1);
    }
    else {
        vec3 unit_direction = make_unit(r.direction());
        t = 0.5*(unit_direction.y() + 1.0);
        return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.6, 0.8, 1.0);
    }
}

int main () {
   int nx = 1200;
   int ny = 600;
   std::cout << "P3\n" << nx << " " << ny << "\n255\n";

   vec3 lower_left_corner(-2.0, -1.0, -2.0);
   vec3 horizontal(4.0, 0.0, 0.0);
   vec3 vertical(0.0, 2.0, 0.0);
   vec3 origin(0.0, 0.0, 0.0);

   for (int j = ny-1; j >= 0; j--) {
      for (int i = 0; i < nx; i++) {
         float u = float(i) / float(nx);
         float v = float(j) / float(ny);
         ray r(origin, lower_left_corner + u*horizontal + v*vertical);
         vec3 col = color(r);
         int r_int = int(240*col.r());
         int g_int = int(240*col.g());
         int b_int = int(240*col.b());

         std::cout << r_int << " " << g_int << " " << b_int << "\n";
      }
   }
}
