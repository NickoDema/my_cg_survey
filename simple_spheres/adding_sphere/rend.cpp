#include <iostream>
#include "ray.h"

bool hit_sphere(const vec3& sphere_center, float radius, const ray& r) {
    vec3 oc = r.origin() - sphere_center;
    float a = dot(r.direction(), r.direction());
    float b = 2.0 * dot(oc, r.direction());
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - 4*a*c;
    return (discriminant > 0);
}

vec3 color(const ray& r) {
    if (hit_sphere(vec3(-0.6, 0,-1), 0.2, r))
        return vec3(0.3, 0, 0);
    vec3 unit_direction = make_unit(r.direction());
    float t = 0.5*(unit_direction.y() + 1.0);
    return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.6, 0.8, 1.0);
}

int main () {
   int nx = 2000;
   int ny = 1000;
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
