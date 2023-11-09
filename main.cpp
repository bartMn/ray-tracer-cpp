#define _USE_MATH_DEFINES  // Define this before including <cmath>
#include <iostream>
#include "Camera.h"
#include "vector.h"
#include "Sphere.h"
//#include "cylinder.h"

int main() {
    
    World world;

    // Create a sphere and add it to the world
    Sphere mySphere(vec3(4, 0, +2), 2);
    world.addHittable(std::make_shared<Sphere>(mySphere));
    Sphere mySphere2(vec3(0, 0, -100), 100);
    world.addHittable(std::make_shared<Sphere>(mySphere2));


    //Cylinder myCylinder(vec3(4, 3, 0), 4, 2);
    //world.addHittable(std::make_shared<Cylinder>(myCylinder));

    // Rest of your code
    //std::cout << theta;
    Camera cam(vec3(-10,0,3), //position
               vec3(4,0,2), //lookAt
               vec3(0,0,1), // up
               45,          //fov
               1,//double(16)/9,           //aspectRatio
               1,           //aperture
               1,           //focus
               512         //width
               );        

    cam.render(1, world);
    return 0;
}