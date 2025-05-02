#include "triangle.h"

// Checks for a grid-based intersection with the triangle.
/**
 * @param r The ray to test.
 * @param t_min The minimum t value for a valid hit.
 * @param t_max The maximum t value for a valid hit.
 * @param rec The record to store hit information.
 * @return True if the ray intersects the grid, false otherwise.
 */
bool Triangle::gridHit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    double t0 = std::numeric_limits<double>::lowest();
    double t1 = std::numeric_limits<double>::max();
    if (!hitBoundingBox(r, t0, t1))
        return false;

    t0 = std::max(t0, t_min);
    t1 = std::min(t1, t_max);

    if (t0 >= t1)
        return false;

    return true;
}

// Computes the distance between two points.
/**
 * @param p1 The first point.
 * @param p2 The second point.
 * @return The distance between the two points.
 */
double distance(const vec3& p1, const vec3& p2) {
    return std::sqrt((p1.x - p2.x) * (p1.x - p2.x) +
                     (p1.y - p2.y) * (p1.y - p2.y) +
                     (p1.z - p2.z) * (p1.z - p2.z));
}

// Computes the area of a triangle given its side lengths.
/**
 * @param a The length of the first side.
 * @param b The length of the second side.
 * @param c The length of the third side.
 * @return The area of the triangle.
 */
double area(double a, double b, double c) {
    double s = (a + b + c) / 2;
    return std::sqrt(s * (s - a) * (s - b) * (s - c));
}

// Checks for a ray-bounding box intersection.
/**
 * @param r The ray to test.
 * @param t0 The minimum t value for a valid hit.
 * @param t1 The maximum t value for a valid hit.
 * @return True if the ray intersects the bounding box, false otherwise.
 */
bool Triangle::hitBoundingBox(const Ray& r, double t0, double t1) const {
    double a = distance(v0, v2);
    double b = distance(v1, v2);
    double c = distance(v1, v0);

    double K = area(a, b, c);
    double R = (a * b * c) / (4 * K);

    vec3 center;
    center.x = (v0.x + v1.x + v2.x) / 3;
    center.y = (v0.y + v1.y + v2.y) / 3;
    center.z = (v0.z + v1.z + v2.z) / 3;

    double radius = R;

    vec3 center_min = center - vec3(radius, radius, radius);
    vec3 center_max = center + vec3(radius, radius, radius);

    double center_min_arr[] = {center_min.x, center_min.y, center_min.z};
    double center_max_arr[] = {center_max.x, center_max.y, center_max.z};
    vec3 ray_direction = r.getDirection();
    double ray_direction_arr[] = {ray_direction.x, ray_direction.y, ray_direction.z};
    vec3 ray_origin = r.getOrigin();
    double ray_origin_arr[] = {ray_origin.x, ray_origin.y, ray_origin.z};

    for (int i = 0; i < 3; ++i) {
        double invD = 1.0 / (0.0001 + ray_direction_arr[i]);
        double tNear = (center_min_arr[i] - ray_origin_arr[i]) * invD;
        double tFar = (center_max_arr[i] - ray_origin_arr[i]) * invD;

        if (invD < 0.0)
            std::swap(tNear, tFar);

        t0 = tNear > t0 ? tNear : t0;
        t1 = tFar < t1 ? tFar : t1;

        if (t0 >= t1)
            return false;
    }

    return true;
}

// Checks for a ray-triangle intersection using the Möller–Trumbore algorithm.
/**
 * @param r The ray to test.
 * @param t_min The minimum t value for a valid hit.
 * @param t_max The maximum t value for a valid hit.
 * @param rec The record to store hit information.
 * @return True if the ray intersects the triangle, false otherwise.
 */
bool Triangle::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    if (!gridHit(r, t_min, t_max, rec)) return false;

    const double EPSILON = 1e-6;

    vec3 edge1 = v1 - v0;
    vec3 edge2 = v2 - v0;
    vec3 h = vec3::cross(r.getDirection(), edge2);
    double a = vec3::dot(edge1, h);

    if (a > -EPSILON && a < EPSILON)
        return false;

    double f = 1.0 / a;
    vec3 s = r.getOrigin() - v0;
    double u = f * vec3::dot(s, h);

    if (u < 0.0 || u > 1.0)
        return false;

    vec3 q = vec3::cross(s, edge1);
    double v = f * vec3::dot(r.getDirection(), q);

    if (v < 0.0 || u + v > 1.0)
        return false;

    double t = f * vec3::dot(edge2, q);

    if (t > t_min && t < t_max) {
        rec.material = this->material;
        rec.t = t;
        rec.p = r.pointAtParameter(rec.t);
        rec.normal = vec3::cross(edge1, edge2).return_unit();

        if (textureIsSet) {
            double temp_u = u0 * (1 - u - v) + u1 * u + u2 * v;
            double temp_v = v0_coord * (1 - u - v) + v1_coord * u + v2_coord * v;
            rec.material.setDiffuseColor(material.getTexture(temp_u, temp_v));
        }
        rec.normal.return_unit();
        return true;
    }

    return false;
}

// Sets the material of the triangle.
/**
 * @param material The material to set.
 */
void Triangle::setMaterial(Material material) {
    this->material = material;
}

// Sets the texture of the triangle.
/**
 * @param texturePath The file path to the texture.
 */
void Triangle::setTexture(const std::string& texturePath) {
    this->material.setTexture(texturePath);
    this->textureIsSet = true;
}

// Calculates texture coordinates for the triangle.
void Triangle::calculateTextureCoordinates() {
    u0 = v0.x;
    v0_coord = v0.y;
    u1 = v1.x;
    v1_coord = v1.y;
    u2 = v2.x;
    v2_coord = v2.y;
}