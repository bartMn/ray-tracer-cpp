#include "circle.h"

// Constructor: Initializes a circle with center, radius, normal, and cylinder height.
/**
 * @param center The center of the circle.
 * @param radius The radius of the circle.
 * @param normal The normal vector of the circle.
 * @param cylinderHeight The height of the cylinder the circle is part of.
 */
Circle::Circle(const vec3& center, double radius, const vec3& normal, double cylinderHeight) {
    this->center = center;
    this->radius = radius;
    this->normal = const_cast<vec3&>(normal).return_unit();
    this->textureIsSet = false;
    this->cylinderHeight = cylinderHeight;
}

// Checks for a grid-based intersection with the circle.
/**
 * @param r The ray to test.
 * @param t_min The minimum t value for a valid hit.
 * @param t_max The maximum t value for a valid hit.
 * @param rec The record to store hit information.
 * @return True if the ray intersects the grid, false otherwise.
 */
bool Circle::gridHit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
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
bool Circle::hitBoundingBox(const Ray& r, double& t0, double& t1) const {
    vec3 min = (center - 0.5 * cylinderHeight * normal) - vec3(radius, cylinderHeight / 2, radius);
    vec3 max = (center - 0.5 * cylinderHeight * normal) + vec3(radius, cylinderHeight / 2, radius);

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

// Checks for a ray-circle intersection.
/**
 * @param r The ray to test.
 * @param t_min The minimum t value for a valid hit.
 * @param t_max The maximum t value for a valid hit.
 * @param rec The record to store hit information.
 * @return True if the ray intersects the circle, false otherwise.
 */
bool Circle::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    if (!gridHit(r, t_min, t_max, rec)) return false;

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
        rec.material = this->material;

        if (textureIsSet) {
            double u = 0.5 + atan2(normal.z, normal.x) / (2 * 3.14);
            double v = 0.5 - asin(normal.y) / 3.14;
            rec.material.setDiffuseColor(material.getTexture(u, v));
        }
        rec.normal.return_unit();
        return true;
    }

    return false;
}

// Sets the material of the circle.
/**
 * @param material The material to set.
 */
void Circle::setMaterial(Material material) {
    this->material = material;
}

// Sets the texture of the circle.
/**
 * @param texturePath The file path to the texture.
 */
void Circle::setTexture(const std::string& texturePath) {
    this->material.setTexture(texturePath);
    this->textureIsSet = true;
}