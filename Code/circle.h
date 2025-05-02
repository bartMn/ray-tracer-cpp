#ifndef CIRCLE_H
#define CIRCLE_H

#include "hittable.h"
#include "Material.h"

/**
 * @class Circle
 * @brief Represents a circle that can be hit by rays.
 */
class Circle : public Hittable {
public:
    Circle() : textureIsSet(false) {} // Default constructor.
    Circle(const vec3& center, double radius, const vec3& normal, double cylinderHeight); // Constructor.

    void setMaterial(Material material); // Sets the circle's material.
    void setTexture(const std::string& texturePath); // Sets the circle's texture.
    bool gridHit(const Ray& r, double t_min, double t_max, HitRecord& rec) const; // Grid-based intersection check.
    bool hitBoundingBox(const Ray& r, double& t0, double& t1) const; // Checks for ray-bounding box intersection.

    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override; // Checks for ray-circle intersection.

private:
    vec3 center;          // Circle center.
    double radius;        // Circle radius.
    vec3 normal;          // Circle normal vector.
    Material material;    // Circle material.
    bool textureIsSet;    // Texture flag.
    double cylinderHeight; // Height of the cylinder the circle is part of.
};

#endif // CIRCLE_H