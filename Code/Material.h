#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "vector.h"
#include "json-develop/single_include/nlohmann/json.hpp"
#include "texture.h"

/**
 * @class Material
 * @brief Represents the material properties of an object.
 */
class Material {
private:
    float ks;  // Specular reflection coefficient.
    float kd;  // Diffuse reflection coefficient.
    float specularexponent; // Specular exponent.
    vec3 diffusecolor;   // Diffuse color (RGB).
    vec3 specularcolor;  // Specular color (RGB).
    bool isreflective;   // Reflectivity flag.
    float reflectivity;  // Reflectivity value.
    bool isrefractive;   // Refractivity flag.
    float refractiveindex; // Refractive index.
    Texture texture;     // Texture object.
    bool textureIsSet;   // Texture flag.

public:
    // Default constructor.
    Material() : ks(0.0), kd(0.0), specularexponent(0.0), isreflective(false),
    reflectivity(0.0), isrefractive(false), refractiveindex(0.0), textureIsSet(false)  {}


    vec3 getDiffuseColor(); // Gets the diffuse color.
    vec3 getSpecularColor(); // Gets the specular color.
    void setDiffuseColor(vec3 new_color); // Sets the diffuse color.
    void setSpecularColor(vec3 new_color); // Sets the specular color.
    float getSpecularexponent(); // Gets the specular exponent.
    vec3 getDiffusecolor(); // Gets the diffuse color (alternative).
    vec3 getSpecularcolor(); // Gets the specular color (alternative).
    bool getIsreflective(); // Checks if the material is reflective.
    float getReflectivity(); // Gets the reflectivity value.
    bool getIsrefractive(); // Checks if the material is refractive.
    float getRefractiveindex(); // Gets the refractive index.
    float getKs(); // Gets the specular reflection coefficient.
    float getKd(); // Gets the diffuse reflection coefficient.

    static Material getMaterial(float ks, float kd, float specularexponent, vec3 diffusecolor, vec3 specularcolor, 
                                bool isreflective, float reflectivity, bool isrefractive, float refractiveindex); // Creates a material.

    static Material getMaterialFromJson(const nlohmann::json& jsonInput); // Creates a material from JSON input.

    void setTexture(const std::string& path); // Sets the texture.
    vec3 getTexture(float u, float v) const; // Gets the texture color at UV coordinates.
};

#endif // MATERIAL_H