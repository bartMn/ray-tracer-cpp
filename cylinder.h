// In your header file, say Cylinder.h
#ifndef CYLINDER_H
#define CYLINDER_H

#include "Hittable.h"
#include "Material.h"

class Cylinder : public Hittable {
public:
    Cylinder(): textureIsSet(false) {}
    Cylinder(const vec3& center, double radius, double height, const vec3& axisNormal);

    void setMaterial(Material material);
    void setTexture(const std::string& texturePath);
    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

private:
    vec3 center;
    double radius;
    double height;
    vec3 axisNormal; // New member variable for the cylinder's axis normal
    Material material;
    bool textureIsSet;
};

#endif // CYLINDER_H
