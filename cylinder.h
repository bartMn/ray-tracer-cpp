// In your header file, say Cylinder.h
#ifndef CYLINDER_H
#define CYLINDER_H

#include "Hittable.h"

class Cylinder : public Hittable {
public:
    Cylinder() {}
    Cylinder(const vec3& center, double radius, double height) : center(center), radius(radius), height(height) {}

    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

private:
    vec3 center;
    double radius;
    double height;
};

#endif // CYLINDER_H
