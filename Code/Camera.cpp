#define _USE_MATH_DEFINES  // Define this before including <cmath>
#include <cmath>
#include <limits>
#include "vector.h"  // Your 3D vector class
#include "Ray.h"      // Your ray class
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

bool removeFile(const std::string& filePath) {
#ifdef _WIN32
    return DeleteFileA(filePath.c_str()) != 0;
#else
    return std::remove(filePath.c_str()) == 0;
#endif
}

double random_double(double min, double max) {
    static std::uniform_real_distribution<double> distribution(min, max);
    static std::mt19937 generator;  // Mersenne Twister PRNG
    return distribution(generator);
}

inline vec3 random_in_unit_disk() {
    while (true) {
        auto p = vec3(random_double(-1,1), random_double(-1,1), 0);
        if (p.length_squared() < 1)
            return p;
    }
}

vec3 Camera::defocus_disk_sample() const {
        // Returns a random point in the camera defocus disk.
        auto p = random_in_unit_disk();
        return position + (p.x * defocus_disk_u) + (p.y * defocus_disk_v);
    }

vec3 random_in_unit_sphere() {
    while (true) {
        vec3 p = vec3::random(-1, 1);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

vec3 Camera::pixel_sample_square() const {
        // Returns a random point in the square surrounding a pixel at the origin.
        auto px = -0.5 + random_double(-1,1);
        auto py = -0.5 + random_double(-1,1);
        return (px * pixel_delta_u) + (py * pixel_delta_v);
    }

Ray Camera::get_ray(int i, int j) const {
        // Get a randomly-sampled camera ray for the pixel at location i,j, originating from
        // the camera defocus disk.

        auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
        auto pixel_sample = pixel_center + pixel_sample_square();

        //auto ray_origin = (defocus_angle <= 0) ? position : defocus_disk_sample();
        auto ray_origin =  defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;

        return Ray(ray_origin, ray_direction, vec3(0,0,0), 0);
}

Camera::Camera(const vec3& position, const vec3& lookAt, const vec3& up,
               double fov, double aspectRatio, double aperture, double focusDistance, int imageWidth, bool binaryRender, vec3 background) {
    setCameraParameters(position, lookAt, up, fov, aspectRatio, aperture, focusDistance, imageWidth, binaryRender, background);
}

//Camera::Camera() {
//    setCameraParameters(vec3(0,0,0), vec3(0,0,0), vec3(0,0,0), 1, 1, 1, 1, 1);
//}
vec3 Camera::getPosition() {return position;}

void Camera::setCameraParameters(const vec3& position, const vec3& lookAt, const vec3& up,
                                double fov, double aspectRatio, double aperture, double focusDistance, int imageWidth, bool binaryRender, vec3 background) {
    // Calculate camera parameters
    
    
    //double halfWidth = aspectRatio * halfHeight;
    this->background = background; 
    this->imageWidth = imageWidth;
    this->imageHeight = int(imageWidth /aspectRatio);
    this->position = position;

    // Determine viewport dimensions.
    double focal_length = (position - lookAt).length();
    double theta = fov * M_PI / 180;
    double halfHeight = tan(theta / 2);

    double viewport_height = 2 * halfHeight * focal_length;
    double viewport_width = viewport_height * (double(imageWidth)/imageHeight);

    // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
    w = (position - lookAt).return_unit();
    u = (vec3::cross(up, w)).return_unit();
    v = vec3::cross(w, u);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    vec3 viewport_u = viewport_width * u;    // Vector across viewport horizontal edge
    vec3 viewport_v = viewport_height * -v;  // Vector down viewport vertical edge

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    pixel_delta_u = viewport_u / imageWidth;
    pixel_delta_v = viewport_v / imageHeight;

    // Calculate the location of the upper left pixel.
    vec3 viewport_upper_left = position - (focal_length * w) - viewport_u/2 - viewport_v/2;
    pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    this -> binaryRender = binaryRender;

    auto defocus_radius = focus_dist * tan((defocus_angle * 3.14 / 180.0) / 2);
    defocus_disk_u = u * defocus_radius;
    defocus_disk_v = v * defocus_radius;
    
}

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
                //double u_offset = random_double(0,1);
                //double v_offset = random_double(0,1);

                //vec3 pixel_center = pixel00_loc + ((i + u_offset) * pixel_delta_u) + ((j + v_offset) * pixel_delta_v);

                
                //Ray r(position, ray_direction.return_unit(), vec3(0, 0, 0), 0); // Initialize depth to 0
                Ray r = get_ray(i, j);
                
                HitRecord rec;
                bool hit_return = world.hit(r, 0.001, std::numeric_limits<double>::infinity(), rec, 0); // Pass depth as 0

                if (hit_return) {
                    //vec3 target = rec.p + rec.normal + random_in_unit_sphere();
                    //pixel_color += 0.5 * (target - rec.p).return_unit();
                
                    if (!binaryRender) {
                        temp_color = r.getColor();
                        pixel_color += temp_color;
                    }
                    else{
                        pixel_color += vec3(50,50,50);
                    } 
                }
            }

            if (pixel_color.length() == 0) {
                paintPixel(background, outFile);
            } 
            else if (binaryRender){
                paintPixel(vec3(255,0,0), outFile);
            }
            else{
                pixel_color /= samplesPerPixel;
                paintPixel(pixel_color, outFile);
            }
        }
    }
    std::clog << "\rDone.                 \n";
}


