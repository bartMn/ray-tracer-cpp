#include "cylinder.h"

// Constructor: Initializes a cylinder with center, radius, height, and axis normal.
/**
 * @param center The center of the cylinder.
 * @param radius The radius of the cylinder.
 * @param height The height of the cylinder.
 * @param axisNormal The normalized axis direction of the cylinder.
 */
Cylinder::Cylinder(const vec3& center, double radius, double height, const vec3& axisNormal)
    : center(center), radius(radius), height(height), axisNormal(const_cast<vec3&>(axisNormal).return_unit()), textureIsSet(false) {}

// Checks for a grid-based intersection with the cylinder.
/**
 * @param r The ray to test.
 * @param t_min The minimum t value for a valid hit.
 * @param t_max The maximum t value for a valid hit.
 * @param rec The record to store hit information.
 * @return True if the ray intersects the grid, false otherwise.
 */
bool Cylinder::gridHit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    double t0 = std::numeric_limits<double>::lowest();
    double t1 = std::numeric_limits<double>::max();

    // Check if the ray intersects the bounding box of the cylinder
    if (!hitBoundingBox(r, t0, t1))
        return false;

    // Ensure the intersection is within the valid range
    t0 = std::max(t0, t_min);
    t1 = std::min(t1, t_max);

    // Check if the ray misses the grid
    if (t0 >= t1)
        return false;

    return true;
}

// Checks for a ray-bounding box intersection.
/**
 * @param r The ray to test.
 * @param t0 The minimum t value for a valid hit.
 * @param t1 The maximum t value for a valid hit.
 * @return True if the ray intersects the bounding box, false otherwise.
 */
bool Cylinder::hitBoundingBox(const Ray& r, double& t0, double& t1) const {
    vec3 min = (center + 0.5 * height * axisNormal) - vec3(radius, height / 2, radius);
    vec3 max = (center + 0.5 * height * axisNormal) + vec3(radius, height / 2, radius);

    double min_arr[] = {min.x, min.y, min.z};
    double max_arr[] = {max.x, max.y, max.z};
    vec3 ray_direction = r.getDirection();
    double ray_direction_arr[] = {ray_direction.x, ray_direction.y, ray_direction.z};
    vec3 ray_origin = r.getOrigin();
    double ray_origin_arr[] = {ray_origin.x, ray_origin.y, ray_origin.z};

    for (int i = 0; i < 3; ++i) {
        double invD = 1.0 / (0.0001 + ray_direction_arr[i]);
        double tNear = (min_arr[i] - ray_origin_arr[i]) * invD;
        double tFar = (max_arr[i] - ray_origin_arr[i]) * invD;

        if (invD < 0.0)
            std::swap(tNear, tFar);

        t0 = tNear > t0 ? tNear : t0;
        t1 = tFar < t1 ? tFar : t1;

        if (t0 >= t1)
            return false;
    }

    return true;
}

// Checks for a ray-cylinder intersection.
/**
 * @param r The ray to test.
 * @param t_min The minimum t value for a valid hit.
 * @param t_max The maximum t value for a valid hit.
 * @param rec The record to store hit information.
 * @return True if the ray intersects the cylinder, false otherwise.
 */
bool Cylinder::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    if (!gridHit(r, t_min, t_max, rec)) return false;

    vec3 oc = r.getOrigin() - center;
    double a = vec3::dot(r.getDirection() - axisNormal * vec3::dot(r.getDirection(), axisNormal), r.getDirection() - axisNormal * vec3::dot(r.getDirection(), axisNormal));
    double b = 2 * vec3::dot(oc - axisNormal * vec3::dot(oc, axisNormal), r.getDirection() - axisNormal * vec3::dot(r.getDirection(), axisNormal));
    double c = vec3::dot(oc - axisNormal * vec3::dot(oc, axisNormal), oc - axisNormal * vec3::dot(oc, axisNormal)) - radius * radius;

    double discriminant = b * b - 4 * a * c;

    if (discriminant > 0) {
        double root1 = (-b - sqrt(discriminant)) / (2 * a);
        double root2 = (-b + sqrt(discriminant)) / (2 * a);

        if (root1 < t_max && root1 > t_min) {
            double hit_height = vec3::dot(r.pointAtParameter(root1) - center, axisNormal);

            if (hit_height >= 0 && hit_height <= height) {
                rec.material = this->material;
                rec.t = root1;
                rec.p = r.pointAtParameter(rec.t);
                rec.normal = (rec.p - center - hit_height * axisNormal).return_unit();

                if (textureIsSet) {
                    double phi = atan2(rec.normal.z, rec.normal.x);
                    if (phi < 0) phi += 2 * 3.14;
                    double u = phi / (2 * 3.14);
                    double v = hit_height / height;
                    rec.material.setDiffuseColor(material.getTexture(u, v));
                }
                rec.normal.return_unit();
                return true;
            }
        }

        if (root2 < t_max && root2 > t_min) {
            double hit_height = vec3::dot(r.pointAtParameter(root2) - center, axisNormal);

            if (hit_height >= 0 && hit_height <= height) {
                rec.material = this->material;
                rec.t = root2;
                rec.p = r.pointAtParameter(rec.t);
                rec.normal = (rec.p - center - hit_height * axisNormal).return_unit();

                if (textureIsSet) {
                    double phi = atan2(rec.normal.z, rec.normal.x);
                    if (phi < 0) phi += 2 * 3.14;
                    double u = phi / (2 * 3.14);
                    double v = hit_height / height;
                    rec.material.setDiffuseColor(material.getTexture(u, v));
                }
                rec.normal.return_unit();
                return true;
            }
        }
    }

    return false;
}

// Sets the material of the cylinder.
/**
 * @param material The material to set.
 */
void Cylinder::setMaterial(Material material) {
    this->material = material;
}

// Sets the texture of the cylinder.
/**
 * @param texturePath The file path to the texture.
 */
void Cylinder::setTexture(const std::string& texturePath) {
    this->material.setTexture(texturePath);
    this->textureIsSet = true;
}