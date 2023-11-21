// In your header file, say Circle.h
#ifndef CIRCLE_H
#define CIRCLE_H

#include "Hittable.h"
#include "Material.h"

class Circle : public Hittable {
public:
    Circle(): textureIsSet(false){}
    Circle(const vec3& center, double radius, const vec3& normal, double cylinderHeight);
    
    void setMaterial(Material material);
    void setTexture(const std::string& texturePath);
    bool gridHit(const Ray& r, double t_min, double t_max, HitRecord& rec) const;
    bool hitBoundingBox(const Ray& r, double& t0, double& t1) const;

    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

private:
    vec3 center;
    double radius;
    vec3 normal;
    Material material;
    bool textureIsSet;
    double cylinderHeight;
};

#endif // CIRCLE_H
