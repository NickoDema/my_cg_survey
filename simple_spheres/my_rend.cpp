#include <iostream>
#include "vec3.h"

int main () {
   int nx = 2000;
   int ny = 1000;
   std::cout << "P3\n" << nx << " " << ny << "\n255\n";
   for (int j = ny-1; j >= 0; j--) {
      for (int i = 0; i < nx; i++) {
         vec3 col(float(i) / float(nx), 0.1, float(j) / float(ny));
         int r_int = int(160*col.r());
         int g_int = int(160*col.b());
         int b_int = int(160*col.b());
         std::cout << r_int << " " << g_int << " " << b_int << "\n";
      }
   }
}
