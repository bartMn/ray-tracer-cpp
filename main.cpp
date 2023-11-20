#define _USE_MATH_DEFINES  // Define this before including <cmath>
#include <iostream>
#include "Camera.h"
#include "vector.h"

//#include "cylinder.h"

int main() {
    
    World world;


    Camera cam;
    //world.loadScene("D:\\labs\\MScCS\\CGr\\CGRCW2\\binary_primitves.json", cam);
    std::vector<std::string> scenes = {"binary_primitves", "mirror_image", "simple_phong", "scene"};
    //std::vector<std::string> scenes = {"mirror_image", "simple_phong"};
    
    for (const auto& scene : scenes){
        std::cout << "rendering " + scene<< std::endl;
        world.loadScene("D:\\labs\\MScCS\\CGr\\CGRCW2\\" + scene + ".json", cam);
        cam.render(10, world, "out" + scene + ".ppm");
        std::cout << "finished rendering " + scene<< std::endl;
        
    }
        
    return 0;
}