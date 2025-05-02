#define _USE_MATH_DEFINES  // Define this before including <cmath>
#include <cmath>
#include <limits>
#include "vector.h" 
#include "Ray.h"    
#include "Camera.h"
#include <iostream>
#include "color.h"
#include "hittable.h"
#include <sstream>
#include <thread>
#include <stdio.h>
#include <filesystem>
#include <random>
#include <fstream>
#include "combine_ppms.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

// Removes a file from the filesystem.
/**
* @param filePath The path to the file to remove.
* @return True if the file was successfully removed, false otherwise.
*/
bool removeFile(const std::string& filePath) {
   #ifdef _WIN32
       return DeleteFileA(filePath.c_str()) != 0;
   #else
       return std::remove(filePath.c_str()) == 0;
   #endif
   }
   
// Generates a random double in the range [min, max].
/**
 * @param min The minimum value.
 * @param max The maximum value.
 * @return A random double in the specified range.
 */
double random_double(double min, double max) {
    static std::uniform_real_distribution<double> distribution(min, max);
    static std::mt19937 generator;  // Mersenne Twister PRNG
    return distribution(generator);
}
    
// Generates a random point inside a unit disk.
/**
 * @return A random point inside a unit disk.
 */
inline vec3 random_in_unit_disk() {
    while (true) {
        auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.length_squared() < 1)
            return p;
    }
}
    
// Samples a random point on the camera's defocus disk.
/**
 * @return A random point on the defocus disk.
 */
vec3 Camera::defocus_disk_sample() const {
    auto p = random_in_unit_disk();
    return position + (p.x * defocus_disk_u) + (p.y * defocus_disk_v);
}

// Generates a random point inside a unit sphere.
/**
 * @return A random point inside a unit sphere.
 */
vec3 random_in_unit_sphere() {
    while (true) {
        auto p = vec3(random_double(-1, 1), random_double(-1, 1), random_double(-1, 1));
        if (p.length_squared() < 1)
            return p;
    }
}
    
// Generates a random point within a pixel.
/**
 * @return A random point within the square surrounding a pixel.
 */
vec3 Camera::pixel_sample_square() const {
    auto px = -0.5 + random_double(-1, 1);
    auto py = -0.5 + random_double(-1, 1);
    return (px * pixel_delta_u) + (py * pixel_delta_v);
}
    
// Generates a ray for a specific pixel.
/**
 * @param i The horizontal pixel index.
 * @param j The vertical pixel index.
 * @return A ray originating from the camera through the specified pixel.
 */
Ray Camera::get_ray(int i, int j) const {
    auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
    auto pixel_sample = pixel_center + pixel_sample_square();
    auto ray_origin = defocus_disk_sample();
    auto ray_direction = pixel_sample - ray_origin;
    return Ray(ray_origin, ray_direction, vec3(0, 0, 0), 0);
}
    
// Constructor: Initializes the camera with specified parameters.
/**
 * @param position The camera's position.
 * @param lookAt The point the camera is looking at.
 * @param up The up vector for the camera.
 * @param fov The field of view in degrees.
 * @param aspectRatio The aspect ratio of the image.
 * @param aperture The aperture size for depth of field.
 * @param focusDistance The focus distance.
 * @param imageWidth The width of the image in pixels.
 * @param binaryRender Flag for binary rendering mode.
 * @param background The background color.
 */
Camera::Camera(const vec3& position, const vec3& lookAt, const vec3& up,
               double fov, double aspectRatio, double aperture, double focusDistance, int imageWidth, bool binaryRender, vec3 background) {
    setCameraParameters(position, lookAt, up, fov, aspectRatio, aperture, focusDistance, imageWidth, binaryRender, background);
}

// Gets the camera's position.
/**
 * @return The position of the camera.
 */
vec3 Camera::getPosition() {
    return position;
}
    
// Sets the camera parameters.
/**
 * @param position The camera's position.
 * @param lookAt The point the camera is looking at.
 * @param up The up vector for the camera.
 * @param fov The field of view in degrees.
 * @param aspectRatio The aspect ratio of the image.
 * @param aperture The aperture size for depth of field.
 * @param focusDistance The focus distance.
 * @param imageWidth The width of the image in pixels.
 * @param binaryRender Flag for binary rendering mode.
 * @param background The background color.
 */
