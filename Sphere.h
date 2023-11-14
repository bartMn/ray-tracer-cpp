// In your header file, say Sphere.h
#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.h"
#include "Material.h"

class Sphere : public Hittable {
public:
    Sphere() {}
    Sphere(const vec3& center, double radius) : center(center), radius(radius) {}

    void setMaterial(Material material);
    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

private:
    vec3 center;
    double radius;
    Material material;
};

#endif // SPHERE_H
