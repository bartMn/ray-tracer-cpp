// In your cpp file, say World.cpp
#include "World.h"


bool World::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    HitRecord temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;

    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

void World::createAndAddSphere(vec3 center, double radius){    
    Sphere newSphere(center, radius);
    World::addHittable(std::make_shared<Sphere>(newSphere));
}

void World::createAndAddTriangle(vec3 vertex1, vec3 vertex2, vec3 vertex3){
    Triangle newTriangle(vertex1,
                         vertex2,
                         vertex3);
    World::addHittable(std::make_shared<Triangle>(newTriangle));
}

void World::createAndAddCylinder(vec3 bottomCenter, double radius, double height, vec3 normalVector){
    
    vec3 upperCenter = bottomCenter + normalVector*height;
    Circle topCircle (upperCenter, radius, normalVector);
    Circle bottomCircle(bottomCenter, radius, normalVector*(-1));

    Cylinder cylinder(bottomCenter, //center of the bottom circle 
                      radius,             //radious
                      height,            //height
                      normalVector);

    World::addHittable(std::make_shared<Cylinder>(cylinder));
    World::addHittable(std::make_shared<Circle>(topCircle));
    World::addHittable(std::make_shared<Circle>(bottomCircle));
}

void World::createAndAddFloor(vec3 floorCenter, double floorSize){

    double halfSize = floorSize/2;
    World::createAndAddTriangle(floorCenter + vec3(-halfSize, -halfSize, 0),
                                floorCenter + vec3(halfSize, -halfSize, 0),
                                floorCenter + vec3(halfSize, halfSize, 0));
    World::createAndAddTriangle(floorCenter + vec3(-halfSize, -halfSize, 0),
                                floorCenter + vec3(halfSize, halfSize, 0),
                                floorCenter + vec3(-halfSize, halfSize, 0));
}
