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

// Gets the current working directory on Windows.
/**
 * @return The current working directory as a string.
 */
std::string getCurrentWorkingDirectory() {
    const DWORD bufferSize = MAX_PATH;
    char buffer[bufferSize];

    if (GetCurrentDirectoryA(bufferSize, buffer) != 0) {
        return std::string(buffer);
    } else {
        return "Error getting current working directory.";
    }
}

// Moves up one directory level on Windows.
/**
 * @return True if the operation was successful, false otherwise.
 */
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

// Moves into a specified folder on Windows.
/**
 * @param folderName The name of the folder to move into.
 * @return True if the operation was successful, false otherwise.
 */
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

// Gets the current working directory on Linux.
/**
 * @return The current working directory as a string.
 */
std::string getCurrentWorkingDirectory() {
    char buffer[FILENAME_MAX];
    if (getcwd(buffer, FILENAME_MAX) != nullptr) {
        return std::string(buffer);
    } else {
        return "Error getting current working directory.";
    }
}

// Moves up one directory level on Linux.
/**
 * @return True if the operation was successful, false otherwise.
 */
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

// Moves into a specified folder on Linux.
/**
 * @param folderName The name of the folder to move into.
 * @return True if the operation was successful, false otherwise.
 */
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
    std::string TestSuiteLocation = prjLocation + os_sep + "TestSuite";
    std::string VideoLocation = prjLocation + os_sep + "Video";
    std::string VideoFramesLocation = prjLocation + os_sep + "VideoFrames";
    std::string jsonFilesLocation = prjLocation + os_sep + "jsonFiles";
    #endif

    // Print directory paths for debugging
    std::cout << "Code Directory: " << CodeLocation << std::endl;
    std::cout << "TestSuiteLocation Directory: " << TestSuiteLocation << std::endl;
    std::cout << "VideoLocation Directory: " << VideoLocation << std::endl;
    std::cout << "VideoFramesLocation Directory: " << VideoFramesLocation << std::endl;
    std::cout << "jsonFilesLocation Directory: " << jsonFilesLocation << std::endl;

    Camera cam;

    // List of scenes to render
    std::vector<std::string> scenes = {"mirror_image", "simple_phong", "binary_primitves", "mirror_image", "scene", "scene2"};

    int threads_to_run = 10; // Number of threads for parallel rendering
    int num_of_pixel_samples = 10; // Number of samples per pixel

    for (const auto& scene : scenes) {
        std::cout << "Rendering " + scene << std::endl;
        std::cout << jsonFilesLocation << std::endl;
        std::cout << jsonFilesLocation + os_sep + scene + ".json" << std::endl;

        // Load the scene
        world.loadScene(jsonFilesLocation + os_sep + scene + ".json", cam, jsonFilesLocation);

        // Render the scene in parallel
        cam.renderParallel(threads_to_run, num_of_pixel_samples, world, TestSuiteLocation + os_sep + scene + ".ppm");
        std::cout << "Finished rendering " + scene << std::endl;

        // Perform tone mapping
        std::string inputFilename = TestSuiteLocation + os_sep + scene + ".ppm";
        std::string outputFilename = TestSuiteLocation + os_sep + "tonemapped_" + scene + ".ppm";

        float key = 0.18f; // Key value for tone mapping
        int width, height;
        std::vector<Pixel> image;

        // Read the input image
        readPPM(inputFilename, image, width, height);

        // Apply Reinhard tone mapping
        toneMapReinhard(image, width, height, key);

        // Write the output image
        writePPM(outputFilename, image, width, height);
    }

    return 0;
}