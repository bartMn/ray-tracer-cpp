#include "vector.h"
#include <cmath>
#include <iostream>

#include <random>

double random_double_vec(double min, double max) {
    static std::uniform_real_distribution<double> distribution(min, max);
    static std::mt19937 generator;  // Mersenne Twister PRNG
    return distribution(generator);
}

vec3::vec3() : x(0), y(0), z(0) {}
vec3::vec3(double x, double y, double z) : x(x), y(y), z(z) {}
double vec3::length() const {return std::sqrt(x * x + y * y + z * z);}
double vec3::length_squared() const {return x * x + y * y + z * z;}




void vec3::normalize() {
    double len = length();
    if (len != 0) {
        x /= len;
        y /= len;
        z /= len;
    }
}

vec3 vec3::random(double min, double max) {
    return vec3(random_double_vec(min, max), random_double_vec(min, max), random_double_vec(min, max));
}

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
void vec3::printVector(const vec3& a){
    printf("\tx = %f\n,", a.x);
    printf("\t\t\ty = %f\n,", a.y);
    printf("\t\t\tz = %f\n\n,", a.z);
}
double vec3::dot(const vec3& a, const vec3& b) {return a.x * b.x + a.y * b.y + a.z * b.z;}
vec3 vec3::cross(const vec3& a, const vec3& b) {return vec3(a.y * b.z - a.z * b.y,
                                                            a.z * b.x - a.x * b.z,
                                                            a.x * b.y - a.y * b.x);}

vec3& vec3::operator+=(const vec3& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

vec3& vec3::operator-=(const vec3& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

vec3& vec3::operator*=(double scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

vec3& vec3::operator/=(double scalar) {
    if (scalar != 0) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
    }
    return *this;
}

vec3 operator+(const vec3& a, const vec3& b) {return vec3(a.x + b.x, a.y + b.y, a.z + b.z);}
vec3 operator-(const vec3& a, const vec3& b) {return vec3(a.x - b.x, a.y - b.y, a.z - b.z);}
vec3 operator*(const vec3& v, double scalar) {return vec3(v.x * scalar, v.y * scalar, v.z * scalar);}
vec3 operator*(double scalar, const vec3& v) {return v * scalar;}
vec3 operator/(const vec3& v, double scalar) {
    if (scalar != 0) {
        return vec3(v.x / scalar, v.y / scalar, v.z / scalar);
    }
    return v;
}

vec3 operator-(const vec3& v) {return vec3(-v.x, -v.y, -v.z);}