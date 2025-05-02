#ifndef TEXTURE_H
#define TEXTURE_H

#include <vector>
#include <string>
#include "vector.h"

/**
 * @class Texture
 * @brief Represents a texture that can be applied to materials.
 */
class Texture {
public:
    Texture() {} // Default constructor.
    Texture(std::string texturePath) { loadPPM(texturePath); } // Constructor that loads a texture.

    vec3 getColor(float u, float v) const; // Gets the color at specific texture coordinates.
    bool loadPPM(const std::string& filename); // Loads a texture from a PPM file.

private:
    std::vector<unsigned char> image_; // The texture image data.
    int width_;  // Texture width.
    int height_; // Texture height.
};

#endif // TEXTURE_H