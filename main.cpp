#define _USE_MATH_DEFINES  // Define this before including <cmath>
#include <iostream>
#include "Camera.h"
#include "vector.h"

int main() {
    double fov = 90; // Your field of view value
    //double theta = fov * M_PI / 180;
    
    // Rest of your code
    //std::cout << theta;
    Camera cam(vec3(1,1,1), //position
               vec3(2,1,1), //lookAt
               vec3(0,0,1), // up
               45,          //fov
               1,//double(16)/9,           //aspectRatio
               1,           //aperture
               1,           //focus
               1024         //width
               );        

        cam.render(1);
    return 0;
}