void Camera::setupFromJson(const nlohmann::json& jsonInputCam, std::string RenderModeString, vec3 background){
    //"type":"pinhole", 
    //        "width":1200, 
    //        "height":800,
    //        "position":[0.0, 0, 0],
    //        "lookAt":[0.0, 0, 1.0],
    //        "upVector":[0.0, 1.0, 0.0],
    //        "fov":45.0,
    //        "exposure":0.1
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

    //std::cout << "getting render mode"<< std::endl;
    bool binaryRender_loc = RenderModeString == "binary" ? true: false; 


    setCameraParameters(position_loc,       lookAt_loc,
                        up_loc,             fov_loc,
                        aspectRatio_loc,    1,
                        1,              imageWidth_loc,
                        binaryRender_loc, background);
}


void Camera::renderChunk(int samplesPerPixel, World world, const std::string& outputFile, int startX, int endX) const {
    // Loop over each pixel in the image
    
    std::ofstream outFile(outputFile);
    outFile << "P3\n" << imageWidth << ' ' << endX - startX << "\n255\n";

    for (int j = startX; j < endX; ++j) {
        //std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
        for (int i = 0; i < imageWidth; ++i) {

            vec3 pixel_color(0, 0, 0);
            vec3 temp_color(0, 0, 0);

            for (int s = 0; s < samplesPerPixel; ++s) {
                Ray r = get_ray(i, j);
                HitRecord rec;
                bool hit_return = world.hit(r, 0.001, std::numeric_limits<double>::infinity(), rec, 0); // Pass depth as 0

                if (hit_return) {
                    //vec3 target = rec.p + rec.normal + random_in_unit_sphere();
                    //pixel_color += 0.5 * (target - rec.p).return_unit();
                
                    if (!binaryRender) {
                        temp_color = r.getColor();
                        pixel_color += temp_color;
                    }
                    else{
                        pixel_color += vec3(50,50,50);
                    } 
                }
            }
            
            if (pixel_color.length() == 0) {
                paintPixel(background, outFile);
            } 
            else if (binaryRender){
                paintPixel(vec3(255,0,0), outFile);
            }
            else{
                pixel_color /= samplesPerPixel;
                paintPixel(pixel_color, outFile);
            }
        }
    }
    std::clog << "\rDone.                 \n";
}


// Function to save the image to a file
void Camera::combineImagesIntoOne(const std::string& filename, const std::vector<std::string>& chunkFiles)
{
    std::ofstream outFile(filename);

    // Write PPM header
    outFile << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    // Concatenate individual chunk files into the final image
    for (const auto& chunkFile : chunkFiles)
    {
        std::ifstream chunkInFile(chunkFile);
        outFile << chunkInFile.rdbuf();
    }
}


void Camera::renderParallel(int numThreads, int samplesPerPixel, World& world, const std::string& outputFileName) {
    
    // Calculate the number of rows each thread will handle
    std::vector<std::string> temp_files;
    for (int i=0; i<numThreads; i++){
        temp_files.push_back( "temp_out" + std::to_string(i)+ ".ppm");
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
    
    for (auto& temp_file : temp_files){
        removeFile(temp_file);
    }

}