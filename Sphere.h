// In your header file, say Sphere.h
#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "Material.h"

class Sphere : public Hittable {
public:
    Sphere(): textureIsSet(false) {}
    Sphere(const vec3& center, double radius) : center(center), radius(radius), textureIsSet(false) {}

    vec3 getLightColour();
    void setLightColour(vec3 lightCol);
    vec3 getPosition();
    void setMaterial(Material material);
    void setTexture(const std::string& texturePath);
    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;
    bool gridHit(const Ray& r, double t_min, double t_max, HitRecord& rec) const;
    bool hitBoundingBox(const Ray& r, double t0, double t1) const;

private:
    vec3 center;
    double radius;
    Material material;
    vec3 ligthColour;
    bool textureIsSet;
};

#endif // SPHERE_H
