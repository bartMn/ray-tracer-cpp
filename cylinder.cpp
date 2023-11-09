#include "Cylinder.h"

Cylinder::Cylinder(const vec3& center, double radius, double height)
    : center(center), radius(radius), height(height) {}


bool Cylinder::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    vec3 oc = r.getOrigin() - center;
       
    
    double a = r.getDirection().x * r.getDirection().x + r.getDirection().z * r.getDirection().z;
    double b = 2 * (oc.x * r.getDirection().x + oc.z * r.getDirection().z);
    double c = oc.x * oc.x + oc.z * oc.z - radius * radius;

    double discriminant = b * b - 4 * a * c;

    if (discriminant > 0) {
        double root1 = (-b - sqrt(discriminant)) / (2 * a);
        double root2 = (-b + sqrt(discriminant)) / (2 * a);

        // Check if the roots are within the valid range
        if (root1 < t_max && root1 > t_min) {
            double y = r.pointAtParameter(root1).y;
            if (y >= center.y && y <= center.y + height) {
                rec.t = root1;
                rec.p = r.pointAtParameter(rec.t);
                rec.normal = vec3((rec.p.x - center.x) / radius, 0, (rec.p.z - center.z) / radius);
                return true;
            }
        }

        if (root2 < t_max && root2 > t_min) {
            double y = r.pointAtParameter(root2).y;
            if (y >= center.y && y <= center.y + height) {
                rec.t = root2;
                rec.p = r.pointAtParameter(rec.t);
                rec.normal = vec3((rec.p.x - center.x) / radius, 0, (rec.p.z - center.z) / radius);
                return true;
            }
        }
    }

    return false;
}
