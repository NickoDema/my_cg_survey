#include <iostream>
#include <math.h>
#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"
#include "my_random.h"
#include "material.h"


vec3 color(const ray& r, Hitable *world, int depth) {
    hit_record rec;
    if (world->hit(r, 0.001, 1000.0, rec)) {
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && rec.m->scatter(r, rec, attenuation, scattered)) {
            return attenuation*color(scattered, world, depth+1);
        }
        else {
            return vec3(0, 0, 0);
        }
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
   int ns = 200;
   std::cout << "P3\n" << nx << " " << ny << "\n255\n";

   // Hitable* list[3];
   // list[0] = new Sphere(vec3(0, 0, -1.2), 0.2);
   // list[1] = new Sphere(vec3(0, -2.4, -2.8), 2.6);
   // list[2] = new Sphere(vec3(0.2, 0.06, -0.8), 0.1);
   // Hitable *world = new Hitable_list(list, 3);

   Hitable* list[4];
   list[0] = new Sphere(vec3(0, 0, -2), 0.5, new Lambertian(vec3(0.2, 0.8, 0.3)));
   list[1] = new Sphere(vec3(0, -100.5, -2), 100, new Lambertian(vec3(0.2, 0.2, 0.2)));
   list[2] = new Sphere(vec3(1.2, 0, -2.5), 0.5, new Metal(vec3(0.8, 0.6, 0.2)));
   list[3] = new Sphere(vec3(-1.1, 0, -1.8), 0.5, new Metal(vec3(0.8, 0.8, 0.8)));
   Hitable *world = new Hitable_list(list, 4);

   Camera cam;
   for (int j = ny-1; j >= 0; j--) {
      for (int i = 0; i < nx; i++) {
         vec3 col(0, 0, 0);
         for (int s = 0; s < ns; s++) {
            float u = float(i + random_0to1()) / float(nx);
            float v = float(j + random_0to1()) / float(ny);
            ray r = cam.get_ray(u, v);
            col += color(r, world, 0);
         }
         col /= float(ns);
         col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
         int r_int = int(240*col.r());
         int g_int = int(240*col.g());
         int b_int = int(240*col.b());

            std::cout << r_int << " " << g_int << " " << b_int << "\n";
      }
   }
}
