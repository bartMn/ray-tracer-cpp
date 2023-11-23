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
    #ifdef _WIN32
    std::string os_sep = "\\";
    std::string CodeLocation = getCurrentWorkingDirectory();
    moveUpOneLevel();
    moveIntoFolder("TestSuite");
    std::string TestSuiteLocation = getCurrentWorkingDirectory();
    moveUpOneLevel();
    moveIntoFolder("Video");
    std::string VideoLocation = getCurrentWorkingDirectory();
    moveUpOneLevel();
    moveIntoFolder("VideoFrames");
    std::string VideoFramesLocation = getCurrentWorkingDirectory();
    moveUpOneLevel();
    moveIntoFolder("jsonFiles");
    std::string jsonFilesLocation = getCurrentWorkingDirectory();
    moveUpOneLevel();
    moveIntoFolder("Code");
    #else
    std::string os_sep = "/";
    std::string CodeLocation = getCurrentWorkingDirectory();
    moveUpOneLevel();
    std::string prjLocation = getCurrentWorkingDirectory();
    std::string TestSuiteLocation = prjLocation +os_sep+"TestSuite";
    std::string VideoLocation = prjLocation +os_sep+"Video";
    std::string VideoFramesLocation = prjLocation +os_sep+"VideoFrames";
    std::string jsonFilesLocation = prjLocation +os_sep+"jsonFiles";

    #endif
    


    std::cout << "Code Directory: " << CodeLocation << std::endl;
    std::cout << "TestSuiteLocation Directory: " << TestSuiteLocation << std::endl;
    std::cout << "VideoLocation Directory: " << VideoLocation << std::endl;
    std::cout << "VideoFramesLocation Directory: " << VideoFramesLocation << std::endl;
    
    std::cout << "jsonFilesLocation Directory: " << jsonFilesLocation << std::endl;
    
    Camera cam;
    //world.loadScene("D:\\labs\\MScCS\\CGr\\CGRCW2\\binary_primitves.json", cam);
    //td::vector<std::string> scenes = {"binary_primitves", "mirror_image", "simple_phong", "scene"};
    //std::vector<std::string> scenes = {"mirror_image", "simple_phong"};
    std::vector<std::string> scenes = {"scene"};
    //cam.renderParallel(5, 10, world, chunkFiles);
    
    int threads_to_run = 10;
    int num_of_pixel_samples = 10;
    for (const auto& scene : scenes){
        std::cout << "rendering " + scene<< std::endl;
        std::cout << jsonFilesLocation<< std::endl;
        std::cout << jsonFilesLocation +os_sep+ scene + ".json"<< std::endl;
        world.loadScene(jsonFilesLocation +os_sep+ scene + ".json", cam, jsonFilesLocation);
        //cam.render(10, world, "out_" + scene + ".ppm");
        cam.renderParallel(threads_to_run, num_of_pixel_samples, world, TestSuiteLocation +os_sep+  scene + ".ppm");
        std::cout << "finished rendering " + scene<< std::endl;
        
        // Replace 'input.ppm' and 'output.ppm' with your input and output file names
        std::string inputFilename = TestSuiteLocation +os_sep+  scene + ".ppm";
        std::string outputFilename = TestSuiteLocation +os_sep+"tonemapped_" + scene + ".ppm";


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
