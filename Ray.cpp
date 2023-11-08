#include "vector.h" // Assuming you have a vec3 class for 3D vectors
#include "Ray.h"


Ray::Ray() {}
Ray::Ray(const vec3& origin, const vec3& direction, const vec3& color): origin(origin), direction(direction), color(color) {}
vec3 Ray::getOrigin() const { return origin; }
vec3 Ray::getDirection() const { return direction; }
vec3 Ray::getColor() const { return color; }
vec3 Ray::pointAtParameter(float t) const {return origin + t * direction;}
