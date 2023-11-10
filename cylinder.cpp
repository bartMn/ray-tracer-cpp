// In your cpp file, say Cylinder.cpp
#include "Cylinder.h"

Cylinder::Cylinder(const vec3& center, double radius, double height, const vec3& axisNormal)
    : center(center), radius(radius), height(height), axisNormal(const_cast<vec3&>(axisNormal).return_unit()) {}

bool Cylinder::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    // Calculate the intersection of the ray with the finite cylinder
    vec3 oc = r.getOrigin() - center;
    double a = vec3::dot(r.getDirection() - axisNormal * vec3::dot(r.getDirection(), axisNormal), r.getDirection() - axisNormal * vec3::dot(r.getDirection(), axisNormal));
    double b = 2 * vec3::dot(oc - axisNormal * vec3::dot(oc, axisNormal), r.getDirection() - axisNormal * vec3::dot(r.getDirection(), axisNormal));
    double c = vec3::dot(oc - axisNormal * vec3::dot(oc, axisNormal), oc - axisNormal * vec3::dot(oc, axisNormal)) - radius * radius;

    double discriminant = b * b - 4 * a * c;

    if (discriminant > 0) {
        // Two intersection points
        double root1 = (-b - sqrt(discriminant)) / (2 * a);
        double root2 = (-b + sqrt(discriminant)) / (2 * a);

        // Check if the intersection points are within the specified range
        if (root1 < t_max && root1 > t_min) {
            double hit_height = vec3::dot(r.pointAtParameter(root1) - center, axisNormal);

            if (hit_height >= 0 && hit_height <= height) {
                rec.t = root1;
                rec.p = r.pointAtParameter(rec.t);
                rec.normal = (rec.p - center - hit_height * axisNormal).return_unit();  // Dynamic normal calculation
                return true;
            }
        }

        if (root2 < t_max && root2 > t_min) {
            double hit_height = vec3::dot(r.pointAtParameter(root2) - center, axisNormal);

            if (hit_height >= 0 && hit_height <= height) {
                rec.t = root2;
                rec.p = r.pointAtParameter(rec.t);
                rec.normal = (rec.p - center - hit_height * axisNormal).return_unit();  // Dynamic normal calculation
                return true;
            }
        }
    }

    return false;
}
