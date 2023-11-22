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
    //std::vector<std::string> scenes = {"binary_primitves", "mirror_image", "simple_phong", "scene"};
    //std::vector<std::string> scenes = {"mirror_image", "simple_phong"};
    std::vector<std::string> scenes = {"scene"};

    //std::cout << "rendering  scene" << std::endl;
    //world.loadScene("D:\\labs\\MScCS\\CGr\\CGRCW2\\scene.json", cam);
    //cam.renderParallel(4, 10, world, scenes);
    //std::cout << "finished rendering scene" << std::endl;
//
    std::string filename = "a_combi.ppm";
    std::vector<std::string> chunkFiles = {"a_out1.ppm", "a_out2.ppm", "a_out3.ppm", "a_out4.ppm"}; 
    //cam.combineImagesIntoOne(filename, chunkFiles);

    
    // Replace these with the actual names of your images
    

    // Replace this with the desired output file name
    std::string outputFileName = "a_combined_image.ppm";

    // Combine images vertically
    combineImagesVertically(chunkFiles, outputFileName);

    return 0;


    return 0;

    for (const auto& scene : scenes){
        std::cout << "rendering " + scene<< std::endl;
        world.loadScene("D:\\labs\\MScCS\\CGr\\CGRCW2\\" + scene + ".json", cam);
        cam.render(10, world, "out_" + scene + ".ppm");
        std::cout << "finished rendering " + scene<< std::endl;
        
        // Replace 'input.ppm' and 'output.ppm' with your input and output file names
        std::string inputFilename = "out_" + scene + ".ppm";
        std::string outputFilename = "out_tonemapped_" + scene + ".ppm";


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

