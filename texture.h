#include <vector>
#include <string>
#include "vector.h"

// Define a simple texture class
class Texture {
public:
    Texture() {}
    Texture(std::string texturePath){ loadPPM(texturePath); }

    // Function to get the color at specific texture coordinates
    vec3 getColor(float u, float v) const;
    bool loadPPM(const std::string& filename);

private:
    std::vector<unsigned char> image_;
    int width_;
    int height_;
};
