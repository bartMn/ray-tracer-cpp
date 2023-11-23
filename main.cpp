#define _USE_MATH_DEFINES  // Define this before including <cmath>
#include <iostream>
#include "Camera.h"
#include "vector.h"
#include "tonemapping.h"
#include "combine_ppms.h"
#include <filesystem>
#include <iostream>

#include <string>

#ifdef _WIN32
#include <Windows.h>

std::string getCurrentWorkingDirectory() {
    const DWORD bufferSize = MAX_PATH;
    char buffer[bufferSize];

    if (GetCurrentDirectoryA(bufferSize, buffer) != 0) {
        return std::string(buffer);
    } else {
        return "Error getting current working directory.";
    }
}

bool moveUpOneLevel() {
    std::string currentPath = getCurrentWorkingDirectory();

    // Find the last directory separator
    size_t lastSeparator = currentPath.find_last_of("\\/");
    if (lastSeparator != std::string::npos) {
        // Remove the last component
        currentPath = currentPath.substr(0, lastSeparator);
        if (SetCurrentDirectoryA(currentPath.c_str()) != 0) {
            return true;
        }
    }

    return false;
}

bool moveIntoFolder(const std::string& folderName) {
    std::string currentPath = getCurrentWorkingDirectory();
    currentPath += "\\" + folderName;  // Use "\\" for Windows separator

    if (SetCurrentDirectoryA(currentPath.c_str()) != 0) {
        return true;
    }

    return false;
}

#else
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

std::string getCurrentWorkingDirectory() {
    char buffer[FILENAME_MAX];
    if (getcwd(buffer, FILENAME_MAX) != nullptr) {
        return std::string(buffer);
    } else {
        return "Error getting current working directory.";
    }
}

bool moveUpOneLevel() {
    std::string currentPath = getCurrentWorkingDirectory();

    // Find the last directory separator
    size_t lastSeparator = currentPath.find_last_of("/");
    if (lastSeparator != std::string::npos) {
        // Remove the last component
        currentPath = currentPath.substr(0, lastSeparator);
        if (chdir(currentPath.c_str()) == 0) {
            return true;
        }
    }

    return false;
}

bool moveIntoFolder(const std::string& folderName) {
    std::string currentPath = getCurrentWorkingDirectory();
    currentPath += "/" + folderName;  // Use "/" for Linux separator

    // Create the folder if it doesn't exist
    if (mkdir(currentPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0) {
        if (chdir(currentPath.c_str()) == 0) {
            return true;
        }
    }

    return false;
}

#endif


int main() {
    
    World world;
    std::cout<<getCurrentWorkingDirectory() <<std::endl;
    
    std::cout << "Current Working Directory: " << getCurrentWorkingDirectory() << std::endl;

    // Move up one level
    if (moveUpOneLevel()) {
        std::cout << "Moved up one level. Current Working Directory: " << getCurrentWorkingDirectory() << std::endl;
    }

    // Move into a folder
    if (moveIntoFolder("TestSuite")) {
        std::cout << "Moved into folder. Current Working Directory: " << getCurrentWorkingDirectory() << std::endl;
    }

    return 0;
    return 0;
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

