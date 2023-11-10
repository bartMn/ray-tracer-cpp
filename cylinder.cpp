#include "Cylinder.h"
bool Cylinder::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    // Calculate the intersection of the ray with the finite cylinder
    vec3 oc = r.getOrigin() - center;
    double a = r.getDirection().x * r.getDirection().x + r.getDirection().y * r.getDirection().y;
    double b = 2 * (oc.x * r.getDirection().x + oc.y * r.getDirection().y);
    double c = oc.x * oc.x + oc.y * oc.y - radius * radius;

    double discriminant = b * b - 4 * a * c;

    if (discriminant > 0) {
        // Two intersection points
        double root1 = (-b - sqrt(discriminant)) / (2 * a);
        double root2 = (-b + sqrt(discriminant)) / (2 * a);

        // Check if the intersection points are within the specified range
        if (root1 < t_max && root1 > t_min) {
            double hit_height = r.pointAtParameter(root1).z - center.z;

            if (hit_height >= 0 && hit_height <= height) {
                rec.t = root1;
                rec.p = r.pointAtParameter(rec.t);
                rec.normal = (rec.p - center).return_unit();  // Dynamic normal calculation
                return true;
            }
        }

        if (root2 < t_max && root2 > t_min) {
            double hit_height = r.pointAtParameter(root2).z - center.z;

            if (hit_height >= 0 && hit_height <= height) {
                rec.t = root2;
                rec.p = r.pointAtParameter(rec.t);
                rec.normal = (rec.p - center).return_unit();  // Dynamic normal calculation
                return true;
            }
        }
    }

 
    return false;
}
