#define _USE_MATH_DEFINES  // Define this before including <cmath>
#include <cmath>
#include <limits>
#include "vector.h"  // Your 3D vector class
#include "Ray.h"      // Your ray class
#include "Camera.h"
#include <iostream>
#include "color.h"
#include "hittable.h"


Camera::Camera(const vec3& position, const vec3& lookAt, const vec3& up,
               double fov, double aspectRatio, double aperture, double focusDistance, int imageWidth) {
    setCameraParameters(position, lookAt, up, fov, aspectRatio, aperture, focusDistance, imageWidth);
}

void Camera::setCameraParameters(const vec3& position, const vec3& lookAt, const vec3& up,
                                double fov, double aspectRatio, double aperture, double focusDistance, int imageWidth) {
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
    
    
    if (0){
    printf("camera position = ");
    vec3::printVector(position);
    
    printf("camera lookAt = ");
    vec3::printVector(lookAt);
    
    printf("w = \t\t");
    vec3::printVector(w);
    
    printf("u = \t\t");
    vec3::printVector(u);

    printf("v = \t\t");
    vec3::printVector(v);
    
    printf("pixel_delta_u = ");
    vec3::printVector(pixel_delta_u);
    
    printf("pixel_delta_v = ");
    vec3::printVector(pixel_delta_v);
    
    printf("pixel00_loc = ");
    vec3::printVector(pixel00_loc);
    
    }
}

void Camera::render(int samplesPerPixelm, World world) const {
        // Loop over each pixel in the image
    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int j = 0; j < imageHeight; ++j) {
        std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
        for (int i = 0; i < imageWidth; ++i) {

            vec3 pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            vec3 ray_direction = pixel_center - position;
            Ray r(position, ray_direction, vec3(100,100,100));
            HitRecord rec;
            if (world.hit(r, 0.0, std::numeric_limits<double>::infinity(), rec)) { 
                paintPixel(255, 255, 255);  
            }
            else paintPixel(0,0,0);
            
            
            
            
            
            
            
            
            
            
            
            
            //vec3 pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            //vec3 ray_direction = pixel_center - position;
            //Ray r(position, ray_direction);


            //double r_val = ray_direction.x;
            //double g_val = ray_direction.y;
            //double b_val = ray_direction.z; 
            //auto r_val = double(i) / (imageWidth-1);
            //auto g_val = double(j) / (imageHeight-1);
            //auto b_val = 0; 
            //paintPixel(((255*double(j)/imageHeight)), 255*double(i)/imageWidth, double(0));
            //paintPixel(255*r_val, 0*255*g_val, 0*255*b_val);
            
        }
   }
   std::clog << "\rDone.                 \n";
}

//Ray Camera::getRay(double u, double v) const {
//    vec3 rd = lensRadius * randomInUnitDisk();
//    vec3 offset = u * rd.x() + v * rd.y();
//    return Ray(origin + offset, lowerLeftCorner + u * horizontal + v * vertical - origin - offset);
//}
//
//class Renderer {
//public:
//    Renderer(int imageWidth, int imageHeight, int samplesPerPixel) 
//        : imageWidth(imageWidth), imageHeight(imageHeight), samplesPerPixel(samplesPerPixel) {
//        // Initialize your renderer with image dimensions and samples per pixel
//        // Create a Camera instance with your desired parameters
//        camera = Camera(/* camera parameters here */);
//    }
//
//    void render() {
//        // Create an image or framebuffer to store the rendered pixels
//        // Loop over each pixel in the image
//        for (int j = imageHeight - 1; j >= 0; --j) {
//            for (int i = 0; i < imageWidth; ++i) {
//                Color pixelColor(0, 0, 0);
//
//                for (int s = 0; s < samplesPerPixel; ++s) {
//                    // Generate a ray through the pixel using the camera
//                    double u = (i + randomDouble()) / (imageWidth - 1);
//                    double v = (j + randomDouble()) / (imageHeight - 1);
//                    Ray ray = camera.getRay(u, v);
//
//                    // Accumulate color by tracing the ray into the scene
//                    pixelColor += rayColor(ray, /* your scene or hittable list */);
//                }
//
//                // Output the final pixel color after averaging
//                // This is a simple example; you might want gamma correction, etc.
//                writeColor(pixelColor, samplesPerPixel);
//            }
//        }
//    }
//
//private:
//    Camera camera;
//    int imageWidth;
//    int imageHeight;
//    int samplesPerPixel;
//};//