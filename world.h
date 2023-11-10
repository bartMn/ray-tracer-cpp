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

class World {
public:
    World() {}

    void addHittable(std::shared_ptr<Hittable> hittable) {
        objects.push_back(hittable);
    }

    void createAndAddTriangle(vec3 vertex1, vec3 vertex2, vec3 vertex3);
    void createAndAddSphere(vec3 center, double radius);    
    void createAndAddCylinder(vec3 bottomCenter, double radius, double height, vec3 normalVector);
    void createAndAddFloor(vec3 floorCenter, double floorSize);

    bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const;

private:
    std::vector<std::shared_ptr<Hittable>> objects;
};

#endif // WORLD_H
