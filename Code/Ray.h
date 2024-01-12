#ifndef RAY_H
#define RAY_H

#include "vector.h" // Assuming you have a vec3 class for 3D vectors

class Ray {

private:
    vec3 origin;
    vec3 direction;
    vec3 color;
    int depth;
   
public:
       
    Ray();
    Ray(const vec3& origin, const vec3& direction, const vec3& color, int depth);
    vec3 getOrigin() const;
    vec3 getDirection() const;
    void setDirection(vec3 new_direction);
    vec3 getColor() const;
    void setColor(vec3 newcolor);
    vec3 pointAtParameter(float t) const;
    vec3 get_normalized() const;
    int getDepth() const;
    void clampColour();
   

};

#endif