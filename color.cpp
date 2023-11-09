#include "color.h"
#include "vector.h"
#include <iostream>

int returnColVal(double in_val){
    if (in_val < 0 ) in_val *= -1;
    if (in_val >255) return 255;
    else return int(in_val);
}

void paintPixel(double R, double G, double B)
{
    int ir = returnColVal(R);
    int ig = returnColVal(G);
    int ib = returnColVal(B); 
    std::cout << ir << ' ' << ig << ' ' << ib << '\n';
}


//static_cast<int>(255.999 * R)
//static_cast<int>(255.999 * G)
//static_cast<int>(255.999 * B)