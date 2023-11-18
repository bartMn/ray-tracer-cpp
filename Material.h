#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "vector.h"
#include ".\json-develop\single_include\nlohmann\json.hpp"


// Define a class for the material properties
class Material {
private:
    float ks;  // Specular reflection coefficient
    float kd;  // Diffuse reflection coefficient
    float specularexponent;
    vec3 diffusecolor;   // RGB values for diffuse color
    vec3 specularcolor;  // RGB values for specular color
    bool isreflective;
    float reflectivity;
    bool isrefractive;
    float refractiveindex;
    
public:
    // Constructor to initialize material properties
    Material() : ks(0.0), kd(0.0), specularexponent(0.0), isreflective(false),
                reflectivity(0.0), isrefractive(false), refractiveindex(0.0) {}

    vec3 getDiffuseColor();
    vec3 getSpecularColor();
    void setDiffuseColor(vec3 new_color);
    void setSpecularColor(vec3 new_color);
    static Material getMaterial(float ks,  // Specular reflection coefficient
                                float kd,  // Diffuse reflection coefficient
                                float specularexponent,
                                vec3 diffusecolor,   // RGB values for diffuse color
                                vec3 specularcolor,  // RGB values for specular color
                                bool isreflective,
                                float reflectivity,
                                bool isrefractive,
                                float refractiveindex);

    static Material getMaterialFromJson(const nlohmann::json& jsonInput);

};

#endif