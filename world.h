// In your header file, say World.h
#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include "Hittable.h"
#include <memory>
#include "Sphere.h"
#include "triangle.h"
#include "circle.h"
#include "cylinder.h"
#include "Camera.h"
#include "vector.h"
#include ".\json-develop\single_include\nlohmann\json.hpp"

class Camera;

class World {
public:
    World() {maxBounces = 3;}

    void addHittable(std::shared_ptr<Hittable> hittable) {
        objects.push_back(hittable);
    }

    void createAndAddTriangle(const nlohmann::json& jsonInput);
    void createAndAddTriangle(vec3 vertex1, vec3 vertex2, vec3 vertex3);
    void createAndAddSphere(const nlohmann::json& jsonInput);    
    void createAndAddCylinder(const nlohmann::json& jsonInput);
    void createAndAddFloor(vec3 floorCenter, double floorSize);
    void loadScene(const std::string& filename, Camera& camera);

    bool hit(Ray& r, double t_min, double t_max, HitRecord& rec, int depth);

    Ray compute_reflected_ray(Ray& r, HitRecord& rec);

    vec3 reflect(const vec3& v, const vec3& normal);

private:
    std::vector<std::shared_ptr<Hittable>> objects;
    int maxBounces;
};

#endif // WORLD_H
