#ifndef CAMERA_H
#define CAMERA_H

#define _USE_MATH_DEFINES  // Define this before including <cmath>
#include <cmath>
#include "vector.h"  // Your 3D vector class
#include "Ray.h"      // Your ray class
#include "world.h"

class Camera {
public:
    // Constructors
    Camera() {}
    Camera(const vec3& position, const vec3& lookAt, const vec3& up, 
           double fov, double aspectRatio, double aperture, double focusDistance, int imageWidth);

    // Setters for camera parameters
    void setCameraParameters(const vec3& position, const vec3& lookAt, const vec3& up,
                             double fov, double aspectRatio, double aperture, double focusDistance, int imageWidth);

    // Generate a ray for a given pixel (u, v) on the image plane
    //Ray getRay(double u, double v) const;
    void render(int samplesPerPixel, World world) const;

private:
    vec3 position;
    vec3 lowerLeftCorner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    vec3 lowerLeftCorner_pixel;
    double lensRadius;
    int imageWidth;
    int imageHeight;
    vec3   pixel00_loc;    // Location of pixel 0, 0
    vec3   pixel_delta_u;  // Offset to pixel to the right
    vec3   pixel_delta_v;
};


//Ray Camera::getRay(double u, double v) const {
//    vec3 rd = lensRadius * randomInUnitDisk();
//    vec3 offset = u * rd.x + v * rd.y;
//    return Ray(origin + offset, lowerLeftCorner + u * horizontal + v * vertical - origin - offset);
//}

#endif