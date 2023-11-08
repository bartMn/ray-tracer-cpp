//#include <iostream>
//#include "vector.h"
//
//int main(void){
//
//    vec3 v1(1,2,3);
//    vec3 v2(4,5,6);
//    std::cout << vec3::dot(v1,v2);
//    return 0;
//}   

//#include <iostream>
//
//int main() {
//
//    // Image
//
//    int image_width = 256;
//    int image_height = 256;
//
//    // Render
//
//    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
//
//      for (int j = 0; j < image_height; ++j) {
//        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
//        for (int i = 0; i < image_width; ++i) {
//            auto r = double(i) / (image_width-1);
//            auto g = double(j) / (image_height-1);
//            auto b = 0;
//
//            int ir = static_cast<int>(255.999 * r);
//            int ig = static_cast<int>(255.999 * g);
//            int ib = static_cast<int>(255.999 * b);
//
//            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
//        }
//    }
//
//    std::clog << "\rDone.                 \n";
//}

#define _USE_MATH_DEFINES  // Define this before including <cmath>
#include <iostream>
#include <math.h>
#include "Camera.h"
#include "vector.h"

int main() {
    double fov = 90; // Your field of view value
    //double theta = fov * M_PI / 180;
    
    // Rest of your code
    //std::cout << theta;
    Camera cam(vec3(1,0,0), //position
               vec3(4,0,0), //lookAt
               vec3(0,1,0), // up
               45,          //fov
               double(16)/9,           //aspectRatio
               1,           //aperture
               1,           //focus
               256         //width
               );        

        cam.render(1);
    return 0;
}