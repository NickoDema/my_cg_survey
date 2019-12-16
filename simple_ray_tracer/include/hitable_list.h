#ifndef HITABLELISTH
#define HITABLELISTH

#include "hitable.h"

class Hitable_list : public Hitable {
    public:
        Hitable_list() {}
        Hitable_list(Hitable **l, int n) {list = l; list_size = n;}
        virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
        Hitable **list;
        int list_size;
};

bool Hitable_list::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    // std::cout << "in list hit" << "\n";
    // std::cout << "ls: " << list_size << "\n";
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;
    for (int i = 0; i < list_size; i++) {
        // std::cout << "in list for " << "\n";
        if (list[i]->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

#endif
