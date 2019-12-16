#ifndef MYRANDH
#define MYRANDH

#include <random>

std::random_device rdev{};
static std::default_random_engine rand_engine{rdev()};
std::uniform_real_distribution<> rand_dist{0, 1};

float random_0to1() {
    return rand_dist(rand_engine);
}

vec3 rand_in_unit_sphere() {
    vec3 p;
    do {
        p = 2*vec3(random_0to1(), random_0to1(), random_0to1()) - vec3(1.0, 1.0, 1.0);
    } while (p.squared_length() >= 1.0);
    return p;
}

vec3 rand_in_unit_disk() {
    vec3 p;
    do {
        p = 2*vec3(random_0to1(), random_0to1(), 0) - vec3(1.0, 1.0, 0.0);
    } while (dot(p, p) >= 1.0);
    return p;
}

#endif
