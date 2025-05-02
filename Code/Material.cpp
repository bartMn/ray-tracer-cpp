#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "Material.h"
#include "json-develop/single_include/nlohmann/json.hpp"

// Creates a material with the specified properties.
/**
 * @param ks Specular reflection coefficient.
 * @param kd Diffuse reflection coefficient.
 * @param specularexponent Specular exponent.
 * @param diffusecolor Diffuse color (RGB).
 * @param specularcolor Specular color (RGB).
 * @param isreflective Reflectivity flag.
 * @param reflectivity Reflectivity value.
 * @param isrefractive Refractivity flag.
 * @param refractiveindex Refractive index.
 * @return A Material object with the specified properties.
 */
Material Material::getMaterial(float ks, float kd, float specularexponent, vec3 diffusecolor, vec3 specularcolor,
                               bool isreflective, float reflectivity, bool isrefractive, float refractiveindex) {
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

// Creates a material from JSON input.
/**
 * @param jsonInput The JSON object containing material properties.
 * @return A Material object created from the JSON input.
 */
Material Material::getMaterialFromJson(const nlohmann::json& jsonInput) {
    return getMaterial(float(jsonInput["ks"]), float(jsonInput["kd"]), float(jsonInput["specularexponent"]),
                       vec3(jsonInput["diffusecolor"][0], jsonInput["diffusecolor"][1], jsonInput["diffusecolor"][2]),
                       vec3(jsonInput["specularcolor"][0], jsonInput["specularcolor"][1], jsonInput["specularcolor"][2]),
                       bool(jsonInput["isreflective"]), float(jsonInput["reflectivity"]),
                       bool(jsonInput["isrefractive"]), float(jsonInput["refractiveindex"]));
}

// Gets the diffuse color of the material.
/**
 * @return The diffuse color (RGB).
 */
vec3 Material::getDiffuseColor() {
    return diffusecolor;
}

// Gets the specular color of the material.
/**
 * @return The specular color (RGB).
 */
vec3 Material::getSpecularColor() {
    return specularcolor;
}

// Sets the diffuse color of the material.
/**
 * @param new_color The new diffuse color (RGB).
 */
void Material::setDiffuseColor(vec3 new_color) {
    this->diffusecolor = new_color;
}

// Sets the specular color of the material.
/**
 * @param new_color The new specular color (RGB).
 */
void Material::setSpecularColor(vec3 new_color) {
    this->specularcolor = new_color;
}

// Gets the specular exponent of the material.
/**
 * @return The specular exponent.
 */
float Material::getSpecularexponent() {
    return specularexponent;
}

// Gets the diffuse color (alternative method).
/**
 * @return The diffuse color (RGB).
 */
vec3 Material::getDiffusecolor() {
    return diffusecolor;
}

// Gets the specular color (alternative method).
/**
 * @return The specular color (RGB).
 */
vec3 Material::getSpecularcolor() {
    return specularcolor;
}

// Checks if the material is reflective.
/**
 * @return True if the material is reflective, false otherwise.
 */
bool Material::getIsreflective() {
    return isreflective;
}

// Gets the reflectivity value of the material.
/**
 * @return The reflectivity value.
 */
float Material::getReflectivity() {
    return reflectivity;
}

// Checks if the material is refractive.
/**
 * @return True if the material is refractive, false otherwise.
 */
bool Material::getIsrefractive() {
    return isrefractive;
}

// Gets the refractive index of the material.
/**
 * @return The refractive index.
 */
float Material::getRefractiveindex() {
    return refractiveindex;
}

// Gets the specular reflection coefficient.
/**
 * @return The specular reflection coefficient.
 */
float Material::getKs() {
    return ks;
}

// Gets the diffuse reflection coefficient.
/**
 * @return The diffuse reflection coefficient.
 */
float Material::getKd() {
    return kd;
}

// Sets the texture of the material.
/**
 * @param path The file path to the texture.
 */
void Material::setTexture(const std::string& path) {
    texture.loadPPM(path);
}

// Gets the texture color at the specified UV coordinates.
/**
 * @param u The U coordinate.
 * @param v The V coordinate.
 * @return The texture color at the specified UV coordinates.
 */
vec3 Material::getTexture(float u, float v) const {
    return texture.getColor(u, v);
}