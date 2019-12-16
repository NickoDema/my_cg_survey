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
        if (depth < 10 && rec.m->scatter(r, rec, attenuation, scattered)) {
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

Hitable* random_scene() {
    int n = 500;
    Hitable **list = new Hitable*[n+1];
    list[0] =  new Sphere(vec3(0,-1000,0), 1000, new Lambertian(vec3(0.5, 0.5, 0.5)));
    int i = 1;
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = random_0to1();
            vec3 center(a+0.9*random_0to1(),0.2,b+0.9*random_0to1());
            if ((center-vec3(4,0.2,0)).length() > 0.9) {
                if (choose_mat < 0.5) {  // diffuse
                    list[i++] = new Sphere(
                        center, 0.2,
                        new Lambertian(vec3(random_0to1()*random_0to1(),
                                            random_0to1()*random_0to1(),
                                            random_0to1()*random_0to1()))
                    );
                }
                else if (choose_mat < 0.85) { // metal
                    list[i++] = new Sphere(
                        center, 0.2,
                        new Metal(vec3(0.5*(1 + random_0to1()),
                                       0.5*(1 + random_0to1()),
                                       0.5*(1 + random_0to1())),
                                  0.5*random_0to1())
                    );
                }
                else {  // glass
                    list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
                }
            }
        }
    }

    list[i++] = new Sphere(vec3(0, 1, 0), 1.0, new Dielectric(1.5));
    list[i++] = new Sphere(vec3(-4, 1, 0), 1.0, new Lambertian(vec3(0.4, 0.2, 0.1)));
    list[i++] = new Sphere(vec3(4, 1, 0), 1.0, new Metal(vec3(0.7, 0.6, 0.5), 0.0));

    return new Hitable_list(list,i);
}


Hitable* my_scene() {
   // Hitable* list[7];
   Hitable **list = new Hitable*[7];
   list[0] = new Sphere(vec3(0,    -0.6,   -1),   0.5,  new Lambertian(vec3(0.2, 0.8, 0.3)));
   list[1] = new Sphere(vec3(-0.2,  0.15,  -0.3), 0.06, new Dielectric(1.3));
   list[2] = new Sphere(vec3(0.8,   0,     -1.8), 0.5,  new Metal(vec3(0.8, 0.6, 0.2), 0.0));
   list[3] = new Sphere(vec3(-1.1,  0,     -1.2), 0.5,  new Metal(vec3(0.8, 0.8, 0.8), 0.0));
   list[4] = new Sphere(vec3(-0.3, -0.26,  -0),   0.1,  new Dielectric(1.1));
   list[5] = new Sphere(vec3(0.3,   0.1,   -1.3), 0.2,  new Dielectric(1.4));
   list[6] = new Sphere(vec3(0,    -201.5,  0),   200,  new Lambertian(vec3(0.2, 0.2, 0.2)));
   return new Hitable_list(list, 7);
}

int main () {

   Hitable *world = random_scene();

   int nx = 1200;
   int ny = 600;
   int ns = 100;

   std::cout << "P3\n" << nx << " " << ny << "\n255\n";

   // vec3 lookfrom(0,0,1);
   // vec3 lookat(0, 0.0, -1);
   // float dist_to_focus = (lookfrom - lookat).length();
   // float aperture = 0.01;

   // Camera cam(lookfrom, lookat, vec3(0,1,0), 90, float(nx)/float(ny), aperture, dist_to_focus);

   vec3 lookfrom(13,2,3);
   vec3 lookat(0,0,0);
   float dist_to_focus = 10.0;
   float aperture = 0.001;

   Camera cam(lookfrom, lookat, vec3(0,1,0), 20, float(nx)/float(ny), aperture, dist_to_focus);

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
