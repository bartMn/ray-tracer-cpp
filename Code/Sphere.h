#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "Material.h"

/**
 * @class Sphere
 * @brief Represents a sphere that can be hit by rays.
 */
class Sphere : public Hittable {
public:
    Sphere() : textureIsSet(false) {} // Default constructor.
    Sphere(const vec3& center, double radius) : center(center), radius(radius), textureIsSet(false) {} // Constructor.

    vec3 getLightColour(); // Gets the light color.
    void setLightColour(vec3 lightCol); // Sets the light color.
    vec3 getPosition(); // Gets the sphere's position.
    void setMaterial(Material material); // Sets the sphere's material.
    void setTexture(const std::string& texturePath); // Sets the sphere's texture.
    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override; // Checks for ray-sphere intersection.
    bool gridHit(const Ray& r, double t_min, double t_max, HitRecord& rec) const; // Grid-based intersection check.
    bool hitBoundingBox(const Ray& r, double t0, double t1) const; // Checks for ray-bounding box intersection.

private:
    vec3 center;          // Sphere center.
    double radius;        // Sphere radius.
    Material material;    // Sphere material.
    vec3 ligthColour;     // Light color.
    bool textureIsSet;    // Texture flag.
};

#endif // SPHERE_H