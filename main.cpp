#define _USE_MATH_DEFINES  // Define this before including <cmath>
#include <iostream>
#include "Camera.h"
#include "vector.h"
#include "tonemapping.h"
#include "combine_ppms.h"

//#include "cylinder.h"

int main() {
    
    World world;


    Camera cam;
    //world.loadScene("D:\\labs\\MScCS\\CGr\\CGRCW2\\binary_primitves.json", cam);
    //td::vector<std::string> scenes = {"binary_primitves", "mirror_image", "simple_phong", "scene"};
    //std::vector<std::string> scenes = {"mirror_image", "simple_phong"};
    std::vector<std::string> scenes = {"scene"};
    //cam.renderParallel(5, 10, world, chunkFiles);
    
    int threads_to_run = 40;
    int num_of_pixel_samples = 10;
    for (const auto& scene : scenes){
        std::cout << "rendering " + scene<< std::endl;
        world.loadScene("D:\\labs\\MScCS\\CGr\\CGRCW2\\" + scene + ".json", cam);
        //cam.render(10, world, "out_" + scene + ".ppm");
        cam.renderParallel(threads_to_run, num_of_pixel_samples, world, "Aout_" + scene + ".ppm");
        std::cout << "finished rendering " + scene<< std::endl;
        
        // Replace 'input.ppm' and 'output.ppm' with your input and output file names
        std::string inputFilename = "Aout_" + scene + ".ppm";
        std::string outputFilename = "Bout_tonemapped_" + scene + ".ppm";


        // Replace 0.18 with your desired key value
        float key = 0.18f;
        // Read the input image
        int width, height;
        std::vector<Pixel> image;
        readPPM(inputFilename, image, width, height);
        // Perform tone mapping
        toneMapReinhard(image, width, height, key);
        // Write the output image
        writePPM(outputFilename, image, width, height);
        
    }
        
    return 0;
}

