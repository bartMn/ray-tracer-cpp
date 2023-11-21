// In your cpp file, say Triangle.cpp
#include "Triangle.h"

bool Triangle::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    // Möller–Trumbore intersection algorithm
    
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