void Camera::setCameraParameters(const vec3& position, const vec3& lookAt, const vec3& up,
                                  double fov, double aspectRatio, double aperture, double focusDistance, int imageWidth, bool binaryRender, vec3 background) {
    this->background = background;
    this->imageWidth = imageWidth;
    this->imageHeight = int(imageWidth / aspectRatio);
    this->position = position;

    double focal_length = (position - lookAt).length();
    double theta = fov * M_PI / 180;
    double halfHeight = tan(theta / 2);

    double viewport_height = 2 * halfHeight * focal_length;
    double viewport_width = viewport_height * (double(imageWidth) / imageHeight);

    w = (position - lookAt).return_unit();
    u = (vec3::cross(up, w)).return_unit();
    v = vec3::cross(w, u);

    vec3 viewport_u = viewport_width * u;
    vec3 viewport_v = viewport_height * -v;

    pixel_delta_u = viewport_u / imageWidth;
    pixel_delta_v = viewport_v / imageHeight;

    vec3 viewport_upper_left = position - (focal_length * w) - viewport_u / 2 - viewport_v / 2;
    pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    this->binaryRender = binaryRender;

    auto defocus_radius = focus_dist * tan((defocus_angle * 3.14 / 180.0) / 2);
    defocus_disk_u = u * defocus_radius;
    defocus_disk_v = v * defocus_radius;
}


// Renders the scene.
/**
 * @param samplesPerPixel The number of samples per pixel.
 * @param world The world to render.
 * @param outputFile The output file path for the rendered image.
 */
 void Camera::render(int samplesPerPixel, World world, const std::string& outputFile) const {
    // Loop over each pixel in the image
    
    std::ofstream outFile(outputFile);
    outFile << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int j = 0; j < imageHeight; ++j) {
        std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
        for (int i = 0; i < imageWidth; ++i) {

            vec3 pixel_color(0, 0, 0);
            vec3 temp_color(0, 0, 0);

            for (int s = 0; s < samplesPerPixel; ++s) {
                // Generate a ray for the current pixel
                Ray r = get_ray(i, j);
                
                HitRecord rec;
                // Check if the ray hits any object in the world
                bool hit_return = world.hit(r, 0.001, std::numeric_limits<double>::infinity(), rec, 0);

                if (hit_return) {
                    if (!binaryRender) {
                        // Accumulate the color from the ray
                        temp_color = r.getColor();
                        pixel_color += temp_color;
                    } else {
                        // Add a fixed color for binary rendering
                        pixel_color += vec3(50, 50, 50);
                    }
                }
            }

            if (pixel_color.length() == 0) {
                // Paint the background color if no object is hit
                paintPixel(background, outFile);
            } 
            else if (binaryRender) {
                // Paint a fixed color for binary rendering
                paintPixel(vec3(255, 0, 0), outFile);
            }
            else {
                // Average the accumulated color and paint the pixel
                pixel_color /= samplesPerPixel;
                paintPixel(pixel_color, outFile);
            }
        }
    }
    std::clog << "\rDone.                 \n";
}

// Sets up the camera from JSON input.
/**
 * @param jsonInputCam The JSON object containing camera parameters.
 * @param RenderModeString The rendering mode ("binary" or other).
 * @param background The background color.
 */
void Camera::setupFromJson(const nlohmann::json& jsonInputCam, std::string RenderModeString, vec3 background) {
    vec3 position_loc = vec3(jsonInputCam["position"][0],
                             jsonInputCam["position"][1],
                             jsonInputCam["position"][2]);

    vec3 lookAt_loc = vec3(jsonInputCam["lookAt"][0],
                           jsonInputCam["lookAt"][1],
                           jsonInputCam["lookAt"][2]);

    vec3 up_loc = vec3(jsonInputCam["upVector"][0],
                       jsonInputCam["upVector"][1],
                       jsonInputCam["upVector"][2]);

    double fov_loc = jsonInputCam["fov"];
    int imageWidth_loc = jsonInputCam["width"];
    double aspectRatio_loc = double(jsonInputCam["width"]) / double(jsonInputCam["height"]);
    bool binaryRender_loc = RenderModeString == "binary" ? true: false; 

    setCameraParameters(position_loc,
                        lookAt_loc,
                        up_loc,
                        fov_loc,
                        aspectRatio_loc,
                        1,
                        1,
                        imageWidth_loc,
                        binaryRender_loc,
                        background);
}

