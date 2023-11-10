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

    // Check for intersection with the top and bottom surfaces
    double t_top = (center.z + height - r.getOrigin().z) / r.getDirection().z;
    double t_bottom = (center.z - r.getOrigin().z) / r.getDirection().z;

    if (t_top < t_max && t_top > t_min) {
        vec3 intersection_top = r.pointAtParameter(t_top);

        // Check if the intersection point is within the circular top surface
        if (intersection_top.x * intersection_top.x + intersection_top.y * intersection_top.y <= radius * radius) {
            rec.t = t_top;
            rec.p = intersection_top;
            rec.normal = vec3(0, 0, 1);  // Dynamic normal calculation
            return true;
        }
    }

    if (t_bottom < t_max && t_bottom > t_min) {
        vec3 intersection_bottom = r.pointAtParameter(t_bottom);

        // Check if the intersection point is within the circular bottom surface
        if (intersection_bottom.x * intersection_bottom.x + intersection_bottom.y * intersection_bottom.y <= radius * radius) {
            rec.t = t_bottom;
            rec.p = intersection_bottom;
            rec.normal = vec3(0, 0, -1);  // Dynamic normal calculation
            return true;
        }
    }

    return false;
}
