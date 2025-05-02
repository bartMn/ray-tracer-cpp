#include "vector.h"
#include <cmath>
#include <iostream>
#include <random>

// Generates a random double in the range [min, max].
/**
 * @param min The minimum value.
 * @param max The maximum value.
 * @return A random double in the specified range.
 */
double random_double_vec(double min, double max) {
    static std::uniform_real_distribution<double> distribution(min, max);
    static std::mt19937 generator;  // Mersenne Twister PRNG
    return distribution(generator);
}

// Default constructor: initializes the vector to (0, 0, 0).
vec3::vec3() : x(0), y(0), z(0) {}

// Parameterized constructor: initializes the vector with given x, y, and z components.
/**
 * @param x The x component of the vector.
 * @param y The y component of the vector.
 * @param z The z component of the vector.
 */
vec3::vec3(double x, double y, double z) : x(x), y(y), z(z) {}

// Computes the length (magnitude) of the vector.
/**
 * @return The length of the vector.
 */
double vec3::length() const { return std::sqrt(x * x + y * y + z * z); }

// Computes the squared length of the vector.
/**
 * @return The squared length of the vector.
 */
double vec3::length_squared() const { return x * x + y * y + z * z; }

// Normalizes the vector to have a length of 1.
/**
 * If the vector has a non-zero length, its components are scaled to make its length equal to 1.
 */
void vec3::normalize() {
    double len = length();
    if (len != 0) {
        x /= len;
        y /= len;
        z /= len;
    }
}

// Generates a random vector with components in the range [min, max].
/**
 * @param min The minimum value for each component.
 * @param max The maximum value for each component.
 * @return A random vector with components in the specified range.
 */
vec3 vec3::random(double min, double max) {
    return vec3(random_double_vec(min, max), random_double_vec(min, max), random_double_vec(min, max));
}

// Returns a unit vector in the same direction as this vector.
/**
 * @return A normalized vector with the same direction as this vector.
 */
vec3 vec3::return_unit() {
    double x_new, y_new, z_new;
    double len = length();
    if (len != 0) {
        x_new = x / len;
        y_new = y / len;
        z_new = z / len;
    }
    return vec3(x_new, y_new, z_new);
}

// Prints the components of the vector to the console.
/**
 * @param a The vector to print.
 */
void vec3::printVector(const vec3& a) {
    printf("\tx = %f\n,", a.x);
    printf("\t\t\ty = %f\n,", a.y);
    printf("\t\t\tz = %f\n\n,", a.z);
}

// Computes the dot product of two vectors.
/**
 * @param a The first vector.
 * @param b The second vector.
 * @return The dot product of the two vectors.
 */
double vec3::dot(const vec3& a, const vec3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Computes the cross product of two vectors.
/**
 * @param a The first vector.
 * @param b The second vector.
 * @return The cross product of the two vectors.
 */
vec3 vec3::cross(const vec3& a, const vec3& b) {
    return vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

// Adds another vector to this vector.
/**
 * @param other The vector to add.
 * @return A reference to this vector after addition.
 */
vec3& vec3::operator+=(const vec3& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

// Subtracts another vector from this vector.
/**
 * @param other The vector to subtract.
 * @return A reference to this vector after subtraction.
 */
vec3& vec3::operator-=(const vec3& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

// Multiplies this vector by a scalar.
/**
 * @param scalar The scalar to multiply by.
 * @return A reference to this vector after multiplication.
 */
vec3& vec3::operator*=(double scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

// Divides this vector by a scalar.
/**
 * @param scalar The scalar to divide by.
 * @return A reference to this vector after division.
 */
vec3& vec3::operator/=(double scalar) {
    if (scalar != 0) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
    }
    return *this;
}

// Adds two vectors component-wise.
/**
 * @param a The first vector.
 * @param b The second vector.
 * @return The resulting vector after addition.
 */
vec3 operator+(const vec3& a, const vec3& b) {
    return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

// Subtracts the second vector from the first vector component-wise.
/**
 * @param a The first vector.
 * @param b The second vector.
 * @return The resulting vector after subtraction.
 */
vec3 operator-(const vec3& a, const vec3& b) {
    return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

// Multiplies two vectors component-wise.
/**
 * @param a The first vector.
 * @param b The second vector.
 * @return The resulting vector after component-wise multiplication.
 */
vec3 operator*(const vec3& a, const vec3& b) {
    return vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}

// Multiplies a vector by a scalar.
/**
 * @param v The vector to multiply.
 * @param scalar The scalar to multiply by.
 * @return The resulting vector after multiplication.
 */
vec3 operator*(const vec3& v, double scalar) {
    return vec3(v.x * scalar, v.y * scalar, v.z * scalar);
}

// Multiplies a scalar by a vector.
/**
 * @param scalar The scalar to multiply by.
 * @param v The vector to multiply.
 * @return The resulting vector after multiplication.
 */
vec3 operator*(double scalar, const vec3& v) {
    return v * scalar;
}

// Divides a vector by a scalar.
/**
 * @param v The vector to divide.
 * @param scalar The scalar to divide by.
 * @return The resulting vector after division.
 */
vec3 operator/(const vec3& v, double scalar) {
    if (scalar != 0) {
        return vec3(v.x / scalar, v.y / scalar, v.z / scalar);
    }
    return v;
}

// Negates a vector (reverses its direction).
/**
 * @param v The vector to negate.
 * @return The negated vector.
 */
vec3 operator-(const vec3& v) {
    return vec3(-v.x, -v.y, -v.z);
}