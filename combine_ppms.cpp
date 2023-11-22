#include "combine_ppms.h"

void combineImagesVertically(const std::vector<std::string>& imageNames, const std::string& outputFileName) {
    // Vector to store image data
    std::vector<std::vector<std::vector<int>>> imageData;

    // Read image data from each file
    for (const auto& imageName : imageNames) {
        std::ifstream inputFile(imageName);
        if (!inputFile.is_open()) {
            std::cerr << "Error opening file: " << imageName << std::endl;
            exit(EXIT_FAILURE);
        }

        // Read PPM header
        std::string magicNumber;
        int width, height, maxColor;
        inputFile >> magicNumber >> width >> height >> maxColor;

        // Validate the header and resize the image vector
        if (magicNumber != "P3" && magicNumber != "P6") {
            std::cerr << "Error: Unsupported PPM format in file: " << imageName << std::endl;
            exit(EXIT_FAILURE);
        }

        std::vector<std::vector<int>> image;

        // Read image data
        if (magicNumber == "P3") {
            // ASCII PPM
            image.resize(height, std::vector<int>(width * 3, 0));
            for (int i = 0; i < height; ++i) {
                for (int j = 0; j < width * 3; ++j) {
                    inputFile >> image[i][j];
                }
            }
        } else if (magicNumber == "P6") {
            // Binary PPM
            image.resize(height, std::vector<int>(width * 3, 0));
            inputFile.read(reinterpret_cast<char*>(image[0].data()), height * width * 3);
        }

        imageData.push_back(image);
        inputFile.close();
    }

    // Combine images vertically
    int totalHeight = 0;
    int maxWidth = imageData[0][0].size();

    for (const auto& image : imageData) {
        totalHeight += image.size();
        maxWidth = std::max(maxWidth, static_cast<int>(image[0].size()));
    }

    // Create the combined image
    std::ofstream outputFile(outputFileName);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file: " << outputFileName << std::endl;
        exit(EXIT_FAILURE);
    }

    outputFile << "P3\n" << maxWidth / 3 << " " << totalHeight << "\n255\n";

    for (const auto& image : imageData) {
        for (const auto& row : image) {
            for (const auto& pixel : row) {
                outputFile << pixel << " ";
            }
        }
    }

    outputFile.close();
}