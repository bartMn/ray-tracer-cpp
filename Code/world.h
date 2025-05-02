#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <iostream>
#include <string>
#include <random>
#include <memory>
#include "json-develop/single_include/nlohmann/json.hpp"

#include "hittable.h"
#include "Sphere.h"
#include "triangle.h"
#include "circle.h"
#include "cylinder.h"
#include "Camera.h"
#include "vector.h"

class Camera;

/**
 * @class World
 * @brief Represents the 3D world containing objects and light sources.
 */
class World {
public:
    World() {} // Default constructor.

    void addHittable(std::shared_ptr<Hittable> hittable) {
        objects.push_back(hittable);
    }
// Adds a hittable object to the world.
void addLightSource(std::shared_ptr<Sphere> hittable) {
    lightSources.push_back(hittable);
}
// Adds a light source to the world.

    void createAndLight(const nlohmann::json& jsonInput); // Creates and adds light sources from JSON input.
    void createAndAddTriangle(const nlohmann::json& jsonInput, const std::string& pathToTextures); // Adds a triangle from JSON input.
    void createAndAddTriangle(vec3 vertex1, vec3 vertex2, vec3 vertex3); // Adds a triangle from vertices.
    void createAndAddSphere(const nlohmann::json& jsonInput, const std::string& pathToTextures); // Adds a sphere from JSON input.
    void createAndAddCylinder(const nlohmann::json& jsonInput, const std::string& pathToTextures); // Adds a cylinder from JSON input.
    void createAndAddFloor(vec3 floorCenter, double floorSize); // Adds a floor to the world.
    void loadScene(const std::string& filename, Camera& camera, const std::string& pathToTextures); // Loads a scene from a file.

    bool hit(Ray& r, double t_min, double t_max, HitRecord& rec, int depth); // Checks for ray-object intersections.

    Ray compute_reflected_ray(Ray& r, HitRecord& rec); // Computes the reflected ray.

    vec3 reflect(const vec3& v, const vec3& normal); // Reflects a vector around a normal.
    vec3 randomUnitVector(const vec3& normal); // Generates a random unit vector around a normal.

private:
    std::vector<std::shared_ptr<Hittable>> objects; // List of objects in the world.
    std::vector<std::shared_ptr<Sphere>> lightSources; // List of light sources in the world.
    Camera *camPtr; // Pointer to the camera.
    int maxBounces; // Maximum number of ray bounces.
};

#endif // WORLD_H