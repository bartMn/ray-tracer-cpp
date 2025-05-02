#ifndef CAMERA_H
#define CAMERA_H

#define _USE_MATH_DEFINES  // Define this before including <cmath>
#include <cmath>
#include "vector.h"  
#include "Ray.h"     
#include "world.h"
#include <fstream>
#include <thread>
#include "json-develop/single_include/nlohmann/json.hpp"

class World;

/**
 * @class Camera
 * @brief Represents a camera in the ray tracing scene.
 */
class Camera {
public:
    double defocus_angle = 3;  // Variation angle of rays through each pixel
    double focus_dist = 0.5;

    Camera() {} // Default constructor.
    Camera(const vec3& position, const vec3& lookAt, const vec3& up, 
           double fov, double aspectRatio, double aperture, double focusDistance, int imageWidth, bool binaryRender, vec3 background); // Constructor.

    void setCameraParameters(const vec3& position, const vec3& lookAt, const vec3& up,
                             double fov, double aspectRatio, double aperture, double focusDistance, int imageWidth, bool binaryRender, vec3 background); // Sets camera parameters.

    void render(int samplesPerPixel, World world, const std::string& outputFile) const; // Renders the scene.
    void setupFromJson(const nlohmann::json& jsonInputCam, std::string RenderModeString, vec3 background); // Sets up the camera from JSON input.
    vec3 getPosition(); // Gets the camera's position.
    void renderChunk(int samplesPerPixel, World world, const std::string& outputFile, int startX, int endX) const; // Renders a chunk of the image.
    void combineImagesIntoOne(const std::string& filename, const std::vector<std::string>& chunkFiles); // Combines image chunks into one.
    void renderParallel(int numThreads, int samplesPerPixel, World& world, const std::string& outputFileName); // Renders the scene in parallel.
    Ray getRay(double u, double v) const; // Generates a ray for a given pixel (u, v).
    vec3 defocus_disk_sample() const; // Samples a point on the defocus disk.
    Ray get_ray(int i, int j) const; // Generates a ray for a specific pixel.
    vec3 pixel_sample_square() const; // Samples a point within a pixel.

private:
    vec3 position;          // Camera position.
    vec3 lowerLeftCorner;   // Lower-left corner of the image plane.
    vec3 horizontal;        // Horizontal vector of the image plane.
    vec3 vertical;          // Vertical vector of the image plane.
    vec3 u, v, w;           // Camera basis vectors.
    vec3 lowerLeftCorner_pixel; // Lower-left corner of the first pixel.
    double lensRadius;      // Lens radius for depth of field.
    int imageWidth;         // Image width in pixels.
    int imageHeight;        // Image height in pixels.
    vec3 pixel00_loc;       // Location of pixel (0, 0).
    vec3 pixel_delta_u;     // Offset to the next pixel to the right.
    vec3 pixel_delta_v;     // Offset to the next pixel below.
    bool binaryRender;      // Flag for binary rendering mode.
    vec3 background;        // Background color.
    vec3 defocus_disk_u;    // Horizontal radius of the defocus disk.
    vec3 defocus_disk_v;    // Vertical radius of the defocus disk.
};

#endif // CAMERA_H