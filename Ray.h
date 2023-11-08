#ifndef RAY_H
#define RAY_H

#include "vector.h" // Assuming you have a vec3 class for 3D vectors

class Ray {
public:
    Ray();
    Ray(const vec3& origin, const vec3& direction, const vec3& color);
    vec3 getOrigin() const;
    vec3 getDirection() const;
    vec3 getColor() const;
    vec3 pointAtParameter(float t) const;

private:
    vec3 origin;
    vec3 direction;
    vec3 color;
};

#endif