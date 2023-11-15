// In your cpp file, say Circle.cpp
#include "Circle.h"

Circle::Circle(const vec3& center, double radius, const vec3& normal){
    this-> center= center;
    this-> radius = radius;
    this-> normal = const_cast<vec3&>(normal).return_unit();
}

bool Circle::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    // Check if the ray is parallel to the circle's normal
    
    rec.material = this-> material;
    double denom = vec3::dot(r.getDirection(), normal);
    if (std::abs(denom) < 1e-6) {
        return false;
    }

    // Calculate the parameter t for the intersection point
    double t = vec3::dot(center - r.getOrigin(), normal) / denom;

    // Check if the intersection point is within the given range
    if (t < t_min || t > t_max) {
        return false;
    }

    // Calculate the intersection point and record the information
    vec3 intersection_point = r.pointAtParameter(t);
    vec3 to_center = intersection_point - center;
    double distance_squared = to_center.length_squared();

    if (distance_squared <= radius * radius) {
        rec.t = t;
        rec.p = intersection_point;
        rec.normal = normal;
        return true;
    }

    return false;
}

void Circle::setMaterial(Material material){
    this ->material = material;
}