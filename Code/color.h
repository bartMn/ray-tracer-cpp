#ifndef COLOR_H
#define COLOR_H
#include <fstream>
#include "vector.h"

void paintPixelNormalVec(double R, double G, double B, bool hit, std::ofstream& outFile);
void paintPixel(vec3 color, std::ofstream& outFile);

#endif