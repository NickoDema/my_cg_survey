#include <iostream>

int main () {
   int nx = 2000;
   int ny = 1000;
   std::cout << "P3\n" << nx << " " << ny << "\n255\n";
   for (int j = ny-1; j >= 0; j--) {
      for (int i = 0; i < nx; i++) {
         float r = float(i) / float(nx);
         float g = 0.1;
         float b = float(j) / float(ny);
         int r_int = int(160*r);
         int g_int = int(160*g);
         int b_int = int(160*b);
         std::cout << r_int << " " << g_int << " " << b_int << "\n";
      }
   }
}
