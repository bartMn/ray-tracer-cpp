#ifndef RAY_H
#define RAY_H

#include "vector.h" // Assuming you have a vec3 class for 3D vectors

/**
 * @class Ray
 * @brief Represents a ray in 3D space with an origin, direction, color, and depth.
 */
class Ray {
private:
    vec3 origin;      // Ray origin.
    vec3 direction;   // Ray direction.
    vec3 color;       // Ray color.
    int depth;        // Ray depth.

public:
    Ray(); // Default constructor.
    Ray(const vec3& origin, const vec3& direction, const vec3& color, int depth); // Constructor.

    vec3 getOrigin() const; // Gets the ray's origin.
    vec3 getDirection() const; // Gets the ray's direction.
    void setDirection(vec3 new_direction); // Sets the ray's direction.
    vec3 getColor() const; // Gets the ray's color.
    void setColor(vec3 newcolor); // Sets the ray's color.
    vec3 pointAtParameter(float t) const; // Computes a point along the ray at parameter t.
    vec3 get_normalized() const; // Gets the normalized direction of the ray.
    int getDepth() const; // Gets the ray's depth.
    void clampColour(); // Clamps the ray's color values.
};

#endif // RAY_H