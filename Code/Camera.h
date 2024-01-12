#ifndef CAMERA_H
#define CAMERA_H

#define _USE_MATH_DEFINES  // Define this before including <cmath>
#include <cmath>
#include "vector.h"  // Your 3D vector class
#include "Ray.h"      // Your ray class
#include "world.h"
#include <fstream>
#include <thread>

#include "json-develop/single_include/nlohmann/json.hpp"

class World;

class Camera {
public:
    double defocus_angle = 3;  // Variation angle of rays through each pixel
    double focus_dist = 0.5; 
    // Constructors
    Camera() {}
    Camera(const vec3& position, const vec3& lookAt, const vec3& up, 
           double fov, double aspectRatio, double aperture, double focusDistance, int imageWidth, bool binaryRender, vec3 background);

    // Setters for camera parameters
    void setCameraParameters(const vec3& position, const vec3& lookAt, const vec3& up,
                             double fov, double aspectRatio, double aperture, double focusDistance, int imageWidth, bool binaryRender, vec3 background);

    // Generate a ray for a given pixel (u, v) on the image plane
    //Ray getRay(double u, double v) const;
    void render(int samplesPerPixel, World world, const std::string& outputFile) const;
    void setupFromJson(const nlohmann::json& jsonInputCam, std::string RenderModeString, vec3 background);
    vec3 getPosition();
    void renderChunk(int samplesPerPixel, World world, const std::string& outputFile, int startX, int endX/*, int startY, int endY, int samplesPerPixel, World world, const std::string& chunkFile*/) const;
    void combineImagesIntoOne(const std::string& filename, const std::vector<std::string>& chunkFiles);
    void renderParallel(int numThreads, int samplesPerPixel, World& world, const std::string& outputFileName);
    Ray getRay(double u, double v) const;
    vec3 defocus_disk_sample() const;
    Ray get_ray(int i, int j) const;
    vec3 pixel_sample_square() const;

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
    bool binaryRender;
    vec3 background;
    vec3   defocus_disk_u;  // Defocus disk horizontal radius
    vec3   defocus_disk_v;  // Defocus disk vertical radius
};


//Ray Camera::getRay(double u, double v) const {
//    vec3 rd = lensRadius * randomInUnitDisk();
//    vec3 offset = u * rd.x + v * rd.y;
//    return Ray(origin + offset, lowerLeftCorner + u * horizontal + v * vertical - origin - offset);
//}

#endif