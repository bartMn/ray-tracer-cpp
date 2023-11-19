#define _USE_MATH_DEFINES  // Define this before including <cmath>
#include <cmath>
#include <limits>
#include "vector.h"  // Your 3D vector class
#include "Ray.h"      // Your ray class
#include "Camera.h"
#include <iostream>
#include "color.h"
#include "hittable.h"

#include <random>
#include <fstream>

double random_double(double min, double max) {
    static std::uniform_real_distribution<double> distribution(min, max);
    static std::mt19937 generator;  // Mersenne Twister PRNG
    return distribution(generator);
}


vec3 random_in_unit_sphere() {
    while (true) {
        vec3 p = vec3::random(-1, 1);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

Camera::Camera(const vec3& position, const vec3& lookAt, const vec3& up,
               double fov, double aspectRatio, double aperture, double focusDistance, int imageWidth, bool binaryRender) {
    setCameraParameters(position, lookAt, up, fov, aspectRatio, aperture, focusDistance, imageWidth, binaryRender);
}

//Camera::Camera() {
//    setCameraParameters(vec3(0,0,0), vec3(0,0,0), vec3(0,0,0), 1, 1, 1, 1, 1);
//}
vec3 Camera::getPosition() {return position;}

void Camera::setCameraParameters(const vec3& position, const vec3& lookAt, const vec3& up,
                                double fov, double aspectRatio, double aperture, double focusDistance, int imageWidth, bool binaryRender) {
    // Calculate camera parameters
    
    
    //double halfWidth = aspectRatio * halfHeight;

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
                double u_offset = random_double(0,1);
                double v_offset = random_double(0,1);

                vec3 pixel_center = pixel00_loc + ((i + u_offset) * pixel_delta_u) + ((j + v_offset) * pixel_delta_v);

                vec3 ray_direction = pixel_center - position;
                Ray r(position, ray_direction.return_unit(), vec3(0, 0, 0), 0); // Initialize depth to 0
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
                paintPixel(vec3(0,0,0), false, outFile);
            } 
            else if (binaryRender){
                paintPixel(vec3(255,0,0), true, outFile);
            }
            else{
                pixel_color /= samplesPerPixel;
                paintPixel(pixel_color, true, outFile);
            }
        }
    }
    std::clog << "\rDone.                 \n";
}


void Camera::setupFromJson(const nlohmann::json& jsonInputCam, std::string RenderModeString){
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
                        binaryRender_loc);
}