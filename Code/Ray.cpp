#include "vector.h" // Assuming you have a vec3 class for 3D vectors
#include "Ray.h"


Ray::Ray() {
    origin = vec3(0,0,0);
    direction = vec3(0,0,0);
    color = vec3(0,0,0);
    depth = 0;
}
Ray::Ray(const vec3& origin, const vec3& direction, const vec3& color, int depth)
    : origin(origin), direction(direction), color(color), depth(depth) {}

vec3 Ray::getOrigin() const { return origin; }
vec3 Ray::getDirection() const { return direction; }
void Ray::setDirection(vec3 new_direction) { direction = new_direction.return_unit(); }
vec3 Ray::getColor() const { return color; }
void Ray::setColor(vec3 newcolor) {color = newcolor;}
int Ray::getDepth() const { return depth; }
vec3 Ray::pointAtParameter(float t) const {return origin + t * direction;}
void Ray::clampColour(){
    color = color.return_unit();
}

vec3 Ray::get_normalized() const {
    return const_cast<vec3&>(direction).return_unit();
}

