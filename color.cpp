#include "color.h"
#include "vector.h"
#include <iostream>


int returnColValForNormalVec(double in_val, bool hit){
    if (!hit) return 0;
    in_val += 1;
    in_val *= 100;
    if (in_val >255) return 255;
    else return int(in_val);
}

void paintPixelNormalVec(double R, double G, double B, bool hit, std::ofstream& outFile)
{
    int ir = returnColValForNormalVec(R, hit);
    int ig = returnColValForNormalVec(G, hit);
    int ib = returnColValForNormalVec(B, hit); 
    outFile << ir << ' ' << ig << ' ' << ib << '\n';
}

int returnColVal(double in_val){
    if (in_val < 0) return 0;
    //in_val += 1;
    //in_val *= 100;
    //if (in_val >255) return 255;

    else return int(in_val*120);
}

void paintPixel(vec3 color, std::ofstream& outFile)
{
    int ir = returnColVal(color.x);
    int ig = returnColVal(color.y);
    int ib = returnColVal(color.z); 
    outFile << ir << ' ' << ig << ' ' << ib << '\n';
}


//static_cast<int>(255.999 * R)
//static_cast<int>(255.999 * G)
//static_cast<int>(255.999 * B)