#define _USE_MATH_DEFINES  // Define this before including <cmath>
#include <iostream>
#include "Camera.h"
#include "vector.h"
#include "Sphere.h"
#include "triangle.h"
#include "Cylinder.h"
#include "circle.h"
//#include "cylinder.h"

int main() {
    
    World world;

    // Create a sphere and add it to the world
    Sphere mySphere(vec3(5, 0, +2), 2);
    Sphere mySphere1(vec3(2, 2, +1.5), 1.5);
    Sphere mySphere2(vec3(0, 0, -100), 100);

    double cylinderRadious = 3;
    double cylinderHeight = 2;
    double xycylinderCenter[] = {-1, 3};
    Circle myCircle (vec3(xycylinderCenter[0], xycylinderCenter[1], cylinderHeight), cylinderRadious, vec3(0, 0, 1));
    Circle myCircle1(vec3(xycylinderCenter[0], xycylinderCenter[1], 0), cylinderRadious, vec3(0, 0, -1));
    Cylinder cylinder(vec3(xycylinderCenter[0], xycylinderCenter[1], 0), //center of the bottom circle 
                      cylinderRadious,             //radious
                      cylinderHeight            //height
                       );
    

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

    
    
    world.addHittable(std::make_shared<Triangle>(triangle));
    world.addHittable(std::make_shared<Triangle>(triangle2));
    world.addHittable(std::make_shared<Triangle>(triangle3));
    //

    
    //
    world.addHittable(std::make_shared<Sphere>(mySphere));


    world.addHittable(std::make_shared<Cylinder>(cylinder));
    world.addHittable(std::make_shared<Circle>(myCircle));
    world.addHittable(std::make_shared<Circle>(myCircle1));
    
    //world.addHittable(std::make_shared<Sphere>(mySphere1));

    //world.addHittable(std::make_shared<Sphere>(mySphere2));


    //Cylinder myCylinder(vec3(4, 3, 0), 4, 2);
    
    // Rest of your code
    //std::cout << theta;
    Camera cam(vec3(-10,0, 4), //position
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