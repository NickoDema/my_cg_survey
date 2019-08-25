#include <iostream>
#include "sphere.h"
#include "hitable_list.h"
#include "float.h"


vec3 color(const ray& r, hitable *world) {
    hit_record rec;
    // vec3 p = r.point_at_param(1.0);
    // std::cout << "p: " << p << "\n";
    if (world->hit(r, 0.0, 2.0, rec)) {
        return 0.5*vec3(rec.n.x()+1, rec.n.y()+1, rec.n.z()+1);
    }
    else {
        vec3 unit_direction = make_unit(r.direction());
        float t = 0.5*(unit_direction.y() + 1.0);
        return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 0.9);
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

   hitable *list[2];
   list[0] = new sphere(vec3(0, 0, -1), 0.2);
   list[1] = new sphere(vec3(0, -2.08, -1), 2);
   hitable *world = new hitable_list(list, 2);

   for (int j = ny-1; j >= 0; j--) {
      for (int i = 0; i < nx; i++) {
         float u = float(i) / float(nx);
         float v = float(j) / float(ny);
         ray r(origin, lower_left_corner + u*horizontal + v*vertical);
         // vec3 p = r.point_at_param(1.0);
         // std::cout << "p: " << p << "\n";
         vec3 col = color(r, world);
         int r_int = int(240*col.r());
         int g_int = int(240*col.g());
         int b_int = int(240*col.b());

         std::cout << r_int << " " << g_int << " " << b_int << "\n";
      }
   }
}
