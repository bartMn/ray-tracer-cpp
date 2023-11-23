// In your header file, say triangle.h
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "Material.h"

class Triangle : public Hittable {
public:
    Triangle() {}
    Triangle(const vec3& v0, const vec3& v1, const vec3& v2) : v0(v0), v1(v1), v2(v2), textureIsSet(false) {}

    void setMaterial(Material material);
    void setTexture(const std::string& texturePath);
    bool gridHit(const Ray& r, double t_min, double t_max, HitRecord& rec) const;
    bool hitBoundingBox(const Ray& r, double t0, double t1) const;

    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

private:
    vec3 v0, v1, v2;
    Material material;
    double u0, v0_coord, u1, v1_coord, u2, v2_coord;
    bool textureIsSet;

    void calculateTextureCoordinates();
};

#endif // TRIANGLE_H
