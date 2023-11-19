// In your cpp file, say Sphere.cpp
#include "Sphere.h"


bool Sphere::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    rec.material = this-> material;
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

