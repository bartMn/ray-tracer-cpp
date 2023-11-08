#define _USE_MATH_DEFINES  // Define this before including <cmath>
#include <iostream>
#include "Camera.h"
#include "vector.h"
#include "Sphere.h"

int main() {
    //double fov = 90; // Your field of view value
    //double theta = fov * M_PI / 180;
    World world;

    // Create a sphere and add it to the world
    Sphere mySphere(vec3(4, 0, 0), 2);
    world.addHittable(std::make_shared<Sphere>(mySphere));

    // Rest of your code
    //std::cout << theta;
    Camera cam(vec3(-10,0,0), //position
               vec3(4,0,0), //lookAt
               vec3(0,0,1), // up
               45,          //fov
               1,//double(16)/9,           //aspectRatio
               1,           //aperture
               1,           //focus
               1024         //width
               );        

    cam.render(1, world);
    return 0;
}