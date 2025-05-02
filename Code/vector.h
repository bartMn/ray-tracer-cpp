#ifndef VEC_H
#define VEC_H

#include <cmath>

/**
 * @class vec3
 * @brief Represents a 3D vector with utility functions.
 */
class vec3 {
public:
    double x, y, z; ///< Vector components.

    vec3(); // Default constructor.
    vec3(double x, double y, double z); // Parameterized constructor.

    double length() const; // Returns the vector's magnitude.
    double length_squared() const; // Returns the squared magnitude.
    void normalize(); // Normalizes the vector.
    vec3 return_unit(); // Returns a unit vector.

    static void printVector(const vec3& a); // Prints the vector.
    static double dot(const vec3& a, const vec3& b); // Dot product.
    static vec3 cross(const vec3& a, const vec3& b); // Cross product.
    static vec3 random(double min, double max); // Generates a random vector.

    vec3& operator+=(const vec3& other); // Adds another vector.
    vec3& operator-=(const vec3& other); // Subtracts another vector.
    vec3& operator*=(double scalar); // Multiplies by a scalar.
    vec3& operator/=(double scalar); // Divides by a scalar.
};

// Vector operators
vec3 operator+(const vec3& a, const vec3& b); // Vector addition.
vec3 operator-(const vec3& a, const vec3& b); // Vector subtraction.
vec3 operator*(const vec3& v, double scalar); // Scalar multiplication.
vec3 operator*(const vec3& a, const vec3& b); // Component-wise multiplication.
vec3 operator*(double scalar, const vec3& v); // Scalar multiplication.
vec3 operator/(const vec3& v, double scalar); // Scalar division.
vec3 operator-(const vec3& v); // Negates the vector.

#endif // VEC_H