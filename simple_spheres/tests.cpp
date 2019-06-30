#include <iostream>
#include "vec3.h"

int main () {
    vec3 col(1, 2, 3);
    vec3 col2 = col;
    std::cout << col << "\n";
    col.make_unit();
    std::cout << col << "\n";
    make_unit(col2);
    std::cout << col << "\n";
}
