#ifndef HITTABLE_H
#define HITTABLE_H

#include "Ray.h"
#include "vector.h"
#include "Material.h"

/**
 * @struct HitRecord
 * @brief Stores information about a ray-object intersection.
 */
struct HitRecord {
    double t;       ///< The parameter t at which the ray intersects the object.
    vec3 p;         ///< The intersection point.
    vec3 normal;    ///< The surface normal at the intersection point.
    Material material; ///< The material of the intersected object.
};

/**
 * @class Hittable
 * @brief Abstract base class for objects that can be hit by rays.
 */
class Hittable {
public:
    /**
     * @brief Determines if a ray hits the object.
     * @param r The ray to test.
     * @param t_min The minimum t value for a valid hit.
     * @param t_max The maximum t value for a valid hit.
     * @param rec The record to store hit information.
     * @return True if the ray hits the object, false otherwise.
     */
    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const = 0;
};

#endif // HITTABLE_H