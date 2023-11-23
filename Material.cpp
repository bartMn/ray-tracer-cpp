#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "Material.h"
#include "json-develop\single_include\nlohmann\json.hpp"

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
    material.textureIsSet = false; 

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

vec3 Material::getDiffuseColor(){
    return diffusecolor;
}

vec3 Material::getSpecularColor(){
    return specularcolor;
}

void Material::setDiffuseColor(vec3 new_color){
    this->diffusecolor = new_color;
}
void Material::setSpecularColor(vec3 new_color){
    this->specularcolor = new_color;
}

float Material::getSpecularexponent() {return specularexponent;}
vec3 Material::getDiffusecolor() {return diffusecolor;}
vec3 Material::getSpecularcolor() {return specularcolor;}
bool Material::getIsreflective() {return isreflective;}
float Material::getReflectivity() {return reflectivity;}
bool Material::getIsrefractive() {return isrefractive;}
float Material::getRefractiveindex() {return refractiveindex;};
float Material::getKs() {return ks;}
float Material::getKd() {return kd;}

void Material::setTexture(const std::string& path) {
    texture.loadPPM(path);
}

vec3 Material::getTexture(float u, float v) const{
    return texture.getColor(u, v);
}