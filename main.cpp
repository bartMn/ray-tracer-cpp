#define _USE_MATH_DEFINES  // Define this before including <cmath>
#include <iostream>
#include "Camera.h"
#include "vector.h"

//#include "cylinder.h"

int main() {
    
    World world;

    world.createAndAddSphere(vec3(5, 0, +2), 2);

    world.createAndAddTriangle(vec3(2, -2, 0),
                               vec3(6, -5, 3),
                               vec3(2, -2, 6));

    world.createAndAddFloor(vec3(0,0,0), 20);

    world.createAndAddCylinder(vec3(-1, 3, 0),
                               3,
                               2,
                               vec3(0,1,1).return_unit());

    Camera cam(vec3(-10,-6, 4), //position
               vec3(0,0,0), //lookAt
               vec3(0,0,1), // up
               45,          //fov
               1,           //double(16)/9, //aspectRatio
               1,           //aperture
               1,           //focus
               512          //width
               );        

    cam.render(10, world);
    return 0;
}