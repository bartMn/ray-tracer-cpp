#include "tonemapping.h"



// Function to perform Reinhard tone mapping on an image
void toneMapReinhard(std::vector<Pixel>& image, int width, int height, float key) {
    // Calculate luminance and adapt the exposure
    float sum = 0.0f;
    for (const auto& pixel : image) {
        sum += 0.2126f * pixel.r + 0.7152f * pixel.g + 0.0722f * pixel.b;
    }
    float averageLuminance = sum / (width * height);
    float exposure = key / averageLuminance;

    // Apply tone mapping
    for (auto& pixel : image) {
        pixel.r = pixel.r * exposure / (1.0f + pixel.r * exposure);
        pixel.g = pixel.g * exposure / (1.0f + pixel.g * exposure);
        pixel.b = pixel.b * exposure / (1.0f + pixel.b * exposure);
    }
}

// Function to read a PPM file
void readPPM(const std::string& filename, std::vector<Pixel>& image, int& width, int& height) {
    std::ifstream file(filename);
    std::string type;
    file >> type >> width >> height;
    int maxValue;
    file >> maxValue;

    image.resize(width * height);

    for (auto& pixel : image) {
        file >> pixel.r >> pixel.g >> pixel.b;
        pixel.r /= maxValue;
        pixel.g /= maxValue;
        pixel.b /= maxValue;
    }

    file.close();
}

// Function to write a PPM file
void writePPM(const std::string& filename, const std::vector<Pixel>& image, int width, int height) {
    std::ofstream file(filename);
    file << "P3\n" << width << " " << height << "\n255\n";

    for (const auto& pixel : image) {
        file << static_cast<int>(pixel.r * 255.0f) << " "
             << static_cast<int>(pixel.g * 255.0f) << " "
             << static_cast<int>(pixel.b * 255.0f) << "\n";
    }

    file.close();
}


