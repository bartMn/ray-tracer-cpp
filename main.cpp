#define _USE_MATH_DEFINES  // Define this before including <cmath>
#include <iostream>
#include "Camera.h"
#include "vector.h"
#include "Sphere.h"
#include "triangle.h"
#include "Cylinder.h"
//#include "cylinder.h"

int main() {
    
    World world;

    // Create a sphere and add it to the world
    Sphere mySphere(vec3(5, 0, +2), 2);
    Sphere mySphere1(vec3(2, 2, +1.5), 1.5);
    Sphere mySphere2(vec3(0, 0, -100), 100);

    int half_floor_length = 10;
    Triangle triangle(vec3(2, -2, 0),
                      vec3(6, -5, 3),
                      vec3(2, -2, 6));
    Triangle triangle2(vec3(-half_floor_length, -half_floor_length, 0),
                       vec3(half_floor_length, -half_floor_length, 0),
                       vec3(half_floor_length, half_floor_length, 0));
                       //vec3(half_floor_length, -half_floor_length, 0));
    Triangle triangle3(vec3(-half_floor_length, -half_floor_length, 0),
                       vec3(half_floor_length, half_floor_length, 0),
                       vec3(-half_floor_length, half_floor_length, 0));

    Cylinder cylinder(vec3(0, 0, 0), //center of the bottom circle 
                      4,             //radious
                      2              //height
                       );
    
    Cylinder cylinder1(vec3(0, 0, -1), //center of the bottom circle 
                      100,             //radious
                      1              //height
                       );
    
    //world.addHittable(std::make_shared<Triangle>(triangle));
    //world.addHittable(std::make_shared<Triangle>(triangle2));
    //world.addHittable(std::make_shared<Triangle>(triangle3));
    //
    world.addHittable(std::make_shared<Cylinder>(cylinder));
    //
    world.addHittable(std::make_shared<Sphere>(mySphere));
    //world.addHittable(std::make_shared<Sphere>(mySphere1));

    //world.addHittable(std::make_shared<Sphere>(mySphere2));


    //Cylinder myCylinder(vec3(4, 3, 0), 4, 2);
    //world.addHittable(std::make_shared<Cylinder>(myCylinder));

    // Rest of your code
    //std::cout << theta;
    Camera cam(vec3(-10,0, 10), //position
               vec3(0,0,0), //lookAt
               vec3(0,0,1), // up
               45,          //fov
               1,//double(16)/9,           //aspectRatio
               1,           //aperture
               1,           //focus
               512         //width
               );        

    cam.render(10, world);
    return 0;
}