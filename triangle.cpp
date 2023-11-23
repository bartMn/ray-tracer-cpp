// In your cpp file, say Triangle.cpp
#include "triangle.h"

bool Triangle::gridHit(const Ray& r, double t_min, double t_max, HitRecord& rec) const{

    double t0 = std::numeric_limits<double>::lowest();
    double t1 = std::numeric_limits<double>::max();
    if (!hitBoundingBox(r, t0, t1))
        return false;

    // Ensure the intersection is within the valid range
    t0 = std::max(t0, t_min);
    t1 = std::min(t1, t_max);

    // Check if the ray misses the grid
    if (t0 >= t1)
        return false;

    // Check for intersection with the sphere

    return true;
}

double distance(const vec3& p1, const vec3& p2) {
    return std::sqrt((p1.x - p2.x) * (p1.x - p2.x) +
                     (p1.y - p2.y) * (p1.y - p2.y) +
                     (p1.z - p2.z) * (p1.z - p2.z));
}

double area(double a, double b, double c) {
    double s = (a + b + c) / 2;
    return std::sqrt(s * (s - a) * (s - b) * (s - c));
}

bool Triangle::hitBoundingBox(const Ray& r, double t0, double t1) const{
    // Assuming a simple bounding box for each sphere
    
    double a = distance(v0, v2);
    double b = distance(v1, v2);
    double c = distance(v1, v0);

    double K = area(a, b, c);
    double R = (a * b * c) / (4 * K);

    // The center of the circumscribed sphere is the centroid of the triangle
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



bool Triangle::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    // Möller–Trumbore intersection algorithm
    
    if (!gridHit(r, t_min, t_max, rec)) return false;

    const double EPSILON = 1e-6;

    vec3 edge1 = v1 - v0;
    vec3 edge2 = v2 - v0;
    vec3 h = vec3::cross(r.getDirection(), edge2);
    double a = vec3::dot(edge1, h);

    if (a > -EPSILON && a < EPSILON)
        return false; // Ray is parallel to the triangle

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
        rec.material = this-> material;
        rec.t = t;
        rec.p = r.pointAtParameter(rec.t);
        rec.normal = vec3::cross(edge1, edge2).return_unit();
        
        if (textureIsSet){
            // Interpolate texture coordinates
            double temp_u = u0 * (1 - u - v) + u1 * u + u2 * v;
            double temp_v = v0_coord * (1 - u - v) + v1_coord * u + v2_coord * v;
            // Use the getTexture function
            rec.material.setDiffuseColor(material.getTexture(temp_u, temp_v));
        }
        
        return true;
    }

    return false;
}

void Triangle::setMaterial(Material material){
    this ->material = material;
}

void Triangle::setTexture(const std::string& texturePath) {
    this->material.setTexture(texturePath);
    this->textureIsSet = true;
}

void Triangle::calculateTextureCoordinates() {
    // Assuming simple mapping where texture coordinates are the same as vertex coordinates
    u0 = v0.x;
    v0_coord = v0.y;
    u1 = v1.x;
    v1_coord = v1.y;
    u2 = v2.x;
    v2_coord = v2.y;
}
