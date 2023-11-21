#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

struct Pixel {
    float r, g, b;

    Pixel() : r(0.0f), g(0.0f), b(0.0f) {}
};

void toneMapReinhard(std::vector<Pixel>& image, int width, int height, float key);
void readPPM(const std::string& filename, std::vector<Pixel>& image, int& width, int& height);
void writePPM(const std::string& filename, const std::vector<Pixel>& image, int width, int height);