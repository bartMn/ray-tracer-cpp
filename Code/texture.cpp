#include "texture.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

// Gets the color at specific texture coordinates.
/**
 * @param u The U coordinate (horizontal) in the texture.
 * @param v The V coordinate (vertical) in the texture.
 * @return The color at the specified texture coordinates as a vec3 (RGB).
 */
vec3 Texture::getColor(float u, float v) const {
    int x = static_cast<int>(u * width_) % width_;
    int y = static_cast<int>(v * height_) % height_;

    int index = (y * width_ + x) * 3;
    float r = static_cast<float>(image_[index]) / 255.0f;
    float g = static_cast<float>(image_[index + 1]) / 255.0f;
    float b = static_cast<float>(image_[index + 2]) / 255.0f;

    return vec3(r, g, b);
}

// Loads a texture from a PPM file.
/**
 * @param filename The file path to the PPM texture file.
 * @return True if the texture was successfully loaded, false otherwise.
 */
bool Texture::loadPPM(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return false;
    }

    std::string format;
    file >> format;
    if (format != "P6") {
        std::cerr << "Invalid PPM format. Only P6 is supported." << std::endl;
        return false;
    }

    file >> width_ >> height_;
    int maxColor;
    file >> maxColor;

    // Ignore newline character
    file.ignore(1);

    // Resize the image vector to hold all pixel data
    image_.resize(width_ * height_ * 3);  // Each pixel has three color channels (RGB)

    // Read pixel data
    file.read(reinterpret_cast<char*>(image_.data()), image_.size());

    file.close();
    return true;
}