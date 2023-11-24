// In your cpp file, say Sphere.cpp
#include "Sphere.h"

bool Sphere::gridHit(const Ray& r, double t_min, double t_max, HitRecord& rec) const{

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

bool Sphere::hitBoundingBox(const Ray& r, double t0, double t1) const{
    // Assuming a simple bounding box for each sphere
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

bool Sphere::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    
    if (!gridHit(r, t_min, t_max, rec)) return false;

    vec3 oc = r.getOrigin() - center;
    double a = vec3::dot(r.getDirection(), r.getDirection());
    double b = vec3::dot(oc, r.getDirection());
    double c = vec3::dot(oc, oc) - radius * radius;
    double discriminant = b * b - a * c;

    if (discriminant > 0) {
        double temp = (-b - sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            rec.material = this-> material;
            rec.t = temp;
            rec.p = r.pointAtParameter(rec.t);
            rec.normal = (rec.p - center) / radius;

            if (textureIsSet) {
                double u = 0.5 + atan2(rec.normal.z, rec.normal.x) / (2 * 3.14);
                double v = 0.5 - asin(rec.normal.y) / 3.14;
                rec.material.setDiffuseColor(material.getTexture(u, v)); // You need to implement texture sampling
            }
            rec.normal.return_unit();
            return true;
        }
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            rec.material = this-> material;
            rec.t = temp;
            rec.p = r.pointAtParameter(rec.t);
            rec.normal = (rec.p - center) / radius;

            if (textureIsSet) {
                double u = 0.5 + atan2(rec.normal.z, rec.normal.x) / (2 * 3.14);
                double v = 0.5 - asin(rec.normal.y) / 3.14;
                rec.material.setDiffuseColor(material.getTexture(u, v)); // You need to implement texture sampling
            }
            rec.normal.return_unit();
            return true;
        }
    }
    return false;
}

void Sphere::setMaterial(Material material){
    this ->material = material;
}

void Sphere::setLightColour(vec3 lightCol){
    this->ligthColour = lightCol;
}

vec3 Sphere::getLightColour(){
    return ligthColour;
}

vec3 Sphere::getPosition(){
    return center;
}

void Sphere::setTexture(const std::string& texturePath) {
    this->material.setTexture(texturePath);
    this->textureIsSet = true;
}


