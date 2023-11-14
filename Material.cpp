#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "Material.h"
#include ".\json-develop\single_include\nlohmann\json.hpp"

// Function to parse the JSON file and extract material properties
Material Material::getMaterial(float ks,  // Specular reflection coefficient
                               float kd,  // Diffuse reflection coefficient
                               float specularexponent,
                               vec3 diffusecolor,   // RGB values for diffuse color
                               vec3 specularcolor,  // RGB values for specular color
                               bool isreflective,
                               float reflectivity,
                               bool isrefractive,
                               float refractiveindex)
{
    Material material;
    
    material.ks = ks;
    material.kd = kd;
    material.specularexponent = specularexponent;
    material.diffusecolor = diffusecolor;
    material.specularcolor = specularcolor;
    material.isreflective = isreflective;
    material.reflectivity = reflectivity;
    material.isrefractive = isrefractive;
    material.refractiveindex = refractiveindex;

    return material;
}

Material Material::getMaterialFromJson(const nlohmann::json& jsonInput){
    return getMaterial(float(jsonInput["ks"]),  // Specular reflection coefficient
                       float(jsonInput["kd"]),  // Diffuse reflection coefficient
                       float(jsonInput["specularexponent"]),
                       vec3 (jsonInput["diffusecolor"][0], jsonInput["diffusecolor"][1], jsonInput["diffusecolor"][2]),   // RGB values for diffuse color
                       vec3 (jsonInput["specularcolor"][0], jsonInput["specularcolor"][1], jsonInput["specularcolor"][2]),   // RGB values for diffuse color
                       bool (jsonInput["isreflective"]),
                       float(jsonInput["reflectivity"]),
                       bool (jsonInput["isrefractive"]),
                       float(jsonInput["refractiveindex"]));
}