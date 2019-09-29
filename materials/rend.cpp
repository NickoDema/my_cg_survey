#include <iostream>
#include <random>
#include <math.h>
#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"

std::random_device rdev{};
static std::default_random_engine rand_engine{rdev()};
std::uniform_real_distribution<> rand_dist{0, 1};

float my_rand() {
    return rand_dist(rand_engine);
}

vec3 rand_in_unit_sphere() {
    vec3 p;
    do {
        p = 2*vec3(my_rand(), my_rand(), my_rand()) - vec3(1.0, 1.0, 1.0);
    } while (p.squared_length() >= 1.0);
    return p;

}

vec3 color(const ray& r, Hitable *world) {
    hit_record rec;
    if (world->hit(r, 0.0001, 2.0, rec)) {
        vec3 target = rec.p + rec.n + rand_in_unit_sphere();
        return 0.5*color( ray(rec.p, target-rec.p), world);
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
   int ns = 20;
   std::cout << "P3\n" << nx << " " << ny << "\n255\n";

   Hitable* list[2];
   list[0] = new Sphere(vec3(0, 0, -1), 0.2);
   list[1] = new Sphere(vec3(0, -2.2, -1), 2);
   Hitable *world = new Hitable_list(list, 2);

   Camera cam;
   for (int j = ny-1; j >= 0; j--) {
      for (int i = 0; i < nx; i++) {
         vec3 col(0, 0, 0);
         for (int s = 0; s < ns; s++) {
            float u = float(i + rand_dist(rand_engine)) / float(nx);
            float v = float(j + rand_dist(rand_engine)) / float(ny);
            ray r = cam.get_ray(u, v);
            // vec3 p = r.point_at_param(1.0);
            // std::cout << "p: " << p << "\n";
            col += color(r, world);
         }
         col /= float(ns);
         // col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
         int r_int = int(240*col.r());
         int g_int = int(240*col.g());
         int b_int = int(240*col.b());

            std::cout << r_int << " " << g_int << " " << b_int << "\n";
      }
   }
}
