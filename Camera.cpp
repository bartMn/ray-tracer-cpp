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
    
}

void Camera::render(int samplesPerPixel, World world) const {
        // Loop over each pixel in the image
    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int j = 0; j < imageHeight; ++j) {
        std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
        for (int i = 0; i < imageWidth; ++i) {

            vec3 pixel_color(0, 0, 0);

            for (int s = 0; s < samplesPerPixel; ++s) {
                double u_offset = random_double(0,1);
                double v_offset = random_double(0,1);

                vec3 pixel_center = pixel00_loc + ((i + u_offset) * pixel_delta_u) + ((j + v_offset) * pixel_delta_v);

                vec3 ray_direction = pixel_center - position;
                Ray r(position, ray_direction.return_unit(), vec3(100, 100, 100));

                HitRecord rec;
                bool hit_return = world.hit(r, 0.001, std::numeric_limits<double>::infinity(), rec);

                if (hit_return) {
                    vec3 target = rec.p + rec.normal + random_in_unit_sphere();
                    pixel_color += 0.5 * (target - rec.p).return_unit();
                }
            }

            if (pixel_color.length() == 0) {
                paintPixelNormalVec(0, 0, 0, false);
            } else {
                pixel_color /= samplesPerPixel;
                paintPixelNormalVec(pixel_color.x, pixel_color.y, pixel_color.z, true);
            }
        }
    }
    std::clog << "\rDone.                 \n";
}
