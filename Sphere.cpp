// In your cpp file, say Sphere.cpp
#include "Sphere.h"


bool Sphere::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    vec3 oc = r.getOrigin() - center;
    double a = vec3::dot(r.getDirection(), r.getDirection());
    double b = vec3::dot(oc, r.getDirection());
    double c = vec3::dot(oc, oc) - radius * radius;
    double discriminant = b * b - a * c;

    if (discriminant > 0) {
        double temp = (-b - sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.pointAtParameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            return true;
        }
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.pointAtParameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            return true;
        }
    }
    return false;
}
