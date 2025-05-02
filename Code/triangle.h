#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "Material.h"

/**
 * @class Triangle
 * @brief Represents a triangle that can be hit by rays.
 */
class Triangle : public Hittable {
public:
    Triangle() {} // Default constructor.
    Triangle(const vec3& v0, const vec3& v1, const vec3& v2) : v0(v0), v1(v1), v2(v2), textureIsSet(false) {} // Constructor.

    void setMaterial(Material material); // Sets the triangle's material.
    void setTexture(const std::string& texturePath); // Sets the triangle's texture.
    bool gridHit(const Ray& r, double t_min, double t_max, HitRecord& rec) const; // Grid-based intersection check.
    bool hitBoundingBox(const Ray& r, double t0, double t1) const; // Checks for ray-bounding box intersection.

    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override; // Checks for ray-triangle intersection.

private:
    vec3 v0, v1, v2;       // Triangle vertices.
    Material material;     // Triangle material.
    double u0, v0_coord, u1, v1_coord, u2, v2_coord; // Texture coordinates.
    bool textureIsSet;     // Texture flag.

    void calculateTextureCoordinates(); // Calculates texture coordinates.
};

#endif // TRIANGLE_H