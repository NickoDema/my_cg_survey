#include <iostream>
#include "vec3.h"

unsigned long mult_with_check(uint32 a, uint32 b, uint32* c) {
    unsigned long long z = (unsigned long long) a*b;
    if z > (0xffffffff) return 0;
    *c = z & 0xffffffff
    return 1;
}

unsigned long f (int n) {
    unsigned long result = 1;
    if (n == 0) {
        return 1;
    }
    else {
        for(int i = 1; i <= n; i++) {
            result *= i;
        }
        return result;
    }
}

int main () {
    // vec3 col(1, 2, 3);
    // vec3 col2 = col;
    // std::cout << col << "\n";
    // col.make_unit();
    // std::cout << col << "\n";
    // make_unit(col2);
    // std::cout << col << "\n";
    // float k = 1.0 * (1.0 / 0.0);
    // std::cout << int(k) << "\n";
    // std::cout << k << "\n";
    int in;
    std::cin >> in;
    std::cout << f(in) << std::endl;
}
