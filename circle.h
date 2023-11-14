// In your header file, say Circle.h
#ifndef CIRCLE_H
#define CIRCLE_H

#include "Hittable.h"
#include "Material.h"

class Circle : public Hittable {
public:
    Circle() {}
    Circle(const vec3& center, double radius, const vec3& normal);
    
    void setMaterial(Material material);
    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

private:
    vec3 center;
    double radius;
    vec3 normal;
    Material material;
};

#endif // CIRCLE_H
