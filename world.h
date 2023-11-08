// In your header file, say World.h
#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include "Hittable.h"
#include <memory>

class World {
public:
    World() {}

    void addHittable(std::shared_ptr<Hittable> hittable) {
        objects.push_back(hittable);
    }

    bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const;

private:
    std::vector<std::shared_ptr<Hittable>> objects;
};

#endif // WORLD_H
