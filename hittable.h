// In your header file, say Hittable.h
#ifndef HITTABLE_H
#define HITTABLE_H

#include "Ray.h"
#include "vector.h"
// In Hittable.h, update the class definition

struct HitRecord {
    double t;
    vec3 p;
    vec3 normal;
};

class Hittable {
public:
    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const = 0;
};


#endif // HITTABLE_H
