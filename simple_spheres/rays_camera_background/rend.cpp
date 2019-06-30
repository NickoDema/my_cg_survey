#include <iostream>
#include "ray.h"

vec3 background_color(const ray& r) {
    vec3 unit_direction = make_unit(r.direction());
    float t = 0.5*(unit_direction.y() + 1.0);
    return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.6, 0.8, 1.0);
}

int main () {
   int nx = 2000;
   int ny = 1000;
   std::cout << "P3\n" << nx << " " << ny << "\n255\n";

   vec3 lower_left_corner(-2.0, -1.0, -1.0);
   vec3 horizontal(4.0, 0.0, 0.0);
   vec3 vertical(0.0, 2.0, 0.0);
   vec3 origin(0.0, 0.0, 0.0);

   for (int j = ny-1; j >= 0; j--) {
      for (int i = 0; i < nx; i++) {
         float u = float(i) / float(nx);
         float v = float(j) / float(ny);
         ray r(origin, lower_left_corner + u*horizontal + v*vertical);
         vec3 col = background_color(r);
         int r_int = int(240*col.r());
         int g_int = int(240*col.g());
         int b_int = int(240*col.b());

         std::cout << r_int << " " << g_int << " " << b_int << "\n";
      }
   }
}
