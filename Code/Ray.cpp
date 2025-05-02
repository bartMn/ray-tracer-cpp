#include "vector.h" // Assuming you have a vec3 class for 3D vectors
#include "Ray.h"

// Default constructor: Initializes the ray with default values.
/**
 * Initializes the ray's origin, direction, color, and depth to default values.
 */
Ray::Ray() {
    origin = vec3(0, 0, 0);
    direction = vec3(0, 0, 0);
    color = vec3(0, 0, 0);
    depth = 0;
}

// Constructor: Initializes the ray with specified values.
/**
 * @param origin The origin of the ray.
 * @param direction The direction of the ray.
 * @param color The color of the ray.
 * @param depth The depth of the ray.
 */
Ray::Ray(const vec3& origin, const vec3& direction, const vec3& color, int depth)
    : origin(origin), direction(direction), color(color), depth(depth) {}

// Gets the ray's origin.
/**
 * @return The origin of the ray.
 */
vec3 Ray::getOrigin() const { return origin; }

// Gets the ray's direction.
/**
 * @return The direction of the ray.
 */
vec3 Ray::getDirection() const { return direction; }

// Sets the ray's direction.
/**
 * @param new_direction The new direction to set for the ray.
 */
void Ray::setDirection(vec3 new_direction) { direction = new_direction.return_unit(); }

// Gets the ray's color.
/**
 * @return The color of the ray.
 */
vec3 Ray::getColor() const { return color; }

// Sets the ray's color.
/**
 * @param newcolor The new color to set for the ray.
 */
void Ray::setColor(vec3 newcolor) { color = newcolor; }

// Gets the ray's depth.
/**
 * @return The depth of the ray.
 */
int Ray::getDepth() const { return depth; }

// Computes a point along the ray at parameter t.
/**
 * @param t The parameter value along the ray.
 * @return The point at parameter t.
 */
vec3 Ray::pointAtParameter(float t) const { return origin + t * direction; }

// Clamps the ray's color values to a unit vector.
/**
 * Ensures the ray's color values are normalized.
 */
void Ray::clampColour() {
    color = color.return_unit();
}

// Gets the normalized direction of the ray.
/**
 * @return The normalized direction of the ray.
 */
vec3 Ray::get_normalized() const {
    return const_cast<vec3&>(direction).return_unit();
}