// Renders a chunk of the image.
/**
 * @param samplesPerPixel The number of samples per pixel.
 * @param world The world to render.
 * @param outputFile The output file path for the chunk.
 * @param startX The starting row of the chunk.
 * @param endX The ending row of the chunk.
 */
void Camera::renderChunk(int samplesPerPixel, World world, const std::string& outputFile, int startX, int endX) const {
    std::ofstream outFile(outputFile);
    outFile << "P3\n" << imageWidth << ' ' << (endX - startX) << "\n255\n";

    for (int j = startX; j < endX; ++j) {
        for (int i = 0; i < imageWidth; ++i) {
            vec3 pixel_color(0, 0, 0);
            vec3 temp_color(0, 0, 0);

            for (int s = 0; s < samplesPerPixel; ++s) {
                Ray r = get_ray(i, j);
                HitRecord rec;
                // Check if the ray hits any object in the world
                bool hit_return = world.hit(r, 0.001, std::numeric_limits<double>::infinity(), rec, 0);

                if (hit_return) {
                    if (!binaryRender) {
                        // Accumulate the color from the ray
                        temp_color = r.getColor();
                        pixel_color += temp_color;
                    }
                    else{
                        // Add a fixed color for binary rendering
                        pixel_color += vec3(50, 50, 50);
                    }
                }
            }

            if (pixel_color.length() == 0) {
                // Paint the background color if no object is hit
                paintPixel(background, outFile);
            } 
            else if (binaryRender) {
                // Paint a fixed color for binary rendering
                paintPixel(vec3(255, 0, 0), outFile);
            }
            else {
                // Average the accumulated color and paint the pixel
                pixel_color /= samplesPerPixel;
                paintPixel(pixel_color, outFile);
            }
        }
    }
    std::clog << "\rDone.                 \n";
}

// Combines image chunks into one final image.
/**
 * @param filename The output file path for the combined image.
 * @param chunkFiles A vector of file paths for the image chunks.
 */
void Camera::combineImagesIntoOne(const std::string& filename, const std::vector<std::string>& chunkFiles) {
    std::ofstream outFile(filename);

    // Write PPM header
    outFile << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    // Concatenate individual chunk files into the final image
    for (const auto& chunkFile : chunkFiles) {
        std::ifstream chunkInFile(chunkFile);
        outFile << chunkInFile.rdbuf();
    }
}

// Renders the scene in parallel using multiple threads.
/**
 * @param numThreads The number of threads to use.
 * @param samplesPerPixel The number of samples per pixel.
 * @param world The world to render.
 * @param outputFileName The output file path for the rendered image.
 */
void Camera::renderParallel(int numThreads, int samplesPerPixel, World& world, const std::string& outputFileName) {
    std::vector<std::string> temp_files;
    for (int i = 0; i < numThreads; i++) {
        temp_files.push_back("temp_out" + std::to_string(i) + ".ppm");
    }
    int rowsPerThread = imageHeight / numThreads;

    // Create threads
    std::vector<std::thread> threads;
    for (int t = 0; t < numThreads; ++t) {
        int startRow = t * rowsPerThread;
        int endRow = (t + 1 == numThreads) ? imageHeight : (t + 1) * rowsPerThread;

        const std::string& sceneFile = temp_files[t];
        std::cout <<sceneFile <<std::endl; 
        threads.emplace_back(std::bind(&Camera::renderChunk, this, samplesPerPixel, std::ref(world), sceneFile, startRow, endRow));
    }

    // Join threads
    for (auto& thread : threads) {
        thread.join();
    }

    // Combine images vertically
    combineImagesVertically(temp_files, outputFileName);

    // Remove temporary files
    for (auto& temp_file : temp_files) {
        removeFile(temp_file);
    }
}