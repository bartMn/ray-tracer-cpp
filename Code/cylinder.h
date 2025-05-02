#ifndef CYLINDER_H
#define CYLINDER_H

#include "hittable.h"
#include "Material.h"

/**
 * @class Cylinder
 * @brief Represents a cylinder that can be hit by rays.
 */
class Cylinder : public Hittable {
public:
    Cylinder() : textureIsSet(false) {} // Default constructor.
    Cylinder(const vec3& center, double radius, double height, const vec3& axisNormal); // Constructor.

    void setMaterial(Material material); // Sets the cylinder's material.
    void setTexture(const std::string& texturePath); // Sets the cylinder's texture.
    bool gridHit(const Ray& r, double t_min, double t_max, HitRecord& rec) const; // Grid-based intersection check.
    bool hitBoundingBox(const Ray& r, double& t0, double& t1) const; // Checks for ray-bounding box intersection.
    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override; // Checks for ray-cylinder intersection.

private:
    vec3 center;          // Cylinder center.
    double radius;        // Cylinder radius.
    double height;        // Cylinder height.
    vec3 axisNormal;      // Cylinder axis normal.
    Material material;    // Cylinder material.
    bool textureIsSet;    // Texture flag.
};

#endif // CYLINDER_H